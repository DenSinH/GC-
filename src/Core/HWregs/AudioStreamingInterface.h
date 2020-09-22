#ifndef GC__AUDIOSTREAMINGINTERFACE_H
#define GC__AUDIOSTREAMINGINTERFACE_H

#include "default.h"
#include "hwreg_utils.h"

#define AI_SHIFT 2

typedef struct s_AI {
    u8 regs[0x20];
    HW_REG_WRITE_CALLBACK((*write[0x20 >> AI_SHIFT]), AI);
    HW_REG_READ_PRECALL((*read[0x20 >> AI_SHIFT]), AI);
    struct s_GameCube* system;
} s_AI;

HW_REG_INIT_FUNCTION(AI);

#endif //GC__AUDIOSTREAMINGINTERFACE_H
