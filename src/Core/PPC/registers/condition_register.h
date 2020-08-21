#ifndef GC__CONDITION_REGISTER_H
#define GC__CONDITION_REGISTER_H

#include <stdbool.h>

#include "default.h"

#define GET_CRn(CR, n, CRn) CRn.raw = (CR.raw >> ((7 - n) << 2))
#define SET_CRn(CR, n, CRn) CR.raw = (CR.raw & ~(0xf << ((7 - n) << 2))) | (CRn.raw << ((7 - n) << 2))
#define SET_CRn_RAW(CR, n, val) CR.raw = (CR.raw & ~(0xf << ((7 - n) << 2))) | (val << ((7 - n) << 2))
#define UPDATE_CR_FROM_FPSCR(CR, FPSCR) CR.direct.CR1 = FPSCR.block.CR1

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

    unsigned raw;
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

#endif //GC__CONDITION_REGISTER_H
