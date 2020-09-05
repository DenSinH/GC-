#ifndef GC__DVDINTERFACE_H
#define GC__DVDINTERFACE_H

#include "default.h"
#include "hwreg_utils.h"

typedef struct s_DI {
    u8 regs[0x40];
    HW_REG_WRITE_CALLBACK((*write[0x10]), DI);
    HW_REG_READ_PRECALL((*read[0x10]), DI);
    struct s_GameCube* system;
} s_DI;


HW_REG_INIT_FUNCTION(DI);

#endif //GC__DVDINTERFACE_H
