#include "default.h"
#include "src/Core/system.h"
#include "src/Debugger/debugger.h"


s_GameCube* global_system;

CONSOLE_COMMAND(pause) {

    for (int i = 0; i < argc; i++) {
        printf("[arg] %s\n", args[i]);
    }

    global_system->paused ^= true;

    return "System paused";
}

void init() {
    global_system = init_system();

    debugger_init(&global_system->shutdown);

    add_command("PAUSE", "Pauses the system", pause);
}