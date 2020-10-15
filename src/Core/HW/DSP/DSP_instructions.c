#include "DSP_instructions.h"

#include "DSP.h"
#include "DSP_memory.h"
#include "DSP_util.h"

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

static inline DSP_INSTR(DSP_CALL_cc) {
    u16 addr = DSP_read_imem(DSP, DSP->pc++);
    log_dsp_verbose("CALL cc(%x) -> %04x", instruction & 0xf, addr);

    if (DSP_condition(DSP, instruction & 0xf)) {
        log_dsp_verbose(" +-> Condition met");
        DSP_PUSH_STACK(DSP, 0, DSP->pc);  // address of next instruction
        DSP->pc = addr;
    }
}

DSP_INSTR(DSP_0000_001r) {
    switch (instruction & 0x3ff) {
        case 0x02b0 ... 0x02bf:
            DSP_CALL_cc(DSP, instruction);
            break;
        default:
            DSP_unimplemented(DSP, instruction);
    }
}

DSP_INSTR(DSP_SBSET) {
    log_dsp_verbose("SBSET %d + 6 (%04x)", instruction & 0x07, instruction);

    DSP->sr.raw |= 1 << ((instruction & 0x07) + 6);
}

DSP_INSTR(DSP_SET16) {
    log_dsp_verbose("SET16 (%04x)", instruction);

    DSP->sr.SXM = 0;
    DSP_ext_instruction(DSP, (u8)instruction);
}

DSP_INSTR(DSP_SET40) {
    log_dsp_verbose("SET16 (%04x)", instruction);

    DSP->sr.SXM = 1;
    DSP_ext_instruction(DSP, (u8)instruction);
}

DSP_INSTR(DSP_SET15) {
    log_dsp_verbose("SET15 (%04x)", instruction);

    DSP->sr.SU = 1;
    DSP_ext_instruction(DSP, (u8)instruction);
}

DSP_INSTR(DSP_CLR15) {
    log_dsp_verbose("CLR15 (%04x)", instruction);

    DSP->sr.SU = 0;
    DSP_ext_instruction(DSP, (u8)instruction);
}

DSP_INSTR(DSP_M0) {
    log_dsp_verbose("M0 (%04x)", instruction);

    DSP->sr.AM = 1;
    DSP_ext_instruction(DSP, (u8)instruction);
}

DSP_INSTR(DSP_M2) {
    log_dsp_verbose("M2 (%04x)", instruction);

    DSP->sr.AM = 0;
    DSP_ext_instruction(DSP, (u8)instruction);
}

DSP_INSTR(DSP_SI) {
    u16 m   = (u16)((i16)((i8)(instruction & 0xff)));
    u16 imm = DSP_read_imem(DSP, DSP->pc++);

    log_dsp_verbose("SI %04x -> [%04x] (%04x)", imm, m, instruction);
    DSP_write_dmem(DSP, m, imm);
}

DSP_INSTR(DSP_CLR) {
    u8 r = (instruction & 0x0800) >> 11;

    log_dsp_verbose("CLR ac%x (%04x)", r, instruction);
    DSP->ac[r].raw = 0;
}