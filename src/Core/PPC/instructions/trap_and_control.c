#include "PPC/instructions.h"

INLINE_GEKKO_INSTR(mfmsr) {
    GEKKO_INSTR_HEADER
    ASSERT_SUPERVISOR

    log_cpu("mfmsr %08x", instruction.raw);
    cpu->GPR[instruction.general_DAB.D] = cpu->MSR.raw;
}

INLINE_GEKKO_INSTR(mtmsr) {
    GEKKO_INSTR_HEADER
    ASSERT_SUPERVISOR

    log_cpu("mtmsr %08x", instruction.raw);
    // todo: state change effective immediately?
    cpu->MSR.raw = cpu->GPR[instruction.general_SAB.S];
}

INLINE_GEKKO_INSTR(mfspr) {
    GEKKO_INSTR_HEADER
    // supervisor AND user instruction
    // todo: privileged instruction type program exception or an illegal instruction type program exception

    log_cpu("mfspr %08x, SPR_D %d, GPR%d", instruction.raw, (instruction.SPR_D.SPR_hi << 5) | instruction.SPR_D.SPR_lo, instruction.SPR_D.D);

    assert(cpu->SPR[(instruction.SPR_D.SPR_hi << 5) | instruction.SPR_D.SPR_lo] != NULL /* Attempted to access unimplemented SPR assignment */);
    GET_SPR(cpu, (instruction.SPR_D.SPR_hi << 5) | instruction.SPR_D.SPR_lo, &cpu->GPR[instruction.SPR_D.D]);
}

INLINE_GEKKO_INSTR(mtspr) {
    GEKKO_INSTR_HEADER
    // supervisor AND user instruction
    // todo: privileged instruction type program exception or an illegal instruction type program exception

    log_cpu("mtspr %08x, SPR_D %d", instruction.raw, (instruction.SPR_S.SPR_hi << 5) | instruction.SPR_S.SPR_lo);

    assert(cpu->SPR[(instruction.SPR_S.SPR_hi << 5) | instruction.SPR_S.SPR_lo] != NULL /* Attempted to access unimplemented SPR assignment */);
    SET_SPR(cpu, (instruction.SPR_S.SPR_hi << 5) | instruction.SPR_S.SPR_lo, &cpu->GPR[instruction.SPR_S.S]);
}