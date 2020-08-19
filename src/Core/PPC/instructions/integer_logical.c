#include "PPC/instructions.h"


GEKKO_INSTR(ori) {
    STATIC_ASSERT
    log("ori %08x", instruction.raw);
    cpu->GPR[instruction.arithmetic_uimm.A] = cpu->GPR[instruction.arithmetic_uimm.A] | (u32)instruction.arithmetic_uimm.UIMM;
}