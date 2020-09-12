#ifndef GC__GX_CONSTANTS_H
#define GC__GX_CONSTANTS_H

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

typedef enum e_XF_regs {
    XF_reg_projection_A = 0x20,
    XF_reg_projection_B = 0x21,
    XF_reg_projection_C = 0x22,
    XF_reg_projection_D = 0x23,
    XF_reg_projection_E = 0x24,
    XF_reg_projection_F = 0x25,
} e_XF_regs;

#endif //GC__GX_CONSTANTS_H
