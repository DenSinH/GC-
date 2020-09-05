#ifndef GC__EXTERNALINTERFACE_H
#define GC__EXTERNALINTERFACE_H

#include "default.h"
#include "hwreg_utils.h"

typedef struct s_EXI {
    u8 regs[0x40];
    HW_REG_WRITE_CALLBACK((*write[0x10]), EXI);
    HW_REG_READ_PRECALL((*read[0x10]), EXI);
    struct s_GameCube* system;
} s_EXI;


HW_REG_INIT_FUNCTION(EXI);

#endif //GC__EXTERNALINTERFACE_H
