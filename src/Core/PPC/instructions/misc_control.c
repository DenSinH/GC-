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

INLINE_GEKKO_INSTR(dcbf) {
    GEKKO_INSTR_HEADER

    log_cpu("dcbf %08x", instruction.raw);
}

INLINE_GEKKO_INSTR(icbi) {
    GEKKO_INSTR_HEADER

    log_cpu("icbi %08x", instruction.raw);
}