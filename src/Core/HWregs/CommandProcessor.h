#ifndef GC__COMMANDPROCESSOR_H
#define GC__COMMANDPROCESSOR_H

#include "default.h"
#include <stdbool.h>
#include "hwreg_utils.h"

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
    u8 draw_argc[8];   // expected length of arguments (in bytes) for each of the formats that can be specified
    bool draw_argc_valid[8];  // keep track of whether we need to recalculate the value
    // initial values for draw_argc are 0, since when VCD == 0, nothing is enabled and no arguments will be sent
} s_CP;


HW_REG_INIT_FUNCTION(CP);
void execute_buffer(s_CP* CP, const u8* buffer_ptr, u8 buffer_size);

#endif //GC__COMMANDPROCESSOR_H
