#include "init.h"
#include "src/Core/PPC/MMU.h"
#include "src/Frontend/interface.h"

#include "debugging.h"
#include "default.h"


static s_GameCube* global_system;

static CONSOLE_COMMAND(reset_system) {
#ifdef DO_DEBUGGER
    if (argc > 1 && (
            strcmp(args[1], "freeze") == 0 ||
            strcmp(args[1], "pause")  == 0 ||
            strcmp(args[1], "break")  == 0
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
    if (argc < 2) {
        global_system->paused = true;
        global_system->stepcount = 1;
        STRCPY(output, MAX_OUTPUT_LENGTH, "Stepping system for one step");
    }
    else if (strcmp(args[1], "dsp") == 0) {
        global_system->paused = false;  // in this case, we want to keep running the system until it steps the DSP
        global_system->HW_regs.DSPI.step_DSP = true;
        SPRINTF(output, MAX_OUTPUT_LENGTH, "Stepping DSP");
    }
    else {
        global_system->paused = true;
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
            return global_system->memory[MASK_24MB(off)];
        case 0xd00 ... 0xd1f:
            // sneak ARAM in here (this is NOT actually the case)
            return global_system->HW_regs.DSPI.DSP.ARAM[off & 0x00ffffff];
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

static s_framebuffer frontend_render(uint32_t time_left) {
    return render_Flipper(&global_system->flipper, time_left);
}

static void frontend_destroy() {
    destroy_Flipper(&global_system->flipper);
}

static u64 ticks, prev_ticks;
static OVERLAY_INFO(cpu_ticks) {
    ticks = global_system->cpu.TBR.raw;
    SPRINTF(output, output_length, "CPU ticks/s: %.1f", (float)(ticks - prev_ticks) / delta_time);
    prev_ticks = ticks;
}

static float accum_time;
static OVERLAY_INFO(fps_counter) {
    accum_time += delta_time;
    SPRINTF(output, output_length, "FPS        : %.1f", (double)(global_system->flipper.frame) / accum_time);
    if (accum_time > 3) {
        global_system->flipper.frame = 10 * delta_time * (double)(global_system->flipper.frame) / accum_time;
        accum_time = 10 * delta_time;
    }
}

static void parse_input(s_controller* controller) {
    // currently only supporting one controller
    if (controller->A)     global_system->HW_regs.SI.gamepad[0].buttons |= button_A;
    if (controller->B)     global_system->HW_regs.SI.gamepad[0].buttons |= button_B;
    if (controller->X)     global_system->HW_regs.SI.gamepad[0].buttons |= button_X;
    if (controller->Y)     global_system->HW_regs.SI.gamepad[0].buttons |= button_Y;
    if (controller->start) global_system->HW_regs.SI.gamepad[0].buttons |= button_start;
    if (controller->left)  global_system->HW_regs.SI.gamepad[0].buttons |= button_left;
    if (controller->right) global_system->HW_regs.SI.gamepad[0].buttons |= button_right;
    if (controller->down)  global_system->HW_regs.SI.gamepad[0].buttons |= button_down;
    if (controller->up)    global_system->HW_regs.SI.gamepad[0].buttons |= button_up;

    // todo: this goes wrong for negative numbers
    global_system->HW_regs.SI.gamepad[0].stick_x = 0x80 + (controller->left_x >> 8);
    global_system->HW_regs.SI.gamepad[0].stick_y = 0x80 + (controller->left_y >> 8);
    global_system->HW_regs.SI.gamepad[0].C_x     = 0x80 + (controller->right_x >> 8);
    global_system->HW_regs.SI.gamepad[0].C_y     = 0x80 + (controller->right_y >> 8);
}

s_GameCube* init() {
    global_system = init_system();

    bind_video_init(frontend_video_init);
    bind_video_render(frontend_render);
    bind_video_destroy(frontend_destroy);

    frontend_init(
            &global_system->shutdown,
            &global_system->cpu.PC,
            global_system->cpu.DMMU.memory_ptr,
            0x100000000,
            valid_address_check,
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

    int DSP_tab = add_register_tab("DSP");
    add_register_data("ar0", &global_system->HW_regs.DSPI.DSP.ar[0], 2, DSP_tab);
    add_register_data("ar1", &global_system->HW_regs.DSPI.DSP.ar[1], 2, DSP_tab);
    add_register_data("ar2", &global_system->HW_regs.DSPI.DSP.ar[2], 2, DSP_tab);
    add_register_data("ar3", &global_system->HW_regs.DSPI.DSP.ar[3], 2, DSP_tab);

    add_register_data("ix0", &global_system->HW_regs.DSPI.DSP.ix[0], 2, DSP_tab);
    add_register_data("ix1", &global_system->HW_regs.DSPI.DSP.ix[1], 2, DSP_tab);
    add_register_data("ix2", &global_system->HW_regs.DSPI.DSP.ix[2], 2, DSP_tab);
    add_register_data("ix3", &global_system->HW_regs.DSPI.DSP.ix[3], 2, DSP_tab);

    add_register_data("wr0", &global_system->HW_regs.DSPI.DSP.wr[0], 2, DSP_tab);
    add_register_data("wr1", &global_system->HW_regs.DSPI.DSP.wr[1], 2, DSP_tab);
    add_register_data("wr2", &global_system->HW_regs.DSPI.DSP.wr[2], 2, DSP_tab);
    add_register_data("wr3", &global_system->HW_regs.DSPI.DSP.wr[3], 2, DSP_tab);

    add_register_data("st0", &global_system->HW_regs.DSPI.DSP.st[0], 2, DSP_tab);
    add_register_data("st1", &global_system->HW_regs.DSPI.DSP.st[1], 2, DSP_tab);
    add_register_data("st2", &global_system->HW_regs.DSPI.DSP.st[2], 2, DSP_tab);
    add_register_data("st3", &global_system->HW_regs.DSPI.DSP.st[3], 2, DSP_tab);

    add_register_data("sr", &global_system->HW_regs.DSPI.DSP.sr, 2, DSP_tab);
    add_register_data("config", &global_system->HW_regs.DSPI.DSP.config, 2, DSP_tab);

    add_register_data("ac0", &global_system->HW_regs.DSPI.DSP.ac[0], 8, DSP_tab);
    add_register_data("ac1", &global_system->HW_regs.DSPI.DSP.ac[1], 8, DSP_tab);
    add_register_data("prod", &global_system->HW_regs.DSPI.DSP.prod, 8, DSP_tab);

    add_register_data("ax0", &global_system->HW_regs.DSPI.DSP.ax[0], 4, DSP_tab);
    add_register_data("ax1", &global_system->HW_regs.DSPI.DSP.ax[1], 4, DSP_tab);

    add_register_data("", NULL, 2, DSP_tab);
    add_register_data("", NULL, 2, DSP_tab);

    add_register_data("DMBH", &global_system->HW_regs.DSPI.DSP.DMBH, 2, DSP_tab);
    add_register_data("DMBL", &global_system->HW_regs.DSPI.DSP.DMBL, 2, DSP_tab);
    add_register_data("CMBH", &global_system->HW_regs.DSPI.DSP.CMBH, 2, DSP_tab);
    add_register_data("CMBL", &global_system->HW_regs.DSPI.DSP.CMBL, 2, DSP_tab);

    add_register_data("DSMAH", &global_system->HW_regs.DSPI.DSP.DSMAH, 2, DSP_tab);
    add_register_data("DSMAL", &global_system->HW_regs.DSPI.DSP.DSMAL, 2, DSP_tab);
    add_register_data("DSPA", &global_system->HW_regs.DSPI.DSP.DSPA, 2, DSP_tab);
    add_register_data("DSCR", &global_system->HW_regs.DSPI.DSP.DSCR, 2, DSP_tab);
    add_register_data("DSBL", &global_system->HW_regs.DSPI.DSP.DSBL, 2, DSP_tab);

    add_register_data("", NULL, 2, DSP_tab);

    add_register_data("ACSA", &global_system->HW_regs.DSPI.DSP.ACSA, 4, DSP_tab);
    add_register_data("ACEA", &global_system->HW_regs.DSPI.DSP.ACEA, 4, DSP_tab);
    add_register_data("ACCA", &global_system->HW_regs.DSPI.DSP.ACCA, 4, DSP_tab);

    add_register_data("DIRQ", &global_system->HW_regs.DSPI.DSP.DIRQ, 2, DSP_tab);

    add_register_data("", NULL, 2, DSP_tab);

    add_register_data("PC", &global_system->HW_regs.DSPI.DSP.pc, 2, DSP_tab);

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
        sprintf(name, "D%02x", i);
        add_register_data(name, &global_system->HW_regs.DSPI.regs[i], 1, HWIO_tab);
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

    add_register_data("", NULL, 4, HWIO_tab);

    add_register_data("SICOMCSR", &global_system->HW_regs.SI.COMCSR, 4, HWIO_tab);
    add_register_data("SISR", &global_system->HW_regs.SI.SISR, 4, HWIO_tab);

    add_register_data("", NULL, 1, HWIO_tab);
    add_register_data("", NULL, 1, HWIO_tab);

    for (int i = 0; i < 0x40; i++) {
        sprintf(name, "EI%02x", i);
        add_register_data(name, &global_system->HW_regs.EXI.regs[i], 1, HWIO_tab);
    }

    add_register_data("", NULL, 1, HWIO_tab);
    add_register_data("", NULL, 4, HWIO_tab);

    add_register_data("AICR", &global_system->HW_regs.AI.AICR, 4, HWIO_tab);
    add_register_data("AIVR", &global_system->HW_regs.AI.AIVR, 4, HWIO_tab);
    add_register_data("AISCNT", &global_system->HW_regs.AI.AISCNT, 4, HWIO_tab);
    add_register_data("AIIT", &global_system->HW_regs.AI.AIIT, 4, HWIO_tab);

    int CP_tab = add_register_tab("internal CP");

    for (int i = 0; i < INTERNAL_CP_REGISTER_SIZE; i++) {
        sprintf(name, "CP 0x%02x", INTERNAL_CP_REGISTER_BASE + i);
        add_register_data(name, &global_system->HW_regs.CP.internal_registers.CP_regs[i], 4, CP_tab);
    }

    int BP_tab = add_register_tab("BP regs");

    for (int i = 0; i < INTERNAL_BP_REGISTER_SIZE; i++) {
        sprintf(name, "BP 0x%02x", i);
        add_register_data(name, &global_system->HW_regs.CP.internal_registers.BP_regs[i], 4, BP_tab);
    }

    int XF_reg_tab = add_register_tab("XF regs");

    for (int i = 0; i < INTERNAL_XF_REGISTER_SIZE; i++) {
        sprintf(name, "XF 0x%02x", INTERNAL_XF_REGISTER_BASE + i);
        add_register_data(name, &global_system->HW_regs.CP.internal_registers.XF_regs[i], 4, XF_reg_tab);
    }

    int XF_mem_tab = add_register_tab("XF mem");

    for (int i = 0; i < 0x100; i++) {
        sprintf(name, "XFA[0x%02x]", i);
        add_register_data(name, &global_system->HW_regs.CP.internal_registers.XF_mem[0][i], 4, XF_mem_tab);
    }

    add_register_data("", NULL, 4, XF_mem_tab);
    add_register_data("", NULL, 4, XF_mem_tab);

    for (int i = 0; i < 0x60; i++) {
        sprintf(name, "XFB[0x%02x]", i);
        add_register_data(name, &global_system->HW_regs.CP.internal_registers.XF_mem[1][i], 4, XF_mem_tab);
    }

    add_register_data("", NULL, 4, XF_mem_tab);
    add_register_data("", NULL, 4, XF_mem_tab);

    for (int i = 0; i < 0x100; i++) {
        sprintf(name, "XFC[0x%02x]", i);
        add_register_data(name, &global_system->HW_regs.CP.internal_registers.XF_mem[2][i], 4, XF_mem_tab);
    }

    add_register_data("", NULL, 4, XF_mem_tab);
    add_register_data("", NULL, 4, XF_mem_tab);

    for (int i = 0; i < 0x80; i++) {
        sprintf(name, "XFD[0x%02x]", i);
        add_register_data(name, &global_system->HW_regs.CP.internal_registers.XF_mem[3][i], 4, XF_mem_tab);
    }

    add_command("RESET", "Resets the system. Add 'pause/freeze/break' to freeze on reload.", reset_system);
    add_command("PAUSE", "Pauses the system.", pause_system);
    add_command("CONTINUE", "Unpauses the system.", unpause_system);
    add_command("BREAK", "Add breakpoint to system at PC = $1.", break_system);
    add_command("UNBREAK", "Remove breakpoint to system at PC = $1.", unbreak_system);
    add_command("STEP", "Step system for $1 CPU steps (defaults to 1 step).", step_system);
    add_command("MEMORY", "Read word(s) from memory between $1 and $2", dump_memory_range);
    add_command("STATE", "Dump the current register state to the console", get_state);

    add_overlay_info(cpu_ticks);
    add_overlay_info(fps_counter);

    return global_system;
}