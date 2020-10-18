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

/*
 * Dolphin's code seemed very efficient for this
 * */

static inline u16 DSP_increment_ar(s_DSP* DSP, int index) {
    u32 ar = DSP->ar[index];
    u32 wr = DSP->wr[index];

    if (wr == 0xffff) {
        // wrapping is not needed in this case
        DSP->ar[index]++;
        return DSP->ar[index];
    }

    u32 nar = ar + 1;
    // basically: if the overflow from the addition operation is more than the wrapping amount, subtract it again
    if ((nar ^ ar) > ((wr | 1) << 1)) {
        nar -= wr + 1;
    }

    return DSP->ar[index] = nar;
}

static inline u16 DSP_decrement_ar(s_DSP* DSP, int index) {
    u32 ar = DSP->ar[index];
    u32 wr = DSP->wr[index];

    if (wr == 0xffff) {
        // wrapping is not needed in this case
        DSP->ar[index]--;
        return DSP->ar[index];
    }

    u32 nar = ar + wr;
    // basically: if the overflow from the addition operation is more than the wrapping amount, subtract it again
    if (((nar ^ ar) & ((wr | 1) << 1)) > wr) {
        nar -= wr + 1;
    }

    return DSP->ar[index] = nar;
}

static inline u16 DSP_increase_ar(s_DSP* DSP, int index, i16 amount) {
    u32 ar = DSP->ar[index];
    u32 wr = DSP->wr[index];
    i32 ix = amount;

    if (wr == 0xffff) {
        // wrapping is not needed in this case
        DSP->ar[index] += amount;
        return DSP->ar[index];
    }

    u32 mx = (wr | 1) << 1;
    u32 nar = ar + ix;
    u32 dar = (nar ^ ar ^ ix) & mx;

    if (ix >= 0)
    {
        if (dar > wr)  // overflow
            nar -= wr + 1;
    }
    else
    {
        if ((((nar + wr + 1) ^ nar) & dar) <= wr)
            nar += wr + 1;
    }

    return DSP->ar[index] = nar;
}

static inline u16 DSP_decrease_ar(s_DSP* DSP, int index, i16 amount) {
    u32 ar = DSP->ar[index];
    u32 wr = DSP->wr[index];
    i32 ix = amount;

    if (wr == 0xffff) {
        // wrapping is not needed in this case
        DSP->ar[index] -= amount;
        return DSP->ar[index];
    }

    u32 mx = (wr | 1) << 1;
    u32 nar = ar - ix;
    u32 dar = (nar ^ ar ^ ~ix) & mx;

    if ((u32)ix > 0xFFFF8000)  // (ix < 0 && ix != -0x8000)
    {
        if (dar > wr)  // overflow
            nar -= wr + 1;
    }
    else
    {
        if ((((nar + wr + 1) ^ nar) & dar) <= wr)  // underflow or below min for mask
            nar += wr + 1;
    }

    return DSP->ar[index] = nar;
}

#endif //GC__DSP_UTIL_H
