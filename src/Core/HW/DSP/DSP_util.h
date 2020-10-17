#ifndef GC__DSP_UTIL_H
#define GC__DSP_UTIL_H

#include "DSP.h"

#include "log.h"

#include <stdbool.h>

static inline void DSP_PUSH_STACK(s_DSP* DSP, unsigned index, u16 value) {
    DSP->stacks[index][DSP->st[index]++] = value;
}

static inline u16 DSP_POP_STACK(s_DSP* DSP, unsigned index) {
    return DSP->stacks[index][--DSP->st[index]];
}

static inline u16* DSP_PEEK_STACK(s_DSP* DSP, unsigned index) {
    return &DSP->stacks[index][DSP->st[index] - 1];
}

static inline void DSP_ext_instruction(s_DSP* DSP, u8 instruction) {
    DSP->ext_instructions[instruction](DSP, instruction);
}

static inline void DSP_FLAGS(s_DSP* DSP, i64 result, bool carry, bool overflow) {
    DSP->sr.C = carry;
    DSP->sr.O = overflow;
    DSP->sr.OS |= overflow;
    DSP->sr.Z = result == 0;
    DSP->sr.S = result < 0;
    DSP->sr.AS = ((i32)result) != result;  // above s32
    DSP->sr.TT = ((result & 0xc0000000) == 0) || ((result & 0xc0000000) == 0xc0000000);
}

static inline bool DSP_condition(s_DSP* DSP, u8 cond) {
    switch (cond) {
        // top 4 from the ARM7TDMI data sheet:
        case DSP_cond_GE:
            return DSP->sr.O == DSP->sr.S;
        case DSP_cond_L:
            return DSP->sr.O != DSP->sr.S;
        case DSP_cond_G:
            return (DSP->sr.O == DSP->sr.S) && !DSP->sr.Z;
        case DSP_cond_LE:
            return (DSP->sr.O != DSP->sr.S) || DSP->sr.Z;
        // rest is described in the GC DSP datasheet on my GCResources repo
        case DSP_cond_NE:
            return !DSP->sr.Z;
        case DSP_cond_EQ:
            return DSP->sr.Z;
        case DSP_cond_NC:
            return !DSP->sr.C;
        case DSP_cond_C:
            return DSP->sr.C;
        case DSP_cond_B32:
            return !DSP->sr.AS;
        case DSP_cond_A32:
            return DSP->sr.AS;
        case DSP_cond_NZ:
            return !DSP->sr.LZ;
        case DSP_cond_ZR:
            return DSP->sr.LZ;
        case DSP_cond_O:
            return DSP->sr.O;
        case DSP_cond_ALWAYS:
            return true;
        default:
            log_fatal("Invalid DSP condition: %x", cond);
    }
}

#endif //GC__DSP_UTIL_H
