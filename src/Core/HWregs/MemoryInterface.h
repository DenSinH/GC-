#ifndef GC__MEMORYINTERFACE_H
#define GC__MEMORYINTERFACE_H

#include "default.h"
#include "hwreg_utils.h"

#define MI_SHIFT 2

typedef struct s_MI {
    u8 regs[0x100];
    HW_REG_WRITE_CALLBACK((*write[0x100 >> MI_SHIFT]), MI);
    HW_REG_READ_PRECALL((*read[0x100 >> MI_SHIFT]), MI);
    struct s_GameCube* system;
} s_MI;

HW_REG_INIT_FUNCTION(MI);

#endif //GC__MEMORYINTERFACE_H
