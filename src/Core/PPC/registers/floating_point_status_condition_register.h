#ifndef GC__FLOATING_POINT_STATUS_CONDITION_REGISTER_H
#define GC__FLOATING_POINT_STATUS_CONDITION_REGISTER_H

#include "default.h"

#define UPDATE_INVALID_OP_EXCEPTION(FPSCR) FPSCR.VX = ((FPSCR.block.VXhi != 0) || (FPSCR.block.VXlo != 0))

// todo: bit 20 is 0 (mask out?)
typedef union s_FPSCR {
    struct {
        unsigned RN: 3;       // floating point rounding control
        unsigned NI: 1;       // floating point non-IEEE mode (implementation dependent)
        unsigned XE: 1;       // floating point inexact exception enable
        unsigned ZE: 1;       // IEEE floating point zero divide exception enable
        unsigned UE: 1;       // IEEE floating point underflow exception enable
        unsigned OE: 1;       // IEEE floating point overflow exception enable
        unsigned VXCVI: 1;    // Invalid op exception for integer convert
        unsigned VXSQRT: 1;   // Invalid op exception for square root
        unsigned VXSOFT: 1;   // Invalid op exception for software request
        unsigned reserved: 1; // Result flags
        unsigned FPRF: 5;     // Result flags
        unsigned FL: 1;       // Fraction inexact
        unsigned FR: 1;       // Fraction rounded
        unsigned VXVC: 1;     // Invalid op exception for compare         STICKY
        unsigned VXIMZ: 1;    // Invalid op exception for infty * 0       STICKY
        unsigned VXZDZ: 1;    // Invalid op exception for 0 / 0           STICKY
        unsigned VXIDI: 1;    // Invalid op exception for infty / infty   STICKY
        unsigned VXISI: 1;    // Invalid op exception for infty - infty   STICKY
        unsigned VXSNAN: 1;   // Invalid op exception for SNaN
        unsigned XX: 1;       // Inexact exception                        STICKY
        unsigned ZX: 1;       // Zero divide exception                    STICKY
        unsigned UX: 1;       // Underflow exception                      STICKY
        unsigned OX: 1;       // Overflow exception                       STICKY
        unsigned VX: 1;       // Invalid operation summary
        unsigned FEX: 1;      // Enabled exception summary
        unsigned FX: 1;       // Exception summary                        STICKY
    };

    struct {
        unsigned: 3;
        unsigned ExceptionEnable: 5;
        unsigned VXlo: 3;
        unsigned: 1;
        unsigned FPRF: 5;
        unsigned: 2;
        unsigned VXhi: 6;
        unsigned:3;
        unsigned CR1: 4;
    } block;

    unsigned raw;
} s_FPSCR;

enum {
    RN_ROUND_TO_NEAREST = 0b00,
    RN_ROUND_TOWARD_ZERO = 0b01,
    RN_ROUND_TOWARD_POS_INFTY = 0b10,
    RN_ROUND_TOWARD_NEG_INFTY = 0b11,
} RN_modes;

#endif //GC__FLOATING_POINT_STATUS_CONDITION_REGISTER_H
