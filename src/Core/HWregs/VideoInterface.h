#ifndef GC__VIDEOINTERFACE_H
#define GC__VIDEOINTERFACE_H

#include "default.h"
#include "hwreg_utils.h"
#include "../Scheduler/scheduler.h"

#include "ProcessorInterface.h"

#include <stdbool.h>

/*
 * See the math at https://github.com/DenSinH/GameCubeResources/wiki/Display
 * */

#if SCREEN_TYPE == SCREEN_PAL
    #define LINES_PER_FRAME 625
#elif SCREEN_TYPE == SCREEN_NTSC
    #define LINES_PER_FRAME 525
#else
    #error Incorrect screen type selected in flags.h
#endif

#define VI_DI_DATA 0x0fffffff
#define VI_DI_ENABLED 0x10000000
#define VI_DI_ACTIVE 0x80000000

typedef enum e_VI_regs {
    VI_reg_VTR  = 0x0000,
    VI_reg_DCR  = 0x0002,
    VI_reg_HTR0 = 0x0004,
    VI_reg_HTR1 = 0x0006,
    VI_reg_VTO  = 0x0008,
    VI_reg_VTE  = 0x000a,

    VI_reg_DPV  = 0x002c,
    VI_reg_DPH  = 0x002e,
    VI_reg_DI0  = 0x0030,
    VI_reg_DI1  = 0x0034,
    VI_reg_DI2  = 0x0038,
    VI_reg_DI3  = 0x003c,
} e_VI_regs;


typedef struct s_VI {
    u8 regs[0x100];
    HW_REG_WRITE_CALLBACK((*write[0x40]), VI);
    HW_REG_READ_PRECALL((*read[0x40]), VI);
    struct s_GameCube* system;

    /* internal function */
    s_PI* PI;

    u32 HLW;               // HalfLineWidth, set in VI.HTR0
    u32 current_halfline;  // for current VPos: / 2 + (halflines per field * current_field)
    bool current_field;    // false: odd, true: even

    u32 DI[4];

    s_event halfline_count_event;  // permanently in the scheduler
    s_event DI_event[4];           // only in scheduler when enabled, I don't use the top 2 fields
} s_VI;


HW_REG_INIT_FUNCTION(VI);

#endif //GC__VIDEOINTERFACE_H
