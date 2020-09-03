#ifndef GC__PROCESSORINTERFACE_H
#define GC__PROCESSORINTERFACE_H

#include "default.h"
#include "hwreg_utils.h"

typedef struct s_PI {
    u8 regs[0x100];
    HW_REG_WRITE_CALLBACK((*write[0x40]), PI);
    HW_REG_READ_PRECALL((*read[0x40]), PI);
    struct s_GameCube* system;
} s_PI;


HW_REG_INIT_FUNCTION(PI);

#endif //GC__PROCESSORINTERFACE_H
