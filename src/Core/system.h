#ifndef GC__SYSTEM_H
#define GC__SYSTEM_H

#include "PPC/Gekko.h"
#include "Flipper/Flipper.h"

#include "default.h"
#include "flags.h"

#include "Scheduler/scheduler.h"
#include "Breakpoints/breakpoints.h"

typedef struct s_GameCube {
    u8 memory[0x1800000];
    s_Gekko cpu;
    s_Flipper flipper;
    s_hardware_registers HW_regs;

    s_scheduler* scheduler;

    bool shutdown;

#if defined(DO_BREAKPOINTS) || defined(DO_DEBUGGER)
    s_breakpoints breakpoints;
#endif

#ifdef DO_DEBUGGER
    bool paused;
    u32 stepcount;
#endif

} s_GameCube ;

s_GameCube* init_system(void);
void destroy_system(s_GameCube* system);
void run_system(s_GameCube* system);

#endif //GC__SYSTEM_H
