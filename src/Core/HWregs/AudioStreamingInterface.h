#ifndef GC__AUDIOSTREAMINGINTERFACE_H
#define GC__AUDIOSTREAMINGINTERFACE_H

#include "default.h"
#include "hwreg_utils.h"

#include "ProcessorInterface.h"

#include "../Scheduler/scheduler.h"

typedef enum e_AI_regs {
    AI_reg_AICR   = 0x00,
    AI_reg_AIVR   = 0x04,
    AI_reg_AISCNT = 0x08,
    AI_reg_AIIT   = 0x0c,
} e_AI_regs;

typedef enum e_AICR_bits {
    AICR_PSTAT     = 0x01,
    AICR_AFR       = 0x02,
    AICR_AIINTMASK = 0x04,
    AICR_AIINT     = 0x08,
    AICR_AIINTVLD  = 0x10,
    AICR_SCRESET   = 0x20,
    AICR_DSPFR     = 0x40,
} e_AICR_bits;

#define AI_SHIFT 2

typedef struct s_AI {
    u8 regs[0x20];
    HW_REG_WRITE_CALLBACK((*write[0x20 >> AI_SHIFT]), AI);
    HW_REG_READ_PRECALL((*read[0x20 >> AI_SHIFT]), AI);
    struct s_GameCube* system;

    /* internal function */
    s_PI* PI;

    // AI holds only a few registers, it's nicest to just keep these in separate variables anyway
    u32 AICR, AIVR, AIIT;


    /* Keep track of the time at which AISCNT is changed.
     * On reads, we interpolate what the value should be.
     * AISCNT should be corrected for whenever:
     *      - AICR_PSTAT is changed
     *      - AICR_AFR is changed
     * */
    u32 AISCNT, AISCNT_prev;
    u64 AISCNT_time;
    u32 ticks_per_sample;

    // event to trigger AISCNT == AIIT
    // update whenever AISCNT is recalculated or AIIT is changed
    s_event AIIT_event;
} s_AI;

HW_REG_INIT_FUNCTION(AI);

#endif //GC__AUDIOSTREAMINGINTERFACE_H
