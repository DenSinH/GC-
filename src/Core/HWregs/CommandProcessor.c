#include "CommandProcessor.h"
#include "core_utils.h"
#include "log.h"
#include "flags.h"
#include "helpers.h"

#ifdef CHECK_CP_COMMAND
#include <assert.h>
#endif


HW_REG_INIT_FUNCTION(CP) {
    for (int i = 0; i < 8; i++) {
        CP->draw_argc_valid[i] = true;
    }
}

u32 get_CP_reg(s_CP* CP, e_CP_regs reg_hi, e_CP_regs reg_lo) {
    return (READ16(CP->regs, reg_hi) << 16) | READ16(CP->regs, reg_lo);
}

void set_CP_reg(s_CP* CP, e_CP_regs reg_hi, e_CP_regs reg_lo, u32 value) {
    WRITE16(CP->regs, reg_hi, value >> 16);
    WRITE16(CP->regs, reg_lo, value & 0xffff);
}

// indexed by VAT.POSCNT, VAT.POSFMT
static const u8 coord_argc[2][8] = {
        // POSCNT = 0
        { 3, 3, 6, 6, 12, 0, 0, 0 },
        // POSCNT = 1
        { 4, 4, 8, 8, 16, 0, 0, 0 }
};

// indexed by VAT.NRMCNT, VAT.NRMFMT
static const u8 norm_argc[2][8] = {
        // POSCNT = 0
        { 0, 3, 0, 6, 12, 0, 0, 0 },
        // POSCNT = 1
        { 0, 9, 0, 18, 36, 0, 0, 0 }
};

// indexed by VAT.COLxCNT, VAT.COLxFMT (poscnt actually does not impact this)
static const u8 color_argc[2][8] = {
        // POSCNT = 0
        { 2, 3, 4, 2, 3, 4, 0, 0 },
        // POSCNT = 1
        { 2, 3, 4, 2, 3, 4, 0, 0 }
};

// indexed by VAT.TEXxCNT, VAT.TEXxFMT
static const u8 tex_argc[2][8] = {
        // POSCNT = 0
        { 1, 1, 2, 2, 4, 0, 0, 0 },
        // POSCNT = 1
        { 2, 2, 4, 4, 8, 0, 0, 0 }
};

#define VERTEX_ATTRIBUTE_ARGC(VCD_attr, draw_arg, lut, cnt, fmt) \
if (VCD_attr) { \
    CP->draw_arg_index[format][used_arg_index] = draw_arg; \
    if ((VCD_attr) != 1) { \
        CP->draw_arg_len[format][used_arg_index++] = (VCD_attr) - 1; \
    } \
    else { \
        CP->draw_arg_len[format][used_arg_index++] = lut[cnt][fmt]; \
    } \
} \

void update_CP_draw_argc(s_CP* CP, int format) {
    s_VCD_lo VCD_lo = { .raw = CP->internalCPregs[0x50 + format - INTERNAL_CP_REGISTER_BASE] };
    s_VCD_hi VCD_hi = { .raw = CP->internalCPregs[0x60 + format - INTERNAL_CP_REGISTER_BASE] };
    s_VAT_A VAT_A = { .raw = CP->internalCPregs[0x70 + format - INTERNAL_CP_REGISTER_BASE] };
    s_VAT_B VAT_B = { .raw = CP->internalCPregs[0x80 + format - INTERNAL_CP_REGISTER_BASE] };
    s_VAT_C VAT_C = { .raw = CP->internalCPregs[0x90 + format - INTERNAL_CP_REGISTER_BASE] };

    int used_arg_index = 0;
    for (int arg_index = 0; arg_index <= draw_arg_TEX7MTXIDX; arg_index++) {
        if (VCD_lo.raw & (1 << arg_index)) {
            // these parameters are of size 1 if present
            CP->draw_arg_index[format][used_arg_index] = arg_index;
            CP->draw_arg_len[format][used_arg_index++] = 1;
        }
    }

    VERTEX_ATTRIBUTE_ARGC(VCD_lo.POS, draw_arg_POS, coord_argc, VAT_A.POSCNT, VAT_A.POSFMT)
    VERTEX_ATTRIBUTE_ARGC(VCD_lo.NRM, draw_arg_NRM, norm_argc, VAT_A.NRMCNT, VAT_A.NRMFMT)
    VERTEX_ATTRIBUTE_ARGC(VCD_lo.COL0, draw_arg_CLR0, color_argc, VAT_A.COL0CNT, VAT_A.COL0FMT)
    VERTEX_ATTRIBUTE_ARGC(VCD_lo.COL1, draw_arg_CLR1, color_argc, VAT_A.COL1CNT, VAT_A.COL1FMT)

    VERTEX_ATTRIBUTE_ARGC(VCD_hi.TEX0, draw_arg_TEX0, color_argc, VAT_A.TEX0CNT, VAT_A.TEX0FMT)
    VERTEX_ATTRIBUTE_ARGC(VCD_hi.TEX1, draw_arg_TEX1, color_argc, VAT_B.TEX1CNT, VAT_B.TEX1FMT)
    VERTEX_ATTRIBUTE_ARGC(VCD_hi.TEX2, draw_arg_TEX2, color_argc, VAT_B.TEX2CNT, VAT_B.TEX2FMT)
    VERTEX_ATTRIBUTE_ARGC(VCD_hi.TEX3, draw_arg_TEX3, color_argc, VAT_B.TEX3CNT, VAT_B.TEX3FMT)
    VERTEX_ATTRIBUTE_ARGC(VCD_hi.TEX4, draw_arg_TEX4, color_argc, VAT_B.TEX4CNT, VAT_B.TEX4FMT)
    VERTEX_ATTRIBUTE_ARGC(VCD_hi.TEX5, draw_arg_TEX5, color_argc, VAT_C.TEX5CNT, VAT_C.TEX5FMT)
    VERTEX_ATTRIBUTE_ARGC(VCD_hi.TEX6, draw_arg_TEX6, color_argc, VAT_C.TEX6CNT, VAT_C.TEX6FMT)
    VERTEX_ATTRIBUTE_ARGC(VCD_hi.TEX7, draw_arg_TEX7, color_argc, VAT_C.TEX7CNT, VAT_C.TEX7FMT)

    CP->draw_arg_index[format][used_arg_index] = draw_arg_UNUSED;
    CP->draw_argc_valid[format] = true;
    for (int i = 0; i < used_arg_index; i++) {
        log_cp("Expecting %i bytes for argument %d", CP->draw_arg_len[format][i], CP->draw_arg_index[format][i]);
    }
}

inline void load_CP_reg(s_CP* CP, u8 RID, u32 value) {
    log_cp("Load CP register %02x with %08x", RID, value);
#ifdef CHECK_CP_COMMAND
    assert(RID - INTERNAL_CP_REGISTER_BASE < INTERNAL_CP_REGISTER_SIZE  /* invalid RID for CP command */);
#endif
    CP->internalCPregs[RID - INTERNAL_CP_REGISTER_BASE] = value;
    if (RID >= 0x50 && RID < 0x98) {
        // invalidate argc because of VAT/VCD update
        CP->draw_argc_valid[RID & 7] = false;
    }
}

inline void load_XF_regs(s_CP* CP, u16 length, u16 base_addr, const u8* values_buffer) {
    log_cp("Load %d XF registers starting at %04x (first value %08x)", length, base_addr, READ32(values_buffer, 0));

#ifdef CHECK_CP_COMMAND
    assert(base_addr - INTERNAL_XF_REGISTER_BASE + length <= INTERNAL_XF_REGISTER_SIZE  /* invalid RID for CP command */);
#endif

    for (int i = 0; i < length; i++) {
        CP->internalXFregs[base_addr + i - INTERNAL_XF_REGISTER_BASE] = READ32(values_buffer, i << 2);
    }
}

inline void load_INDX(s_CP* CP, e_CP_cmd opcode, u16 index, u8 length, u16 base_addr) {
    log_cp("Load %d XF indexed (%02x, index value %02x) starting at %04x", length, opcode, index, base_addr);
}

inline void call_DL(s_CP* CP, u32 list_addr, u32 list_size) {
    log_cp("Call DL at %08x of size %08x", list_addr, list_size);
}

inline void load_BP_reg(s_CP* CP, u32 value) {
    // value contains RID _and_ value here
    log_cp("Load BP reg: %08x", value);

    // RID is a byte, so it always fits into the range for the internal BP registers
    CP->internalBPregs[value >> 24] = value;

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
            log_cp("data %d", data)
            if (CP->argc >= 2) {
                // split the arguments over the different buffers
                size_t current_draw_arg = CP->argc - 2;
                size_t format = CP->command & 7;

                // buffer argument as LE in the next buffer
                CP->draw_arg_buffer[current_draw_arg][
                        (CP->vertex_count + 1) * CP->draw_arg_len[format][current_draw_arg] - (++CP->sub_argc)
                ] = data;

                // go to next argument
                if (CP->sub_argc == CP->draw_arg_len[format][current_draw_arg]) {
                    CP->sub_argc = 0;

                    // go to next vertex
                    if (CP->draw_arg_index[format][++CP->argc - 2] == draw_arg_UNUSED) {  // last argument signifier
                        CP->argc = 2;

                        // done
                        if (++CP->vertex_count == READ16(CP->args, 0)) {
                            log_cp("Drawing primitive %02x", CP->command);

                            for (int i = 0; CP->draw_arg_index[format][i] != draw_arg_UNUSED; i++) {
                                for (int v = 0; v < CP->vertex_count; v++) {
                                    log_cp("first byte for vertex %d, argument %d: %02x", v, i, CP->draw_arg_buffer[i][
                                            v * CP->draw_arg_len[format][i]
                                    ]);
                                }
                            }

                            CP->vertex_count = 0;
                            CP->fetching = false;
                        }
                    }
                }
            }
            else {
                CP->args[CP->argc++] = data;
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
            if (CP->command >= CP_cmd_QUADS && !CP->draw_argc_valid[CP->command & 7]) {
                update_CP_draw_argc(CP, CP->command & 7);
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