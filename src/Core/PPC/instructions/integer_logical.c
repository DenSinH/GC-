#include "PPC/instructions.h"


GEKKO_INSTR(ori) {
    GEKKO_INSTR_HEADER
    log_cpu("ori %08x", instruction.raw);
    cpu->GPR[instruction.arithmetic_uimm.A] = cpu->GPR[instruction.arithmetic_uimm.S] | (u32)instruction.arithmetic_uimm.UIMM;
}

GEKKO_INSTR(oris) {
    GEKKO_INSTR_HEADER
    log_cpu("oris %08x", instruction.raw);
    cpu->GPR[instruction.arithmetic_uimm.A] = cpu->GPR[instruction.arithmetic_uimm.S] | ((u32)instruction.arithmetic_uimm.UIMM << 16);
}

INLINE_GEKKO_INSTR(orx) {
    GEKKO_INSTR_HEADER
    log_cpu("orx %08x", instruction.raw);
    cpu->GPR[instruction.general_SAB.A] = cpu->GPR[instruction.general_SAB.S] | cpu->GPR[instruction.general_SAB.B];
    if (instruction.general_SAB.Rc) {
        UPDATE_CR0_RESULT32(cpu, cpu->GPR[instruction.general_SAB.A]);
    }
}

INLINE_GEKKO_INSTR(andx) {
    GEKKO_INSTR_HEADER
    log_cpu("andx %08x", instruction.raw);
    cpu->GPR[instruction.general_SAB.A] = cpu->GPR[instruction.general_SAB.S] & cpu->GPR[instruction.general_SAB.B];
    if (instruction.general_SAB.Rc) {
        UPDATE_CR0_RESULT32(cpu, cpu->GPR[instruction.general_SAB.A]);
    }
}

GEKKO_INSTR(andi) {
    GEKKO_INSTR_HEADER
    log_cpu("andi %08x", instruction.raw);
    cpu->GPR[instruction.arithmetic_uimm.A] = cpu->GPR[instruction.arithmetic_uimm.S] & (u32)instruction.arithmetic_uimm.UIMM;
    UPDATE_CR0_RESULT32(cpu, cpu->GPR[instruction.general_SAB.A]);
}

GEKKO_INSTR(andis) {
    GEKKO_INSTR_HEADER
    log_cpu("andis %08x", instruction.raw);
    cpu->GPR[instruction.arithmetic_uimm.A] = cpu->GPR[instruction.arithmetic_uimm.S] & ((u32)instruction.arithmetic_uimm.UIMM << 16);
    UPDATE_CR0_RESULT32(cpu, cpu->GPR[instruction.general_SAB.A]);
}

INLINE_GEKKO_INSTR(nor) {
    GEKKO_INSTR_HEADER
    log_cpu("nor %08x", instruction.raw);
    cpu->GPR[instruction.general_SAB.A] = ~(cpu->GPR[instruction.general_SAB.S] | cpu->GPR[instruction.general_SAB.B]);
    if (instruction.general_SAB.Rc) {
        UPDATE_CR0_RESULT32(cpu, cpu->GPR[instruction.general_SAB.A]);
    }
}

INLINE_GEKKO_INSTR(andc) {
    GEKKO_INSTR_HEADER
    log_cpu("andc %08x", instruction.raw);
    cpu->GPR[instruction.general_SAB.A] = cpu->GPR[instruction.general_SAB.S] & ~cpu->GPR[instruction.general_SAB.B];
    if (instruction.general_SAB.Rc) {
        UPDATE_CR0_RESULT32(cpu, cpu->GPR[instruction.general_SAB.A]);
    }
}

INLINE_GEKKO_INSTR(cntlzw) {
    GEKKO_INSTR_HEADER
    log_cpu("cntlzw %08x", instruction.raw);
    u8 n;
    for (n = 0; n < 32; n++) {
        if (cpu->GPR[instruction.general_SAB.S] & (0x80000000 >> n)) {
            break;
        }
    }

    cpu->GPR[instruction.general_SAB.A] = n;

    if (instruction.general_SAB.Rc) {
        UPDATE_CR0_RESULT32(cpu, cpu->GPR[instruction.general_SAB.A]);
    }
}