#include "instructions.h"

#include "MMU.h"

GEKKO_INSTR(stfd) {
    GEKKO_INSTR_HEADER
    log_cpu_verbose("stfd %08x", instruction.raw);

    u32 EA = (instruction.general_SAd.A ? cpu->GPR[instruction.general_SAd.A] : 0) + (i32)((i16)instruction.general_SAd.d);
    write64(&cpu->DMMU, EA, cpu->FPR[instruction.general_SAd.S].PS0.u);
}

GEKKO_INSTR(lfd) {
    GEKKO_INSTR_HEADER
    log_cpu_verbose("lfd %08x", instruction.raw);

    u32 EA = (instruction.general_DAd.A ? cpu->GPR[instruction.general_DAd.A] : 0) + (i32)((i16)instruction.general_DAd.d);
    cpu->FPR[instruction.general_DAd.D].PS0.u = read64(&cpu->DMMU, EA);
}

INLINE_GEKKO_INSTR(lfdx) {
    GEKKO_INSTR_HEADER
    log_cpu_verbose("lfd %08x", instruction.raw);

    u32 EA = (instruction.general_DAB.A ? cpu->GPR[instruction.general_DAB.A] : 0) + cpu->GPR[instruction.general_DAB.B];
    cpu->FPR[instruction.general_DAB.D].PS0.u = read64(&cpu->DMMU, EA);
}


GEKKO_INSTR(stfs) {
    GEKKO_INSTR_HEADER
    log_cpu_verbose("lfs %08x", instruction.raw);

    u32 EA = (instruction.general_SAd.A ? cpu->GPR[instruction.general_SAd.A] : 0) + (i32)((i16)instruction.general_SAd.d);
    bit_float single_raw = {.f = (float)cpu->FPR[instruction.general_SAd.S].PS0.d };
    write32(&cpu->DMMU, EA, single_raw.u);
}

GEKKO_INSTR(lfs) {
    GEKKO_INSTR_HEADER
    log_cpu_verbose("lfs %08x", instruction.raw);

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
    log_cpu_verbose("stfiwx %08x", instruction.raw);

    u32 EA = (instruction.general_SAB.A ? cpu->GPR[instruction.general_SAB.A] : 0) + cpu->GPR[instruction.general_SAB.B];
    write32(&cpu->DMMU, EA, (u32)cpu->FPR[instruction.general_SAB.S].PS0.u);
}