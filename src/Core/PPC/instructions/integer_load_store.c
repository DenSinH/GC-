#include "PPC/instructions.h"
#include "../MMU.h"

GEKKO_INSTR(stw) {
    ASSERT_BITFIELD_SIZE

    log_cpu("stw %x", instruction.raw);

    u32 EA = (instruction.int_store.A ? cpu->GPR[instruction.int_store.A] : 0) + (i32)((i16)(instruction.int_store.d));
    write32(&cpu->DMMU, EA, cpu->GPR[instruction.int_store.S]);
}

GEKKO_INSTR(stwu) {
    ASSERT_BITFIELD_SIZE

    log_cpu("stwu %x", instruction.raw);

    u32 EA = cpu->GPR[instruction.int_store.A] + (i32)((i16)(instruction.int_store.d));
    write32(&cpu->DMMU, EA, cpu->GPR[instruction.int_store.S]);
    cpu->GPR[instruction.int_store.A] = EA;
}

GEKKO_INSTR(lwz) {
    ASSERT_BITFIELD_SIZE

    log_cpu("lwz %x", instruction.raw);

    u32 EA = (instruction.int_load.A ? cpu->GPR[instruction.int_load.A] : 0) + (i32)((i16)(instruction.int_load.d));
    cpu->GPR[instruction.int_load.D] = read32(&cpu->DMMU, EA);
}