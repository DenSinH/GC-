#include "AudioStreamingInterface.h"

#include "core_utils.h"
#include "default.h"

#include "../system.h"

#ifndef NDEBUG
#include <assert.h>
#endif

static const u32 ticks_per_sample[2] = {
        CLOCK_FREQUENCY / 32000,
        CLOCK_FREQUENCY / 48000
};

static inline void recalculate_AISCNT(s_AI* AI) {
    // calculate AISCNT without resetting anything
    if (!(AI->AICR & AICR_PSTAT)) {
        // not playing
        AI->AISCNT = AI->AISCNT_prev;
        return;
    }

    u64 dt = get_time(AI->system->scheduler) - AI->AISCNT_time;

    // NOTE: we must always recalculate AISCNT BEFORE we change the sample rate
    AI->AISCNT = AI->AISCNT_prev + (dt / AI->ticks_per_sample);

    log_dsp("Recalculate AISCNT to %x", AI->AISCNT);
}

static inline void flush_AISCNT(s_AI* AI) {
    // calculate AISCNT with resetting
    if (!(AI->AICR & AICR_PSTAT)) {
        // not playing
        AI->AISCNT = AI->AISCNT_prev;
    }
    else {
        u64 dt = get_time(AI->system->scheduler) - AI->AISCNT_time;

        // NOTE: we must always recalculate AISCNT BEFORE we change the sample rate
        AI->AISCNT_prev = AI->AISCNT = AI->AISCNT_prev + (dt / AI->ticks_per_sample);
    }
    log_dsp("Flush AISCNT to %x", AI->AISCNT);
    AI->AISCNT_time = get_time(AI->system->scheduler);
}

static inline void recalculate_AIINT_event(s_AI* AI) {
    // NOTE: we must always call this AFTER changing the sample rate
#ifndef NDEBUG
    assert(AI->AISCNT_time == get_time(AI->system->scheduler)
    /* AISCNT must be up to date before we can update the event */
    );
#endif

    if (!(AI->AICR & AICR_PSTAT)) {
        // not playing
        if (AI->AIIT_event.active) {
            remove_event(AI->system->scheduler, &AI->AIIT_event);
        }
        return;
    }

    if (AI->AIIT < AI->AISCNT) {
        // the event will never happen in the foreseeable future
        return;
    }

    u64 dt = (AI->AIIT - AI->AISCNT) * AI->ticks_per_sample;
    if (AI->AIIT_event.time == get_time(AI->system->scheduler) + dt) {
        // event already up-to-date
        return;
    }

    reschedule_event(AI->system->scheduler, &AI->AIIT_event, get_time(AI->system->scheduler) + dt);
}

SCHEDULER_EVENT(AI_AIINT_event) {
    s_AI* AI = (s_AI*)caller;

    log_dsp("AI sample interrupt");

    AI->AICR |= AICR_AIINT;
    if (AI->AICR & AICR_AIINTMASK) {
        // immediate polling
        set_PI_intsr(AI->PI, PI_intr_AI, 0);
    }

    // we do not reschedule this event, since it takes a AIIT write/AISCR reset to allow it to happen again
}

HW_REG_WRITE_CALLBACK(write_AI_CR, AI) {
    u32 value = READ32(AI->regs, AI_reg_AICR);

    bool do_recalculate_AIINT_event = false;

    if (value & AICR_SCRESET) {
        // reset sample counter
        AI->AISCNT_time = get_time(AI->system->scheduler);
        AI->AISCNT = 0;
        do_recalculate_AIINT_event = true;
    }

    if ((AI->AICR & AICR_AFR) != (value & AICR_AFR)) {
        // recalculate sample rate
        flush_AISCNT(AI);
        AI->ticks_per_sample = ticks_per_sample[(value & AICR_AFR) != 0];
        do_recalculate_AIINT_event = true;
    }

    if ((AI->AICR & AICR_PSTAT) != (value & AICR_PSTAT)) {
        // event changed from starting/stopping it
        flush_AISCNT(AI);
        do_recalculate_AIINT_event = true;
    }

    if (do_recalculate_AIINT_event) {
        recalculate_AIINT_event(AI);
    }

    if ((AI->AICR & AICR_AIINT) && (value & AICR_AIINT)) {
        // clear interrupt
        clear_PI_intsr(AI->PI, PI_intr_AI);
    }

    AI->AICR = (value & ~(AICR_AIINT | AICR_SCRESET));
}

HW_REG_WRITE_CALLBACK(write_AI_VR, AI) {
    AI->AIVR = READ32(AI->regs, AI_reg_AIVR);
}

/*
 * AISCNT is read-only
 * */

HW_REG_WRITE_CALLBACK(write_AI_IT, AI) {
    u32 value = READ32(AI->regs, AI_reg_AIIT);
    if (AI->AIIT != value) {
        AI->AIIT = value;
        recalculate_AIINT_event(AI);
    }
}

HW_REG_READ_PRECALL(read_AI_CR, AI) {
    WRITE32(AI->regs, AI_reg_AICR, AI->AICR);
}

HW_REG_READ_PRECALL(read_AI_SCNT, AI) {
    // update value
    recalculate_AISCNT(AI);
    WRITE32(AI->regs, AI_reg_AISCNT, AI->AISCNT);
}

HW_REG_INIT_FUNCTION(AI) {
    AI->AIIT_event = (s_event) {
        .caller = AI,
        .callback = AI_AIINT_event
    };

    AI->ticks_per_sample = ticks_per_sample[0];

    AI->write[AI_reg_AICR >> AI_SHIFT] = write_AI_CR;
    AI->write[AI_reg_AIVR >> AI_SHIFT] = write_AI_VR;
    AI->write[AI_reg_AIIT >> AI_SHIFT] = write_AI_IT;

    AI->read[AI_reg_AICR   >> AI_SHIFT] = read_AI_CR;
    AI->read[AI_reg_AISCNT >> AI_SHIFT] = read_AI_SCNT;
}
