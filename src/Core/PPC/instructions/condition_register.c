#include "PPC/instructions.h"

INLINE_GEKKO_INSTR(crxor) {
    GEKKO_INSTR_HEADER
    log_cpu("crxor %x", instruction.raw);

    u32 bitA = (cpu->CR.raw >> instruction.general_DAB.A) & 1;
    u32 bitB = (cpu->CR.raw >> instruction.general_DAB.B) & 1;
    cpu->CR.raw = (cpu->CR.raw & ~(1 << instruction.general_DAB.D));
    if (bitA ^ bitB) {
        cpu->CR.raw |= 1 << instruction.general_DAB.D;
    }
}