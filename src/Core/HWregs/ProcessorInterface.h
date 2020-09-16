#ifndef GC__PROCESSORINTERFACE_H
#define GC__PROCESSORINTERFACE_H

#include "default.h"
#include "hwreg_utils.h"

typedef enum e_PI_interrupt {
    PI_intr_ERROR = 0x000000001,
    PI_intr_RSW = 0x000000002,
    PI_intr_DI = 0x000000004,
    PI_intr_SI = 0x000000008,
    PI_intr_EXI = 0x000000010,
    PI_intr_AI = 0x000000020,
    PI_intr_DSP = 0x000000040,
    PI_intr_MI = 0x000000080,
    PI_intr_VI = 0x000000100,
    PI_intr_PE_TOKEN = 0x000000200,
    PI_intr_PE_FINISH = 0x000000400,
    PI_intr_CP = 0x000000800,
    PI_intr_DEBUG = 0x000001000,
    PI_intr_HSP = 0x000002000,
    PI_intr_RSWST = 0x000010000
} e_PI_interrupt;

typedef struct s_PI {
    u8 regs[0x100];

    u32 INTMR;  // interrupt mask

    HW_REG_WRITE_CALLBACK((*write[0x40]), PI);
    HW_REG_READ_PRECALL((*read[0x40]), PI);
    struct s_GameCube* system;
} s_PI;


HW_REG_INIT_FUNCTION(PI);

#endif //GC__PROCESSORINTERFACE_H
