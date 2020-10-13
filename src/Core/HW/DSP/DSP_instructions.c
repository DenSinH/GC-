#include "DSP_instructions.h"

#include "DSP_memory.h"
#include "log.h"

DSP_INSTR(DSP_unimplemented) {
    log_fatal("[DSP] Unimplemented instruction at DSP.pc=%04x (%04x)", DSP->pc - 1, instruction);
}

static inline DSP_INSTR(DSP_LRI) {
    log_dsp_verbose("LRI %d #%02x (%04x)", instruction & 0x1f, DSP_read_imem(DSP, DSP->pc), instruction);

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
    log_dsp_verbose("SBSET %d + 6 (%04x)", instruction & 0x03, instruction);

    DSP->sr.raw |= 1 << ((instruction & 0x03) + 6);
}

DSP_INSTR(DSP_SET16) {
    log_dsp_verbose("SET16 (%04x)", instruction);

    DSP->sr.SXM = 0;
    // todo: extended opcode
}

DSP_INSTR(DSP_SET40) {
    log_dsp_verbose("SET16 (%04x)", instruction);

    DSP->sr.SXM = 1;
    // todo: extended opcode
}
