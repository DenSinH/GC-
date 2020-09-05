#ifndef GC__PIXELENGINE_H
#define GC__PIXELENGINE_H

#include "default.h"
#include "hwreg_utils.h"

typedef struct s_PE {
    u8 regs[0x100];
    HW_REG_WRITE_CALLBACK((*write[0x40]), PE);
    HW_REG_READ_PRECALL((*read[0x40]), PE);
    struct s_GameCube* system;
} s_PE;


HW_REG_INIT_FUNCTION(PE);

#endif //GC__PIXELENGINE_H
