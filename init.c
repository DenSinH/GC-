#include "init.h"
#include "src/Core/PPC/MMU.h"
#include "src/Frontend/interface.h"

#include "debugging.h"
#include "default.h"


s_GameCube* global_system;

static CONSOLE_COMMAND(reset_system) {
#ifdef DO_DEBUGGER
    if (argc > 1 && (
            strcmp(args[1], "freeze") != 0 ||
            strcmp(args[1], "pause")  != 0 ||
            strcmp(args[1], "break")  != 0
            )
        ) {
        global_system->paused = true;
    }

    load_DOL_to_Gekko(&global_system->cpu, global_system->cpu.dol_file_name);
    STRCPY(output, MAX_OUTPUT_LENGTH, "Reloaded DOL file");
#endif
}

static CONSOLE_COMMAND(pause_system) {
#ifdef DO_DEBUGGER
    global_system->paused = true;

    STRCPY(output, MAX_OUTPUT_LENGTH, "System paused");
#endif
}

static CONSOLE_COMMAND(unpause_system) {
#ifdef DO_DEBUGGER
    global_system->paused = false;

    STRCPY(output, MAX_OUTPUT_LENGTH, "System unpaused");
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
    SPRINTF(output, MAX_OUTPUT_LENGTH, "Added breakpoint at %08x", breakpoint);
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
    SPRINTF(output, MAX_OUTPUT_LENGTH, "Removed breakpoint at %08x", breakpoint);
#endif
}

static CONSOLE_COMMAND(step_system) {
#ifdef DO_DEBUGGER
    global_system->paused = true;
    if (argc < 2) {
        global_system->stepcount = 1;
        STRCPY(output, MAX_OUTPUT_LENGTH, "Stepping system for one step");
    }
    else {
        u32 steps = parsedec(args[1]);
        global_system->stepcount = steps;
        SPRINTF(output, MAX_OUTPUT_LENGTH, "Stepping system for %d steps", steps);
    }
#endif
}

static CONSOLE_COMMAND(dump_memory_range) {
    if (argc < 2) {
        STRCPY(output, MAX_OUTPUT_LENGTH, "Provide an address/range to dump from");
    }
    else if (argc < 3) {
        u32 address = parsehex(args[1]) & (~3);
        SPRINTF(output, MAX_OUTPUT_LENGTH, "Value at %08x: %08x", address, get_word(&global_system->cpu.DMMU, address));
    }
    else {
        u32 start = parsehex(args[1]) & (~3);
        u32 end = parsehex(args[2]) & (~3);
        dump_range(&global_system->cpu.DMMU, start, end);
        STRCPY(output, MAX_OUTPUT_LENGTH, "Dumped range to console");
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

static void frontend_video_init() {
    video_init_Flipper(&global_system->flipper);
}

static s_framebuffer frontend_render() {
    return render_Flipper(&global_system->flipper);
}

static void parse_input(s_controller* controller) {
    if (controller->A) {
        printf("A button pressed!\n");
    }
}

s_GameCube* init() {
    global_system = init_system();

    bind_video_init(frontend_video_init);
    bind_video_render(frontend_render);

    frontend_init(
            &global_system->shutdown,
            &global_system->cpu.PC,
            global_system->cpu.DMMU.memory_ptr,
            0x100000000,
            valid_address_check,
            (u64*)&global_system->cpu.TBR,
            view_byte,
            parse_input
            );

    char name[32];

    int Gekko_tab = add_register_tab("Gekko");

    add_register_data("PC", &global_system->cpu.PC, 4, Gekko_tab);
    add_register_data("LR", &global_system->cpu.LR, 4, Gekko_tab);
    add_register_data("SP", &global_system->cpu.SP, 4, Gekko_tab);
    add_register_data("CR", &global_system->cpu.CR, 4, Gekko_tab);

    add_register_data("MSR", &global_system->cpu.MSR, 4, Gekko_tab);
    add_register_data("XER", &global_system->cpu.XER, 4, Gekko_tab);
    add_register_data("CTR", &global_system->cpu.CTR, 4, Gekko_tab);
    add_register_data("FPSCR", &global_system->cpu.FPSCR, 4, Gekko_tab);

    add_register_data("", NULL, 4, Gekko_tab);

    for (int i = 0; i < 32; i++) {
        sprintf(name, "GPR%02d", i);
        add_register_data(name, &global_system->cpu.GPR[i], 4, Gekko_tab);
    }

    add_register_data("", NULL, 4, Gekko_tab);

    add_register_data("HID0", &global_system->cpu.HID[0], 4, Gekko_tab);
    add_register_data("HID1", &global_system->cpu.HID[1], 4, Gekko_tab);
    add_register_data("HID2", &global_system->cpu.HID2, 4, Gekko_tab);
    add_register_data("WPAR", &global_system->cpu.WPAR, 4, Gekko_tab);

    add_register_data("DEC", &global_system->cpu.DEC, 4, Gekko_tab);
    add_register_data("TBR", &global_system->cpu.TBR, 4, Gekko_tab);  // technically not enough bits, but eh
    add_register_data("SRR0", &global_system->cpu.SRR0, 4, Gekko_tab);
    add_register_data("SRR1", &global_system->cpu.SRR1, 4, Gekko_tab);

    add_register_data("", NULL, 4, Gekko_tab);

    for (int i = 0; i < 8; i++) {
        sprintf(name, "GQR%02d", i);
        add_register_data(name, &global_system->cpu.GQR[i], 4, Gekko_tab);
    }

    add_register_data("", NULL, 4, Gekko_tab);

    for (int i = 0; i < 32; i++) {
        sprintf(name, "FPR%02d:PS0", i);
        add_register_data(name, &global_system->cpu.FPR[i].PS0, 8, Gekko_tab);
        sprintf(name, "FPR%02d:PS1", i);
        add_register_data(name, &global_system->cpu.FPR[i].PS1, 8, Gekko_tab);
    }

    add_register_data("", NULL, 4, Gekko_tab);

    for (int i = 0; i < 4; i++) {
        sprintf(name, "IBAT%02d", i);
        add_register_data(name, &global_system->cpu.IMMU.BAT[i], 8, Gekko_tab);
    }

    for (int i = 0; i < 4; i++) {
        sprintf(name, "DBAT%02d", i);
        add_register_data(name, &global_system->cpu.DMMU.BAT[i], 8, Gekko_tab);
    }

    add_register_data("", NULL, 4, Gekko_tab);

    for (int i = 0; i < 16; i++) {
        sprintf(name, "SR%02d", i);
        add_register_data(name, &global_system->cpu.SR[i], 4, Gekko_tab);
    }

    int HWIO_tab = add_register_tab("HWIO");
    for (int i = 0; i < 0x80; i++) {
        sprintf(name, "CP%02x", i);
        add_register_data(name, &global_system->HW_regs.CP.regs[i], 1, HWIO_tab);
    }

    add_register_data("", NULL, 1, HWIO_tab);
    add_register_data("", NULL, 1, HWIO_tab);

    for (int i = 0; i < 0x100; i++) {
        sprintf(name, "PE%02x", i);
        add_register_data(name, &global_system->HW_regs.PE.regs[i], 1, HWIO_tab);
    }

    add_register_data("", NULL, 1, HWIO_tab);
    add_register_data("", NULL, 1, HWIO_tab);

    for (int i = 0; i < 0x100; i++) {
        sprintf(name, "VI%02x", i);
        add_register_data(name, &global_system->HW_regs.VI.regs[i], 1, HWIO_tab);
    }

    add_register_data("", NULL, 1, HWIO_tab);

    sprintf(name, "VI HLW");
    add_register_data(name, &global_system->HW_regs.VI.HLW, 4, HWIO_tab);

    sprintf(name, "VI HLCount");
    add_register_data(name, &global_system->HW_regs.VI.current_halfline, 4, HWIO_tab);

    sprintf(name, "VI DI0");
    add_register_data(name, &global_system->HW_regs.VI.DI[0], 4, HWIO_tab);

    sprintf(name, "VI DI1");
    add_register_data(name, &global_system->HW_regs.VI.DI[1], 4, HWIO_tab);

    add_register_data("", NULL, 1, HWIO_tab);
    add_register_data("", NULL, 1, HWIO_tab);

    for (int i = 0; i < 0x100; i++) {
        sprintf(name, "PI%02x", i);
        add_register_data(name, &global_system->HW_regs.PI.regs[i], 1, HWIO_tab);
    }

    add_register_data("", NULL, 1, HWIO_tab);

    sprintf(name, "PI INTSR");
    add_register_data(name, &global_system->HW_regs.PI.INTSR, 4, HWIO_tab);

    sprintf(name, "PI INTMR");
    add_register_data(name, &global_system->HW_regs.PI.INTMR, 4, HWIO_tab);

    add_register_data("", NULL, 1, HWIO_tab);
    add_register_data("", NULL, 1, HWIO_tab);

    for (int i = 0; i < 0x80; i++) {
        sprintf(name, "MI%02x", i);
        add_register_data(name, &global_system->HW_regs.MI.regs[i], 1, HWIO_tab);
    }

    add_register_data("", NULL, 1, HWIO_tab);
    add_register_data("", NULL, 1, HWIO_tab);

    for (int i = 0; i < 0x200; i++) {
        sprintf(name, "DSP%02x", i);
        add_register_data(name, &global_system->HW_regs.DSP.regs[i], 1, HWIO_tab);
    }

    add_register_data("", NULL, 1, HWIO_tab);
    add_register_data("", NULL, 1, HWIO_tab);

    for (int i = 0; i < 0x40; i++) {
        sprintf(name, "DI%02x", i);
        add_register_data(name, &global_system->HW_regs.DI.regs[i], 1, HWIO_tab);
    }

    add_register_data("", NULL, 1, HWIO_tab);
    add_register_data("", NULL, 1, HWIO_tab);

    for (int i = 0; i < 0x100; i++) {
        sprintf(name, "SI%02x", i);
        add_register_data(name, &global_system->HW_regs.SI.regs[i], 1, HWIO_tab);
    }

    add_register_data("", NULL, 1, HWIO_tab);
    add_register_data("", NULL, 1, HWIO_tab);

    for (int i = 0; i < 0x40; i++) {
        sprintf(name, "EI%02x", i);
        add_register_data(name, &global_system->HW_regs.EXI.regs[i], 1, HWIO_tab);
    }

    add_register_data("", NULL, 1, HWIO_tab);
    add_register_data("", NULL, 1, HWIO_tab);

    for (int i = 0; i < 0x20; i++) {
        sprintf(name, "AI%02x", i);
        add_register_data(name, &global_system->HW_regs.AI.regs[i], 1, HWIO_tab);
    }

    int CP_tab = add_register_tab("internal CP");

    for (int i = 0; i < INTERNAL_CP_REGISTER_SIZE; i++) {
        sprintf(name, "CP 0x%02x", INTERNAL_CP_REGISTER_BASE + i);
        add_register_data(name, &global_system->HW_regs.CP.internal_CP_regs[i], 4, CP_tab);
    }

    int BP_tab = add_register_tab("BP regs");

    for (int i = 0; i < INTERNAL_BP_REGISTER_SIZE; i++) {
        sprintf(name, "BP 0x%02x", i);
        add_register_data(name, &global_system->HW_regs.CP.internal_BP_regs[i], 4, BP_tab);
    }

    int XF_reg_tab = add_register_tab("XF regs");

    for (int i = 0; i < INTERNAL_XF_REGISTER_SIZE; i++) {
        sprintf(name, "XF 0x%02x", INTERNAL_XF_REGISTER_BASE + i);
        add_register_data(name, &global_system->HW_regs.CP.internal_XF_regs[i], 4, XF_reg_tab);
    }

    int XF_mem_tab = add_register_tab("XF mem");

    for (int i = 0; i < 0x100; i++) {
        sprintf(name, "XFA[0x%02x]", i);
        add_register_data(name, &global_system->HW_regs.CP.internal_XF_mem[0][i], 4, XF_mem_tab);
    }

    add_register_data("", NULL, 4, XF_mem_tab);
    add_register_data("", NULL, 4, XF_mem_tab);

    for (int i = 0; i < 0x60; i++) {
        sprintf(name, "XFB[0x%02x]", i);
        add_register_data(name, &global_system->HW_regs.CP.internal_XF_mem[1][i], 4, XF_mem_tab);
    }

    add_register_data("", NULL, 4, XF_mem_tab);
    add_register_data("", NULL, 4, XF_mem_tab);

    for (int i = 0; i < 0x100; i++) {
        sprintf(name, "XFC[0x%02x]", i);
        add_register_data(name, &global_system->HW_regs.CP.internal_XF_mem[2][i], 4, XF_mem_tab);
    }

    add_register_data("", NULL, 4, XF_mem_tab);
    add_register_data("", NULL, 4, XF_mem_tab);

    for (int i = 0; i < 0x80; i++) {
        sprintf(name, "XFD[0x%02x]", i);
        add_register_data(name, &global_system->HW_regs.CP.internal_XF_mem[3][i], 4, XF_mem_tab);
    }

    add_command("RESET", "Resets the system. Add 'pause/freeze/break' to freeze on reload.", reset_system);
    add_command("PAUSE", "Pauses the system.", pause_system);
    add_command("CONTINUE", "Unpauses the system.", unpause_system);
    add_command("BREAK", "Add breakpoint to system at PC = $1.", break_system);
    add_command("UNBREAK", "Remove breakpoint to system at PC = $1.", unbreak_system);
    add_command("STEP", "Step system for $1 CPU steps (defaults to 1 step).", step_system);
    add_command("MEMORY", "Read word(s) from memory between $1 and $2", dump_memory_range);
    add_command("STATE", "Dump the current register state to the console", get_state);

    return global_system;
}