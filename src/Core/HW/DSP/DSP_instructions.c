#include "DSP_instructions.h"

#include "DSP_memory.h"
#include "log.h"

DSP_INSTR(DSP_unimplemented) {
    log_fatal("[DSP] Unimplemented instruction at %04x (%04x)", DSP->pc - 1, instruction);
}

static inline DSP_INSTR(DSP_LRI) {
    *DSP->r[instruction & 0x1f] = DSP_read_imem(DSP, DSP->pc++);
}

DSP_INSTR(DSP_0000_0000) {
    switch (instruction & 0xff) {
        case 0x80 ... 0x9f:
            DSP_LRI(DSP, instruction);
            break;
        default:
            DSP_unimplemented(DSP, instruction);
    }
}

DSP_INSTR(DSP_SBSET) {
    DSP->sr.raw |= 1 << ((instruction & 0x3) + 6);
}