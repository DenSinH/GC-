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
//    add_breakpoint(&GameCube->breakpoints, 0x80008638);
//    add_breakpoint(&GameCube->breakpoints, 0x00000500);
//    add_breakpoint(&GameCube->breakpoints, 0x8000586c);
#endif
    return GameCube;
}

#ifdef _WIN32
#define ROOT_DIR "D:/Data/CProjects/GameCubeResources/Tests/"
#else
#define ROOT_DIR "/mnt/d/"
#endif

// #define TEST_DOL ROOT_DIR "Cube/Cube.dol"
// #define TEST_DOL ROOT_DIR "Textures/Textures.dol"
#define TEST_DOL ROOT_DIR "Sprites/Sprites.dol"

void run_system(s_GameCube* system) {
    load_DOL_to_Gekko(&system->cpu, TEST_DOL);

    while (!system->shutdown) {

        step_Gekko(&system->cpu);
        if ((system->cpu.PC & 0x0fffffff) > 0x01800000) {
            log_fatal("Jumped to invalid address: %08x", system->cpu.PC);
        }

        if (should_do_events(&system->scheduler, system->cpu.TBR.raw)) {
            do_events(&system->scheduler, system->cpu.TBR.raw);
        }

#if defined(DO_BREAKPOINTS) || defined(DO_DEBUGGER)
        if (check_breakpoints(&system->breakpoints, system->cpu.PC)) {
            log_debug("Hit breakpoint %08x", system->cpu.PC);
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
