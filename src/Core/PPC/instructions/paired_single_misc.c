#include "PPC/instructions.h"


INLINE_GEKKO_INSTR(ps_sum0) {
    GEKKO_INSTR_HEADER
    ASSERT_PAIRED_SINGLE
    log_cpu("ps_sum0 %x", instruction.raw);

    s_float_result result = float_add(&cpu->FPSCR, cpu->FPR[instruction.general_DABC.A].PS0, cpu->FPR[instruction.general_DABC.B].PS1);

    if (!(cpu->FPSCR.VE && result.exception)) {
        cpu->FPR[instruction.general_DABC.D].PS0.u = TO_SINGLE(result.value).u;
        cpu->FPR[instruction.general_DABC.D].PS1.u = cpu->FPR[instruction.general_DABC.C].PS1.u;

        UPDATE_FPRF_RESULT_BIT_DOUBLE(cpu, result.value);
    }

    if (instruction.general_DAB.Rc) {
        UPDATE_CR1_FROM_FPSCR(cpu->CR, cpu->FPSCR);
    }
}

INLINE_GEKKO_INSTR(ps_sum1) {
    // similar to ps_sum0
    GEKKO_INSTR_HEADER
    ASSERT_PAIRED_SINGLE
    log_cpu("ps_sum1 %x", instruction.raw);

    s_float_result result = float_add(&cpu->FPSCR, cpu->FPR[instruction.general_DABC.A].PS0, cpu->FPR[instruction.general_DABC.B].PS1);

    if (!(cpu->FPSCR.VE && result.exception)) {
        cpu->FPR[instruction.general_DABC.D].PS0.u = cpu->FPR[instruction.general_DABC.C].PS0.u;
        cpu->FPR[instruction.general_DABC.D].PS1.u = TO_SINGLE(result.value).u;

        UPDATE_FPRF_RESULT_BIT_DOUBLE(cpu, result.value);
    }

    if (instruction.general_DAB.Rc) {
        UPDATE_CR1_FROM_FPSCR(cpu->CR, cpu->FPSCR);
    }
}

INLINE_GEKKO_INSTR(ps_merge00) {
    GEKKO_INSTR_HEADER
    ASSERT_PAIRED_SINGLE
    log_cpu("ps_merge00 %x", instruction.raw);

    cpu->FPR[instruction.general_DAB.D].PS0.u = cpu->FPR[instruction.general_DAB.A].PS0.u;
    cpu->FPR[instruction.general_DAB.D].PS1.u = cpu->FPR[instruction.general_DAB.B].PS0.u;

    if (instruction.general_DAB.Rc) {
        UPDATE_CR1_FROM_FPSCR(cpu->CR, cpu->FPSCR);
    }
}

INLINE_GEKKO_INSTR(ps_merge01) {
    GEKKO_INSTR_HEADER
    ASSERT_PAIRED_SINGLE
    log_cpu("ps_merge01 %x", instruction.raw);

    cpu->FPR[instruction.general_DAB.D].PS0.u = cpu->FPR[instruction.general_DAB.A].PS0.u;
    cpu->FPR[instruction.general_DAB.D].PS1.u = cpu->FPR[instruction.general_DAB.B].PS1.u;

    if (instruction.general_DAB.Rc) {
        UPDATE_CR1_FROM_FPSCR(cpu->CR, cpu->FPSCR);
    }
}

INLINE_GEKKO_INSTR(ps_merge10) {
    GEKKO_INSTR_HEADER
    ASSERT_PAIRED_SINGLE
    log_cpu("ps_merge10 %x", instruction.raw);

    cpu->FPR[instruction.general_DAB.D].PS0.u = cpu->FPR[instruction.general_DAB.A].PS1.u;
    cpu->FPR[instruction.general_DAB.D].PS1.u = cpu->FPR[instruction.general_DAB.B].PS0.u;

    if (instruction.general_DAB.Rc) {
        UPDATE_CR1_FROM_FPSCR(cpu->CR, cpu->FPSCR);
    }
}

INLINE_GEKKO_INSTR(ps_merge11) {
    GEKKO_INSTR_HEADER
    ASSERT_PAIRED_SINGLE
    log_cpu("ps_merge11 %x", instruction.raw);

    cpu->FPR[instruction.general_DAB.D].PS0.u = cpu->FPR[instruction.general_DAB.A].PS1.u;
    cpu->FPR[instruction.general_DAB.D].PS1.u = cpu->FPR[instruction.general_DAB.B].PS1.u;

    if (instruction.general_DAB.Rc) {
        UPDATE_CR1_FROM_FPSCR(cpu->CR, cpu->FPSCR);
    }
}