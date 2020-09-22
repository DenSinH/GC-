#ifndef GC__PIXELENGINE_H
#define GC__PIXELENGINE_H

#include "default.h"
#include "hwreg_utils.h"
#include "core_utils.h"

#include "ProcessorInterface.h"

typedef enum e_PE_interrupts {
    PE_intr_DONE = 0x8,
    PE_intr_TOKEN = 0x4
} e_PE_interrupts;

typedef enum e_PE_regs {
    PE_reg_interrupt_status = 0x0a,
    PE_reg_token = 0x0e,
} e_PE_regs;

#define PE_SHIFT 1

typedef struct s_PE {
    u8 regs[0x100];
    HW_REG_WRITE_CALLBACK((*write[0x100 >> PE_SHIFT]), PE);
    HW_REG_READ_PRECALL((*read[0x100 >> PE_SHIFT]), PE);
    struct s_GameCube* system;

    /* internal function */
    s_PI* PI;

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
