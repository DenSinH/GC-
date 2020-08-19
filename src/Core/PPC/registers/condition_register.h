#ifndef GC__CONDITION_REGISTER_H
#define GC__CONDITION_REGISTER_H

#include <stdbool.h>

#include "default.h"

#define GET_GRn(CR, n, GRn) GRn.raw = (CR.raw >> ((7 - n) << 2))
#define SET_GRn(CR, n, GRn) CR.raw = (CR.raw & ~(0xf << ((7 - n) << 2))) | CRn.raw

typedef union s_GRn {
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
} s_GRn;

typedef union s_CR {
    struct {
        unsigned:28;
        unsigned SO: 1;  // Summary Overflow - copy of XER[SO] after instruction
        unsigned EQ: 1;  // Zero - result is 0
        unsigned GT: 1;  // Positive - result is strictly positive
        unsigned LT: 1;  // Negative - result is negative
    } GR0;

    struct {
        unsigned:24;
        unsigned OX: 1;   // Overflow exception
        unsigned VX: 1;   // Invalid operation summary
        unsigned FEX: 1;  // Enabled exception summary
        unsigned FX: 1;   // Exception summary
    } GR1;

    unsigned raw;
} s_CR;

#endif //GC__CONDITION_REGISTER_H
