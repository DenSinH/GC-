#ifndef GC__GX_FIFO_H
#define GC__GX_FIFO_H

#include "default.h"
#include "hwreg_utils.h"

typedef struct s_GX_FIFO {
    u8 buffer[8];
    HW_REG_WRITE_CALLBACK((*write), GX_FIFO);
    struct s_GameCube* system;
} s_GX_FIFO;


HW_REG_INIT_FUNCTION(GX_FIFO);

#endif //GC__GX_FIFO_H
