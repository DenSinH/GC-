#ifndef GC__INTERRUPTS_H
#define GC__INTERRUPTS_H

#include "Gekko.h"
#include "../Scheduler/scheduler.h"

// must be some small value, higher might increase performance, but decrease accuracy
#define INTERRUPT_POLL_DELAY 10

typedef enum e_interrupts {
    interrupt_DEC = 0x00000001
} e_interrupts;

#define EXTERNAL_INTERRUPTS (interrupt_DEC)
#define INTERNAL_INTERRUPTS (0)

SCHEDULER_EVENT(handle_interrupts);

SCHEDULER_EVENT(DEC_intr);

// bit 13 is forced to 0
#define INTERRUPT_SRR1_MASK 0x87c0ff73
static inline void do_interrupt(s_Gekko* cpu, u32 target_address) {
    cpu->SRR0 = cpu->PC;    // PC is 4 ahead already
    cpu->SRR1 = cpu->MSR.raw & INTERRUPT_SRR1_MASK;

    // from Dolphin:
    cpu->MSR.LE = cpu->MSR.ILE;
    cpu->MSR.raw &= ~0x04EF36;

    cpu->PC = target_address;
}

void start_interrupt_poll(s_Gekko* cpu);

#endif //GC__INTERRUPTS_H
