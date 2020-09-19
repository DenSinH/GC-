#ifndef GC__INTERRUPTS_H
#define GC__INTERRUPTS_H

#include "Gekko.h"
#include "../Scheduler/scheduler.h"

// must be some small value, higher might increase performance, but decrease accuracy
#define INTERRUPT_POLL_DELAY 1

typedef enum e_interrupts {
    interrupt_DEC = 0x00000001
} e_interrupts;

#define EXTERNAL_INTERRUPTS (interrupt_DEC)
#define INTERNAL_INTERRUPTS (0)

// bit 13 is forced to 0
#define INTERRUPT_SRR1_MASK 0x87c0ff73

SCHEDULER_EVENT(handle_interrupts);

SCHEDULER_EVENT(DEC_intr);

void start_interrupt_poll(s_Gekko* cpu);

static inline void do_interrupt(s_Gekko* cpu, e_interrupts interrupt) {
    cpu->interrupts |= interrupt;
    start_interrupt_poll(cpu);
}

#endif //GC__INTERRUPTS_H
