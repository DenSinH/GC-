#include "instructions.h"

#define PS_SET_RESULT_VX !(cpu->FPSCR.VE && (result_ps0.exception || result_ps1.exception))

INLINE_GEKKO_INSTR(ps_neg) {
    GEKKO_INSTR_HEADER
    ASSERT_PAIRED_SINGLE
    log_cpu("ps_neg %x", instruction.raw);

    cpu->FPR[instruction.general_DAB.D].PS0.u = cpu->FPR[instruction.general_DAB.B].PS0.u ^ DOUBLE_SIGN;
    cpu->FPR[instruction.general_DAB.D].PS1.u = cpu->FPR[instruction.general_DAB.B].PS1.u ^ DOUBLE_SIGN;

    if (instruction.general_DABC.Rc) {
        UPDATE_CR1_FROM_FPSCR(cpu->CR, cpu->FPSCR);
    }
}

INLINE_GEKKO_INSTR(ps_mul) {
    // similar to fmuls
    GEKKO_INSTR_HEADER
    ASSERT_PAIRED_SINGLE
    log_cpu("ps_mul %x", instruction.raw);

    s_float_result result_ps0 = float_mul(
            &cpu->FPSCR,
            cpu->FPR[instruction.general_DABC.A].PS0,
            cpu->FPR[instruction.general_DABC.C].PS0
    );
    s_float_result result_ps1 = float_mul(
            &cpu->FPSCR,
            cpu->FPR[instruction.general_DABC.A].PS1,
            cpu->FPR[instruction.general_DABC.C].PS1
    );

    if (PS_SET_RESULT_VX) {
        cpu->FPR[instruction.general_DABC.D].PS0.u = TO_SINGLE(result_ps0.value).u;
        cpu->FPR[instruction.general_DABC.D].PS1.u = TO_SINGLE(result_ps1.value).u;
        UPDATE_FPRF_RESULT_BIT_DOUBLE(cpu, result_ps0.value);
    }

    if (instruction.general_DABC.Rc) {
        UPDATE_CR1_FROM_FPSCR(cpu->CR, cpu->FPSCR);
    }
}

INLINE_GEKKO_INSTR(ps_muls0) {
    // similar to ps_muls
    GEKKO_INSTR_HEADER
    ASSERT_PAIRED_SINGLE
    log_cpu("ps_muls0 %x", instruction.raw);

    s_float_result result_ps0 = float_mul(
            &cpu->FPSCR,
            cpu->FPR[instruction.general_DABC.A].PS0,
            cpu->FPR[instruction.general_DABC.C].PS0
    );
    s_float_result result_ps1 = float_mul(
            &cpu->FPSCR,
            cpu->FPR[instruction.general_DABC.A].PS1,
            cpu->FPR[instruction.general_DABC.C].PS0
    );

    if (PS_SET_RESULT_VX) {
        cpu->FPR[instruction.general_DABC.D].PS0.u = TO_SINGLE(result_ps0.value).u;
        cpu->FPR[instruction.general_DABC.D].PS1.u = TO_SINGLE(result_ps1.value).u;
        UPDATE_FPRF_RESULT_BIT_DOUBLE(cpu, result_ps0.value);
    }

    if (instruction.general_DABC.Rc) {
        UPDATE_CR1_FROM_FPSCR(cpu->CR, cpu->FPSCR);
    }
}

INLINE_GEKKO_INSTR(ps_muls1) {
    // similar to ps_muls0
    GEKKO_INSTR_HEADER
    ASSERT_PAIRED_SINGLE
    log_cpu("ps_muls1 %x", instruction.raw);

    s_float_result result_ps0 = float_mul(
            &cpu->FPSCR,
            cpu->FPR[instruction.general_DABC.A].PS0,
            cpu->FPR[instruction.general_DABC.C].PS1
    );
    s_float_result result_ps1 = float_mul(
            &cpu->FPSCR,
            cpu->FPR[instruction.general_DABC.A].PS1,
            cpu->FPR[instruction.general_DABC.C].PS1
    );

    if (PS_SET_RESULT_VX) {
        cpu->FPR[instruction.general_DABC.D].PS0.u = TO_SINGLE(result_ps0.value).u;
        cpu->FPR[instruction.general_DABC.D].PS1.u = TO_SINGLE(result_ps1.value).u;
        UPDATE_FPRF_RESULT_BIT_DOUBLE(cpu, result_ps0.value);
    }

    if (instruction.general_DABC.Rc) {
        UPDATE_CR1_FROM_FPSCR(cpu->CR, cpu->FPSCR);
    }
}


INLINE_GEKKO_INSTR(ps_madd) {
    // similar to fmadds
    GEKKO_INSTR_HEADER
    ASSERT_PAIRED_SINGLE
    log_cpu("ps_madd %x", instruction.raw);

    s_float_result result_ps0 = float_madd(
            &cpu->FPSCR,
            cpu->FPR[instruction.general_DABC.A].PS0,
            cpu->FPR[instruction.general_DABC.B].PS0,
            cpu->FPR[instruction.general_DABC.C].PS0
    );
    s_float_result result_ps1 = float_madd(
            &cpu->FPSCR,
            cpu->FPR[instruction.general_DABC.A].PS1,
            cpu->FPR[instruction.general_DABC.B].PS1,
            cpu->FPR[instruction.general_DABC.C].PS1
    );

    if (PS_SET_RESULT_VX) {
        cpu->FPR[instruction.general_DABC.D].PS0.u = TO_SINGLE(result_ps0.value).u;
        cpu->FPR[instruction.general_DABC.D].PS1.u = TO_SINGLE(result_ps1.value).u;
        UPDATE_FPRF_RESULT_BIT_DOUBLE(cpu, result_ps0.value);
    }

    if (instruction.general_DABC.Rc) {
        UPDATE_CR1_FROM_FPSCR(cpu->CR, cpu->FPSCR);
    }
}

INLINE_GEKKO_INSTR(ps_madds0) {
    // similar to fmadds
    GEKKO_INSTR_HEADER
    ASSERT_PAIRED_SINGLE
    log_cpu("ps_madds0 %x", instruction.raw);

    s_float_result result_ps0 = float_madd(
            &cpu->FPSCR,
            cpu->FPR[instruction.general_DABC.A].PS0,
            cpu->FPR[instruction.general_DABC.B].PS0,
            cpu->FPR[instruction.general_DABC.C].PS0
    );
    s_float_result result_ps1 = float_madd(
            &cpu->FPSCR,
            cpu->FPR[instruction.general_DABC.A].PS1,
            cpu->FPR[instruction.general_DABC.B].PS1,
            cpu->FPR[instruction.general_DABC.C].PS0
    );

    if (PS_SET_RESULT_VX) {
        cpu->FPR[instruction.general_DABC.D].PS0.u = TO_SINGLE(result_ps0.value).u;
        cpu->FPR[instruction.general_DABC.D].PS1.u = TO_SINGLE(result_ps1.value).u;
        UPDATE_FPRF_RESULT_BIT_DOUBLE(cpu, result_ps0.value);
    }

    if (instruction.general_DABC.Rc) {
        UPDATE_CR1_FROM_FPSCR(cpu->CR, cpu->FPSCR);
    }
}

INLINE_GEKKO_INSTR(ps_madds1) {
    // similar to fmadds
    GEKKO_INSTR_HEADER
    ASSERT_PAIRED_SINGLE
    log_cpu("ps_madds1 %x", instruction.raw);

    s_float_result result_ps0 = float_madd(
            &cpu->FPSCR,
            cpu->FPR[instruction.general_DABC.A].PS0,
            cpu->FPR[instruction.general_DABC.B].PS0,
            cpu->FPR[instruction.general_DABC.C].PS1
    );
    s_float_result result_ps1 = float_madd(
            &cpu->FPSCR,
            cpu->FPR[instruction.general_DABC.A].PS1,
            cpu->FPR[instruction.general_DABC.B].PS1,
            cpu->FPR[instruction.general_DABC.C].PS1
    );

    if (PS_SET_RESULT_VX) {
        cpu->FPR[instruction.general_DABC.D].PS0.u = TO_SINGLE(result_ps0.value).u;
        cpu->FPR[instruction.general_DABC.D].PS1.u = TO_SINGLE(result_ps1.value).u;
        UPDATE_FPRF_RESULT_BIT_DOUBLE(cpu, result_ps0.value);
    }

    if (instruction.general_DABC.Rc) {
        UPDATE_CR1_FROM_FPSCR(cpu->CR, cpu->FPSCR);
    }
}

INLINE_GEKKO_INSTR(ps_msub) {
    // similar to ps_madd
    GEKKO_INSTR_HEADER
    ASSERT_PAIRED_SINGLE
    log_cpu("ps_msub %x", instruction.raw);

    s_float_result result_ps0 = float_msub(
            &cpu->FPSCR,
            cpu->FPR[instruction.general_DABC.A].PS0,
            cpu->FPR[instruction.general_DABC.B].PS0,
            cpu->FPR[instruction.general_DABC.C].PS0
    );
    s_float_result result_ps1 = float_msub(
            &cpu->FPSCR,
            cpu->FPR[instruction.general_DABC.A].PS1,
            cpu->FPR[instruction.general_DABC.B].PS1,
            cpu->FPR[instruction.general_DABC.C].PS1
    );

    if (PS_SET_RESULT_VX) {
        cpu->FPR[instruction.general_DABC.D].PS0.u = TO_SINGLE(result_ps0.value).u;
        cpu->FPR[instruction.general_DABC.D].PS1.u = TO_SINGLE(result_ps1.value).u;
        UPDATE_FPRF_RESULT_BIT_DOUBLE(cpu, result_ps0.value);
    }

    if (instruction.general_DABC.Rc) {
        UPDATE_CR1_FROM_FPSCR(cpu->CR, cpu->FPSCR);
    }
}