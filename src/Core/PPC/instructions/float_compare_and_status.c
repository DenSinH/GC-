#include "instructions.h"
#include "float_utils.h"

INLINE_GEKKO_INSTR(fcmpu) {
    GEKKO_INSTR_HEADER
    log_cpu("fcmpu %x", instruction.raw);

    bit_double A = cpu->FPR[instruction.cmp_float.A].PS0;
    bit_double B = cpu->FPR[instruction.cmp_float.B].PS0;
    s_CRn c = { .raw = GET_CRn_CMP_FLOAT(A.d, B.d) };

    if (IS_SNAN(A) || IS_SNAN(B)) {
        SET_VX_EXCEPTIONS(&cpu->FPSCR, VX_VXSNAN);
    }

    cpu->FPSCR.FPCC = c.raw;
    SET_CRn(cpu->CR, instruction.cmp_float.crfD, c);
}

INLINE_GEKKO_INSTR(mtfsf) {
    GEKKO_INSTR_HEADER
    log_cpu("mtfsf %x", instruction.raw);

    // bits 1 and 2 (or as I would call them: 29 and 30) cannot be cleared this way
    cpu->FPSCR.raw = GET_FPR(cpu, instruction.mtfsf.B) & field_mask[instruction.mtfsf.FM] & 0x9fffffff;
    // todo: GET_FPR? also, floating point exceptions?
    if (instruction.mtfsf.Rc) {
        UPDATE_CR1_FROM_FPSCR(cpu->CR, cpu->FPSCR);
    }
}

INLINE_GEKKO_INSTR(mtfsb0) {
    GEKKO_INSTR_HEADER
    log_cpu("mtfsb0 %08x", instruction.raw);

    cpu->FPSCR.raw &= ~(1 << instruction.general_DAB.D);
    if (instruction.general_DAB.Rc) {
        UPDATE_CR1_FROM_FPSCR(cpu->CR, cpu->FPSCR);
    }
}

INLINE_GEKKO_INSTR(mtfsb1) {
    GEKKO_INSTR_HEADER
    log_cpu("mtfsb0 %08x", instruction.raw);

    cpu->FPSCR.raw |= 1 << instruction.general_DAB.D;
    if (instruction.general_DAB.Rc) {
        UPDATE_CR1_FROM_FPSCR(cpu->CR, cpu->FPSCR);
    }
}