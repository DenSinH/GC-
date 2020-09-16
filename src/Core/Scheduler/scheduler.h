#ifndef GC__SCHEDULER_H
#define GC__SCHEDULER_H

#include "default.h"
#include <stdbool.h>

#define SCHEDULER_EVENT(name) void name(void* caller, struct s_event* event)
#define SCHEDULER_MAX_EVENTS 64

// todo: string field in event to view top event name in debugger?
typedef struct s_event {
    bool active;   // signifies if event is in the scheduler or not
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
s_event* pop_event(s_scheduler* scheduler);  // mostly only used for debugging the scheduler
void remove_event(s_scheduler* scheduler, s_event* event);
void reschedule_event(s_scheduler* scheduler, s_event* event, u64 new_time);
void change_event(s_scheduler* scheduler, s_event* event, u64 new_time);
void delay_event_by(s_scheduler* scheduler, s_event* event, uint64_t dt);
void hasten_event_by(s_scheduler* scheduler, s_event* event, uint64_t dt);
void do_events(s_scheduler* scheduler, uint64_t time);

static inline bool should_do_events(s_scheduler* scheduler, uint64_t time) {
    return scheduler->events[0]->time < time;
}

#endif //GC__SCHEDULER_H
