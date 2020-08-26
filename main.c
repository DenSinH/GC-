#include "debugthread.h"

#include <stdio.h>
#include <stdlib.h>

#include "init.c"

void exception_handler() {
    format_Gekko(&global_system->cpu);
    printf("[CORE DUMP]:\n%s\n", global_system->cpu.log_line);
#ifdef DO_CALL_STACK
    dump_Gekko_stack_trace(&global_system->cpu);
    printf("[CALL STACK]: (%d calls)\n%s", global_system->cpu.call_stack_pointer, global_system->cpu.log_line);
#endif

    dump_Gekko_mem_range(&global_system->cpu, 0, 0x8000);
}

int main() {
    init();

    atexit(exception_handler);

    START_DEBUG

    run_system(global_system);

    return 0;
}
