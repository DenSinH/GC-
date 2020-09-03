#ifndef GC__COMMANDPROCESSOR_H
#define GC__COMMANDPROCESSOR_H

#include "default.h"
#include "hwreg_utils.h"

typedef struct s_CP {
    u8 regs[0x80];
    HW_REG_WRITE_CALLBACK((*write[0x40]), CP);
    HW_REG_READ_PRECALL((*read[0x40]), CP);
    struct s_GameCube* system;
} s_CP;


HW_REG_INIT_FUNCTION(CP);

#endif //GC__COMMANDPROCESSOR_H
