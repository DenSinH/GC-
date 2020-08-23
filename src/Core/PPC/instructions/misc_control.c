#include "PPC/instructions.h"

INLINE_GEKKO_INSTR(mtsr) {
    GEKKO_INSTR_HEADER
    ASSERT_SUPERVISOR
    log_cpu("mtsr %08x", instruction.raw);

    cpu->SR[instruction.mxsr.SR] = cpu->GPR[instruction.mxsr.DS];
}

INLINE_GEKKO_INSTR(mfsr) {
    GEKKO_INSTR_HEADER
    ASSERT_SUPERVISOR
    log_cpu("mtsr %08x", instruction.raw);

    cpu->GPR[instruction.mxsr.DS] = cpu->SR[instruction.mxsr.SR];
}