#ifndef GC__CONDITION_REGISTER_H
#define GC__CONDITION_REGISTER_H

#include <stdbool.h>

#include "default.h"

#define GET_GRn(CR, n, GRn) GRn.raw = (CR.raw >> (n << 2))
#define SET_GRn(CR, n, GRn) CR.raw = (CR.raw & ~(0xf << (n << 2))) | CRn.raw

typedef union s_GRn {
    struct {
        bool LT: 1;  // Negative - result is negative
        bool GT: 1;  // Positive - result is strictly positive
        bool EQ: 1;  // Zero - result is 0
        bool SO: 1;  // Summary Overflow - copy of XER[SO] after instruction
    };

    struct {
        bool FX: 1;   // Exception summary
        bool FEX: 1;  // Enabled exception summary
        bool VX: 1;   // Invalid operation summary
        bool OX: 1;   // Overflow exception
    };

    u8 raw;
} s_GRn;

typedef union s_CR {
    struct {
        bool LT: 1;  // Negative - result is negative
        bool GT: 1;  // Positive - result is strictly positive
        bool EQ: 1;  // Zero - result is 0
        bool SO: 1;  // Summary Overflow - copy of XER[SO] after instruction
    } GR0;

    struct {
        u32:4;
        bool FX: 1;   // Exception summary
        bool FEX: 1;  // Enabled exception summary
        bool VX: 1;   // Invalid operation summary
        bool OX: 1;   // Overflow exception
    } GR1;

    u32 raw;
} s_CR;

#endif //GC__CONDITION_REGISTER_H
