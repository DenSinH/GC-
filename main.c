#include <stdio.h>
#include <stdlib.h>

#include "default.h"
#include "src/Core/system.h"

s_GameCube* global_system;

void exception_handler() {
    format_Gekko(&global_system->cpu);
    printf("[CORE DUMP]:\n%s", global_system->cpu.log_line);
}

int main() {
    atexit(exception_handler);

    global_system = init_system();
    run_system(global_system);

    return 0;
}
