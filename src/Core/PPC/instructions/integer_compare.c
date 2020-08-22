#include "PPC/instructions.h"

GEKKO_INSTR(cmpli) {
    GEKKO_INSTR_HEADER
    log_cpu("cmpli %x", instruction.raw);

    u8 c = GET_CRn_CMP_U32(cpu, cpu->GPR[instruction.cmp_uimm.A], instruction.cmp_uimm.UIMM);
    SET_CRn_RAW(cpu->CR, instruction.cmp_uimm.crfD, c);
}

INLINE_GEKKO_INSTR(cmp) {
    GEKKO_INSTR_HEADER
    log_cpu("cmp %x", instruction.raw);

    u8 c = GET_CRn_CMP_U32(cpu, cpu->GPR[instruction.cmp.A], cpu->GPR[instruction.cmp.B]);
    SET_CRn_RAW(cpu->CR, instruction.cmp.crfD, c);
}