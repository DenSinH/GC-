#include "PPC/instructions.h"

INLINE_GEKKO_INSTR(fmr) {
    GEKKO_INSTR_HEADER
    log_cpu("fmr %08x", instruction.raw);

    // todo: when is FPR a paired single? Dolphin always just moves PS0...
    // todo: exception flags?
    cpu->FPR[instruction.general_DAB.D].PS0 = cpu->FPR[instruction.general_DAB.B].PS0;
    if (instruction.general_DAB.Rc) {
        UPDATE_CR_FROM_FPSCR(cpu->CR, cpu->FPSCR);
    }
}