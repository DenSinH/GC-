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
    add_breakpoint(&GameCube->breakpoints, 0x80012d1c);
    add_breakpoint(&GameCube->breakpoints, 0x80012d20);
#endif

    return GameCube;
}

#define test_DOL "D:\\CodeBlocks\\Projects\\GCHBTest\\GCHBTest.dol"

void run_system(s_GameCube* GameCube) {
    load_DOL_to_Gekko(test_DOL, &GameCube->cpu);

    while (true) {
        step_Gekko(&GameCube->cpu);
        if (GameCube->cpu.PC > 0x81800000) {
            log_fatal("Jumped to invalid address: %08x", GameCube->cpu.PC);
        }

#ifdef DO_BREAKPOINTS
        if (check_breakpoints(&GameCube->breakpoints, GameCube->cpu.PC)) {
            log_debug("Hit breakpoint %08x", GameCube->cpu.PC);
            getchar();
        }
#endif
    }
}