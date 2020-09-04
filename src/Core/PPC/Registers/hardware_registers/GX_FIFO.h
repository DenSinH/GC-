#ifndef GC__GX_FIFO_H
#define GC__GX_FIFO_H

#include "CommandProcessor.h"

#include "default.h"
#include "hwreg_utils.h"

// we only really need 39 bytes (most that the 32 byte threshold can be overwritten by is 7 bytes from a 64 bit write)
#define GX_FIFO_BUFFER_LENGTH 0x40

typedef struct s_GX_FIFO {
    HW_REG_WRITE_CALLBACK((*write), GX_FIFO);
    s_CP* CP_ptr;

    u8 buffer[GX_FIFO_BUFFER_LENGTH];
    u8 buffer_size;

    struct s_GameCube* system;
} s_GX_FIFO;


HW_REG_INIT_FUNCTION(GX_FIFO);

#endif //GC__GX_FIFO_H
