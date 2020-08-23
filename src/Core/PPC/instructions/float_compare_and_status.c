#include "PPC/instructions.h"

INLINE_GEKKO_INSTR(mtfsf) {
    GEKKO_INSTR_HEADER
    log_cpu("mtfsf %x", instruction.raw);

    // bits 1 and 2 (or as I would call them: 29 and 30) cannot be cleared this way
    cpu->FPSCR.raw = GET_FPR(cpu, instruction.mtfsf.B) & field_mask[instruction.mtfsf.FM] & 0x9fffffff;
    // todo: GET_FPR? also, floating point exceptions?
    if (instruction.mtfsf.Rc) {
        UPDATE_CR_FROM_FPSCR(cpu->CR, cpu->FPSCR);
    }
}

INLINE_GEKKO_INSTR(mtfsb0) {
    GEKKO_INSTR_HEADER
    log_cpu("mtfsb0 %08x", instruction.raw);

    cpu->FPSCR.raw &= ~(1 << instruction.general_DAB.D);
    if (instruction.general_DAB.Rc) {
        UPDATE_CR_FROM_FPSCR(cpu->CR, cpu->FPSCR);
    }
}

INLINE_GEKKO_INSTR(mtfsb1) {
    GEKKO_INSTR_HEADER
    log_cpu("mtfsb0 %08x", instruction.raw);

    cpu->FPSCR.raw |= 1 << instruction.general_DAB.D;
    if (instruction.general_DAB.Rc) {
        UPDATE_CR_FROM_FPSCR(cpu->CR, cpu->FPSCR);
    }
}