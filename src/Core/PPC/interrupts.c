#include "interrupts.h"

#include "Gekko.h"
#include "../system.h"

#include "log.h"

SCHEDULER_EVENT(DEC_intr) {
    s_Gekko* cpu = (s_Gekko*)caller;
    log_cpu("DEC interrupt!");

    do_interrupt(cpu, 0x00000900);

    // at this point, the event was popped from the heap
    cpu->DEC_intr_event.time += 0x100000000ULL;
    add_event(&cpu->system->scheduler, &cpu->DEC_intr_event);
}