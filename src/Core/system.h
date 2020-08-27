#ifndef GC__SYSTEM_H
#define GC__SYSTEM_H

#include "PPC/Gekko.h"

#include "default.h"
#include "flags.h"

#ifdef DO_BREAKPOINTS
    #include "Breakpoints/breakpoints.h"
#endif

typedef struct s_GameCube {
    s_Gekko cpu;

    bool shutdown;

#ifdef DO_BREAKPOINTS
    s_breakpoints breakpoints;
#endif

#ifdef DO_DEBUGGER
    bool paused;
    u32 stepcount;
#endif

} s_GameCube ;

s_GameCube* init_system();
void run_system(s_GameCube* GameCube);

#endif //GC__SYSTEM_H
