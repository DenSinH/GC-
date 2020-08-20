#include "PPC/instructions.h"

#include "PPC/MMU.h"

GEKKO_INSTR(lfd) {
    ASSERT_BITFIELD_SIZE
    log_cpu("lfd %08x", instruction.raw);

    u32 EA = (instruction.general_DAd.A ? cpu->GPR[instruction.general_DAd.A] : 0) + (i32)((i16)instruction.general_DAd.d);
    cpu->FPR[instruction.general_DAd.D] = read64(&cpu->DMMU, EA);
}