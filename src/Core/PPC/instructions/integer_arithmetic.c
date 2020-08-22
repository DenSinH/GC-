#include "PPC/instructions.h"

GEKKO_INSTR(addi) {
    GEKKO_INSTR_HEADER
    log_cpu("addi %08x", instruction.raw);
    if (!instruction.arithmetic_simm.A) {
        cpu->GPR[instruction.arithmetic_simm.D] = (u32)((i32)((i16)(instruction.arithmetic_simm.SIMM)));
    }
    else {
        cpu->GPR[instruction.arithmetic_simm.D] = cpu->GPR[instruction.arithmetic_simm.A] + (i32)((i16)(instruction.arithmetic_simm.SIMM));
    }
}

GEKKO_INSTR(addis) {
    GEKKO_INSTR_HEADER
    log_cpu("addis %08x", instruction.raw);
    if (!instruction.arithmetic_simm.A) {
        cpu->GPR[instruction.arithmetic_simm.D] = (u32)(((i32)((i16)(instruction.arithmetic_simm.SIMM))) << 16);
    }
    else {
        cpu->GPR[instruction.arithmetic_simm.D] = cpu->GPR[instruction.arithmetic_simm.A] + (i32)(((i16)(instruction.arithmetic_simm.SIMM)) << 16);
    }
}

INLINE_GEKKO_INSTR(subf) {
    GEKKO_INSTR_HEADER
    log_cpu("subf %08x", instruction.raw);

    u32 result = ~cpu->GPR[instruction.general_DAB.A] + cpu->GPR[instruction.general_DAB.B] + 1;

    if (instruction.general_DAB.OE) {
        UPDATE_XER_OV(cpu->XER, ADD_OVERFLOW32(~cpu->GPR[instruction.general_DAB.A], cpu->GPR[instruction.general_DAB.B], result));
    }

    if (instruction.general_DAB.Rc) {
        UPDATE_CR0_RESULT32(cpu, result);
    }

    cpu->GPR[instruction.general_DAB.D] = result;
}