#ifndef GC__VIDEOINTERFACE_H
#define GC__VIDEOINTERFACE_H

#include "default.h"
#include "hwreg_utils.h"

typedef struct s_VI {
    u8 regs[0x100];
    HW_REG_WRITE_CALLBACK((*write[0x40]), VI);
    HW_REG_READ_PRECALL((*read[0x40]), VI);
    struct s_GameCube* system;
} s_VI;


HW_REG_INIT_FUNCTION(VI);

#endif //GC__VIDEOINTERFACE_H
