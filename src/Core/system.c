#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <stdbool.h>

#include "system.h"
#include "Gekko.h"
#include "sleeping.h"
#include "const.h"

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

    GameCube->scheduler.timer = &GameCube->cpu.TBR.raw;

#ifdef DO_BREAKPOINTS
//    add_breakpoint(&GameCube->breakpoints, 0x80008638);
//    add_breakpoint(&GameCube->breakpoints, 0x00000500);
//    add_breakpoint(&GameCube->breakpoints, 0x8000586c);
#endif
    return GameCube;
}

void run_system(s_GameCube* system) {
    load_DOL_to_Gekko(&system->cpu, TEST_DOL);

    while (!system->shutdown) {

        step_Gekko(&system->cpu);
        if ((system->cpu.PC & 0x0fffffff) > 0x01800000) {
            log_fatal("Jumped to invalid address: %08x", system->cpu.PC);
        }

        if (should_do_events(&system->scheduler)) {
            do_events(&system->scheduler);
        }

#if defined(DO_BREAKPOINTS) || defined(DO_DEBUGGER)
        if (check_breakpoints(&system->breakpoints, system->cpu.PC)) {
            log_debug("Hit breakpoint %08x", system->cpu.PC);
            system->paused = true;
        }
#endif

#ifdef DO_DEBUGGER
        while (system->paused && (system->stepcount == 0) && !system->shutdown) {
            // sleep for a bit to not have a busy wait. This is for debugging anyway, and we are frozen
            // so it's okay if it's not instant
            sleep_ms(16);
        }

        if (system->stepcount > 0) {
            system->stepcount--;
        }
#endif
    }

    destroy_system(system);
}

void destroy_system(s_GameCube* system) {
    destroy_CP(&system->HW_regs.CP);
}