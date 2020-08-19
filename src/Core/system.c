#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <stdbool.h>

#include "system.h"
#include "PPC/Gekko.h"

s_GameCube* init_system() {
    s_GameCube* GameCube = malloc(sizeof(s_GameCube));
    memset(GameCube, 0x00, sizeof(struct s_GameCube));
    init_Gekko(&GameCube->cpu);

    return GameCube;
}

void run_system(s_GameCube* GameCube) {
    load_IPL_to_Gekko(&GameCube->cpu);

    while (true) {
        step_Gekko(&GameCube->cpu);
        getchar();
    }
}