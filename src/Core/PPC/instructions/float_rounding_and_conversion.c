#include "PPC/instructions.h"
#include "float_utils.h"

INLINE_GEKKO_INSTR(fctiwz) {
    GEKKO_INSTR_HEADER
    log_cpu("fctiwz %x", instruction.raw);

    s_float_result result = float_round(&cpu->FPSCR, cpu->FPR[instruction.general_DAB.B].PS0, RN_ROUND_TOWARD_ZERO);

    if (!((cpu->FPSCR.VE && result.exception) || (cpu->FPSCR.ZX && cpu->FPSCR.ZE))) {
        // FPRF undefined
        cpu->FPR[instruction.general_DAB.D].PS0.u = result.value.u;
    }

    if (instruction.general_DAB.Rc) {
        UPDATE_CR1_FROM_FPSCR(cpu->CR, cpu->FPSCR);
    }
}