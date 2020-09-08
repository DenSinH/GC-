#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <stdbool.h>

#include "system.h"
#include "Gekko.h"
#include "sleeping.h"

#include "flags.h"

s_GameCube* init_system() {
    s_GameCube* GameCube = malloc(sizeof(s_GameCube));
    memset(GameCube, 0x00, sizeof(struct s_GameCube));
    GameCube->cpu.system = GameCube;
    GameCube->flipper.system = GameCube;
    init_Gekko(&GameCube->cpu);
    init_Flipper(&GameCube->flipper);

    GameCube->HW_regs.system = GameCube;
    init_HW_regs(&GameCube->HW_regs);
    GameCube->cpu.IMMU.HW_regs_ptr = GameCube->cpu.DMMU.HW_regs_ptr = &GameCube->HW_regs;

#ifdef DO_BREAKPOINTS
    // todo: overflow differs here:
//    add_breakpoint(&GameCube->breakpoints, 0x8000bde0);
//    add_breakpoint(&GameCube->breakpoints, 0x8000bde4);
    // todo: overflow differs here (CR2):
//    add_breakpoint(&GameCube->breakpoints, 0x8000e908);
//    add_breakpoint(&GameCube->breakpoints, 0x8000e90c);
    // todo: different word loaded from memory
//    add_breakpoint(&GameCube->breakpoints, 0x80011f90);
//    add_breakpoint(&GameCube->breakpoints, 0x80011f94);
#endif

//    add_breakpoint(&GameCube->breakpoints, 0x80003e38);
    return GameCube;
}

#define TEST_DOL "D:\\CProjects\\GCResources\\GameCubeResources\\Tests\\Triangle\\Triangle.dol"
// #define TEST_DOL "D:\\CProjects\\GCResources\\GameCubeResources\\Tests\\GCTests\\GCTests.dol"
#define STEP_ON_BREAK

void run_system(s_GameCube* system) {
    load_DOL_to_Gekko(&system->cpu, TEST_DOL);

#ifdef STEP_ON_BREAK
    bool step = false;
#endif

    while (!system->shutdown) {

        step_Gekko(&system->cpu);
        if ((system->cpu.PC & 0x0fffffff) > 0x01800000) {
            log_fatal("Jumped to invalid address: %08x", system->cpu.PC);
        }

#if defined(DO_BREAKPOINTS) || defined(DO_DEBUGGER)
        if (check_breakpoints(&system->breakpoints, system->cpu.PC)) {
            dump_Gekko(&system->cpu);
            log_debug("Hit breakpoint %08x", GameCube->cpu.PC);
            system->paused = true;
        }
#endif

#ifdef DO_DEBUGGER
        while (system->paused && (system->stepcount == 0) && !system->shutdown) {
            sleep_ms(16);
        }

        if (system->stepcount > 0) {
            system->stepcount--;
        }
#endif
    }
}
