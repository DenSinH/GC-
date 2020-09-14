#include "interrupts.h"

#include "Gekko.h"
#include "../system.h"

SCHEDULER_EVENT(DEC_intr) {
    s_Gekko* cpu = (s_Gekko*)caller;
    printf("DEC interrupt!");
    reschedule_event(&cpu->system->scheduler, &cpu->DEC_intr_event, cpu->DEC_intr_event.time + 0x100000000ULL);
}