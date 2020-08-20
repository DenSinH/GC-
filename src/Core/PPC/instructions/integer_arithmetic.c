#include "PPC/instructions.h"

GEKKO_INSTR(addi) {
    ASSERT_BITFIELD_SIZE
    log_cpu("addi %08x", instruction.raw);
    if (!instruction.arithmetic_simm.A) {
        cpu->GPR[instruction.arithmetic_simm.D] = (u32)((i32)((i16)(instruction.arithmetic_simm.SIMM)));
    }
    else {
        cpu->GPR[instruction.arithmetic_simm.D] = cpu->GPR[instruction.arithmetic_simm.A] + (i32)((i16)(instruction.arithmetic_simm.SIMM));
    }
}

GEKKO_INSTR(addis) {
    ASSERT_BITFIELD_SIZE
    log_cpu("addis %08x", instruction.raw);
    if (!instruction.arithmetic_simm.A) {
        cpu->GPR[instruction.arithmetic_simm.D] = (u32)(((i32)((i16)(instruction.arithmetic_simm.SIMM))) << 16);
    }
    else {
        cpu->GPR[instruction.arithmetic_simm.D] = cpu->GPR[instruction.arithmetic_simm.A] + (i32)(((i16)(instruction.arithmetic_simm.SIMM)) << 16);
    }
}