#include "DigitalSoundProcessorInterface.h"

#include "log.h"
#include "core_utils.h"
#include "../system.h"

#include "const.h"

const static u32 CPU_CYCLES_PER_DSP_STEP = CLOCK_FREQUENCY / DSP_CLOCK_FREQUENCY;

#ifdef CHECK_DSP_DMA_ADDR
#include <assert.h>
#endif

#ifdef DO_DEBUGGER
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

/* DSP initialization: DSP is supposed to do nothing, then the CPU reads the mailbox until no mail is left
 * The CPU starts a DMA -> ARAM (IRAM region) for the stub of the exception vectors. The RESET vector will HALT the DSP.
 * The CPU must unhalt the DSP by writing to CSR. Then the DSP will jump to IROM and start doing stuff.
 * Then the CPU waits for mail.
 *
 * The DMA completing might raise an interrupt in the PI (will not in this case)
 * I am unsure what actually starts the DSP, for now I'll just set a bool in the DSP.
 * */

static inline void start_DSP(s_DSPI* DSPI) {
    DSPI->DSP.started = true;
    DSPI->DSP.config |= DSP_CR_INIT;

    DSPI->DSP_step_event.time = *DSPI->system->scheduler.timer;
    add_event(&DSPI->system->scheduler, &DSPI->DSP_step_event);
}

static inline void check_DSPI_interrupts(s_DSPI* DSPI) {
    // strategy from Dolpin: interrupt mask is always right above interrupt, doing this checks all interrupts at once
    if ((DSPI->DSPCSR & (DSPI->DSPCSR >> 1)) | (DSP_CSR_DSPINT | DSP_CSR_ARINT | DSP_CSR_AIDINT)) {
        // immediately raise interrupt
        set_PI_intsr(DSPI->PI, PI_intr_DSP, 0);
    }
}

static void DSPI_interrupt(s_DSPI* DSPI, e_DSP_CSR interrupt) {
    DSPI->DSPCSR |= interrupt;
    check_DSPI_interrupts(DSPI);
}

SCHEDULER_EVENT(DSPI_DSP_step_event) {
    s_DSPI* DSPI = (s_DSPI*)caller;

    // assert(!DSP_halted(&DSPI->DSP));
    int cycles = step_DSP(&DSPI->DSP);

    do_step_DSP(DSPI);

    if (DSPI->DSP.DIRQ) {
        DSPI->DSPCSR |= DSP_CSR_DSPINT;
        check_DSPI_interrupts(DSPI);
    }

    if (DSP_halted(&DSPI->DSP)) {
        DSPI->DSPCSR |= DSP_CSR_HALT;
        log_dsp("DSP halted, not re-adding event");
    }
    else if (DSPI->DSP.started) {
        // reschedule event (we always keep this in the scheduler)
        event->time += cycles * CPU_CYCLES_PER_DSP_STEP;
        add_event(&DSPI->system->scheduler, event);
    }
}

SCHEDULER_EVENT(DSPI_DSP_AR_DMA_done) {
    log_dsp("ARAM DMA done");

    s_DSPI* DSPI = (s_DSPI*)caller;

    /*
     * We are just going to transfer (memcpy) the whole DMA at once, then clear the DMA transfer bit in DSPCNT
     * */
    // todo: this is wrong, but I'm not sure how else it's supposed to copy the entire reset vector region into ARAM
    u32 length = 4 * (DSPI->AR_DMA_CNT & DSP_AR_DMA_LEN);
#ifdef CHECK_DSP_DMA_ADDR
    assert(MASK_24MB(DSPI->AR_DMA_MMADDR) + length < sizeof(DSPI->system->memory) /* MMEM overflow */ );
    assert(DSPI->AR_DMA_ARADDR + length < sizeof(DSPI->DSP.ARAM) /* ARAM overflow */ );
#endif

    if (DSPI->AR_DMA_CNT & DSP_AR_DMA_DIR) {
        // ARAM -> MMEM
        memcpy(DSPI->system->memory + MASK_24MB(DSPI->AR_DMA_MMADDR), DSPI->DSP.ARAM + DSPI->AR_DMA_ARADDR, length);
    }
    else {
        // MMEM -> ARAM
        memcpy(DSPI->DSP.ARAM + DSPI->AR_DMA_ARADDR, DSPI->system->memory + MASK_24MB(DSPI->AR_DMA_MMADDR), length);

        // todo: is this the signal for starting?
        // it would work: the DSP should only run once a reset vector region exists
        if (!DSPI->DSP.started && DSPI->AR_DMA_ARADDR == 0) {
            start_DSP(DSPI);
        }
    }

    // DMA done
    DSPI->DSPCSR &= ~DSP_CSR_DSPDMASTATUS;
    DSPI->DSPCSR |= DSP_CSR_ARINT;
    check_DSPI_interrupts(DSPI);
}

HW_REG_READ_PRECALL(read_DSP_MailboxHiFromDSP, DSPI) {
    log_dsp("CPU read DSP mail (hi) (%04x)", DSPI->DSP.DMBH);
    WRITE16(DSPI->regs, DSPI_reg_MailboxHiFromDSP, DSPI->DSP.DMBH);
}

HW_REG_READ_PRECALL(read_DSP_MailboxLoFromDSP, DSPI) {
    log_dsp("CPU read DSP mail (lo) (%04x)", DSPI->DSP.DMBL);
    DSPI->DSP.DMBH &= 0x7fff;  // clear M bit
    WRITE16(DSPI->regs, DSPI_reg_MailboxLoFromDSP, DSPI->DSP.DMBL);
}

HW_REG_WRITE_CALLBACK(write_DSP_MailboxHiToDSP, DSPI) {
    log_dsp("CPU send DSP mail (%04x)", READ16(DSPI->regs, DSPI_reg_MailboxHiFromDSP));
    DSPI->DSP.CMBH = READ16(DSPI->regs, DSPI_reg_MailboxHiToDSP);
}

HW_REG_READ_PRECALL(read_DSP_MailboxHiToDSP, DSPI) {
    log_dsp("CPU read CPU -> DSP mail status (%04x)", DSPI->DSP.CMBH);
    WRITE16(DSPI->regs, DSPI_reg_MailboxHiToDSP, DSPI->DSP.CMBH & 0x8000);
}

HW_REG_WRITE_CALLBACK(write_DSP_MailboxLoToDSP, DSPI) {
    log_dsp("CPU send DSP mail (lo) (%04x)", READ16(DSPI->regs, DSPI_reg_MailboxLoFromDSP));
    DSPI->DSP.CMBL = READ16(DSPI->regs, DSPI_reg_MailboxLoToDSP);
}

HW_REG_WRITE_CALLBACK(write_DSP_CSR, DSPI) {
    u16 value = READ16(DSPI->regs, DSPI_reg_CSR);

    if (value & DSP_CSR_RES) {
        // reset DSP this happens "instantly"
    }

    if (DSPI->DSP.started && DSP_halted(&DSPI->DSP) && !DSPI->DSP_step_event.active) {
        log_dsp("DSP unhalted, adding step event");
        unhalt_DSP(&DSPI->DSP);
        // DSPI->system->paused = true;
        DSPI->DSP_step_event.time = *DSPI->system->scheduler.timer;
        add_event(&DSPI->system->scheduler, &DSPI->DSP_step_event);
    }

    if (value & DSP_CSR_DSPINT) {
        DSPI->DSP.DIRQ = 0;
    }

    DSPI->DSPCSR = value & ~(DSP_CSR_RES | DSP_CSR_DSPINT | DSP_CSR_ARINT | DSP_CSR_AIDINT);
    check_DSPI_interrupts(DSPI);
}

HW_REG_READ_PRECALL(read_DSP_CSR, DSPI) {
    WRITE16(DSPI->regs, DSPI_reg_CSR, DSPI->DSPCSR);
}

HW_REG_WRITE_CALLBACK(write_DSP_AR_DMA_MMADDR, DSPI) {
    DSPI->AR_DMA_MMADDR = READ32(DSPI->regs, DSPI_reg_AR_DMA_MMADDR_H);
}

HW_REG_WRITE_CALLBACK(write_DSP_AR_DMA_ARADDR, DSPI) {
    DSPI->AR_DMA_ARADDR = READ32(DSPI->regs, DSPI_reg_AR_DMA_ARADDR_H);
}

HW_REG_WRITE_CALLBACK(write_DSP_AR_DMA_CNT_H, DSPI) {
    DSPI->AR_DMA_CNT = READ32(DSPI->regs, DSPI_reg_AR_DMA_CNT_H);
}

HW_REG_WRITE_CALLBACK(write_DSP_AR_DMA_CNT_L, DSPI) {
    DSPI->AR_DMA_CNT = READ32(DSPI->regs, DSPI_reg_AR_DMA_CNT_H);

    // start DMA
    log_dsp("DMA started (%x bytes: MM[%08x] <-> AR[%06x])", DSPI->AR_DMA_CNT, DSPI->AR_DMA_MMADDR, DSPI->AR_DMA_ARADDR);
    DSPI->DSPCSR |= DSP_CSR_DSPDMASTATUS;

    // transfer length has been measured on HW (gotten from Dolphin source:
    // https://github.com/dolphin-emu/dolphin/blob/master/Source/Core/Core/HW/DSP.cpp)
    DSPI->DSP_DMA_done_event.time = *DSPI->system->scheduler.timer + (((DSPI->AR_DMA_CNT & DSP_AR_DMA_LEN) * 246) >> 5);
    add_event(&DSPI->system->scheduler, &DSPI->DSP_DMA_done_event);
}

HW_REG_INIT_FUNCTION(DSPI) {

    DSPI->read[DSPI_reg_CSR >> DSP_SHIFT] = read_DSP_CSR;
    DSPI->read[DSPI_reg_MailboxHiFromDSP >> DSP_SHIFT] = read_DSP_MailboxHiFromDSP;
    DSPI->read[DSPI_reg_MailboxLoFromDSP >> DSP_SHIFT] = read_DSP_MailboxLoFromDSP;
    DSPI->read[DSPI_reg_MailboxHiToDSP >> DSP_SHIFT]   = read_DSP_MailboxHiToDSP;

    DSPI->write[DSPI_reg_CSR >> DSP_SHIFT] = write_DSP_CSR;
    DSPI->write[DSPI_reg_MailboxHiToDSP >> DSP_SHIFT] = write_DSP_MailboxHiToDSP;
    DSPI->write[DSPI_reg_MailboxLoToDSP >> DSP_SHIFT] = write_DSP_MailboxLoToDSP;
    DSPI->write[DSPI_reg_AR_DMA_MMADDR_H >> DSP_SHIFT] = write_DSP_AR_DMA_MMADDR;
    DSPI->write[DSPI_reg_AR_DMA_MMADDR_L >> DSP_SHIFT] = write_DSP_AR_DMA_MMADDR;
    DSPI->write[DSPI_reg_AR_DMA_ARADDR_H >> DSP_SHIFT] = write_DSP_AR_DMA_ARADDR;
    DSPI->write[DSPI_reg_AR_DMA_ARADDR_L >> DSP_SHIFT] = write_DSP_AR_DMA_ARADDR;
    DSPI->write[DSPI_reg_AR_DMA_CNT_H >> DSP_SHIFT] = write_DSP_AR_DMA_CNT_H;
    DSPI->write[DSPI_reg_AR_DMA_CNT_L >> DSP_SHIFT] = write_DSP_AR_DMA_CNT_L;

    DSPI->DSP_step_event = (s_event) {
        .caller = DSPI,
        .callback = DSPI_DSP_step_event
    };

    DSPI->DSP_DMA_done_event = (s_event) {
            .caller = DSPI,
            .callback = DSPI_DSP_AR_DMA_done
    };

    init_DSP(&DSPI->DSP, DSP_ROM_PATH, DSP_COEF_PATH);
}
