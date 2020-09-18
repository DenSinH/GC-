#ifndef GC__PIXELENGINE_H
#define GC__PIXELENGINE_H

#include "default.h"
#include "hwreg_utils.h"
#include "core_utils.h"

typedef enum e_PE_regs {
    PE_reg_interrupt_status = 0x0a,
    PE_reg_token = 0x0e,
} e_PE_regs;

typedef struct s_PE {
    u8 regs[0x100];
    HW_REG_WRITE_CALLBACK((*write[0x80]), PE);
    HW_REG_READ_PRECALL((*read[0x80]), PE);
    struct s_GameCube* system;

    /* internal function */
    u16 intr_status;
} s_PE;

HW_REG_INIT_FUNCTION(PE);

static inline void SET_PE_REG(s_PE* PE, e_PE_regs reg, u16 value) {
    WRITE16(PE->regs, reg >> 1, value);
}

static inline u16 GET_PE_REG(s_PE* PE, e_PE_regs reg) {
    return READ16(PE->regs, reg >> 1);
}

#endif //GC__PIXELENGINE_H
