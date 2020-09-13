#ifndef GC__SCHEDULER_H
#define GC__SCHEDULER_H

#include "default.h"

#define SCHEDULER_EVENT(name) void name(void* caller, struct s_event* event)
#define SCHEDULER_MAX_EVENTS 64

typedef struct s_event {
    SCHEDULER_EVENT((*callback));
    void* caller;
    u64 time;
} s_event;

typedef struct s_scheduler {
    s_event* events[SCHEDULER_MAX_EVENTS];
    size_t count;
} s_scheduler;

/*
 * Idea: make a heap structure, add events sorted on time, remove events sorted on time, then check if callback is equal
 * (function pointers can be compared anyway)
 * */
void add_event(s_scheduler* scheduler, s_event* event);
s_event* pop_event(s_scheduler* scheduler);
void remove_event(s_scheduler* scheduler, s_event* event);
void reschedule_event(s_scheduler* scheduler, s_event* event, u64 new_time);
void change_event(s_scheduler* scheduler, s_event* event, u64 new_time);
void delay_event_by(s_scheduler* scheduler, s_event* event, uint64_t dt);
void hasten_event_by(s_scheduler* scheduler, s_event* event, uint64_t dt);
void do_events(s_scheduler* scheduler, uint64_t time);

#endif //GC__SCHEDULER_H
