#ifndef GC__AUDIOSTREAMINGINTERFACE_H
#define GC__AUDIOSTREAMINGINTERFACE_H

#include "default.h"
#include "hardware_registers.h"

typedef struct s_AI {
    u32 regs[8];
    size_t size;
    HW_REG_READ_FUNCTION((*read[0x20]));
    HW_REG_WRITE_FUNCTION((*write[0x20]));
} s_AI;

#endif //GC__AUDIOSTREAMINGINTERFACE_H
