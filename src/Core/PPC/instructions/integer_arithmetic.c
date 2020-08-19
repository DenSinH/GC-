#include "PPC/instructions.h"

GEKKO_INSTR(addi) {
    STATIC_ASSERT
    log("addi %08x", instruction.raw);
    if (!instruction.arithmetic_simm.A) {
        cpu->GPR[instruction.arithmetic_simm.D] = (u32)((i32)((i16)(instruction.arithmetic_simm.SIMM)));
    }
    else {
        cpu->GPR[instruction.arithmetic_simm.D] = cpu->GPR[instruction.arithmetic_simm.A] + (i32)((i16)(instruction.arithmetic_simm.SIMM));
    }
}

GEKKO_INSTR(addis) {
    STATIC_ASSERT
    log("addis %08x", instruction.raw);
    if (!instruction.arithmetic_simm.A) {
        cpu->GPR[instruction.arithmetic_simm.D] = (u32)(((i32)((i16)(instruction.arithmetic_simm.SIMM))) << 16);
    }
    else {
        cpu->GPR[instruction.arithmetic_simm.D] = cpu->GPR[instruction.arithmetic_simm.A] + (i32)(((i16)(instruction.arithmetic_simm.SIMM)) << 16);
    }
}