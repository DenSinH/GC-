#include "CommandProcessor.h"
#include "PixelEngine.h"

#include "log.h"
#include "flags.h"
#include "helpers.h"
#include "sleeping.h"

#include "../Flipper/shaders/GX_constants.h"
#include "../PPC/interrupts.h"
#include "../system.h"

#ifdef CHECK_CP_COMMAND
#include <assert.h>
#endif

// color that is not in the color spectrum for NTSC/PAL screens (shade of purple) in YUYV format
// NOTE: this value is little endian
static const u32 IMPOSSIBLE_COLOR = ((impossible_Y) | (impossible_U << 8) | (impossible_Y << 16) | (impossible_V << 24));

SCHEDULER_EVENT(CP_frameswap_event) {
    s_CP* CP = (s_CP*)caller;

    if (!CP->PE_copy_frameswap) {
        acquire_mutex(&CP->efb_lock);
        CP->force_frameswap = true;

        // forcing a frameswap requires us to finish the draw command we are doing, and not merge any more
        CP->prev = 0;
        release_mutex(&CP->efb_lock);
    }
    else {
        // reset for next frame
        CP->PE_copy_frameswap = false;
    }

    // reschedule event
    if (CP->VI->HLW) {
        event->time += LINES_PER_FRAME * 2 * 3 * 6 * CP->VI->HLW;
    }
    else {
        // default value in case of "invalid" HLW
        event->time += LINES_PER_FRAME * 2 * 3 * 6 * 0x1AD;  // based on libOGC default initialized value
    }
    add_event(scheduler, event);
}


HW_REG_INIT_FUNCTION(CP) {
    for (int i = 0; i < MAX_DRAW_COMMANDS; i++) {
        CP->draw_command_available[i] = true;
    }

    create_mutex(&CP->availability_lock, false);
    create_mutex(&CP->draw_lock, false);
    create_mutex(&CP->efb_lock, false);

    create_wait_event(&CP->draw_commands_ready, false);

    // initially, there are spots available, so set this to true
    create_wait_event(&CP->draw_command_spot_available, true);

    CP->frameswap_event = (s_event) {
            .callback = CP_frameswap_event,
            .time = 0,  // immediately start doing it
            .caller = CP
    };
    add_event(CP->system->scheduler, &CP->frameswap_event);

    // set first draw command to have all arguments disabled
    // we only send draw commands when a new one is sent, so the very first draw command we send should be empty
    // setting this allows us to not do an extra check for this, and just run the code normally
    memset(&CP->draw_command_data[0].arg_offset, 0xff, sizeof(CP->draw_command_data[0].arg_offset));
    CP->draw_command_data[0].vertices = 0;
}

void destroy_CP(s_CP* CP) {
    // release all mutexes
    if (owns_mutex(&CP->availability_lock)) release_mutex(&CP->availability_lock);
    if (owns_mutex(&CP->draw_lock))         release_mutex(&CP->draw_lock);
    if (owns_mutex(&CP->efb_lock))          release_mutex(&CP->efb_lock);
}

// indexed by VAT.POSCNT, VAT.POSFMT
static const u8 coord_stride[2][8] = {
        // POSCNT = 0
        { 2, 2, 4, 4, 8, 0, 0, 0 },
        // POSCNT = 1
        { 3, 3, 6, 6, 12, 0, 0, 0 }
};

// indexed by VAT.NRMCNT, VAT.NRMFMT
static const u8 nrm_stride[2][8] = {
        // NRMCNT = 0
        { 0, 3, 0, 6, 12, 0, 0, 0 },
        // NRMCNT = 1
        { 0, 9, 0, 18, 36, 0, 0, 0 }
};

// indexed by VAT.COLxCNT, VAT.COLxFMT (poscnt actually does not impact this)
static const u8 color_stride[2][8] = {
        // CLRxCNT = 0
        { 2, 3, 4, 2, 3, 4, 0, 0 },
        // CLRxCNT = 1
        { 2, 3, 4, 2, 3, 4, 0, 0 }
};

// indexed by VAT.TEXxCNT, VAT.TEXxFMT
static const u8 tex_stride[2][8] = {
        // TEXxCNT = 0
        { 1, 1, 2, 2, 4, 0, 0, 0 },
        // TEXxCNT = 1
        { 2, 2, 4, 4, 8, 0, 0, 0 }
};

static inline u32 CP_add_draw_arg(s_CP* CP, u32 offset, u32 mode, e_draw_args arg, const u8 lut[2][8], u32 cnt, u32 fmt)
{
    if (mode) {
        log_cp("arg %d enabled, offset %d, [cnt %d][mode %d]", arg, offset, cnt, mode);
        CP->current_draw_command.arg_offset[arg] = offset;
        CP->current_draw_command.data_offset[arg] = mode > 1;   // signify whether we want to have this in our data
        return (CP->arg_size[arg] = (((mode) == 1) ? lut[cnt][fmt] : (mode) - 1));
    }
    else {
        CP->current_draw_command.arg_offset[arg] = -1;
        return 0;
    }
}

void update_CP_draw_argc(s_CP* CP, int format) {
    s_VCD_lo VCD_lo = { .raw = get_internal_CP_reg(CP, CP_reg_int_VCD_lo_base + format) };
    s_VCD_hi VCD_hi = { .raw = get_internal_CP_reg(CP, CP_reg_int_VCD_hi_base + format) };
    s_VAT_A VAT_A = { .raw = get_internal_CP_reg(CP, CP_reg_int_VAT_A_base + format) };
    s_VAT_B VAT_B = { .raw = get_internal_CP_reg(CP, CP_reg_int_VAT_B_base + format) };
    s_VAT_C VAT_C = { .raw = get_internal_CP_reg(CP, CP_reg_int_VAT_C_base + format) };

    e_draw_args draw_arg = 0;
    size_t offset = 0;
    for (; draw_arg <= draw_arg_TEX7MTXIDX; draw_arg++) {
        // these parameters are of size 1 if present
        CP->current_draw_command.arg_offset[draw_arg] = (VCD_lo.raw & (1 << draw_arg)) ? offset++ : -1;
    }

    offset += CP_add_draw_arg(CP, offset, VCD_lo.POS, draw_arg_POS, coord_stride, VAT_A.POSCNT, VAT_A.POSFMT);
    offset += CP_add_draw_arg(CP, offset, VCD_lo.NRM, draw_arg_NRM, nrm_stride, VAT_A.NRMCNT, VAT_A.NRMFMT);
    offset += CP_add_draw_arg(CP, offset, VCD_lo.COL0, draw_arg_CLR0, color_stride, VAT_A.COL0CNT, VAT_A.COL0FMT);
    offset += CP_add_draw_arg(CP, offset, VCD_lo.COL1, draw_arg_CLR1, color_stride, VAT_A.COL1CNT, VAT_A.COL1FMT);

    offset += CP_add_draw_arg(CP, offset, VCD_hi.TEX0, draw_arg_TEX0, tex_stride, VAT_A.TEX0CNT, VAT_A.TEX0FMT);
    offset += CP_add_draw_arg(CP, offset, VCD_hi.TEX1, draw_arg_TEX1, tex_stride, VAT_B.TEX1CNT, VAT_B.TEX1FMT);
    offset += CP_add_draw_arg(CP, offset, VCD_hi.TEX2, draw_arg_TEX2, tex_stride, VAT_B.TEX2CNT, VAT_B.TEX2FMT);
    offset += CP_add_draw_arg(CP, offset, VCD_hi.TEX3, draw_arg_TEX3, tex_stride, VAT_B.TEX3CNT, VAT_B.TEX3FMT);
    offset += CP_add_draw_arg(CP, offset, VCD_hi.TEX4, draw_arg_TEX4, tex_stride, VAT_B.TEX4CNT, VAT_B.TEX4FMT);
    offset += CP_add_draw_arg(CP, offset, VCD_hi.TEX5, draw_arg_TEX5, tex_stride, VAT_C.TEX5CNT, VAT_C.TEX5FMT);
    offset += CP_add_draw_arg(CP, offset, VCD_hi.TEX6, draw_arg_TEX6, tex_stride, VAT_C.TEX6CNT, VAT_C.TEX6FMT);
    offset += CP_add_draw_arg(CP, offset, VCD_hi.TEX7, draw_arg_TEX7, tex_stride, VAT_C.TEX7CNT, VAT_C.TEX7FMT);

    CP->current_draw_command.vertex_stride = offset;
    log_cp("Expecting %d bytes per vertex", offset);
    for (int i = draw_arg_PNMTXIDX; i < draw_arg_TEX7; i++) {
        log_cp("Argument %d: offset %d", i, CP->current_draw_command.arg_offset[i]);
    }
}

static inline void load_CP_reg(s_CP* CP, u8 RID, u32 value) {
    log_cp("Load CP register %02x with %08x", RID, value);
#ifdef CHECK_CP_COMMAND
    assert(RID - INTERNAL_CP_REGISTER_BASE < INTERNAL_CP_REGISTER_SIZE  /* invalid RID for CP command */);
#endif
    if ((RID >= 0xa0) || (RID >= 0x50 && RID < 0x98 && (RID & 7) == (CP->prev & 7))) {
        if (CP->internal_registers.CP_regs[RID - INTERNAL_CP_REGISTER_BASE] != value) {
            // invalidate arg merging because of VAT/VCD update
            CP->prev = 0;
        }
    }
    CP->internal_registers.CP_regs[RID - INTERNAL_CP_REGISTER_BASE] = value;
}

const static u8 XF_addr_to_section[] = { 0, 0xff, 0xff, 0xff, 1, 2, 3, 0xff };

static inline void load_XF_regs(s_CP* CP, u16 length, u16 base_addr, const u8* values_buffer) {
    log_cp("Load %d XF registers starting at %04x (first value %08x)", length, base_addr, READ32(values_buffer, 0));

    if (base_addr > INTERNAL_XF_REGISTER_BASE) {
#ifdef CHECK_CP_COMMAND
        assert(base_addr - INTERNAL_XF_REGISTER_BASE + length <= INTERNAL_XF_REGISTER_SIZE  /* invalid RID for CP command */);
#endif
        for (int i = 0; i < length; i++) {
            u32 value = READ32(values_buffer, i << 2);
            if (CP->internal_registers.XF_regs[base_addr + i - INTERNAL_XF_REGISTER_BASE] != value) {
                // invalidate draw merging because of XF update
                CP->prev = 0;
            }

            CP->internal_registers.XF_regs[base_addr + i - INTERNAL_XF_REGISTER_BASE] = value;
        }
    }
    else {
        /*
         * base address is either 0 - 0xff / 0x400 - 0x4ff / 0x500 - 0x5ff / 0x600 - 0x6ff
         * so we check ((base_addr + 0x100) >> 8) & 3 for the section
         * */
        u8 section = XF_addr_to_section[base_addr >> 8];
        for (int i = 0; i < length; i++) {
            u32 value = READ32(values_buffer, i << 2);
            log_cp("loaded %08x", value);
            if (CP->internal_registers.XF_mem[section][(base_addr & 0xff) + i] != value) {
                // invalidate draw merging because of XF update
                CP->prev = 0;
            }

            CP->internal_registers.XF_mem[section][(base_addr & 0xff) + i] = value;
        }
    }
}

static inline void load_INDX(s_CP* CP, e_CP_cmd opcode, u16 index, u8 length, u16 base_addr) {
    log_fatal("Load %d XF indexed (%02x, index value %02x) starting at %04x", length, opcode, index, base_addr);
    // todo
}

// todo: all the formats with ?
/*
 * Since we also need to be able to handle 4bit formats, I will shift the outcome right by 1, but first shift it by
 * the amount specified for the format
 * */
static const u8 tex_fmt_shft[16] = {
        0, // I4 ?
        1, // I8 ?
        0, // IA4 ?
        1, // IA8 ?
        2, // RGB565
        2, // RGB5A3
        3, // RGBA8
        0, // unused
        0, // CI4 ?
        1, // CI8 ?
        0, // CIA4 ?
        0, 0, 0, // unused
        1, // CMP ?
        0  // unused
};

static inline void send_draw_command(s_CP* CP) {
    /*
     * When merging commands, the same texture configuration is used for all commands
     * Therefore, it is unnecessary to keep stacking texture data
     * */
    u32 texture_offset = 0;

    // arguments 0 - draw_arg_POS are all always direct (they are indices)
    // instead, we use these to store information on the texture data
    for (int i = 0; i < 8; i++) {
        // loop over all textures (TEX0-TEX7)
        // todo: is this the proper way to check this (should be)
        if (CP->current_draw_command.arg_offset[draw_arg_TEX0 + i] < 0) {
            // texture is disabled
            continue;
        }

        CP->current_draw_command.data_offset[i] = texture_offset;
        s_SETIMAGE0_I setimage0_i = (s_SETIMAGE0_I) { .raw = CP->internal_registers.BP_regs[BP_reg_int_TEX_SET_IMAGE0_l_BASE + i] };
        u32 main_mem_offset = (CP->internal_registers.BP_regs[BP_reg_int_TEX_SET_IMAGE3_l_BASE + i] & 0x00ffffff) << 5;
        u32 data_length = (((setimage0_i.width + 1) * (setimage0_i.height + 1)) << tex_fmt_shft[setimage0_i.format]) >> 1;

        memcpy(&CP->current_texture_data.data + texture_offset, CP->system->memory + main_mem_offset, data_length);
        log_cp("Copied %x bytes of texture data for texture %d to offset %x (starting from %x, stride %x)",
               data_length, i, texture_offset, main_mem_offset, (1 << tex_fmt_shft[setimage0_i.format]) >> 1);

        // todo: align by block size (mostly 32, but 64 bytes for RGBA8)
        texture_offset += (data_length & ~3) + 4;  // keep aligned by 4 bytes for easier processing in shader
#ifdef CHECK_CP_DATA_BUFFER
        assert(texture_offset < DRAW_COMMAND_TEXTURE_BUFFER_SIZE /* texture buffer overflow */);
#endif
    }
    CP->current_texture_data.data_size = texture_offset;

    // we still need to buffer the indirect data, we do that here as well
    // similar argument as for texture_data with regards to argument "stacking"
    u32 data_offset = 0;
    size_t stride, size;
    i16 min_index, max_index;

    for (e_draw_args draw_arg = draw_arg_POS; draw_arg <= draw_arg_TEX7; draw_arg++) {
        if (!CP->current_draw_command.data_offset[draw_arg]) {
            // direct data
            continue;
        }

        stride = get_internal_CP_reg(CP, CP_reg_int_vert_ARRAY_STRIDE + draw_arg - draw_arg_POS);

        // calculate min and max indices for array
        if (CP->arg_size[draw_arg] == 1) {
            min_index = max_index = (i16)(i8)READ8(CP->current_draw_command.args, CP->current_draw_command.arg_offset[draw_arg]);
        }
        else {
            min_index = max_index = READ16(CP->current_draw_command.args, CP->current_draw_command.arg_offset[draw_arg]);
        }
        i16 index;
        // loop over arguments corresponding to array indices for <draw_arg>
        // loop over ALL vertices in the draw command, not just the one from the most recent one (merging)
        for (
            int v = CP->current_draw_command.arg_offset[draw_arg];
            v < CP->current_draw_command.arg_offset[draw_arg] + CP->current_draw_command.vertices * CP->current_draw_command.vertex_stride;
            v += CP->current_draw_command.vertex_stride
        ) {

            // read index
            if (CP->arg_size[draw_arg] == 1) {
                index = (i16)(i8)READ8(CP->current_draw_command.args, v);
            }
            else {
                index = READ16(CP->current_draw_command.args, v);
            }

            // compare
            if (index < min_index) min_index = index;
            else if (index > max_index) max_index = index;
        }

        // calculate data offset, account for min_index not being 0
        CP->current_draw_command.data_offset[draw_arg] = data_offset - min_index;
        size = stride * (((max_index - min_index) & ~3) + 4);   // align by 4 bytes

        // copy data
        memcpy(
                &CP->current_draw_command.data[data_offset],
                CP->system->memory + get_internal_CP_reg(CP, CP_reg_int_vert_ARRAY_BASE + draw_arg - draw_arg_POS) + min_index,
                size
        );
        log_cp("copied array for argument %d (%04x bytes, index %x -> %x (stride %d)) from %08x to offset %x",
               draw_arg, size, min_index, max_index, stride,
               get_internal_CP_reg(CP, CP_reg_int_vert_ARRAY_BASE + draw_arg - draw_arg_POS), data_offset
        );
        data_offset += size;

#ifdef CHECK_CP_DATA_BUFFER
        assert(data_offset < DRAW_COMMAND_DATA_BUFFER_SIZE /* data buffer overflow */);
#endif
    }
    CP->current_draw_command.data_size = data_offset;

    set_wait_event(&CP->draw_commands_ready);
}

static inline void call_DL(s_CP* CP, u32 list_addr, u32 list_size) {
    log_fatal("Call DL at %08x of size %08x (cmd %02x)", list_addr, list_size, CP->command);
    // we can basically just call the execute_buffer function but then on data in memory
}

static inline void XFB_copy(s_CP* CP, u16 command) {
    log_cp("Copying XFB -> EFB");
    u32 width  = (CP->internal_registers.BP_regs[BP_reg_int_EFB_dimensions] & 0x3ff) + 1;
    u32 height = ((CP->internal_registers.BP_regs[BP_reg_int_EFB_dimensions] >> 10) & 0x3ff) + 1;

    if (width == 0) {
        width = 640;  // default
    }

    if (height == 0) {
        height = 480;  // default
    }

    u32 XFB_addr = (CP->internal_registers.BP_regs[BP_reg_int_XFB_address] & 0x00ffffff) << 5;
    u32 EFB_addr = READ32(CP->VI->regs, VI_reg_TFBL);
    EFB_addr &= 0x00ffffff;

    acquire_mutex(&CP->efb_lock);
    if (command & PE_copy_execute) {
        if (EFB_addr) {
            // copy new data XFB -> EFB
            // skip this if it is not initialized
            // each pixel is 2 bytes
            memcpy(CP->system->memory + EFB_addr, CP->system->memory + XFB_addr, width * height << 1);
        }
    }
    release_mutex(&CP->efb_lock);

    // doing stuff to the XFB doesn't need to be synced
    // if XFB is NULL, then don't clear (probably not initialized)
    if (XFB_addr && command & PE_copy_clear) {
        // clear XFB
        for (int i = 0; i < width * height << 1; i += 4) {
            // clear words at a time
            memcpy(CP->system->memory + XFB_addr + i, &IMPOSSIBLE_COLOR, 4);
        }
    }
}

static inline void load_BP_reg(s_CP* CP, u32 value) {
    // value contains RID _and_ value here
    log_cp("Load BP reg: %08x", value);

    // RID is a byte, so it always fits into the range for the internal BP registers
    u8 RID = value >> 24;
    if (CP->internal_registers.BP_regs[RID] != value) {
        // invalidate draw command merging because of BP register update
        CP->prev = 0;
    }
    CP->internal_registers.BP_regs[RID] = value;

    switch (RID) {
        case BP_reg_int_PE_DONE:
            // check if bit 2 is set (likely) and if the PE finish interrupt is masked by PE
            if (CP->internal_registers.BP_regs[BP_reg_int_PE_DONE] & 0x02 && GET_PE_REG(CP->PE, PE_reg_interrupt_status) & 0x02) {
                // frame done
                SET_PE_REG(CP->PE, PE_reg_token, (u16)value);

                // set interrupt to called
                CP->PE->intr_status |= PE_intr_DONE;

                // add interrupt cause to processor interface and call interrupt (instant polling)
                set_PI_intsr(CP->PI, PI_intr_PE_DONE, 0);
            }
            break;
        case BP_reg_int_PE_copy_execute:
            CP->frameswap[CP->draw_command_index] = (u16)value;
            XFB_copy(CP, value);
            CP->PE_copy_frameswap = true;
            // doing a frameswap requires us to finish the draw command we are doing, and not merge any more
            CP->prev = 0;
            break;
        default:
            break;
    }
}

static inline void feed_CP(s_CP* CP, u8 data) {
    // feed from the buffer from some index
    // always read argument (we never get here with a NOP anyway)
    // lower 3 bits are always VAT index, except for 'Load BP REG' command
    switch (CP->command & 0xf8) {
        case CP_cmd_NOP:
            log_fatal("Uncaught NOP in argument feeding");
        case CP_cmd_load_CP_reg:
            CP->args[CP->argc++] = data;
            if (CP->argc == 5) {
                // 8bit RID + 32 bit value
                load_CP_reg(CP, CP->args[0], READ32(CP->args, 1));
                CP->fetching = false;
            }
            break;
        case CP_cmd_load_XF_reg:
            CP->args[CP->argc++] = data;
            if (CP->argc >= 2 && CP->argc == 2 + 2 + ((1 + READ16(CP->args, 0)) << 2)) {
                // 16bit (length - 1) + 16bit (1st addr) + 32bit * length (values)
                // in args we correct for the -1, and so do we do this for the buffer (starts at index 4)
                load_XF_regs(CP, READ16(CP->args, 0) + 1, READ16(CP->args, 2), &CP->args[4]);
                CP->fetching = false;
            }
            break;
        case CP_cmd_load_INDX_A:
        case CP_cmd_load_INDX_B:
        case CP_cmd_load_INDX_C:
        case CP_cmd_load_INDX_D:
            CP->args[CP->argc++] = data;
            if (CP->argc == 4) {
                // 16bit (index value) + 4bit (length - 1) + 12bit (1st address)
                u16 length_and_base_address = READ16(CP->args, 2);
                load_INDX(CP, CP->command, READ16(CP->args, 0), length_and_base_address >> 12, length_and_base_address & 0x0fff);
                CP->fetching = false;
            }
            break;
        case CP_cmd_call_DL:
            CP->args[CP->argc++] = data;
            if (CP->argc == 8) {
                // 32bit (list addr) + 32bit (list size)
                call_DL(CP, READ32(CP->args, 0), READ32(CP->args, 4));
                CP->fetching = false;
            }
            break;
        case CP_cmd_inval_vertex_cache:
            log_fatal("Uncaught invalidate vertex cache instruction in argument feeding");
        case CP_cmd_load_BP_reg & 0xf8:  // should be 0x61, but lower 3 bits are masked out
            CP->args[CP->argc++] = data;
            if (CP->argc == 4) {
                // 8bit (reg addr) + 24bit (value)
                load_BP_reg(CP, READ32(CP->args, 0));
                CP->fetching = false;
            }
            break;
        case CP_cmd_QUADS:
        case CP_cmd_TRIANGLES:
        case CP_cmd_TRIANGLESTRIP:
        case CP_cmd_TRIANGLEFAN:
        case CP_cmd_LINES:
        case CP_cmd_LINESTRIP:
        case CP_cmd_POINTS:
            // all drawing commands
            if (CP->argc == 0) {
                CP->argc++;
                CP->current_vertices = data << 8;
            }
            else if (CP->argc == 1) {
                CP->current_vertices |= data;
                CP->current_draw_command.vertices += CP->current_vertices;
                CP->argc++;
                log_cp("Expecting %d vertices", CP->current_vertices);
            }
            else {
                // first 2 bytes were number of vertices
                // continue from where we left off in the draw argument (from merging this might be nonzero)
                CP->current_draw_command.args[CP->current_draw_command.arg_size + CP->argc++ - 2] = data;

                if ((CP->argc - 2) == CP->current_draw_command.vertex_stride * CP->current_vertices) {
                    log_cp("Got %d bytes of draw data (including #vertices), sending command", CP->argc);
                    CP->fetching = false;

                    // also update new argument size
                    CP->current_draw_command.arg_size += CP->current_draw_command.vertex_stride * CP->current_vertices;
                }
            }
            break;
        default:
            log_fatal("Invalid CP command: %08x", CP->command);
    }
}


static inline void clear_current_command(s_CP* CP) {
    CP->current_draw_command.vertices = 0;
    CP->current_draw_command.arg_size = 0;
    // rest is irrelevant or overwritten by new command anyway

    // copy over current register configuration
    memcpy(
            &CP->current_draw_command.data_stride,
            &CP->internal_registers.CP_regs[CP_reg_int_vert_ARRAY_STRIDE - INTERNAL_CP_REGISTER_BASE],
            12 * sizeof(u32)
    );

    memcpy(
            &CP->current_register_data,
            &CP->internal_registers,
            sizeof(struct s_CP_register_data)
    );
}


static inline bool merge_draw_commands(s_CP* CP) {
    /* Conditions for merging:
     * - Configuration of registers must not have changed (CP->command == CP->prev)
     * - No frameswap must have happened (CP->command == CP->prev)
     * - We must have room left over in our draw argument buffer
     *      We actually know how many bytes to expect from the next draw command, so we can make it fit tightly
     *      Multiply by 8 to still have some leeway
     * */
    u32 expected_arg_bytes = 8 * CP->current_draw_command.vertex_stride * CP->current_vertices;
    return CP->command == CP->prev
           && CP->current_draw_command.arg_size + expected_arg_bytes < DRAW_COMMAND_ARG_BUFFER_SIZE;
}


void execute_buffer(s_CP* CP, const u8* buffer_ptr, u8 buffer_size) {
    u32 write_ptr = get_CP_reg(CP, CP_reg_FIFO_write_ptr_hi, CP_reg_FIFO_write_ptr_lo);
    // we read and parse the commands instantly, we don't need to get the read pointer
    u32 FIFO_base = get_CP_reg(CP, CP_reg_FIFO_base_hi, CP_reg_FIFO_base_lo);
    u32 FIFO_end = get_CP_reg(CP, CP_reg_FIFO_end_hi, CP_reg_FIFO_end_lo);

    u8 i = 0;
    while (i < buffer_size) {

        // log_cp("Should write/read to %08x (%08x -> %08x)", write_ptr++, FIFO_base, FIFO_end);
        if (!CP->fetching) {
            // reset CP command fetching environment
            CP->command = buffer_ptr[i++];
            CP->argc = 0;
            CP->fetching = CP->command != CP_cmd_NOP && CP->command != CP_cmd_inval_vertex_cache;
            if (CP->command >= CP_cmd_QUADS) {

                if (!merge_draw_commands(CP)) {
                    // send previous draw command
                    // todo: send it immediately when CP->prev is cleared
                    send_draw_command(CP);
                    // first draw command will be all zero's, causing nothing to be drawn
                    log_cp("Draw command %d sent", CP->draw_command_index);
                    // draw is automatically queued by setting draw_command_available to false

                    acquire_mutex(&CP->availability_lock);
                    CP->draw_command_available[CP->draw_command_index] = false;  // signify that draw command is used
                    if (++CP->draw_command_index == MAX_DRAW_COMMANDS) {
                        CP->draw_command_index = 0;
                    }
                    clear_current_command(CP);

                    update_CP_draw_argc(CP, CP->command & 7);

                    // check if current draw command in queue is available
                    bool draw_command_available = CP->draw_command_available[CP->draw_command_index];

                    if (!draw_command_available) {
                        // we don't want the event to be set between the last line and this one
                        clear_wait_event(&CP->draw_command_spot_available);
                        release_mutex(&CP->availability_lock);
#ifdef REPORT_GPU_BOTTLENECK
                        // if we get here, we are bottlenecked by the GPU not handling draw commands fast enough
                        log_warn("GPU bottlenecked, waiting for draw command...");
#endif
                        wait_for_event(&CP->draw_command_spot_available);
                    }
                    else {
                        release_mutex(&CP->availability_lock);
                    }

                    // clear frameswap trigger AFTER the new command is available
                    // (this means the old command has been processed fully, and so has the frameswap)
                    acquire_mutex(&CP->efb_lock);
                    CP->frameswap[CP->draw_command_index] = 0;
                    release_mutex(&CP->efb_lock);

                    log_cp("Got draw command spot");
                    // update previous command for next command to be drawn
                    CP->prev = CP->current_draw_command.command = CP->command;
                }
                else {
                    log_cp("Using previous draw command (%02x == %02x)", CP->prev, CP->command);

                    // we don't want triangle strips/fans to connect unwantedly, so we fill this vertex's arguments
                    // with 0xff to produce something we can't render
                    if (((CP->command & 0xf8) == CP_cmd_TRIANGLEFAN) || ((CP->command & 0xf8) == CP_cmd_TRIANGLESTRIP)) {
                        memset(
                                &CP->current_draw_command.args[CP->current_draw_command.arg_size],
                                0xff,
                                CP->current_draw_command.vertex_stride
                        );
                        CP->current_draw_command.vertices++;
                        CP->current_draw_command.arg_size += CP->current_draw_command.vertex_stride;
                    }
                }
            }
        }
        else {
            feed_CP(CP, buffer_ptr[i++]);
        }

        // FIFO_end is inclusive!
        if (write_ptr > FIFO_end) {
            write_ptr = FIFO_base;
        }
    }

    set_CP_reg(CP, CP_reg_FIFO_write_ptr_hi, CP_reg_FIFO_write_ptr_lo, write_ptr);
    set_CP_reg(CP, CP_reg_FIFO_read_ptr_hi, CP_reg_FIFO_read_ptr_lo, write_ptr);
}