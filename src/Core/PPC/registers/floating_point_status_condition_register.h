#ifndef GC__FLOATING_POINT_STATUS_CONDITION_REGISTER_H
#define GC__FLOATING_POINT_STATUS_CONDITION_REGISTER_H

#include <stdbool.h>

#include "default.h"

// todo: bit 20 is 0 (mask out?)
typedef union s_FPSCR {
    struct {
        bool FX: 1;   // Exception summary
        bool FEX: 1;  // Enabled exception summary
        bool VX: 1;   // Invalid operation summary
        bool OX: 1;   // Overflow exception
        bool UX: 1;   // Underflow exception
        bool ZX: 1;   // Zero divide exception
        bool XX: 1;   // Inexact exception
        bool VXSNAN: 1;   // Invalid op exception for SNaN
        bool VXISI: 1;    // Invalid op exception for infty - infty
        bool VXIDI: 1;    // Invalid op exception for infty / infty
        bool VXZDZ: 1;    // Invalid op exception for 0 / 0
        bool VXIMZ: 1;    // Invalid op exception for infty * 0
        bool VXVC: 1;     // Invalid op exception for compare
        bool FR: 1;   // Fraction rounded
        bool FL: 1;   // Fraction inexact
        u8 FPRF: 5;   // Result flags
        bool reserved: 1; // Result flags
        bool VXSOFT: 1;   // Invalid op exception for software request
        bool VXSQRT: 1;   // Invalid op exception for square root
        bool VXCVI: 1;    // Invalid op exception for integer convert
        bool OE: 1;   // IEEE floating point overflow exception enable
        bool UE: 1;   // IEEE floating point underflow exception enable
        bool ZE: 1;   // IEEE floating point zero divide exception enable
        bool XE: 1;   // floating point inexact exception enable
        bool NI: 1;   // floating point non-IEEE mode (implementation dependent)
        u8 RN: 3;     // floating point rounding control
    };
    u32 raw;
} s_FPSCR;

enum {
    RN_ROUND_TO_NEAREST = 0b00,
    RN_ROUND_TOWARD_ZERO = 0b01,
    RN_ROUND_TOWARD_POS_INFTY = 0b10,
    RN_ROUND_TOWARD_NEG_INFTY = 0b11,
} RN_modes;

#endif //GC__FLOATING_POINT_STATUS_CONDITION_REGISTER_H
