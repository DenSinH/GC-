#include "src/Core/system.h"
#include "src/Core/PPC/MMU.h"
#include "src/Debugger/debugger.h"

#include "debugging.h"
#include "default.h"


s_GameCube* global_system;

static CONSOLE_COMMAND(reset_system) {
#ifdef DO_DEBUGGER
    if (argc > 1 && (strcmp(args[1], "freeze") || strcmp(args[1], "pause")|| strcmp(args[1], "break"))) {
        global_system->paused = true;
    }

    load_DOL_to_Gekko(&global_system->cpu, global_system->cpu.dol_file_name);
    strcpy_s(output, MAX_OUTPUT_LENGTH, "Reloaded DOL file");
#endif
}

static CONSOLE_COMMAND(pause_system) {
#ifdef DO_DEBUGGER
    global_system->paused = true;

    strcpy_s(output, MAX_OUTPUT_LENGTH, "System paused");
#endif
}

static CONSOLE_COMMAND(unpause_system) {
#ifdef DO_DEBUGGER
    global_system->paused = false;

    strcpy_s(output, MAX_OUTPUT_LENGTH, "System unpaused");
#endif
}

static CONSOLE_COMMAND(break_system) {
#ifdef DO_DEBUGGER
    if (argc < 2) {
        pause_system(args, argc, output);
        return;
    }

    u32 breakpoint = parsehex(args[1]);
    add_breakpoint(&global_system->breakpoints, breakpoint);
    sprintf_s(output, MAX_OUTPUT_LENGTH, "Added breakpoint at %08x", breakpoint);
#endif
}

static CONSOLE_COMMAND(unbreak_system) {
#ifdef DO_DEBUGGER
    if (argc < 2) {
        unpause_system(args, argc, output);
        return;
    }

    u32 breakpoint = parsehex(args[1]);
    remove_breakpoint(&global_system->breakpoints, breakpoint);
    sprintf_s(output, MAX_OUTPUT_LENGTH, "Removed breakpoint at %08x", breakpoint);
#endif
}

static CONSOLE_COMMAND(step_system) {
#ifdef DO_DEBUGGER
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
#endif
}

static CONSOLE_COMMAND(dump_memory_range) {
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

static CONSOLE_COMMAND(get_state) {
    dump_Gekko(&global_system->cpu);
}

static u8 view_byte(const u8* memory, uint64_t off) {
    switch (off >> 20) {
        case 0x000 ... 0x017:
        case 0x800 ... 0x817:
        case 0xc00 ... 0xc17:
            return memory[MASK_24MB(off)];
        default:
            return 0;
    }
}

static u32 valid_address_check(u32 address) {
    switch (address >> 20) {
        case 0x000 ... 0x017:
        case 0x800 ... 0x817:
        case 0xc00 ... 0xc17:
            return MASK_24MB(address);
        default:
            return 0;
    }
}


static void init() {
    global_system = init_system();

    debugger_init(
            &global_system->shutdown,
            &global_system->cpu.PC,
            global_system->cpu.DMMU.memory_ptr,
            0x100000000,
            valid_address_check,
            (u64*)&global_system->cpu.TBR,
            view_byte
            );

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

    add_register_data("HID0", &global_system->cpu.HID[0], false);
    add_register_data("HID1", &global_system->cpu.HID[1], false);
    add_register_data("HID2", &global_system->cpu.HID2, false);
    add_register_data("WPAR", &global_system->cpu.WPAR, false);

    add_register_data("DEC", &global_system->cpu.DEC, false);
    add_register_data("SRR0", &global_system->cpu.SRR0, false);
    add_register_data("SRR1", &global_system->cpu.SRR1, false);

    add_register_data("", NULL, false);

    for (int i = 0; i < 8; i++) {
        sprintf(name, "GQR%02d", i);
        add_register_data(name, &global_system->cpu.GQR[i], false);
    }

    add_register_data("", NULL, false);

    for (int i = 0; i < 32; i++) {
        sprintf(name, "FPR%02d:PS0", i);
        add_register_data(name, &global_system->cpu.FPR[i].PS0, true);
        sprintf(name, "FPR%02d:PS1", i);
        add_register_data(name, &global_system->cpu.FPR[i].PS1, true);
    }

    add_register_data("", NULL, false);

    for (int i = 0; i < 4; i++) {
        sprintf(name, "IBAT%02d", i);
        add_register_data(name, &global_system->cpu.IMMU.BAT[i], true);
    }

    for (int i = 0; i < 4; i++) {
        sprintf(name, "DBAT%02d", i);
        add_register_data(name, &global_system->cpu.DMMU.BAT[i], true);
    }

    add_register_data("", NULL, false);

    for (int i = 0; i < 16; i++) {
        sprintf(name, "SR%02d", i);
        add_register_data(name, &global_system->cpu.SR[i], false);
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