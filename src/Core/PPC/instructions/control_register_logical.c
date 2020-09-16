#include "instructions.h"

INLINE_GEKKO_INSTR(mcrf) {
    GEKKO_INSTR_HEADER
    log_cpu_verbose("mcrf %x", instruction.raw);

    SET_CRn_RAW(cpu->CR, instruction.mcrf.crfD, GET_CRn_RAW(cpu->CR, instruction.mcrf.crfS));
}

/* remember: bit 0 is most significant bit, so we need to reason from 0x80000000 */
INLINE_GEKKO_INSTR(creqv) {
    GEKKO_INSTR_HEADER
    log_cpu_verbose("creqv %x", instruction.raw);

    u32 bitA = (cpu->CR.raw << instruction.general_DAB.A) & 0x80000000;
    u32 bitB = (cpu->CR.raw << instruction.general_DAB.B) & 0x80000000;
    cpu->CR.raw = (cpu->CR.raw & ~(0x80000000 >> instruction.general_DAB.D));
    if (~(bitA ^ bitB)) {
        cpu->CR.raw |= 0x80000000 >> instruction.general_DAB.D;
    }
}

INLINE_GEKKO_INSTR(cror) {
    GEKKO_INSTR_HEADER
    log_cpu_verbose("cror %x", instruction.raw);

    u32 bitA = (cpu->CR.raw << instruction.general_DAB.A) & 0x80000000;
    u32 bitB = (cpu->CR.raw << instruction.general_DAB.B) & 0x80000000;
    cpu->CR.raw = (cpu->CR.raw & ~(0x80000000 >> instruction.general_DAB.D));
    if (bitA | bitB) {
        cpu->CR.raw |= 0x80000000 >> instruction.general_DAB.D;
    }
}

INLINE_GEKKO_INSTR(crxor) {
    GEKKO_INSTR_HEADER
    log_cpu_verbose("crxor %x", instruction.raw);

    u32 bitA = (cpu->CR.raw << instruction.general_DAB.A) & 0x80000000;
    u32 bitB = (cpu->CR.raw << instruction.general_DAB.B) & 0x80000000;
    cpu->CR.raw = (cpu->CR.raw & ~(0x80000000 >> instruction.general_DAB.D));
    if (bitA ^ bitB) {
        cpu->CR.raw |= 0x80000000 >> instruction.general_DAB.D;
    }
}