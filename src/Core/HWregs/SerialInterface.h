#ifndef GC__SERIALINTERFACE_H
#define GC__SERIALINTERFACE_H

#include "default.h"
#include "hwreg_utils.h"

#include <stdbool.h>

typedef enum e_SI_regs {
    SI_reg_Joy1_base = 0x04,
    SI_reg_Joy2_base = 0x08,
    SI_reg_SICOMCSR = 0x34,
    SI_reg_SISR = 0x38,
    SI_reg_buffer = 0x80,
} e_SI_regs;

#define SI_SHIFT 2

typedef struct s_SI {
    u8 regs[0x100];
    HW_REG_WRITE_CALLBACK((*write[0x100 >> SI_SHIFT]), SI);
    HW_REG_READ_PRECALL((*read[0x100 >> SI_SHIFT]), SI);
    struct s_GameCube* system;

    /* internal function */
    u32 COMCSR;

    // arrays for these for potential support for 4 controllers
    u32 joypad_buttons_1[4];
    u32 joypad_buttons_2[4];
} s_SI;


HW_REG_INIT_FUNCTION(SI);

#endif //GC__SERIALINTERFACE_H
