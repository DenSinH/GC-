#include "PPC/instructions.h"


INLINE_GEKKO_INSTR(sync) {
    GEKKO_INSTR_HEADER
    log_cpu("sync %08x", instruction.raw);
}

INLINE_GEKKO_INSTR(isync) {
    GEKKO_INSTR_HEADER
    log_cpu("isync %08x", instruction.raw);
}