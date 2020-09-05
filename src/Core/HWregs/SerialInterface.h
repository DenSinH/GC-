#ifndef GC__SERIALINTERFACE_H
#define GC__SERIALINTERFACE_H

#include "default.h"
#include "hwreg_utils.h"

typedef struct s_SI {
    u8 regs[0x100];
    HW_REG_WRITE_CALLBACK((*write[0x40]), SI);
    HW_REG_READ_PRECALL((*read[0x40]), SI);
    struct s_GameCube* system;
} s_SI;


HW_REG_INIT_FUNCTION(SI);

#endif //GC__SERIALINTERFACE_H
