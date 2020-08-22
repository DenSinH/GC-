#include "PPC/instructions.h"

#include "PPC/MMU.h"

GEKKO_INSTR(lfd) {
    GEKKO_INSTR_HEADER
    log_cpu("lfd %08x", instruction.raw);

    u32 EA = (instruction.general_DAd.A ? cpu->GPR[instruction.general_DAd.A] : 0) + (i32)((i16)instruction.general_DAd.d);
    cpu->FPR[instruction.general_DAd.D].PS0 = read64(&cpu->DMMU, EA);
}