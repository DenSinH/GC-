#include "PPC/instructions.h"
#include "float_utils.h"

INLINE_GEKKO_INSTR(fdivs) {
    GEKKO_INSTR_HEADER
    log_cpu("fdivs %x", instruction.raw);

    s_float_result result = float_div(&cpu->FPSCR, cpu->FPR[instruction.general_DAB.A].PS0, cpu->FPR[instruction.general_DAB.B].PS0);

    if (!((cpu->FPSCR.VE && result.exception) || (cpu->FPSCR.ZX && cpu->FPSCR.ZE))) {
        cpu->FPR[instruction.general_DAB.D].PS0.u = TO_SINGLE(result.value).d;
        UPDATE_FPRF_RESULT_BIT_DOUBLE(cpu, result.value);
        if (cpu->HID2.PSE) {
            cpu->FPR[instruction.general_DAB.D].PS1.u = cpu->FPR[instruction.general_DAB.D].PS0.u;
        }
    }

    if (instruction.general_DAB.Rc) {
        UPDATE_CR1_FROM_FPSCR(cpu->CR, cpu->FPSCR);
    }
}