#include "instructions.h"
#include "float_utils.h"

INLINE_GEKKO_INSTR(fmr) {
    GEKKO_INSTR_HEADER
    log_cpu("fmr %08x", instruction.raw);

    cpu->FPR[instruction.general_DAB.D].PS0.u = cpu->FPR[instruction.general_DAB.B].PS0.u;
    if (instruction.general_DAB.Rc) {
        UPDATE_CR1_FROM_FPSCR(cpu->CR, cpu->FPSCR);
    }
}

INLINE_GEKKO_INSTR(fabsx) {
    GEKKO_INSTR_HEADER
    log_cpu("fabs %08x", instruction.raw);

    cpu->FPR[instruction.general_DAB.D].PS0.u = cpu->FPR[instruction.general_DAB.B].PS0.u & ~DOUBLE_SIGN;
    if (instruction.general_DAB.Rc) {
        UPDATE_CR1_FROM_FPSCR(cpu->CR, cpu->FPSCR);
    }
}

INLINE_GEKKO_INSTR(fneg) {
    GEKKO_INSTR_HEADER
    log_cpu("fneg %08x", instruction.raw);

    cpu->FPR[instruction.general_DAB.D].PS0.u = cpu->FPR[instruction.general_DAB.B].PS0.u ^ DOUBLE_SIGN;
    if (instruction.general_DAB.Rc) {
        UPDATE_CR1_FROM_FPSCR(cpu->CR, cpu->FPSCR);
    }
}