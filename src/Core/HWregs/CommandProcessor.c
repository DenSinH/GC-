#include "CommandProcessor.h"
#include "PixelEngine.h"

#include "log.h"
#include "flags.h"
#include "helpers.h"
#include "../PPC/interrupts.h"
#include "../system.h"

#include <limits.h>

#ifdef CHECK_CP_COMMAND
#include <assert.h>
#endif


HW_REG_INIT_FUNCTION(CP) {
    for (int i = 0; i < MAX_DRAW_COMMANDS; i++) {
        CP->draw_command_available[i] = true;
    }

//    for (int i = 0; i < 8; i++) {
//        CP->draw_argc_valid[i] = true;
//    }
}

// indexed by VAT.POSCNT, VAT.POSFMT
static const u8 coord_stride[2][8] = {
        // POSCNT = 0
        { 3, 3, 6, 6, 12, 0, 0, 0 },
        // POSCNT = 1
        { 4, 4, 8, 8, 16, 0, 0, 0 }
};

// indexed by VAT.NRMCNT, VAT.NRMFMT
static const u8 nrm_stride[2][8] = {
        // POSCNT = 0
        { 0, 3, 0, 6, 12, 0, 0, 0 },
        // POSCNT = 1
        { 0, 9, 0, 18, 36, 0, 0, 0 }
};

// indexed by VAT.COLxCNT, VAT.COLxFMT (poscnt actually does not impact this)
static const u8 color_stride[2][8] = {
        // POSCNT = 0
        { 2, 3, 4, 2, 3, 4, 0, 0 },
        // POSCNT = 1
        { 2, 3, 4, 2, 3, 4, 0, 0 }
};

// indexed by VAT.TEXxCNT, VAT.TEXxFMT
static const u8 tex_stride[2][8] = {
        // POSCNT = 0
        { 1, 1, 2, 2, 4, 0, 0, 0 },
        // POSCNT = 1
        { 2, 2, 4, 4, 8, 0, 0, 0 }
};

#define ADD_DRAW_ARG(_mode, _arg, _lut, _cnt, _fmt) \
if (_mode) {                                        \
    log_cp("arg %d enabled, offset %d", _arg, offset)                                                \
    CP->current_draw_command.arg_offset[_arg] = offset; \
    offset += (CP->arg_size[_arg] = (((_mode) == 1) ? _lut[_cnt][_fmt] : (_mode) - 1)); \
} \
else {  \
    CP->current_draw_command.arg_offset[_arg] = -1; \
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

    ADD_DRAW_ARG(VCD_lo.POS, draw_arg_POS, coord_stride, VAT_A.POSCNT, VAT_A.POSFMT)
    ADD_DRAW_ARG(VCD_lo.NRM, draw_arg_NRM, nrm_stride, VAT_A.NRMCNT, VAT_A.NRMFMT)
    ADD_DRAW_ARG(VCD_lo.COL0, draw_arg_CLR0, color_stride, VAT_A.COL0CNT, VAT_A.COL0FMT)
    ADD_DRAW_ARG(VCD_lo.COL1, draw_arg_CLR1, color_stride, VAT_A.COL1CNT, VAT_A.COL1FMT)

    ADD_DRAW_ARG(VCD_hi.TEX0, draw_arg_TEX0, tex_stride, VAT_A.TEX0CNT, VAT_A.TEX0FMT)
    ADD_DRAW_ARG(VCD_hi.TEX1, draw_arg_TEX1, tex_stride, VAT_B.TEX1CNT, VAT_B.TEX1FMT)
    ADD_DRAW_ARG(VCD_hi.TEX2, draw_arg_TEX2, tex_stride, VAT_B.TEX2CNT, VAT_B.TEX2FMT)
    ADD_DRAW_ARG(VCD_hi.TEX3, draw_arg_TEX3, tex_stride, VAT_B.TEX3CNT, VAT_B.TEX3FMT)
    ADD_DRAW_ARG(VCD_hi.TEX4, draw_arg_TEX4, tex_stride, VAT_B.TEX4CNT, VAT_B.TEX4FMT)
    ADD_DRAW_ARG(VCD_hi.TEX5, draw_arg_TEX5, tex_stride, VAT_C.TEX5CNT, VAT_C.TEX5FMT)
    ADD_DRAW_ARG(VCD_hi.TEX6, draw_arg_TEX6, tex_stride, VAT_C.TEX6CNT, VAT_C.TEX6FMT)
    ADD_DRAW_ARG(VCD_hi.TEX7, draw_arg_TEX7, tex_stride, VAT_C.TEX7CNT, VAT_C.TEX7FMT)

    CP->current_draw_command.vertex_stride = offset;
//    CP->draw_argc_valid[format] = true;
    log_cp("Expecting %d bytes per vertex", offset);
//    for (int i = draw_arg_PNMTXIDX; i < draw_arg_TEX7; i++) {
//        log_cp("Argument %d: offset %d", i, CP->current_draw_command.arg_offset[i])
//    }
}

static inline void load_CP_reg(s_CP* CP, u8 RID, u32 value) {
    log_cp("Load CP register %02x with %08x", RID, value);
#ifdef CHECK_CP_COMMAND
    assert(RID - INTERNAL_CP_REGISTER_BASE < INTERNAL_CP_REGISTER_SIZE  /* invalid RID for CP command */);
#endif
    CP->internal_CP_regs[RID - INTERNAL_CP_REGISTER_BASE] = value;
//    if (RID >= 0x50 && RID < 0x98) {
//        // invalidate argc because of VAT/VCD update
//        CP->draw_argc_valid[RID & 7] = false;
//    }
}

const static u8 XF_addr_to_section[] = { 0, 0xff, 0xff, 0xff, 1, 2, 3, 0xff };

static inline void load_XF_regs(s_CP* CP, u16 length, u16 base_addr, const u8* values_buffer) {
    log_cp("Load %d XF registers starting at %04x (first value %08x)", length, base_addr, READ32(values_buffer, 0));

    if (base_addr > INTERNAL_XF_REGISTER_BASE) {
#ifdef CHECK_CP_COMMAND
        assert(base_addr - INTERNAL_XF_REGISTER_BASE + length <= INTERNAL_XF_REGISTER_SIZE  /* invalid RID for CP command */);
#endif
        for (int i = 0; i < length; i++) {
            CP->internal_XF_regs[base_addr + i - INTERNAL_XF_REGISTER_BASE] = READ32(values_buffer, i << 2);
        }
    }
    else {
        /*
         * base address is either 0 - 0xff / 0x400 - 0x4ff / 0x500 - 0x5ff / 0x600 - 0x6ff
         * so we check ((base_addr + 0x100) >> 8) & 3 for the section
         * */
        u8 section = XF_addr_to_section[base_addr >> 8];
        for (int i = 0; i < length; i++) {
            log_cp("loaded %08x", READ32(values_buffer, i << 2));
            CP->internal_XF_mem[section][(base_addr & 0xff) + i] = READ32(values_buffer, i << 2);
        }
    }
}

static inline void load_INDX(s_CP* CP, e_CP_cmd opcode, u16 index, u8 length, u16 base_addr) {
    log_fatal("Load %d XF indexed (%02x, index value %02x) starting at %04x", length, opcode, index, base_addr);
    // todo
}

static inline void send_draw_command(s_CP* CP) {
    // we still need to buffer the indirect data, we do that here
    u32 data_offset = 0;
    size_t stride, size;
    i16 min_index, max_index;
    for (e_draw_args draw_arg = draw_arg_POS; draw_arg <= draw_arg_TEX7; draw_arg++) {
        if (CP->current_draw_command.arg_offset[draw_arg] < 0) {
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
        CP->current_draw_command.data_offset[draw_arg - draw_arg_POS] = data_offset - min_index;
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
    }
    CP->current_draw_command.data_size = data_offset;

    memcpy(
            &CP->current_draw_command.data_stride,
            &CP->internal_CP_regs[CP_reg_int_vert_ARRAY_STRIDE - INTERNAL_CP_REGISTER_BASE],
            12 * sizeof(u32)
    );

    log_cp("Draw command %d sent", CP->draw_command_index);
    CP->draw_command_available[CP->draw_command_index++] = false;  // signify that draw command is used
    if (CP->draw_command_index == MAX_DRAW_COMMANDS) {
        CP->draw_command_index = 0;
    }
    // draw is automatically queued by setting draw_command_available to false
}

static inline void call_DL(s_CP* CP, u32 list_addr, u32 list_size) {
    log_fatal("Call DL at %08x of size %08x", list_addr, list_size);
    // we can basically just call the execute_buffer function but then on data in memory
}

static inline void load_BP_reg(s_CP* CP, u32 value) {
    // value contains RID _and_ value here
    log_cp("Load BP reg: %08x", value);

    // RID is a byte, so it always fits into the range for the internal BP registers
    u8 RID = value >> 24;
    CP->internal_BP_regs[RID] = value;

    if (RID == BP_reg_int_PE_DONE) {
        // check if bit 2 is set (likely) and if the PE finish interrupt is masked by PE
        if (CP->internal_BP_regs[BP_reg_int_PE_DONE] & 0x02 && GET_PE_REG(&CP->system->HW_regs.PE, PE_reg_interrupt_status) & 0x02) {
            // todo: should PE_reg_interrupt_status be set in this case even?

            // frame done
            SET_PE_REG(&CP->system->HW_regs.PE, PE_reg_token, (u16)value);

            // set interrupt to called
            SET_PE_REG(&CP->system->HW_regs.PE, PE_reg_interrupt_status, GET_PE_REG(&CP->system->HW_regs.PE, PE_reg_interrupt_status) | 0x08);

            // add interrupt cause to processor interface and call interrupt
            log_cp("PE_DONE interrupt!");
            add_PI_intsr(&CP->system->HW_regs.PI, PI_intr_PE_DONE);
        }
    }
}

inline void feed_CP(s_CP* CP, u8 data) {
    // feed from the buffer from some index
    // always read argument (we never get here with a NOP anyway)
    switch (CP->command) {
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
        case CP_cmd_load_BP_reg:
            CP->args[CP->argc++] = data;
            if (CP->argc == 4) {
                // 8bit (reg addr) + 24bit (value)
                load_BP_reg(CP, READ32(CP->args, 0));
                CP->fetching = false;
            }
            break;
        case CP_cmd_QUADS ... CP_cmd_MAX:
            // all drawing commands
            if (CP->argc == 0) {
                CP->argc++;
                CP->current_draw_command.vertices = data << 8;
            }
            else if (CP->argc == 1) {
                CP->current_draw_command.vertices |= data;
                CP->argc++;
            }
            else {
                CP->current_draw_command.args[CP->argc++ - 2] = data;  // first 2 bytes were number of vertices

                if ((CP->argc - 2) == CP->current_draw_command.vertex_stride * CP->current_draw_command.vertices) {
                    send_draw_command(CP);
                    CP->fetching = false;
                }
            }
            break;
        default:
            log_fatal("Invalid CP command: %08x", CP->command);
    }
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
            if (CP->command >= CP_cmd_QUADS /* && !CP->draw_argc_valid[CP->command & 7] */) {
                update_CP_draw_argc(CP, CP->command & 7);

                // check if current draw command in queue is available
                while (!CP->draw_command_available[CP->draw_command_index]) {
                    // todo: better wait until draw commands finished
                }
                log_cp("Got draw command spot");
                CP->current_draw_command.command = CP->command;
            }
        }
        else {
            feed_CP(CP, buffer_ptr[i++]);
        }

        // FIFO_end is inclusive!
        if (write_ptr > FIFO_end) {
            write_ptr = FIFO_end;
        }
    }

    set_CP_reg(CP, CP_reg_FIFO_write_ptr_hi, CP_reg_FIFO_write_ptr_lo, write_ptr);
    set_CP_reg(CP, CP_reg_FIFO_read_ptr_hi, CP_reg_FIFO_read_ptr_lo, write_ptr);
}