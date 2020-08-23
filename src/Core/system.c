#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <stdbool.h>

#include "system.h"
#include "PPC/Gekko.h"

#include "flags.h"

s_GameCube* init_system() {
    s_GameCube* GameCube = malloc(sizeof(s_GameCube));
    memset(GameCube, 0x00, sizeof(struct s_GameCube));
    init_Gekko(&GameCube->cpu);

#ifdef DO_BREAKPOINTS
    // todo: overflow differs here:
//    add_breakpoint(&GameCube->breakpoints, 0x8000bde0);
//    add_breakpoint(&GameCube->breakpoints, 0x8000bde4);
    // todo: overflow differs here (CR2):
//    add_breakpoint(&GameCube->breakpoints, 0x8000e908);
//    add_breakpoint(&GameCube->breakpoints, 0x8000e90c);

    add_breakpoint(&GameCube->breakpoints, 0x80006ed4);
    add_breakpoint(&GameCube->breakpoints, 0x80006ed8);
    add_breakpoint(&GameCube->breakpoints, 0x80006edc);
    add_breakpoint(&GameCube->breakpoints, 0x80006ee0);
    add_breakpoint(&GameCube->breakpoints, 0x80006ee4);
    add_breakpoint(&GameCube->breakpoints, 0x80006ee8);
    add_breakpoint(&GameCube->breakpoints, 0x80006eec);
#endif

    return GameCube;
}

#define test_DOL "D:\\CodeBlocks\\Projects\\GCHBTest\\GCHBTest.dol"

void run_system(s_GameCube* GameCube) {
    load_DOL_to_Gekko(test_DOL, &GameCube->cpu);

    while (true) {
        if (GameCube->cpu.GPR[3] == 0x00313131) {
            dump_Gekko(&GameCube->cpu);
            getchar();
        }

#ifdef DO_BREAKPOINTS
        if (check_breakpoints(&GameCube->breakpoints, GameCube->cpu.PC)) {
            format_Gekko(&GameCube->cpu);
            log_debug("%s", GameCube->cpu.log_line);
            log_debug("Hit breakpoint %08x", GameCube->cpu.PC);
            getchar();
        }
#endif

        step_Gekko(&GameCube->cpu);
        if ((GameCube->cpu.PC & 0x0fffffff) > 0x01800000) {
            log_fatal("Jumped to invalid address: %08x", GameCube->cpu.PC);
        }
    }
}