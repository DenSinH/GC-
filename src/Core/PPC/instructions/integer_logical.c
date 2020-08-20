#include "PPC/instructions.h"


GEKKO_INSTR(ori) {
    ASSERT_BITFIELD_SIZE
    log_cpu("ori %08x", instruction.raw);
    cpu->GPR[instruction.arithmetic_uimm.A] = cpu->GPR[instruction.arithmetic_uimm.A] | (u32)instruction.arithmetic_uimm.UIMM;
}

GEKKO_INSTR(oris) {
    ASSERT_BITFIELD_SIZE
    log_cpu("oris %08x", instruction.raw);
    cpu->GPR[instruction.arithmetic_uimm.A] = cpu->GPR[instruction.arithmetic_uimm.A] | ((u32)instruction.arithmetic_uimm.UIMM << 16);
}