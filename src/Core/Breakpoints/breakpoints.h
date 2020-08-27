#ifndef GC__BREAKPOINTS_H
#define GC__BREAKPOINTS_H

#include "default.h"

#include <stdbool.h>

#define MAX_BREAKPOINTS 0x100

typedef struct s_breakpoints {
    u32 breakpoints[MAX_BREAKPOINTS];
    size_t number_of_breakpoints;
} s_breakpoints;

void add_breakpoint(s_breakpoints* breakpoints, u32 value);
void remove_breakpoint(s_breakpoints* breakpoints, u32 value);
bool check_breakpoints(s_breakpoints* breakpoints, u32 value);

#endif //GC__BREAKPOINTS_H
