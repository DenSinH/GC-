#include <stdio.h>

#include "src/Core/system.h"

int main() {
    s_GameCube* GameCube = init_system();
    run_system(GameCube);

    return 0;
}
