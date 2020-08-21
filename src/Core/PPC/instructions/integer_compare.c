#include "PPC/instructions.h"

GEKKO_INSTR(cmpli) {
    ASSERT_BITFIELD_SIZE
    log_cpu("cmpli %x", instruction.raw);

    u8 c = cpu->XER.SO;
    if (cpu->GPR[instruction.cmp_uimm.A] < instruction.cmp_uimm.UIMM) {
        c |= 0b1000;
    }
    else if (cpu->GPR[instruction.cmp_uimm.A] > instruction.cmp_uimm.UIMM) {
        c |= 0b0100;
    }
    else {
        c |= 0b0010;
    }
    SET_CRn_RAW(cpu->CR, instruction.cmp_uimm.crfD, c);
}