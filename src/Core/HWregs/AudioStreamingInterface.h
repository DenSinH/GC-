#ifndef GC__AUDIOSTREAMINGINTERFACE_H
#define GC__AUDIOSTREAMINGINTERFACE_H

#include "default.h"
#include "hwreg_utils.h"

typedef struct s_AI {
    u8 regs[0x20];
    HW_REG_WRITE_CALLBACK((*write[8]), AI);
    HW_REG_READ_PRECALL((*read[8]), AI);
    struct s_GameCube* system;
} s_AI;

HW_REG_INIT_FUNCTION(AI);

#endif //GC__AUDIOSTREAMINGINTERFACE_H
