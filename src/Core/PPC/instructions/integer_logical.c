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

GEKKO_INSTR(andi) {
    GEKKO_INSTR_HEADER
    log_cpu("andi %08x", instruction.raw);
    cpu->GPR[instruction.arithmetic_uimm.A] = cpu->GPR[instruction.arithmetic_uimm.S] & (u32)instruction.arithmetic_uimm.UIMM;
}