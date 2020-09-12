#ifndef GC__COMMANDPROCESSOR_H
#define GC__COMMANDPROCESSOR_H

#include "default.h"
#include <stdbool.h>
#include "hwreg_utils.h"
#include "core_utils.h"
#include "../Flipper/shaders/GX_constants.h"

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
    CP_reg_int_MATIDX_REG_A = 0x30,
    CP_reg_int_MATIDX_REG_B = 0x40,
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

/* The Math:
 *
 * For a draw command, we have at most 21 arguments:
 *  - 9 pos/tex matrix indices, at most 1 byte each
 *  - 1 positions arg, at most 12 bytes
 *  - 1 normals arg, at most 36 bytes
 *  - 2 color args, each at most 4 bytes
 *  - 8 texture coordinate args, each at most 8 bytes
 *
 *  so in total: at most 69 bytes per vertex (nice)
 *
 *  We can set a limit for small/medium/large commands to save unnecessary copying to the GPU of argument buffer data
 *  Say we set the limit for small commands to 0x40 vertices, we get a buffer size of (69 * 0x40 = ) 0x1140 bytes
 *  medium we could say 0x100 vertices, giving a buffer of size (69 * 0x100 = ) 0x4500 bytes
 *  large we could way anything else, but we should probably limit it to like 0x400 vertices or whatever, giving us
 *  0x18000 bytes
 *
 *  For the data, we can do another calculation. Indexing can happen with either int8s or int16s. In case of an int8,
 *  I will not even bother to find the min/max indices in the data, and just copy the whole possible array space for
 *  the vectors. Since for now I am not concerning myself with normals yet, this would limit me to 12 * 0x100 bytes per
 *  argument at most.
 *
 *  If indexing happens through int16s, I am going to check what the min and max indices are, and buffer that data only,
 *  This can give quite a bit more data.
 *
 *  Fortunately, stuff like matrices are rather small, and I might not even buffer that. This would mean I probably want
 *  to buffer the (texture) coordinates, colors and normals. For now, I'd want a buffer of size at most
 *  12 * 12 * (max data instances), which is way too much. In the shader I will leave the data array shapeless.
 *  So a buffer of 512kb should be more then plenty
 *
 *  I will also only keep track of the offsets for the arguments into the data array for only the
 *  non-matrix index arguments
 * */

#define DRAW_COMMAND_ARG_BUFFER_SIZE_SMALL 0x1140
#define DRAW_COMMAND_ARG_BUFFER_SIZE_MED 0x4500
#define DRAW_COMMAND_ARG_BUFFER_SIZE_LARGE 0x18000

#define DRAW_COMMAND_DATA_BUFFER_SIZE 0x80000

typedef struct s_draw_command_small {
    u32 vertices;              // number of vertices
    u32 command;               // actual command
    u32 vertex_stride;         // stride for one whole vertex
    i32 arg_offset[21];        // strides for arguments into the args array
    i32 data_offset[12];       // might be negative for correcting for min index
    u32 data_stride[12];       // ARRAY_STRIDE registers
    u32 data_size;             // to save space copying it to the GPU, this is variable in the shader anyways
    u8 args[DRAW_COMMAND_ARG_BUFFER_SIZE_SMALL];
    u8 data[DRAW_COMMAND_DATA_BUFFER_SIZE];
} s_draw_command_small;

#define INTERNAL_CP_REGISTER_SIZE 0xa0
#define INTERNAL_CP_REGISTER_BASE 0x20
#define INTERNAL_BP_REGISTER_SIZE 0x100
#define INTERNAL_XF_REGISTER_SIZE 0x58
#define INTERNAL_XF_MEM_SIZE 0x100
#define INTERNAL_XF_REGISTER_BASE 0x1000
#define MAX_DRAW_COMMANDS 16

#define current_draw_command draw_command[CP->draw_command_index]

typedef struct s_CP {
    // external function
    u8 regs[0x80];
    HW_REG_WRITE_CALLBACK((*write[0x40]), CP);
    HW_REG_READ_PRECALL((*read[0x40]), CP);
    struct s_GameCube* system;

    /* internal function */
    u32 internalCPregs[INTERNAL_CP_REGISTER_SIZE];
    u32 internalBPregs[INTERNAL_BP_REGISTER_SIZE];
    // ORDER IS IMPORTANT FOR THESE 2 FIELDS (passed to GPU)
    u32 internalXFmem[4][INTERNAL_XF_MEM_SIZE];    // 4 regions, A, B, C, D
    u32 internalXFregs[INTERNAL_XF_REGISTER_SIZE];
    /*
     * Region A: position matrix memory (0x100 words)
     * Region B: normal matrix memory (0x20 * 3 words)
     * Region C: dual texture transform matrices (64 * 4 words)
     * Region D: light memory (only 20 msbits for each word, minimum 3 word write, 0x80 words)
     * */

    e_CP_cmd command;
    bool fetching;  // true: needs args; false: needs command;

    // put arguments in a buffer, also in serial
    u8 args[32 * 32];  // length for normal commands is limited to 16 (times 32 bits), so we need more than this
    u32 argc;

    // todo: draw_command_mid, large
    u8 arg_size[21]; // sizes of individual (direct) arguments of current draw command
    s_draw_command_small draw_command[MAX_DRAW_COMMANDS];
    u32 draw_command_index;
    volatile bool draw_command_available[MAX_DRAW_COMMANDS];  // used as flags, set to false by CP, then to true by Flipper
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
