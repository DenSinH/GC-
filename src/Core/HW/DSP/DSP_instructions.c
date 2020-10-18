#include "DSP_instructions.h"

#include "DSP.h"
#include "DSP_memory.h"
#include "DSP_util.h"

#include "helpers.h"
#include "log.h"

#define log_dsp_instr(message, ...) log_dsp_verbose("[%04x] " message " (%04x)", DSP->pc - 1, ##__VA_ARGS__, instruction)

DSP_INSTR(DSP_unimplemented) {
    log_fatal("[DSP] Unimplemented instruction at DSP.pc=%04x (%04x)", DSP->pc - 1, instruction);
    return 1;
}

static inline DSP_INSTR(DSP_HALT) {
    log_dsp_instr("HALT");

    DSP->halted = true;
    return 1;
}

static inline DSP_INSTR(DSP_LR) {
    u8 d = instruction & 0x1f;
    u16 m = DSP_read_imem(DSP, DSP->pc++);
    log_dsp_instr("LR r%x [%02x]", instruction & 0x1f, m);

    *DSP->r[d] = DSP_read_dmem(DSP, m);

    return 1;
}

static inline DSP_INSTR(DSP_LRI) {
    log_dsp_instr("LRI r%x #%02x", instruction & 0x1f, DSP_read_imem(DSP, DSP->pc));

    *DSP->r[instruction & 0x1f] = DSP_read_imem(DSP, DSP->pc++);

    return 1;
}

static inline DSP_INSTR(DSP_LOOP) {
    u8 r     = instruction & 0x1f;
    log_dsp_instr("LOOP %x", r);

    u16 counter = *DSP->r[r];
    int cycles = 0;

    u16 instr = DSP_read_imem(DSP, DSP->pc++);
    while (counter--) {
        log_dsp_instr(" --> loop %x times", counter);
        cycles += DSP->instructions[instr >> 8](DSP, instr);
    }

    return cycles;
}

static inline DSP_INSTR(DSP_BLOOP) {
    u8 r     = instruction & 0x1f;
    u16 addr = DSP_read_imem(DSP, DSP->pc++);

    log_dsp_instr("BLOOP r%x (=%04x), %04x", r, *DSP->r[r], addr);

    DSP_PUSH_STACK(DSP, 0, DSP->pc);     // repeat address
    DSP_PUSH_STACK(DSP, 2, addr);        // end of block address
    DSP_PUSH_STACK(DSP, 3, *DSP->r[r]);  // counter

    u16* st0 = DSP_PEEK_STACK(DSP, 0);
    u16* st2 = DSP_PEEK_STACK(DSP, 2);
    u16* st3 = DSP_PEEK_STACK(DSP, 3);
    int cycles = 1;
    while (*st3) {
        *st3 -= 1;
        log_dsp_instr(" --> loop %x times", *st3);
        do {
            cycles += step_DSP(DSP);
        } while (DSP->pc != (*st2) + 1); // PC is one ahead of the instruction, end is inclusive
        DSP->pc = *st0;
    }
    DSP->pc = addr + 1;

    DSP_POP_STACK(DSP, 0);
    DSP_POP_STACK(DSP, 2);
    DSP_POP_STACK(DSP, 3);

    return cycles;
}

DSP_INSTR(DSP_0000_0000) {
    switch (instruction & 0xff) {
        case 0x00:
            log_dsp_instr("NOP");
            return 1;
        case 0x21:
            return DSP_HALT(DSP, instruction);
        case 0x40 ... 0x5f:
            return DSP_LOOP(DSP, instruction);
        case 0x60 ... 0x7f:
            return DSP_BLOOP(DSP, instruction);
        case 0x80 ... 0x9f:
            return DSP_LRI(DSP, instruction);
        case 0xc0 ... 0xdf:
            return DSP_LR(DSP, instruction);
        default:
            return DSP_unimplemented(DSP, instruction);
    }
}

static inline DSP_INSTR(DSP_JMP_cc) {
    u16 addr = DSP_read_imem(DSP, DSP->pc++);
    log_dsp_instr("JMP cc(%x) -> %04x", instruction & 0xf, addr);

    if (DSP_condition(DSP, instruction & 0xf)) {
        log_dsp_instr(" +-> Condition met");
        DSP->pc = addr;
    }
    return 2;
}

static inline DSP_INSTR(DSP_CALL_cc) {
    u16 addr = DSP_read_imem(DSP, DSP->pc++);
    log_dsp_instr("CALL cc(%x) -> %04x", instruction & 0xf, addr);

    if (DSP_condition(DSP, instruction & 0xf)) {
        log_dsp_instr(" +-> Condition met");
        DSP_PUSH_STACK(DSP, 0, DSP->pc);  // address of next instruction
        DSP->pc = addr;
    }
    return 2;
}

static inline DSP_INSTR(DSP_RET_cc) {
    log_dsp_instr("RET cc(%x)", instruction & 0xf);

    if (DSP_condition(DSP, instruction & 0xf)) {
        log_dsp_instr(" +-> Condition met");
        DSP->pc = DSP_POP_STACK(DSP, 0);
    }
    return 2;
}

static inline DSP_INSTR(DSP_RTI) {
    log_dsp_instr("RTI");

    DSP->pc = DSP_POP_STACK(DSP, 0);
    DSP->sr.raw = DSP_POP_STACK(DSP, 1);
    return 1;
}

static inline DSP_INSTR(DSP_ANDF) {
    u8 r = (instruction & 0x0100) >> 8;
    u16 value = DSP_read_imem(DSP, DSP->pc++);

    log_dsp_instr("ANDF ac%x.m, #%04x", r, value);

    DSP->sr.LZ = (DSP->ac[r].m & value) == 0;
    return 1;
}

static inline DSP_INSTR(DSP_ANDCF) {
    u8 r = (instruction & 0x0100) >> 8;
    u16 value = DSP_read_imem(DSP, DSP->pc++);

    log_dsp_instr("ANDCF ac%x.m, #%04x", r, value);

    DSP->sr.LZ = (DSP->ac[r].m & value) == value;

    return 1;
}

static inline DSP_INSTR(DSP_ILRRI) {
    u8 d = (instruction & 0x0100) >> 8;
    u8 ss = (instruction & 0x0003);

    log_dsp_instr("ILRRI ac%d.m, @ar%d", d, ss);

    DSP->ac[d].m = DSP_read_imem(DSP, DSP->ar[ss]);
    DSP_increment_ar(DSP, ss);

    return 1;
}

DSP_INSTR(DSP_0000_001r) {
    switch (instruction & 0x3ff) {
        case 0x0218 ... 0x021b:
        case 0x0318 ... 0x031b:
            return DSP_ILRRI(DSP, instruction);
        case 0x0290 ... 0x029f:
            return DSP_JMP_cc(DSP, instruction);
        case 0x02b0 ... 0x02bf:
            return DSP_CALL_cc(DSP, instruction);
        case 0x02d0 ... 0x02df:
            return DSP_RET_cc(DSP, instruction);
        // the following 2 opcodes are wrong in the doc:
        case 0x02a0:
        case 0x03a0:
            return DSP_ANDF(DSP, instruction);
        case 0x02c0:
        case 0x03c0:
            return DSP_ANDCF(DSP, instruction);
        case 0x02ff:
            return DSP_RTI(DSP, instruction);
        default:
            return DSP_unimplemented(DSP, instruction);
    }
}

DSP_INSTR(DSP_SBSET) {
    log_dsp_instr("SBSET %d + 6", instruction & 0x07);

    DSP->sr.raw |= 1 << ((instruction & 0x07) + 6);
    return 1;
}

DSP_INSTR(DSP_SET16) {
    log_dsp_instr("SET16");

    DSP->sr.SXM = 0;
    DSP_ext_instruction(DSP, (u8)instruction);
    return 1;
}

DSP_INSTR(DSP_SET40) {
    log_dsp_instr("SET16");

    DSP->sr.SXM = 1;
    DSP_ext_instruction(DSP, (u8)instruction);
    return 1;
}

DSP_INSTR(DSP_SET15) {
    log_dsp_instr("SET15");

    DSP->sr.SU = 1;
    DSP_ext_instruction(DSP, (u8)instruction);
    return 1;
}

DSP_INSTR(DSP_CLR15) {
    log_dsp_instr("CLR15");

    DSP->sr.SU = 0;
    DSP_ext_instruction(DSP, (u8)instruction);
    return 1;
}

DSP_INSTR(DSP_M0) {
    log_dsp_instr("M0");

    DSP->sr.AM = 1;
    DSP_ext_instruction(DSP, (u8)instruction);
    return 1;
}

DSP_INSTR(DSP_M2) {
    log_dsp_instr("M2");

    DSP->sr.AM = 0;
    DSP_ext_instruction(DSP, (u8)instruction);
    return 1;
}

DSP_INSTR(DSP_SI) {
    u16 m   = (u16)((i16)((i8)(instruction & 0xff)));
    u16 imm = DSP_read_imem(DSP, DSP->pc++);

    log_dsp_instr("SI %04x -> [%04x]", imm, m);
    DSP_write_dmem(DSP, m, imm);
    return 1;
}

DSP_INSTR(DSP_CLR) {
    u8 r = (instruction & 0x0800) >> 11;

    log_dsp_instr("CLR ac%x", r);
    DSP->ac[r].raw = 0;
    return 1;
}

DSP_INSTR(DSP_LRS) {
    u16 addr = (i16)((i8)(instruction & 0xff));
    u8 ddd   = (instruction & 0x0700) >> 8;
    log_dsp_instr("LRS r%x <- [%04x]", 0x18 + ddd, addr);

    *DSP->r[0x18 + ddd] = DSP_read_dmem(DSP, addr);
    return 1;
}

DSP_INSTR(DSP_SRS) {
    u16 addr = (i16)((i8)(instruction & 0xff));
    u8 ddd   = (instruction & 0x0700) >> 8;
    log_dsp_instr("SRS r%x -> [%04x]", 0x18 + ddd, addr);

    DSP_write_dmem(DSP, addr, *DSP->r[0x18 + ddd]);
    return 1;
}

DSP_INSTR(DSP_CMP) {
    log_dsp_instr("CMP");

    DSP_FLAGS(DSP,
              DSP->ac[0].lmh - DSP->ac[1].lmh,
              ADD_CARRY((i64)DSP->ac[0].lmh, (i64)DSP->ac[1].lmh),
              ADD_OVERFLOW64((i64)DSP->ac[0].lmh, (i64)DSP->ac[1].lmh, DSP->ac[0].lmh - DSP->ac[1].lmh));

    DSP_ext_instruction(DSP, (u8)instruction);
    return 1;
}

DSP_INSTR(DSP_MRR) {
    u8 d = (instruction & 0x03e0) >> 5;
    u8 s = instruction & 0x1f;
    log_dsp_instr("MRR r%02x, r%02x", d, s);

    *DSP->r[d] = *DSP->r[s];
    return 1;
}

DSP_INSTR(DSP_LRRx) {
    u8 type = (instruction & 0x0180) >> 7;
    u8 s = (instruction & 0x0060) >> 5;
    u8 d = (instruction & 0x1f);

    switch (type) {
        case 0b010:
            log_dsp_instr("LRRI @ar%d, r%x", d, s);
            *DSP->r[d] = DSP_read_dmem(DSP, DSP->ar[s]);
            DSP_increment_ar(DSP, s);
            return 1;
        default:
            return DSP_unimplemented(DSP, instruction);
    }
}

DSP_INSTR(DSP_SRRx) {
    u8 type = (instruction & 0x0180) >> 7;
    u8 d = (instruction & 0x0060) >> 5;
    u8 s = (instruction & 0x1f);

    switch (type) {
        case 0b010:
            log_dsp_instr("SRRI @ar%d, r%x", d, s);
            DSP_write_dmem(DSP, DSP->ar[d], *DSP->r[s]);
            DSP_increment_ar(DSP, d);
            return 1;
        default:
            return DSP_unimplemented(DSP, instruction);
    }
}