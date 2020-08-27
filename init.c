#include "src/Core/system.h"
#include "src/Debugger/debugger.h"

#include "debugging.h"
#include "default.h"


s_GameCube* global_system;

CONSOLE_COMMAND(reset_system) {
    if (argc > 1 && (strcmp(args[1], "freeze") || strcmp(args[1], "pause")|| strcmp(args[1], "break"))) {
        global_system->paused = true;
    }

    load_DOL_to_Gekko(&global_system->cpu, global_system->cpu.dol_file_name);
    strcpy_s(output, MAX_OUTPUT_LENGTH, "Reloaded DOL file");
}

CONSOLE_COMMAND(pause_system) {
    global_system->paused = true;

    strcpy_s(output, MAX_OUTPUT_LENGTH, "System paused");
}

CONSOLE_COMMAND(unpause_system) {
    global_system->paused = false;

    strcpy_s(output, MAX_OUTPUT_LENGTH, "System unpaused");
}

CONSOLE_COMMAND(break_system) {
    if (argc < 2) {
        pause_system(args, argc, output);
        return;
    }

    u32 breakpoint = parsehex(args[1]);
    add_breakpoint(&global_system->breakpoints, breakpoint);
    sprintf_s(output, MAX_OUTPUT_LENGTH, "Added breakpoint at %08x", breakpoint);
}

CONSOLE_COMMAND(unbreak_system) {
    if (argc < 2) {
        unpause_system(args, argc, output);
        return;
    }

    u32 breakpoint = parsehex(args[1]);
    remove_breakpoint(&global_system->breakpoints, breakpoint);
    sprintf_s(output, MAX_OUTPUT_LENGTH, "Removed breakpoint at %08x", breakpoint);
}

CONSOLE_COMMAND(step_system) {
    global_system->paused = true;
    if (argc < 2) {
        global_system->stepcount = 1;
        strcpy_s(output, MAX_OUTPUT_LENGTH, "Stepping system for one step");
    }
    else {
        u32 steps = parsedec(args[1]);
        global_system->stepcount = steps;
        sprintf_s(output, MAX_OUTPUT_LENGTH, "Stepping system for %d steps", steps);
    }
}

CONSOLE_COMMAND(dump_memory_range) {
    if (argc < 2) {
        strcpy_s(output, MAX_OUTPUT_LENGTH, "Provide an address/range to dump from");
    }
    else if (argc < 3) {
        u32 address = parsehex(args[1]) & (~3);
        sprintf_s(output, MAX_OUTPUT_LENGTH, "Value at %08x: %08x", address, get_word(&global_system->cpu.DMMU, address));
    }
    else {
        u32 start = parsehex(args[1]) & (~3);
        u32 end = parsehex(args[2]) & (~3);
        dump_range(&global_system->cpu.DMMU, start, end);
        strcpy_s(output, MAX_OUTPUT_LENGTH, "Dumped range to console");
    }
}

CONSOLE_COMMAND(get_state) {
    dump_Gekko(&global_system->cpu);
}


void init() {
    global_system = init_system();

    debugger_init(&global_system->shutdown);

    char name[32];

    add_register_data("PC", &global_system->cpu.PC, false);
    add_register_data("LR", &global_system->cpu.LR, false);
    add_register_data("SP", &global_system->cpu.SP, false);
    add_register_data("CR", &global_system->cpu.CR, false);

    add_register_data("MSR", &global_system->cpu.MSR, false);
    add_register_data("XER", &global_system->cpu.XER, false);
    add_register_data("CTR", &global_system->cpu.CTR, false);
    add_register_data("FPSCR", &global_system->cpu.FPSCR, false);

    add_register_data("", NULL, false);

    for (int i = 0; i < 32; i++) {
        sprintf(name, "GPR%02d", i);
        add_register_data(name, &global_system->cpu.GPR[i], false);
    }

    add_register_data("", NULL, false);

    for (int i = 0; i < 8; i++) {
        sprintf(name, "GQR%02d", i);
        add_register_data(name, &global_system->cpu.GQR[i], false);
    }

    add_register_data("", NULL, false);

    for (int i = 0; i < 32; i++) {
        sprintf(name, "FPR%02d:SPR0", i);
        add_register_data(name, &global_system->cpu.FPR[i].PS0, true);
        sprintf(name, "FPR%02d:SPR1", i);
        add_register_data(name, &global_system->cpu.FPR[i].PS1, true);
    }

    add_command("RESET", "Resets the system. Add 'pause/freeze/break' to freeze on reload.", reset_system);
    add_command("PAUSE", "Pauses the system.", pause_system);
    add_command("CONTINUE", "Unpauses the system.", unpause_system);
    add_command("BREAK", "Add breakpoint to system at PC = $1.", break_system);
    add_command("UNBREAK", "Remove breakpoint to system at PC = $1.", unbreak_system);
    add_command("STEP", "Step system for $1 CPU steps (defaults to 1 step).", step_system);
    add_command("MEMORY", "Read word(s) from memory between $1 and $2", dump_memory_range);
    add_command("STATE", "Dump the current register state to the console", get_state);
}