#ifndef GC__SERIALINTERFACE_H
#define GC__SERIALINTERFACE_H

#include "ProcessorInterface.h"

#include "default.h"
#include "hwreg_utils.h"

#include <stdbool.h>

typedef enum e_SI_regs {
    SI_reg_SIC0OUTBUF = 0x00,
    SI_reg_Joy1_base  = 0x04,
    SI_reg_Joy2_base  = 0x08,
    SI_reg_SICOMCSR   = 0x34,
    SI_reg_SISR       = 0x38,
    SI_buffer_base    = 0x80,
} e_SI_regs;

typedef enum e_SICOMCSR {
    SICOMCSR_TCINT      = 0x80000000,
    SICOMCSR_TCINTMSK   = 0x40000000,
    SICOMCSR_COMERR     = 0x20000000,
    SICOMCSR_RDSTINT    = 0x10000000,
    SICOMCSR_RDSTINTMSK = 0x08000000,
    SICOMCSR_TSTART     = 0x00000001,
} e_SICOMCSR;

typedef enum e_SISR {
    SISR_Write_Buffer = 0x80000000,
    SISR_RDST0        = 0x20000000,
    SISR_WRST0        = 0x10000000,
    SISR_NOREP0       = 0x08000000,
    SISR_COLL0        = 0x04000000,
    SISR_OVRUN0       = 0x02000000,
    SISR_UNRUN0       = 0x01000000,
} e_SISR;

typedef enum e_SI_commands {
    SI_command_reset       = 0x00,
    SI_command_direct      = 0x40,
    SI_command_origin      = 0x41,
    SI_command_recalibrate = 0x42,
    SI_command_ID          = 0x45,
} e_SI_commands;

// the layout in this struct is the same as the order the data in
// We can just memcpy this because all values are byte sized
//  IFF: we keep into account that the bytes are swapped when setting the buttons
typedef struct s_Controller {
    u16 buttons;
    u8 stick_x;
    u8 stick_y;
    u8 C_x;
    u8 C_y;
    u8 L;
    u8 R;
} s_Controller;

const static s_Controller controller_origin = (s_Controller) {
    .buttons = 0,
    .stick_x = 0x00,  // signed 0
    .stick_y = 0x00,
    .C_x     = 0x00,
    .C_y     = 0x00,
    .L       = 0x00,
    .R       = 0x00,
};

typedef enum e_GCButtons {
    button_A = 0x0001,
} e_GCButtons;

#define SI_SHIFT 2
#define SI_TCINT_DELAY 0x10  // cycles
#define SI_RDSTINT_DELAY 0x10  // cycles

#define SI_CHAN_STRIDE 0xc  // stride for each of the control registers for the channels

#define SI_CONTROLLER 0x09000000
#define SI_NO_CONTROLLER 0x0008     // no response (nothing attached)

typedef struct s_SI {
    u8 regs[0x100];
    HW_REG_WRITE_CALLBACK((*write[0x100 >> SI_SHIFT]), SI);
    HW_REG_READ_PRECALL((*read[0x100 >> SI_SHIFT]), SI);
    struct s_GameCube* system;

    /* internal function */
    u32 COMCSR, SISR;

    s_PI* PI;

    u8 modes[4];  // operating modes for the 4 channels
                  // controlled through the SICnOUTBUF registers

    // arrays for these for potential support for 4 controllers
    u32 joypad_buttons_1[4];
    u32 joypad_buttons_2[4];
} s_SI;


HW_REG_INIT_FUNCTION(SI);

#endif //GC__SERIALINTERFACE_H
