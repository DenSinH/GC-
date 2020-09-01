#include "PPC/instructions.h"

#include "PPC/MMU.h"

GEKKO_INSTR(stfd) {
    GEKKO_INSTR_HEADER
    log_cpu("stfd %08x", instruction.raw);

    u32 EA = (instruction.general_SAd.A ? cpu->GPR[instruction.general_SAd.A] : 0) + (i32)((i16)instruction.general_SAd.d);
    write64(&cpu->DMMU, EA, cpu->FPR[instruction.general_SAd.S].PS0.u);
}

GEKKO_INSTR(lfd) {
    GEKKO_INSTR_HEADER
    log_cpu("lfd %08x", instruction.raw);

    u32 EA = (instruction.general_DAd.A ? cpu->GPR[instruction.general_DAd.A] : 0) + (i32)((i16)instruction.general_DAd.d);
    cpu->FPR[instruction.general_DAd.D].PS0.u = read64(&cpu->DMMU, EA);
}

GEKKO_INSTR(lfs) {
    GEKKO_INSTR_HEADER
    log_cpu("lfs %08x", instruction.raw);

    u32 EA = (instruction.general_DAd.A ? cpu->GPR[instruction.general_DAd.A] : 0) + (i32)((i16)instruction.general_DAd.d);
    bit_float single_raw = {.u = read32(&cpu->DMMU, EA)};

    if (!cpu->HID2.PSE) {
        cpu->FPR[instruction.general_DAd.D].PS0.d = (double)single_raw.f;
    }
    else {
        cpu->FPR[instruction.general_DAd.D].PS0.u = cpu->FPR[instruction.general_DAd.D].PS1.u = CONVERT_TO_DOUBLE(single_raw).u;
    }
}

INLINE_GEKKO_INSTR(stfiwx) {
    GEKKO_INSTR_HEADER
    log_cpu("stfiwx %08x", instruction.raw);

    u32 EA = (instruction.general_SAB.A ? cpu->GPR[instruction.general_SAB.A] : 0) + cpu->GPR[instruction.general_SAB.B];

    write32(&cpu->DMMU, EA, (u32)cpu->FPR[instruction.general_SAB.S].PS0.u);
}