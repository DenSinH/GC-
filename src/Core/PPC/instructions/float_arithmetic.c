#include "instructions.h"
#include "float_utils.h"

#define F_SET_RESULT_VX !(cpu->FPSCR.VE && result.exception)
#define F_SET_RESULT_VX_ZX !((cpu->FPSCR.VE && result.exception) || (cpu->FPSCR.ZX && cpu->FPSCR.ZE))

INLINE_GEKKO_INSTR(fdiv) {
    GEKKO_INSTR_HEADER
    log_cpu_verbose("fdivs %x", instruction.raw);

    s_float_result result = float_div(&cpu->FPSCR, cpu->FPR[instruction.general_DAB.A].PS0, cpu->FPR[instruction.general_DAB.B].PS0);

    if (F_SET_RESULT_VX_ZX) {
        cpu->FPR[instruction.general_DAB.D].PS0.u = result.value.u;
        UPDATE_FPRF_RESULT_BIT_DOUBLE(cpu, result.value);
    }

    if (instruction.general_DAB.Rc) {
        UPDATE_CR1_FROM_FPSCR(cpu->CR, cpu->FPSCR);
    }
}

INLINE_GEKKO_INSTR(fdivs) {
    // identical to fdiv, except the result is cast to a single
    GEKKO_INSTR_HEADER
    log_cpu_verbose("fdivs %x", instruction.raw);

    s_float_result result = float_div(&cpu->FPSCR, cpu->FPR[instruction.general_DAB.A].PS0, cpu->FPR[instruction.general_DAB.B].PS0);

    if (F_SET_RESULT_VX_ZX) {
        cpu->FPR[instruction.general_DAB.D].PS0.u = TO_SINGLE(result.value).u;
        UPDATE_FPRF_RESULT_BIT_DOUBLE(cpu, result.value);
        if (cpu->HID2.PSE) {
            cpu->FPR[instruction.general_DAB.D].PS1.u = cpu->FPR[instruction.general_DAB.D].PS0.u;
        }
    }

    if (instruction.general_DAB.Rc) {
        UPDATE_CR1_FROM_FPSCR(cpu->CR, cpu->FPSCR);
    }
}

INLINE_GEKKO_INSTR(fsub) {
    GEKKO_INSTR_HEADER
    log_cpu_verbose("fsub %x", instruction.raw);

    s_float_result result = float_sub(&cpu->FPSCR, cpu->FPR[instruction.general_DAB.A].PS0, cpu->FPR[instruction.general_DAB.B].PS0);

    if (F_SET_RESULT_VX) {
        cpu->FPR[instruction.general_DAB.D].PS0.u = result.value.u;
        UPDATE_FPRF_RESULT_BIT_DOUBLE(cpu, result.value);
    }

    if (instruction.general_DAB.Rc) {
        UPDATE_CR1_FROM_FPSCR(cpu->CR, cpu->FPSCR);
    }
}

INLINE_GEKKO_INSTR(fsubs) {
    // identical to fsubs except the result is converted to a single after
    GEKKO_INSTR_HEADER
    log_cpu_verbose("fsub %x", instruction.raw);

    s_float_result result = float_sub(&cpu->FPSCR, cpu->FPR[instruction.general_DAB.A].PS0, cpu->FPR[instruction.general_DAB.B].PS0);

    if (F_SET_RESULT_VX) {
        cpu->FPR[instruction.general_DAB.D].PS0.u = TO_SINGLE(result.value).u;
        UPDATE_FPRF_RESULT_BIT_DOUBLE(cpu, result.value);
        if (cpu->HID2.PSE) {
            cpu->FPR[instruction.general_DAB.D].PS1.u = cpu->FPR[instruction.general_DAB.D].PS0.u;
        }
    }

    if (instruction.general_DAB.Rc) {
        UPDATE_CR1_FROM_FPSCR(cpu->CR, cpu->FPSCR);
    }
}

INLINE_GEKKO_INSTR(fadd) {
    GEKKO_INSTR_HEADER
    log_cpu_verbose("fadd %x", instruction.raw);

    s_float_result result = float_add(&cpu->FPSCR, cpu->FPR[instruction.general_DAB.A].PS0, cpu->FPR[instruction.general_DAB.B].PS0);

    if (F_SET_RESULT_VX) {
        cpu->FPR[instruction.general_DAB.D].PS0.u = result.value.u;
        UPDATE_FPRF_RESULT_BIT_DOUBLE(cpu, result.value);
    }

    if (instruction.general_DAB.Rc) {
        UPDATE_CR1_FROM_FPSCR(cpu->CR, cpu->FPSCR);
    }
}


INLINE_GEKKO_INSTR(fadds) {
    // identical to fadd, except the result is converted to a single
    GEKKO_INSTR_HEADER
    log_cpu_verbose("fadds %x", instruction.raw);

    s_float_result result = float_add(&cpu->FPSCR, cpu->FPR[instruction.general_DAB.A].PS0, cpu->FPR[instruction.general_DAB.B].PS0);

    if (F_SET_RESULT_VX) {
        cpu->FPR[instruction.general_DAB.D].PS0.u = TO_SINGLE(result.value).u;
        UPDATE_FPRF_RESULT_BIT_DOUBLE(cpu, result.value);
        if (cpu->HID2.PSE) {
            cpu->FPR[instruction.general_DAB.D].PS1.u = cpu->FPR[instruction.general_DAB.D].PS0.u;
        }
    }

    if (instruction.general_DAB.Rc) {
        UPDATE_CR1_FROM_FPSCR(cpu->CR, cpu->FPSCR);
    }
}

INLINE_GEKKO_INSTR(fmul) {
    GEKKO_INSTR_HEADER
    log_cpu_verbose("fmul %x", instruction.raw);

    s_float_result result = float_mul(
            &cpu->FPSCR,
            cpu->FPR[instruction.general_DABC.A].PS0,
            cpu->FPR[instruction.general_DABC.C].PS0
    );

    if (F_SET_RESULT_VX) {
        cpu->FPR[instruction.general_DABC.D].PS0.u = result.value.u;
        UPDATE_FPRF_RESULT_BIT_DOUBLE(cpu, result.value);
    }

    if (instruction.general_DABC.Rc) {
        UPDATE_CR1_FROM_FPSCR(cpu->CR, cpu->FPSCR);
    }
}

INLINE_GEKKO_INSTR(fmuls) {
    // identical to fmul, except the result is cast to a single
    GEKKO_INSTR_HEADER
    log_cpu_verbose("fmul %x", instruction.raw);

    s_float_result result = float_mul(
            &cpu->FPSCR,
            cpu->FPR[instruction.general_DABC.A].PS0,
            cpu->FPR[instruction.general_DABC.C].PS0
    );

    if (F_SET_RESULT_VX) {
        cpu->FPR[instruction.general_DABC.D].PS0.u = TO_SINGLE(result.value).u;
        UPDATE_FPRF_RESULT_BIT_DOUBLE(cpu, result.value);
        if (cpu->HID2.PSE) {
            cpu->FPR[instruction.general_DAB.D].PS1.u = cpu->FPR[instruction.general_DAB.D].PS0.u;
        }
    }

    if (instruction.general_DABC.Rc) {
        UPDATE_CR1_FROM_FPSCR(cpu->CR, cpu->FPSCR);
    }
}

INLINE_GEKKO_INSTR(fmsubs) {
    // equivalent to fmadd, but with subtraction the result is converted to a single
    GEKKO_INSTR_HEADER
    log_cpu_verbose("fmadd %x", instruction.raw);

    s_float_result result = float_msub(
            &cpu->FPSCR,
            cpu->FPR[instruction.general_DABC.A].PS0,
            cpu->FPR[instruction.general_DABC.B].PS0,
            cpu->FPR[instruction.general_DABC.C].PS0
    );

    if (F_SET_RESULT_VX) {
        cpu->FPR[instruction.general_DABC.D].PS0.u = TO_SINGLE(result.value).u;
        UPDATE_FPRF_RESULT_BIT_DOUBLE(cpu, result.value);
        if (cpu->HID2.PSE) {
            cpu->FPR[instruction.general_DAB.D].PS1.u = cpu->FPR[instruction.general_DAB.D].PS0.u;
        }
    }

    if (instruction.general_DABC.Rc) {
        UPDATE_CR1_FROM_FPSCR(cpu->CR, cpu->FPSCR);
    }
}


INLINE_GEKKO_INSTR(fmadd) {
    GEKKO_INSTR_HEADER
    log_cpu_verbose("fmadd %x", instruction.raw);

    s_float_result result = float_madd(
            &cpu->FPSCR,
            cpu->FPR[instruction.general_DABC.A].PS0,
            cpu->FPR[instruction.general_DABC.B].PS0,
            cpu->FPR[instruction.general_DABC.C].PS0
            );

    if (F_SET_RESULT_VX) {
        cpu->FPR[instruction.general_DABC.D].PS0.u = result.value.u;
        UPDATE_FPRF_RESULT_BIT_DOUBLE(cpu, result.value);
    }

    if (instruction.general_DABC.Rc) {
        UPDATE_CR1_FROM_FPSCR(cpu->CR, cpu->FPSCR);
    }
}

INLINE_GEKKO_INSTR(fmadds) {
    // equivalent to fmadd, but the result is converted to a single
    GEKKO_INSTR_HEADER
    log_cpu_verbose("fmadd %x", instruction.raw);

    s_float_result result = float_madd(
            &cpu->FPSCR,
            cpu->FPR[instruction.general_DABC.A].PS0,
            cpu->FPR[instruction.general_DABC.B].PS0,
            cpu->FPR[instruction.general_DABC.C].PS0
    );

    if (F_SET_RESULT_VX) {
        cpu->FPR[instruction.general_DABC.D].PS0.u = TO_SINGLE(result.value).u;
        UPDATE_FPRF_RESULT_BIT_DOUBLE(cpu, result.value);
        if (cpu->HID2.PSE) {
            cpu->FPR[instruction.general_DAB.D].PS1.u = cpu->FPR[instruction.general_DAB.D].PS0.u;
        }
    }

    if (instruction.general_DABC.Rc) {
        UPDATE_CR1_FROM_FPSCR(cpu->CR, cpu->FPSCR);
    }
}

INLINE_GEKKO_INSTR(fnmsubs) {
    // equivalent to fnmsub, but the result is converted to a single
    GEKKO_INSTR_HEADER
    log_cpu_verbose("fnmsubs %x", instruction.raw);

    s_float_result result = float_msub(
            &cpu->FPSCR,
            cpu->FPR[instruction.general_DABC.A].PS0,
            cpu->FPR[instruction.general_DABC.B].PS0,
            cpu->FPR[instruction.general_DABC.C].PS0
    );

    if (F_SET_RESULT_VX) {
        cpu->FPR[instruction.general_DABC.D].PS0.d = isnan(result.value.d) ? result.value.d : -TO_SINGLE(result.value).d;
        UPDATE_FPRF_RESULT_BIT_DOUBLE(cpu, cpu->FPR[instruction.general_DABC.D].PS0);
        if (cpu->HID2.PSE) {
            cpu->FPR[instruction.general_DAB.D].PS1.u = cpu->FPR[instruction.general_DAB.D].PS0.u;
        }
    }

    if (instruction.general_DABC.Rc) {
        UPDATE_CR1_FROM_FPSCR(cpu->CR, cpu->FPSCR);
    }
}

INLINE_GEKKO_INSTR(fnmadds) {
    // equivalent to fnmsubs, but with -
    GEKKO_INSTR_HEADER
    log_cpu_verbose("fnmadds %x", instruction.raw);

    s_float_result result = float_madd(
            &cpu->FPSCR,
            cpu->FPR[instruction.general_DABC.A].PS0,
            cpu->FPR[instruction.general_DABC.B].PS0,
            cpu->FPR[instruction.general_DABC.C].PS0
    );

    if (F_SET_RESULT_VX) {
        cpu->FPR[instruction.general_DABC.D].PS0.d = isnan(result.value.d) ? result.value.d : -TO_SINGLE(result.value).d;
        UPDATE_FPRF_RESULT_BIT_DOUBLE(cpu, result.value);
        if (cpu->HID2.PSE) {
            cpu->FPR[instruction.general_DAB.D].PS1.u = cpu->FPR[instruction.general_DAB.D].PS0.u;
        }
    }

    if (instruction.general_DABC.Rc) {
        UPDATE_CR1_FROM_FPSCR(cpu->CR, cpu->FPSCR);
    }
}

INLINE_GEKKO_INSTR(frsqrte) {
    GEKKO_INSTR_HEADER
    log_cpu_verbose("frsqrte %x", instruction.raw);

    s_float_result result = float_rsqrte(&cpu->FPSCR, cpu->FPR[instruction.general_DAB.B].PS0);

    if (F_SET_RESULT_VX_ZX) {
        cpu->FPR[instruction.general_DAB.D].PS0.u = result.value.u;
        UPDATE_FPRF_RESULT_BIT_DOUBLE(cpu, result.value);
    }

    if (instruction.general_DAB.Rc) {
        UPDATE_CR1_FROM_FPSCR(cpu->CR, cpu->FPSCR);
    }
}