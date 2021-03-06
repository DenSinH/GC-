#ifndef GC__COMMANDPROCESSOR_H
#define GC__COMMANDPROCESSOR_H

#include "ProcessorInterface.h"
#include "VideoInterface.h"
#include "PixelEngine.h"

#include "../Scheduler/scheduler.h"

#include "default.h"
#include "hwreg_utils.h"
#include "core_utils.h"
#include "custom_threading.h"
#include "../Flipper/shaders/GX_constants.h"


#include <stdbool.h>

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

typedef enum e_BP_regs_internal {
    BP_reg_int_PE_DONE               = 0x45,
    BP_reg_int_EFB_dimensions        = 0x4a,
    BP_reg_int_XFB_address           = 0x4b,
    BP_reg_int_PE_copy_clear_AR      = 0x4f,
    BP_reg_int_PE_copy_clear_GB      = 0x50,
    BP_reg_int_PE_copy_execute       = 0x52,
    BP_reg_int_TEX_SET_IMAGE0_l_BASE = 0x88,
    BP_reg_int_TEX_SET_IMAGE3_l_BASE = 0x94,
} e_BP_regs_internal;

typedef enum e_PE_copy {
    PE_copy_clear = 0x0800,
    PE_copy_execute = 0x4000
} e_PE_copy;

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

typedef union s_SETIMAGE0_I {
    struct {
        unsigned width: 10;
        unsigned height: 10;
        unsigned format: 4;
        unsigned RID: 8;
    };
    u32 raw;
} s_SETIMAGE0_I;

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
 *
 *  THE VALUES I TALKED ABOUT HERE ARE DEFINED IN Flipper/shaders/GX_constants.h,
 *  I need them in the shaders, but it is most relevant in the CommandProcessor context
 * */

typedef struct s_draw_command {
    u32 vertex_stride;         // stride for one whole vertex
    i32 arg_offset[21];        // strides for arguments into the args array
    i32 data_offset[21];       // might be negative for correcting for min index
    u32 data_stride[21];       // ARRAY_STRIDE registers
    u8 args[DRAW_COMMAND_ARG_BUFFER_SIZE];
    u8 data[DRAW_COMMAND_DATA_BUFFER_SIZE];

    /* data unused in shader: */
    u16 vertices;              // number of vertices
    e_CP_cmd command;          // actual command
    u32 arg_size;              // to merge draw commands, this variable is in here
    u32 data_size;             // to save space copying it to the GPU, this is variable in the shader anyways
} s_draw_command;

#define INTERNAL_CP_REGISTER_SIZE 0xa0
#define INTERNAL_CP_REGISTER_BASE 0x20
#define INTERNAL_BP_REGISTER_SIZE 0x100
#define INTERNAL_XF_REGISTER_SIZE 0x58
#define INTERNAL_XF_MEM_SIZE 0x100
#define INTERNAL_XF_REGISTER_BASE 0x1000

typedef struct s_CP_register_data {
    u32 CP_regs[INTERNAL_CP_REGISTER_SIZE];
    u32 BP_regs[INTERNAL_BP_REGISTER_SIZE];
    // ORDER IS IMPORTANT FOR THESE 2 FIELDS (passed to GPU)
    u32 XF_mem[4][INTERNAL_XF_MEM_SIZE];    // 4 regions, A, B, C, D
    u32 XF_regs[INTERNAL_XF_REGISTER_SIZE];
    /*
     * Region A: position matrix memory (0x100 words)
     * Region B: normal matrix memory (0x20 * 3 words)
     * Region C: dual texture transform matrices (64 * 4 words)
     * Region D: light memory (only 20 msbits for each word, minimum 3 word write, 0x80 words)
     * */
} s_CP_register_data;

typedef struct s_texture_data {
    u32 data_size;
    u8 data[DRAW_COMMAND_TEXTURE_BUFFER_SIZE];
} s_texture_data;

#define MAX_DRAW_COMMANDS 128

#define CP_SHIFT 1
#define current_draw_command draw_command_data[CP->draw_command_index]
#define current_register_data register_data[CP->draw_command_index]
#define current_texture_data texture_data[CP->draw_command_index]

typedef struct s_CP {
    // external function
    // todo: is this actually 0x80? does not seem right (0x40 instead?)
    u8 regs[0x80];
    HW_REG_WRITE_CALLBACK((*write[0x80 >> CP_SHIFT]), CP);
    HW_REG_READ_PRECALL((*read[0x80 >> CP_SHIFT]), CP);
    struct s_GameCube* system;

    /* internal function */
    s_PI* PI;
    s_PE* PE;
    s_VI* VI;

    s_CP_register_data internal_registers;

    e_CP_cmd command;
    u16 current_vertices;  // amount of vertices for the current draw command being sent (unmerged)
    /* prev is set to current command whenever a new command is sent
     * if any relevant registers change, set it to 0
     * whenever a new command is sent, if it is the same as the previous one,
     * and enough space is left in the buffers
     * keep using that command to add new vertex data
     *
     * whenever a frameswap is triggered, also set it to 0
     *
     * Doing this prevents waiting on free draw command spots whenever draw commands can be merged
     * */
    e_CP_cmd prev;
    bool fetching;  // true: needs args; false: needs command;

    // put arguments in a buffer, also in serial
    u8 args[32 * 32];  // length for normal commands is limited to 16 (times 32 bits), so we need more than this
    u32 argc;

    u8 arg_size[21]; // sizes of individual (direct) arguments of current draw command
    s_draw_command draw_command_data[MAX_DRAW_COMMANDS];
    s_CP_register_data register_data[MAX_DRAW_COMMANDS];
    s_texture_data texture_data[MAX_DRAW_COMMANDS];

    mutex availability_lock, draw_lock, efb_lock;
    wait_event draw_commands_ready, draw_command_spot_available;

    // these values are also read/set by flipper
    volatile u32 draw_command_index;  // read to make sure flipper does not catch up

    // set to signal that flipper should frameswap after the draw command at a specific index
    volatile u16 frameswap[MAX_DRAW_COMMANDS];  // read by flipper, set to BP value by CP then clear bit 14 by Flipper
    bool PE_copy_frameswap;                     // set if a frameswap command was sent from an xfb -> efb copy command this frame
    volatile bool force_frameswap;              // force frameswap if no PE_copy_frameswap happened
    s_event frameswap_event;
    volatile bool draw_command_available[MAX_DRAW_COMMANDS];  // used as flags, set to false by CP, then to true by Flipper
} s_CP;


HW_REG_INIT_FUNCTION(CP);

void destroy_CP(s_CP* CP);
void execute_buffer(s_CP* CP, const u8* buffer_ptr, u8 buffer_size);

static inline u32 get_CP_reg(s_CP* CP, e_CP_regs reg_hi, e_CP_regs reg_lo) {
    return (READ16(CP->regs, reg_hi) << 16) | READ16(CP->regs, reg_lo);
}

static inline u32 get_internal_CP_reg(s_CP* CP, e_CP_regs_internal reg) {
    return CP->internal_registers.CP_regs[reg - INTERNAL_CP_REGISTER_BASE];
}

static inline void set_CP_reg(s_CP* CP, e_CP_regs reg_hi, e_CP_regs reg_lo, u32 value) {
    WRITE16(CP->regs, reg_hi, value >> 16);
    WRITE16(CP->regs, reg_lo, value & 0xffff);
}

#endif //GC__COMMANDPROCESSOR_H
