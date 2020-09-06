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

typedef union s_VCD_lo {
    struct {
        unsigned PMIDX: 1;
        unsigned T0MIDX: 1;
        unsigned T1MIDX: 1;
        unsigned T2MIDX: 1;
        unsigned T3MIDX: 1;
        unsigned T4MIDX: 1;
        unsigned T5MIDX: 1;
        unsigned T6MIDX: 1;
        unsigned T7MIDX: 1;
        unsigned POS: 2;
        unsigned NRM: 2;
        unsigned COL0: 2;
        unsigned COL1: 2;
        unsigned: 15;
    };

    struct {
        unsigned xxmidx: 9;
        unsigned: 23;
    };

    u32 raw;
} s_VCD_lo;

typedef union s_VCD_hi {
    struct {
        unsigned TEX0: 2;
        unsigned TEX1: 2;
        unsigned TEX2: 2;
        unsigned TEX3: 2;
        unsigned TEX4: 2;
        unsigned TEX5: 2;
        unsigned TEX6: 2;
        unsigned TEX7: 2;
        unsigned: 16;
    };

    struct {
        unsigned tex_coords: 16;
        unsigned: 16;
    };

    u32 raw;
} s_VCD_hi;

typedef union s_VAT_A {
    struct {
        unsigned POSCNT: 1;
        unsigned POSFMT: 3;
        unsigned POSSHFT: 5;
        unsigned NRMCNT: 1;
        unsigned NRMFMT: 3;
        unsigned COL0CNT: 1;
        unsigned COL0FMT: 3;
        unsigned COL1CNT: 1;
        unsigned COL1FMT: 3;
        unsigned TEX0CNT: 1;
        unsigned TEX0FMT: 3;
        unsigned TEX0SHFT: 5;
        unsigned BYTEDEQUANT: 1;
        unsigned NORMALINDEX3: 1;
    };
    u32 raw;
} s_VAT_A;

typedef union s_VAT_B {
    struct {
        unsigned TEX1CNT: 1;
        unsigned TEX1FMT: 3;
        unsigned TEX1SHFT: 5;
        unsigned TEX2CNT: 1;
        unsigned TEX2FMT: 3;
        unsigned TEX2SHFT: 5;
        unsigned TEX3CNT: 1;
        unsigned TEX3FMT: 3;
        unsigned TEX3SHFT: 5;
        unsigned TEX4CNT: 1;
        unsigned TEX4FMT: 3;
        unsigned VCACHE_ENHANCE: 1;  // always 1
    };
    u32 raw;
} s_VAT_B;

typedef union s_VAT_C {
    struct {
        unsigned TEX4SHFT: 5;
        unsigned TEX5CNT: 1;
        unsigned TEX5FMT: 3;
        unsigned TEX5SHFT: 5;
        unsigned TEX6CNT: 1;
        unsigned TEX6FMT: 3;
        unsigned TEX6SHFT: 5;
        unsigned TEX7CNT: 1;
        unsigned TEX7FMT: 3;
        unsigned TEX7SHFT: 5;
    };
    u32 raw;
} s_VAT_C;


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

#define VERTEX_ATTRIBUTE_ARGC(attribute, lut, cnt, fmt) if (attribute) { if (attribute != 1) { argc += attribute - 1; } else { argc += lut[cnt][fmt]; } }

void update_CP_draw_argc(s_CP* CP, int index) {
    s_VCD_lo VCD_lo = { .raw = CP->internalCPregs[0x50 + index - INTERNAL_CP_REGISTER_BASE] };
    s_VCD_hi VCD_hi = { .raw = CP->internalCPregs[0x60 + index - INTERNAL_CP_REGISTER_BASE] };
    s_VAT_A VAT_A = { .raw = CP->internalCPregs[0x70 + index - INTERNAL_CP_REGISTER_BASE] };
    s_VAT_B VAT_B = { .raw = CP->internalCPregs[0x80 + index - INTERNAL_CP_REGISTER_BASE] };
    s_VAT_C VAT_C = { .raw = CP->internalCPregs[0x90 + index - INTERNAL_CP_REGISTER_BASE] };

    u8 argc = popcount(VCD_lo.xxmidx);  // 1 byte per xxmidx variable (always indexed)
    VERTEX_ATTRIBUTE_ARGC(VCD_lo.POS, coord_argc, VAT_A.POSCNT, VAT_A.POSFMT)
    VERTEX_ATTRIBUTE_ARGC(VCD_lo.NRM, norm_argc, VAT_A.NRMCNT, VAT_A.NRMFMT)
    VERTEX_ATTRIBUTE_ARGC(VCD_lo.COL0, color_argc, VAT_A.COL0CNT, VAT_A.COL0FMT)
    VERTEX_ATTRIBUTE_ARGC(VCD_lo.COL1, color_argc, VAT_A.COL1CNT, VAT_A.COL1FMT)

    VERTEX_ATTRIBUTE_ARGC(VCD_hi.TEX0, color_argc, VAT_A.TEX0CNT, VAT_A.TEX0FMT)
    VERTEX_ATTRIBUTE_ARGC(VCD_hi.TEX1, color_argc, VAT_B.TEX1CNT, VAT_B.TEX1FMT)
    VERTEX_ATTRIBUTE_ARGC(VCD_hi.TEX2, color_argc, VAT_B.TEX2CNT, VAT_B.TEX2FMT)
    VERTEX_ATTRIBUTE_ARGC(VCD_hi.TEX3, color_argc, VAT_B.TEX3CNT, VAT_B.TEX3FMT)
    VERTEX_ATTRIBUTE_ARGC(VCD_hi.TEX4, color_argc, VAT_B.TEX4CNT, VAT_B.TEX4FMT)
    VERTEX_ATTRIBUTE_ARGC(VCD_hi.TEX5, color_argc, VAT_C.TEX5CNT, VAT_C.TEX5FMT)
    VERTEX_ATTRIBUTE_ARGC(VCD_hi.TEX6, color_argc, VAT_C.TEX6CNT, VAT_C.TEX6FMT)
    VERTEX_ATTRIBUTE_ARGC(VCD_hi.TEX7, color_argc, VAT_C.TEX7CNT, VAT_C.TEX7FMT)

    CP->draw_argc[index] = argc;
    CP->draw_argc_valid[index] = true;
    log_cp("Expecting %d bytes per vertex", argc);
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
    CP->args[CP->argc++] = data;
    switch (CP->command) {
        case CP_cmd_NOP:
            log_fatal("Uncaught NOP in argument feeding");
        case CP_cmd_load_CP_reg:
            if (CP->argc == 5) {
                // 8bit RID + 32 bit value
                load_CP_reg(CP, CP->args[0], READ32(CP->args, 1));
                CP->fetching = false;
            }
            break;
        case CP_cmd_load_XF_reg:
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
            if (CP->argc == 4) {
                // 16bit (index value) + 4bit (length - 1) + 12bit (1st address)
                u16 length_and_base_address = READ16(CP->args, 2);
                load_INDX(CP, CP->command, READ16(CP->args, 0), length_and_base_address >> 12, length_and_base_address & 0x0fff);
                CP->fetching = false;
            }
            break;
        case CP_cmd_call_DL:
            if (CP->argc == 8) {
                // 32bit (list addr) + 32bit (list size)
                call_DL(CP, READ32(CP->args, 0), READ32(CP->args, 4));
                CP->fetching = false;
            }
            break;
        case CP_cmd_inval_vertex_cache:
            log_fatal("Uncaught invalidate vertex cache instruction in argument feeding");
        case CP_cmd_load_BP_reg:
            if (CP->argc == 4) {
                // 8bit (reg addr) + 24bit (value)
                load_BP_reg(CP, READ32(CP->args, 0));
                CP->fetching = false;
            }
            break;
        case CP_cmd_QUADS ... CP_cmd_MAX:
            // all drawing commands
            if (CP->argc >= 2 && CP->argc == 2 + (READ16(CP->args, 0) * CP->draw_argc[CP->command & 7])) {
                // todo: actually draw primitive
                log_cp("Draw primitive (%02x)", CP->command);
                CP->fetching = false;
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