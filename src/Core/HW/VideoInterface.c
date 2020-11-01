#include "VideoInterface.h"

#include "../system.h"

/*
 * See the math at https://github.com/DenSinH/GameCubeResources/wiki/Display
 * */


SCHEDULER_EVENT(VI_halfline_count) {
    s_VI* VI = (s_VI*)caller;

    VI->current_halfline++;
    if (VI->current_halfline == LINES_PER_FRAME) {
        VI->current_halfline = 0;
        VI->current_field ^= true;
    }

    if (VI->HLW) {
        event->time += 3 * 6 * VI->HLW;
    }
    else {
        log_warn("Halfline Width is invalid (0)");
        event->time = (u64)-1;
    }
    add_event(scheduler, event);
}

void VI_DI_intr(s_Gekko* cpu, s_event* event, const u32 index) {
    log_cpu("VI.DI%d interrupt", index);

    // set interrupt status to active
    cpu->system->HW_regs.VI.DI[index] |= VI_DI_ACTIVE;

    // signal VI interrupt
    set_PI_intsr(&cpu->system->HW_regs.PI, PI_intr_VI, 0);  // instant polling

    // reschedule event for next frame
    event->time += LINES_PER_FRAME * 2 * 3 * 6 * cpu->system->HW_regs.VI.HLW;
    add_event(cpu->system->scheduler, event);
}

SCHEDULER_EVENT(VI_DI0_intr) {
    VI_DI_intr((s_Gekko*)caller, event, 0);
}

SCHEDULER_EVENT(VI_DI1_intr) {
    VI_DI_intr((s_Gekko*)caller, event, 1);
}

SCHEDULER_EVENT(VI_DI2_intr) {
    VI_DI_intr((s_Gekko*)caller, event, 2);
}

SCHEDULER_EVENT(VI_DI3_intr) {
    VI_DI_intr((s_Gekko*)caller, event, 3);
}

void schedule_DI_event(s_VI* VI, u32 index) {
    u32 HCT = VI->DI[index] & 0x3ff;
    u32 VCT = (VI->DI[index] >> 16) & 0x3ff;
    u64 time = get_time(VI->system->scheduler);

    // VCT > (LINES_PER_FRAME >> 1)  <==> field is odd
    if ((VCT > (LINES_PER_FRAME >> 1)) ^ (VI->current_field & 1)) {
        time += LINES_PER_FRAME * 3 * 6 * VI->HLW;  // time offset for one full field
        VCT -= (LINES_PER_FRAME >> 1);
    }

    // calculate the halfline within the field of the interrupt
    VCT = (HCT < VI->HLW) + (VCT << 1);
    if (VCT <= VI->current_halfline) {
        // we don't want to schedule an event now or in the past
        VCT += LINES_PER_FRAME << 1;
    }
    time += (VCT - VI->current_halfline) * 3 * 6 * VI->HLW;
    log_hw_reg("Schedule DI%d event for %lld", index, time);

    // reschedule event
    if (VI->DI_event[index].active) {
        reschedule_event(VI->system->scheduler, &VI->DI_event[index], time);
    }
    else {
        VI->DI_event[index].time = time;
        add_event(VI->system->scheduler, &VI->DI_event[index]);
    }
}

HW_REG_WRITE_CALLBACK(write_VI_HTR0, VI) {
    VI->HLW = READ16(VI->regs, VI_reg_HTR0) & 0x1ff;

    // change event to now to reset it
    reschedule_event(VI->system->scheduler, &VI->halfline_count_event, get_time(VI->system->scheduler));

    // Display Interrupt events might have changed
    for (int i = 0; i < 4; i++) {
        if (VI->DI_event[i].active) {
            // only reschedule if active
            schedule_DI_event(VI, i);
        }
    }
}

static inline void write_VI_DIx(s_VI* VI, const u32 index) {
    // check if an interrupt was active or not
    u32 old_data = VI->DI[index] & VI_DI_DATA;
    VI->DI[index] = READ32(VI->regs, VI_reg_DI0 + (index << 2));

    log_hw_reg("Wrote to VI.DI%d (%08x)", index, VI->DI[index]);

    // if its enabled, schedule or reschedule the event
    if (VI->DI[index] & VI_DI_ENABLED) {

        // only reschedule if the event was inactive or it was actually changed
        // a lot of writes are probably just acknowledgements
        if (!VI->DI_event[index].active || (old_data != (VI->DI[index] & VI_DI_DATA))) {
            log_cpu("Rescheduling DI%d event, cause inactive or %08x != %08x", index, old_data, VI->DI[index] & VI_DI_DATA);
            schedule_DI_event(VI, index);
        }
    }
    else if (VI->DI_event[index].active) {
        remove_event(VI->system->scheduler, &VI->DI_event[index]);
    }

    // clear VI interrupt from PI if none are active
    if (!(VI->DI[index] & VI_DI_ACTIVE)) {
        for (int i = 0 ; i < 4; i++) {
            // one of the interrupts is active still
            if (VI->DI[i] & VI_DI_ACTIVE) {
                return;
            }
        }
        clear_PI_intsr(VI->PI, PI_intr_VI);
    }
}

HW_REG_WRITE_CALLBACK(write_VI_DI0, VI) {
    write_VI_DIx(VI, 0);
}

HW_REG_WRITE_CALLBACK(write_VI_DI1, VI) {
    write_VI_DIx(VI, 1);
}

HW_REG_WRITE_CALLBACK(write_VI_DI2, VI) {
    write_VI_DIx(VI, 2);
}

HW_REG_WRITE_CALLBACK(write_VI_DI3, VI) {
    write_VI_DIx(VI, 3);
}

HW_REG_READ_PRECALL(read_VI_DI0, VI) {
    WRITE32(VI->regs, VI_reg_DI0, VI->DI[0]);
}

HW_REG_READ_PRECALL(read_VI_DI1, VI) {
    WRITE32(VI->regs, VI_reg_DI1, VI->DI[1]);
}

HW_REG_READ_PRECALL(read_VI_DI2, VI) {
    WRITE32(VI->regs, VI_reg_DI2, VI->DI[2]);
}

HW_REG_READ_PRECALL(read_VI_DI3, VI) {
    WRITE32(VI->regs, VI_reg_DI3, VI->DI[3]);
}

HW_REG_INIT_FUNCTION(VI) {
    VI->write[VI_reg_HTR0 >> VI_SHIFT] = write_VI_HTR0;
    // schedule event at time infinity at first, rescheduled when VI is configured properly
    VI->halfline_count_event = (s_event) {
            .callback = VI_halfline_count,
            .time = (u64)-1,
            .caller = VI
    };
    add_event(VI->system->scheduler, &VI->halfline_count_event);


    VI->write[VI_reg_DI0 >> VI_SHIFT] = write_VI_DI0;
    VI->read[VI_reg_DI0 >> VI_SHIFT] = read_VI_DI0;
    VI->write[VI_reg_DI1 >> VI_SHIFT] = write_VI_DI1;
    VI->read[VI_reg_DI1 >> VI_SHIFT] = read_VI_DI1;
    VI->write[VI_reg_DI2 >> VI_SHIFT] = read_VI_DI2;
    VI->read[VI_reg_DI2 >> VI_SHIFT] = read_VI_DI2;
    VI->write[VI_reg_DI3 >> VI_SHIFT] = read_VI_DI3;
    VI->read[VI_reg_DI3 >> VI_SHIFT] = read_VI_DI3;

    // these events are not scheduled by default
    VI->DI_event[0] = (s_event) { .callback = VI_DI0_intr, .caller = &VI->system->cpu };
    VI->DI_event[1] = (s_event) { .callback = VI_DI1_intr, .caller = &VI->system->cpu };
    VI->DI_event[2] = (s_event) { .callback = VI_DI2_intr, .caller = &VI->system->cpu };
    VI->DI_event[3] = (s_event) { .callback = VI_DI3_intr, .caller = &VI->system->cpu };
}