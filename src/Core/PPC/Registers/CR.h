#ifndef GC__CR_H
#define GC__CR_H

#include <stdbool.h>

#include "default.h"

const static u32 field_mask[0x100] = {
        0x00000000,0x0000000f,0x000000f0,0x000000ff,0x00000f00,0x00000f0f,0x00000ff0,0x00000fff,
        0x0000f000,0x0000f00f,0x0000f0f0,0x0000f0ff,0x0000ff00,0x0000ff0f,0x0000fff0,0x0000ffff,
        0x000f0000,0x000f000f,0x000f00f0,0x000f00ff,0x000f0f00,0x000f0f0f,0x000f0ff0,0x000f0fff,
        0x000ff000,0x000ff00f,0x000ff0f0,0x000ff0ff,0x000fff00,0x000fff0f,0x000ffff0,0x000fffff,
        0x00f00000,0x00f0000f,0x00f000f0,0x00f000ff,0x00f00f00,0x00f00f0f,0x00f00ff0,0x00f00fff,
        0x00f0f000,0x00f0f00f,0x00f0f0f0,0x00f0f0ff,0x00f0ff00,0x00f0ff0f,0x00f0fff0,0x00f0ffff,
        0x00ff0000,0x00ff000f,0x00ff00f0,0x00ff00ff,0x00ff0f00,0x00ff0f0f,0x00ff0ff0,0x00ff0fff,
        0x00fff000,0x00fff00f,0x00fff0f0,0x00fff0ff,0x00ffff00,0x00ffff0f,0x00fffff0,0x00ffffff,
        0x0f000000,0x0f00000f,0x0f0000f0,0x0f0000ff,0x0f000f00,0x0f000f0f,0x0f000ff0,0x0f000fff,
        0x0f00f000,0x0f00f00f,0x0f00f0f0,0x0f00f0ff,0x0f00ff00,0x0f00ff0f,0x0f00fff0,0x0f00ffff,
        0x0f0f0000,0x0f0f000f,0x0f0f00f0,0x0f0f00ff,0x0f0f0f00,0x0f0f0f0f,0x0f0f0ff0,0x0f0f0fff,
        0x0f0ff000,0x0f0ff00f,0x0f0ff0f0,0x0f0ff0ff,0x0f0fff00,0x0f0fff0f,0x0f0ffff0,0x0f0fffff,
        0x0ff00000,0x0ff0000f,0x0ff000f0,0x0ff000ff,0x0ff00f00,0x0ff00f0f,0x0ff00ff0,0x0ff00fff,
        0x0ff0f000,0x0ff0f00f,0x0ff0f0f0,0x0ff0f0ff,0x0ff0ff00,0x0ff0ff0f,0x0ff0fff0,0x0ff0ffff,
        0x0fff0000,0x0fff000f,0x0fff00f0,0x0fff00ff,0x0fff0f00,0x0fff0f0f,0x0fff0ff0,0x0fff0fff,
        0x0ffff000,0x0ffff00f,0x0ffff0f0,0x0ffff0ff,0x0fffff00,0x0fffff0f,0x0ffffff0,0x0fffffff,
        0xf0000000,0xf000000f,0xf00000f0,0xf00000ff,0xf0000f00,0xf0000f0f,0xf0000ff0,0xf0000fff,
        0xf000f000,0xf000f00f,0xf000f0f0,0xf000f0ff,0xf000ff00,0xf000ff0f,0xf000fff0,0xf000ffff,
        0xf00f0000,0xf00f000f,0xf00f00f0,0xf00f00ff,0xf00f0f00,0xf00f0f0f,0xf00f0ff0,0xf00f0fff,
        0xf00ff000,0xf00ff00f,0xf00ff0f0,0xf00ff0ff,0xf00fff00,0xf00fff0f,0xf00ffff0,0xf00fffff,
        0xf0f00000,0xf0f0000f,0xf0f000f0,0xf0f000ff,0xf0f00f00,0xf0f00f0f,0xf0f00ff0,0xf0f00fff,
        0xf0f0f000,0xf0f0f00f,0xf0f0f0f0,0xf0f0f0ff,0xf0f0ff00,0xf0f0ff0f,0xf0f0fff0,0xf0f0ffff,
        0xf0ff0000,0xf0ff000f,0xf0ff00f0,0xf0ff00ff,0xf0ff0f00,0xf0ff0f0f,0xf0ff0ff0,0xf0ff0fff,
        0xf0fff000,0xf0fff00f,0xf0fff0f0,0xf0fff0ff,0xf0ffff00,0xf0ffff0f,0xf0fffff0,0xf0ffffff,
        0xff000000,0xff00000f,0xff0000f0,0xff0000ff,0xff000f00,0xff000f0f,0xff000ff0,0xff000fff,
        0xff00f000,0xff00f00f,0xff00f0f0,0xff00f0ff,0xff00ff00,0xff00ff0f,0xff00fff0,0xff00ffff,
        0xff0f0000,0xff0f000f,0xff0f00f0,0xff0f00ff,0xff0f0f00,0xff0f0f0f,0xff0f0ff0,0xff0f0fff,
        0xff0ff000,0xff0ff00f,0xff0ff0f0,0xff0ff0ff,0xff0fff00,0xff0fff0f,0xff0ffff0,0xff0fffff,
        0xfff00000,0xfff0000f,0xfff000f0,0xfff000ff,0xfff00f00,0xfff00f0f,0xfff00ff0,0xfff00fff,
        0xfff0f000,0xfff0f00f,0xfff0f0f0,0xfff0f0ff,0xfff0ff00,0xfff0ff0f,0xfff0fff0,0xfff0ffff,
        0xffff0000,0xffff000f,0xffff00f0,0xffff00ff,0xffff0f00,0xffff0f0f,0xffff0ff0,0xffff0fff,
        0xfffff000,0xfffff00f,0xfffff0f0,0xfffff0ff,0xffffff00,0xffffff0f,0xfffffff0,0xffffffff
};

#define GET_CRn(CR, n, CRn) CRn.raw = (CR.raw >> ((7 - (n)) << 2))
#define GET_CRn_RAW(CR, n) (CR.raw >> ((7 - (n)) << 2))
#define SET_CRn(CR, n, CRn) CR.raw = ((CR.raw & ~(0xf << ((7 - (n)) << 2))) | (CRn.raw << ((7 - (n)) << 2)))
#define SET_CRn_RAW(CR, n, val) CR.raw = ((CR.raw & ~(0xf << ((7 - (n)) << 2))) | ((val) << ((7 - (n)) << 2)))
#define UPDATE_CR1_FROM_FPSCR(CR, FPSCR) CR.direct.CR1 = FPSCR.CR1

typedef union s_CRn {
    // integer:
    struct {
        unsigned SO: 1;  // Summary Overflow - copy of XER[SO] after instruction
        unsigned EQ: 1;  // Zero - result is 0
        unsigned GT: 1;  // Positive - result is strictly positive
        unsigned LT: 1;  // Negative - result is negative
    } INT;

    // float
    struct {
        unsigned OX: 1;   // Overflow exception
        unsigned VX: 1;   // Invalid operation summary
        unsigned FEX: 1;  // Enabled exception summary
        unsigned FX: 1;   // Exception summary
    } FLOAT;

    struct {
        unsigned FU: 1;  // Unordered (one of comparands is NaN)
        unsigned FE: 1;  // Equal
        unsigned FG: 1;  // Greater than
        unsigned FL: 1;  // Less than
    } FLOAT_COMPARE;

    struct {
        unsigned raw: 4;
    };
} s_CRn;

typedef union s_CR {
    struct {
        unsigned:28;
        unsigned SO: 1;  // Summary Overflow - copy of XER[SO] after instruction
        unsigned EQ: 1;  // Zero - result is 0
        unsigned GT: 1;  // Positive - result is strictly positive
        unsigned LT: 1;  // Negative - result is negative
    } CR0;

    struct {
        unsigned:24;
        unsigned OX: 1;   // Overflow exception
        unsigned VX: 1;   // Invalid operation summary
        unsigned FEX: 1;  // Enabled exception summary
        unsigned FX: 1;   // Exception summary
    } CR1;

    struct {
        unsigned CR7: 4;
        unsigned CR6: 4;
        unsigned CR5: 4;
        unsigned CR4: 4;
        unsigned CR3: 4;
        unsigned CR2: 4;
        unsigned CR1: 4;
        unsigned CR0: 4;
    } direct;

    unsigned raw;
} s_CR;

STATIC_ASSERT(sizeof(s_CR) == 4, "CR union was the wrong size!");

#endif //GC__CR_H
