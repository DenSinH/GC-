#ifndef GC__GX_CONSTANTS_H
#define GC__GX_CONSTANTS_H

typedef enum e_VA_locations {
    POS_2D_U8  = 0,
    POS_2D_S8  = 1,
    POS_2D_U16 = 2,
    POS_2D_S16 = 3,
    POS_2D_F32 = 4,

    POS_3D_U8  = 5,
    POS_3D_S8  = 6,
    POS_3D_U16 = 7,
    POS_3D_S16 = 8,
    POS_3D_F32 = 9,

    CLR0_rgb565 = 10,
    CLR0_rgb888 = 11,
    CLR0_rgb888x = 12,
    CLR0_rgba4444 = 13,
    CLR0_rgba6666 = 14,
    CLR0_rgba8888 = 15
} e_VA_locations;

#endif //GC__GX_CONSTANTS_H
