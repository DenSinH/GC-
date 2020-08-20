#include "PPC/instructions.h"

GEKKO_INSTR(rlwinm) {
    ASSERT_BITFIELD_SIZE
    log_cpu("rlwinm %08x", instruction.raw);

    cpu->GPR[instruction.rotate.A] =
            (ROTL32(cpu->GPR[instruction.rotate.S], instruction.rotate.S)) &
            (MASK(instruction.rotate.MB, instruction.rotate.ME));

    if (instruction.rotate.Rc) {
        // alter condition codes
        cpu->CR.GR0.LT = (cpu->GPR[instruction.rotate.A] & 0x80000000) != 0;
        cpu->CR.GR0.GT = (i32)cpu->GPR[instruction.rotate.A] > 0;
        cpu->CR.GR0.EQ = !cpu->GPR[instruction.rotate.A];
        // todo: can this cause overflow?
        cpu->CR.GR0.SO = cpu->XER.SO = 0;
    }
}