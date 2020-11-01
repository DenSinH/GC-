#include "interrupts.h"

#include "Gekko.h"
#include "../system.h"

#include "log.h"
#include "helpers.h"

#include <stdbool.h>

static inline void handle_interrupt(s_Gekko* cpu, u32 target_address) {
    cpu->SRR0 = cpu->PC;    // PC is 4 ahead already
    cpu->SRR1 = cpu->MSR.raw & INTERRUPT_SRR1_MASK;

    // from the general PPC manual:
    cpu->MSR.LE = cpu->MSR.ILE;
    cpu->MSR.raw &= ~0x04EF36;

    cpu->PC = target_address;
}

static inline bool any_enabled(s_Gekko* cpu) {
    // check if any interrupts are enabled (either set, or MSR.EE set and interrupt is set)
    return (cpu->interrupts & INTERNAL_INTERRUPTS) || (cpu->MSR.EE && ((cpu->interrupts & EXTERNAL_INTERRUPTS) || (cpu->system->HW_regs.PI.INTSR & cpu->system->HW_regs.PI.INTMR)));
}

void start_interrupt_poll(s_Gekko* cpu, int delay){
    // only start polling if there are any interrupts
    log_cpu("Request to start interrupt poll (%lld)", cpu->TBR.raw);
    if (!cpu->poll_intr_event.active && any_enabled(cpu)) {
        // start polling for interrupts again
        log_cpu("Request acknowledged");
        cpu->poll_intr_event.time = get_time(cpu->system->scheduler) + delay;
        add_event(cpu->system->scheduler, &cpu->poll_intr_event);
    }
}


SCHEDULER_EVENT(DEC_intr) {
    s_Gekko* cpu = (s_Gekko*)caller;
    // log_cpu("DEC interrupt");

    if (cpu->MSR.EE) {
        // handle immediately
        handle_interrupt(cpu, 0x00000900);
    }
    else {
        // have it be handled by the interrupt polling event
        // this event is enabled again when MSR.EE is enabled
        cpu->interrupts |= interrupt_DEC;
    }

    // at this point, the event was popped from the heap
    // reschedule it to the next time it would overflow (very far away)
    cpu->DEC_intr_event.time += 0x100000000ULL;
    add_event(scheduler, &cpu->DEC_intr_event);
}

SCHEDULER_EVENT(handle_interrupts) {
    s_Gekko* cpu = (s_Gekko*)caller;

    if (cpu->interrupts) {
        // todo: any internal interrupts

        if (cpu->MSR.EE) {
            // todo: other external interrupts
            if (cpu->system->HW_regs.PI.INTSR & cpu->system->HW_regs.PI.INTMR) {
                // external interrupts from PI
                log_cpu("External interrupts (%04x) (%lld)", cpu->system->HW_regs.PI.INTSR & cpu->system->HW_regs.PI.INTMR, cpu->TBR.raw);
                if (cpu->system->HW_regs.PI.INTSR & cpu->system->HW_regs.PI.INTMR & PI_intr_SI) {
                    log_si("SI INTERRUPT");
                }
                handle_interrupt(cpu, 0x00000500);
            }
            else if (cpu->interrupts & interrupt_DEC) {
                handle_interrupt(cpu, 0x00000900);
                cpu->interrupts &= ~interrupt_DEC;
            }
        }

        if (cpu->interrupts && any_enabled(cpu)) {
            event->time = get_time(scheduler) + INTERRUPT_POLL_DELAY;

            // keep it in the scheduler
            add_event(scheduler, event);
        }
        // otherwise: do not keep it in the scheduler, it will be rescheduled once MSR.EE is enabled or when
        // interrupts that cannot fire are sent
    }
    else {
        // interrupts are somehow already done (can't actually happen)
        log_warn("Invalid call to interrupt poll event");
    }
}