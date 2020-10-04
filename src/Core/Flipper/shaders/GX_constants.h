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
    XF_reg_project_ortho = 0x26,
} e_XF_regs;

typedef enum e_BP_regs {
    BP_reg_TX_SETIMAGE0_base_lo = 0x88,
    BP_reg_TX_SETIMAGE0_base_hi = 0xa8,
    BP_reg_TX_SETIMAGE1_base_lo = 0x8c,
    BP_reg_TX_SETIMAGE1_base_hi = 0xac,
    BP_reg_TX_SETIMAGE2_base_lo = 0x90,
    BP_reg_TX_SETIMAGE2_base_hi = 0xb0,
    BP_reg_TX_SETIMAGE3_base_lo = 0x94,
    BP_reg_TX_SETIMAGE3_base_hi = 0xb4,
} e_BP_regs;

typedef enum e_color_formats {
    color_format_I4     = 0,
    color_format_I8     = 1,
    color_format_IA4    = 2,
    color_format_IA8    = 3,
    color_format_RGB565 = 4,
    color_format_RGB5A3 = 5,
    color_format_RGBA8  = 6,
    color_format_C4     = 7,
    color_format_C8     = 8,
    color_format_C14X2  = 9,
    color_format_CMP    = 14
} e_color_formats;

typedef enum e_impossible_color {
    impossible_Y = 89,
    impossible_U = 164,
    impossible_V = 137
} e_impossible_color;

#endif //GC__GX_CONSTANTS_H
