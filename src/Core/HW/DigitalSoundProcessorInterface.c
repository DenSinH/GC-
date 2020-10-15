#include "DigitalSoundProcessorInterface.h"

#include "log.h"
#include "core_utils.h"

#include "const.h"

const static u32 CPU_CYCLES_PER_DSP_STEP = CLOCK_FREQUENCY / DSP_CLOCK_FREQUENCY;

#ifdef DO_DEBUGGER
#include "../system.h"

void do_step_DSP(s_DSPI* DSPI) {
    if (DSPI->step_DSP) {
        DSPI->system->paused = true;
        DSPI->step_DSP = false;
    }
}
#else
void do_step_DSP(s_DSPI* DSPI) {

}
#endif

SCHEDULER_EVENT(DSPI_DSP_step_event) {
    s_DSPI* DSPI = (s_DSPI*)caller;

    step_DSP(&DSPI->DSP);

    do_step_DSP(DSPI);

    // reschedule event (we always keep this in the scheduler)
    event->time += CPU_CYCLES_PER_DSP_STEP;
    add_event(&DSPI->system->scheduler, event);
}

HW_REG_READ_PRECALL(read_DSP_MailboxHiFromDSP, DSPI) {

}

HW_REG_READ_PRECALL(read_DSP_MailboxLoFromDSP, DSPI) {

}

HW_REG_WRITE_CALLBACK(write_DSP_CSR, DSPI) {
    u16 value = READ16(DSPI->regs, DSPI_reg_CSR);

    if (value & DSP_CSR_RES) {
        // reset DSP this happens "instantly"
    }
    DSPI->DSPCSR = value & ~(DSP_CSR_RES);
}

HW_REG_READ_PRECALL(read_DSP_CSR, DSPI) {
    WRITE16(DSPI->regs, DSPI_reg_CSR, DSPI->DSPCSR);
}


HW_REG_INIT_FUNCTION(DSPI) {

    DSPI->read[DSPI_reg_CSR >> DSP_SHIFT] = read_DSP_CSR;

    DSPI->write[DSPI_reg_CSR >> DSP_SHIFT] = write_DSP_CSR;

    DSPI->DSP_step_event = (s_event) {
        .caller = DSPI,
        .callback = DSPI_DSP_step_event,
        .time = CPU_CYCLES_PER_DSP_STEP
    };
    add_event(&DSPI->system->scheduler, &DSPI->DSP_step_event);

    init_DSP(&DSPI->DSP, DSP_ROM_PATH, DSP_COEF_PATH);
}
