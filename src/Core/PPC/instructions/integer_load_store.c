#include "PPC/instructions.h"
#include "../MMU.h"

GEKKO_INSTR(stw) {
    ASSERT_BITFIELD_SIZE

    log_cpu("stw %x", instruction.raw);

    u32 EA = (instruction.general_SAd.A ? cpu->GPR[instruction.general_SAd.A] : 0) + (i32)((i16)(instruction.general_SAd.d));
    write32(&cpu->DMMU, EA, cpu->GPR[instruction.general_SAd.S]);
}

GEKKO_INSTR(stwu) {
    ASSERT_BITFIELD_SIZE

    log_cpu("stwu %x", instruction.raw);

    u32 EA = cpu->GPR[instruction.general_SAd.A] + (i32)((i16)(instruction.general_SAd.d));
    write32(&cpu->DMMU, EA, cpu->GPR[instruction.general_SAd.S]);
    cpu->GPR[instruction.general_SAd.A] = EA;
}

GEKKO_INSTR(lwz) {
    ASSERT_BITFIELD_SIZE

    log_cpu("lwz %x", instruction.raw);

    u32 EA = (instruction.general_DAd.A ? cpu->GPR[instruction.general_DAd.A] : 0) + (i32)((i16)(instruction.general_DAd.d));
    cpu->GPR[instruction.general_DAd.D] = read32(&cpu->DMMU, EA);
}