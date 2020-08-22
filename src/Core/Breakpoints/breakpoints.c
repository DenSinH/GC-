#include "breakpoints.h"

#include "log.h"

void add_breakpoint(s_breakpoints* breakpoints, u32 value) {
    if (breakpoints->number_of_breakpoints == MAX_BREAKPOINTS) {
        log_fatal("Max number of breakpoints reached");
    }

    breakpoints->breakpoints[breakpoints->number_of_breakpoints++] = value;
}

bool check_breakpoints(s_breakpoints* breakpoints, u32 value) {
    for (int i = 0; i < breakpoints->number_of_breakpoints; i++) {
        if (value == breakpoints->breakpoints[i]) {
            return value;
        }
    }
    return false;
}