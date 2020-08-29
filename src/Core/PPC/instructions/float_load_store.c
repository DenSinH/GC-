#include "PPC/instructions.h"

#include "PPC/MMU.h"

GEKKO_INSTR(stfd) {
    GEKKO_INSTR_HEADER
    log_cpu("stfd %08x", instruction.raw);

    u32 EA = (instruction.general_SAd.A ? cpu->GPR[instruction.general_SAd.A] : 0) + (i32)((i16)instruction.general_SAd.d);
    write64(&cpu->DMMU, EA, cpu->FPR[instruction.general_SAd.S].PS0);
}

GEKKO_INSTR(lfd) {
    GEKKO_INSTR_HEADER
    log_cpu("lfd %08x", instruction.raw);

    u32 EA = (instruction.general_DAd.A ? cpu->GPR[instruction.general_DAd.A] : 0) + (i32)((i16)instruction.general_DAd.d);
    cpu->FPR[instruction.general_DAd.D].PS0 = read64(&cpu->DMMU, EA);
}

GEKKO_INSTR(lfs) {
    GEKKO_INSTR_HEADER
    log_cpu("lfs %08x", instruction.raw);

    u32 EA = (instruction.general_DAd.A ? cpu->GPR[instruction.general_DAd.A] : 0) + (i32)((i16)instruction.general_DAd.d);
    u32 single_raw = read32(&cpu->DMMU, EA);

    if (cpu->HID2.PSE) {
        float single;
        memcpy(&single, &single_raw, 4);
        double PS0 = (double)single;
        LOAD_DOUBLE(&cpu->FPR[instruction.general_DAd.D], &PS0);
    }
    else {
        cpu->FPR[instruction.general_DAd.D].PS0 = single_raw;
        cpu->FPR[instruction.general_DAd.D].PS1 = single_raw;
    }
}