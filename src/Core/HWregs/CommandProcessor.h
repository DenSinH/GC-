#ifndef GC__COMMANDPROCESSOR_H
#define GC__COMMANDPROCESSOR_H

#include "default.h"
#include <stdbool.h>
#include "hwreg_utils.h"
#include "core_utils.h"

#define INTERNAL_CP_REGISTER_SIZE 0xa0
#define INTERNAL_CP_REGISTER_BASE 0x20
#define INTERNAL_BP_REGISTER_SIZE 0x100
#define INTERNAL_XF_REGISTER_SIZE 0x58
#define INTERNAL_XF_REGISTER_BASE 0x1000

typedef enum e_CP_regs {
    CP_reg_FIFO_base_lo = 0x20,
    CP_reg_FIFO_base_hi = 0x22,
    CP_reg_FIFO_end_lo = 0x24,
    CP_reg_FIFO_end_hi = 0x26,
    CP_reg_FIFO_hi_watermark_lo = 0x28,
    CP_reg_FIFO_hi_watermark_hi = 0x2a,
    CP_reg_FIFO_lo_watermark_lo = 0x2c,
    CP_reg_FIFO_lo_watermark_hi = 0x2e,
    CP_reg_FIFO_rw_distance_lo = 0x30,
    CP_reg_FIFO_rw_distance_hi = 0x32,
    CP_reg_FIFO_write_ptr_lo = 0x34,
    CP_reg_FIFO_write_ptr_hi = 0x36,
    CP_reg_FIFO_read_ptr_lo = 0x38,
    CP_reg_FIFO_read_ptr_hi = 0x3a,
    CP_reg_FIFO_bp_lo = 0x3c,
    CP_reg_FIFO_bp_hi = 0x3e,
} e_CP_regs;

typedef enum e_CP_regs_internal {
    CP_reg_int_VCD_lo_base = 0x50,
    CP_reg_int_VCD_hi_base = 0x60,
    CP_reg_int_VAT_A_base = 0x70,
    CP_reg_int_VAT_B_base = 0x80,
    CP_reg_int_VAT_C_base = 0x90,
    CP_reg_int_vert_ARRAY_BASE = 0xa0,
    CP_reg_int_nrm_ARRAY_BASE = 0xa1,
    CP_reg_int_clr0_ARRAY_BASE = 0xa2,
    CP_reg_int_clr1_ARRAY_BASE = 0xa3,

    CP_reg_int_tex0_ARRAY_BASE = 0xa3,
    CP_reg_int_tex1_ARRAY_BASE = 0xa4,
    CP_reg_int_tex2_ARRAY_BASE = 0xa5,
    CP_reg_int_tex3_ARRAY_BASE = 0xa6,
    CP_reg_int_tex4_ARRAY_BASE = 0xa7,
    CP_reg_int_tex5_ARRAY_BASE = 0xa8,
    CP_reg_int_tex6_ARRAY_BASE = 0xa9,
    CP_reg_int_tex7_ARRAY_BASE = 0xaa,

    CP_reg_int_vert_ARRAY_STRIDE = 0xb0,
    CP_reg_int_nrm_ARRAY_STRIDE  = 0xb1,
    CP_reg_int_clr0_ARRAY_STRIDE = 0xb2,
    CP_reg_int_clr1_ARRAY_STRIDE = 0xb3,

    CP_reg_int_tex0_ARRAY_STRIDE = 0xb3,
    CP_reg_int_tex1_ARRAY_STRIDE = 0xb4,
    CP_reg_int_tex2_ARRAY_STRIDE = 0xb5,
    CP_reg_int_tex3_ARRAY_STRIDE = 0xb6,
    CP_reg_int_tex4_ARRAY_STRIDE = 0xb7,
    CP_reg_int_tex5_ARRAY_STRIDE = 0xb8,
    CP_reg_int_tex6_ARRAY_STRIDE = 0xb9,
    CP_reg_int_tex7_ARRAY_STRIDE = 0xba,
} e_CP_regs_internal;

// YAGCD: draw commands also take an argument in the bottom 3 bits
typedef enum e_CP_cmd {
    CP_cmd_NOP = 0x00,
    CP_cmd_load_CP_reg = 0x08,
    CP_cmd_load_XF_reg = 0x10,
    CP_cmd_load_INDX_A = 0x20,
    CP_cmd_load_INDX_B = 0x28,
    CP_cmd_load_INDX_C = 0x30,
    CP_cmd_load_INDX_D = 0x38,
    CP_cmd_call_DL = 0x40,
    CP_cmd_inval_vertex_cache = 0x48,
    CP_cmd_load_BP_reg = 0x61,
    /* DRAW COMMANDS: */
    CP_cmd_QUADS = 0x80,
    CP_cmd_TRIANGLES = 0x90,
    CP_cmd_TRIANGLESTRIP = 0x98,
    CP_cmd_TRIANGLEFAN = 0xa0,
    CP_cmd_LINES = 0xa8,
    CP_cmd_LINESTRIP = 0xb0,
    CP_cmd_POINTS = 0xb8,
    CP_cmd_MAX = 0xbf   // used for the switch case
} e_CP_cmd;

typedef enum e_draw_args {
    draw_arg_PNMTXIDX = 0,
    draw_arg_TEX0MTXIDX = 1,
    draw_arg_TEX1MTXIDX = 2,
    draw_arg_TEX2MTXIDX = 3,
    draw_arg_TEX3MTXIDX = 4,
    draw_arg_TEX4MTXIDX = 5,
    draw_arg_TEX5MTXIDX = 6,
    draw_arg_TEX6MTXIDX = 7,
    draw_arg_TEX7MTXIDX = 8,
    draw_arg_POS = 9,
    draw_arg_NRM = 10,
    draw_arg_CLR0 = 11,
    draw_arg_CLR1 = 12,
    draw_arg_TEX0 = 13,
    draw_arg_TEX1 = 14,
    draw_arg_TEX2 = 15,
    draw_arg_TEX3 = 16,
    draw_arg_TEX4 = 17,
    draw_arg_TEX5 = 18,
    draw_arg_TEX6 = 19,
    draw_arg_TEX7 = 20,
    draw_arg_UNUSED = 0xff
} e_draw_args;


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

typedef struct s_draw_arg {
    e_draw_args arg;
    bool direct;
    u8 direct_stride;
    void* direct_buffer;
} s_draw_arg;


typedef struct s_CP {
    // external function
    u8 regs[0x80];
    HW_REG_WRITE_CALLBACK((*write[0x40]), CP);
    HW_REG_READ_PRECALL((*read[0x40]), CP);
    struct s_GameCube* system;

    /* internal function */
    u32 internalCPregs[INTERNAL_CP_REGISTER_SIZE];
    u32 internalBPregs[INTERNAL_BP_REGISTER_SIZE];
    u32 internalXFregs[INTERNAL_XF_REGISTER_SIZE];

    e_CP_cmd command;
    bool fetching;  // true: needs args; false: needs command;

    // put arguments in a buffer, also in serial
    u8 args[32 * 32];  // length for normal commands is limited to 16 (times 32 bits), so we need more than this
    u8 argc;

    u16 vertex_count;
    u8 sub_argc;
    u8 draw_arg_buffer[0x400][21]; // buffers for each of the arguments specified; size is just an arbitrary (large) value
    s_draw_arg draw_args[21][8]; // index of draw_arg in the argument/length buffer for at most 21 argument types
    bool draw_argc_valid[8];  // keep track of whether we need to recalculate the value for arg_len
    // initial values for draw_argc are 0, since when VCD == 0, nothing is enabled and no arguments will be sent
} s_CP;


HW_REG_INIT_FUNCTION(CP);
void execute_buffer(s_CP* CP, const u8* buffer_ptr, u8 buffer_size);

static inline u32 get_CP_reg(s_CP* CP, e_CP_regs reg_hi, e_CP_regs reg_lo) {
    return (READ16(CP->regs, reg_hi) << 16) | READ16(CP->regs, reg_lo);
}

static inline u32 get_internal_CP_reg(s_CP* CP, e_CP_regs_internal reg) {
    return CP->internalCPregs[reg - INTERNAL_CP_REGISTER_BASE];
}

static inline void set_CP_reg(s_CP* CP, e_CP_regs reg_hi, e_CP_regs reg_lo, u32 value) {
    WRITE16(CP->regs, reg_hi, value >> 16);
    WRITE16(CP->regs, reg_lo, value & 0xffff);
}

#endif //GC__COMMANDPROCESSOR_H
