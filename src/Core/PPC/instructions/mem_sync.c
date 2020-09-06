#include "instructions.h"


INLINE_GEKKO_INSTR(sync) {
    GEKKO_INSTR_HEADER
    log_cpu("sync %08x", instruction.raw);
}

INLINE_GEKKO_INSTR(isync) {
    GEKKO_INSTR_HEADER
    log_cpu("isync %08x", instruction.raw);
}

INLINE_GEKKO_INSTR(dcbf) {
    GEKKO_INSTR_HEADER

    log_cpu("dcbf %08x", instruction.raw);
}

INLINE_GEKKO_INSTR(dcbi) {
    GEKKO_INSTR_HEADER

    log_cpu("dcbi %08x", instruction.raw);
}

INLINE_GEKKO_INSTR(icbi) {
    GEKKO_INSTR_HEADER

    log_cpu("icbi %08x", instruction.raw);
}