#ifndef GC__FPSCR_H
#define GC__FPSCR_H

#include "default.h"

typedef enum e_VX  {
    VX_VXCVI  = 1 << 8,
    VX_VXSQRT = 1 << 9,
    VX_VXSOFT = 1 << 10,
    VX_VXVC   = 1 << 19,
    VX_VXIMZ  = 1 << 20,
    VX_VXZDZ  = 1 << 21,
    VX_VXIDI  = 1 << 22,
    VX_VXISI  = 1 << 23,
    VX_VXSNAN = 1 << 24,
    VX_VX     = 1 << 29
} e_VX;

// todo: bit 20 is 0 (mask out?)
typedef union s_FPSCR {
    struct {
        unsigned RN: 2;       // floating point rounding control
        unsigned NI: 1;       // floating point non-IEEE mode (implementation dependent)
        unsigned XE: 1;       // floating point inexact exception enable
        unsigned ZE: 1;       // IEEE floating point zero divide exception enable
        unsigned UE: 1;       // IEEE floating point underflow exception enable
        unsigned OE: 1;       // IEEE floating point overflow exception enable
        unsigned VE: 1;       // IEEE floating point overflow exception enable
        unsigned VXCVI: 1;    // Invalid op exception for integer convert
        unsigned VXSQRT: 1;   // Invalid op exception for square root
        unsigned VXSOFT: 1;   // Invalid op exception for software request
        unsigned reserved: 1; // Result flags
        unsigned FPRF: 5;     // Result flags
        unsigned FI: 1;       // Fraction inexact
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
        unsigned FPCC: 4;
        unsigned: 3;
        unsigned VXhi: 6;
        unsigned:3;
        unsigned CR1: 4;
    };

    unsigned raw;
} s_FPSCR;

typedef enum e_RN_modes {
    RN_ROUND_TO_NEAREST = 0b00,
    RN_ROUND_TOWARD_ZERO = 0b01,
    RN_ROUND_TOWARD_POS_INFTY = 0b10,
    RN_ROUND_TOWARD_NEG_INFTY = 0b11,
} e_RN_modes;

static inline void SET_VX_EXCEPTIONS(s_FPSCR* FPSCR, e_VX exceptions) {
    // if the exception was not set already, we set FX (sticky)
    if ((FPSCR->raw & exceptions) != 0) {
        FPSCR->FX = 1;
    }
    // set exception and VX (summary)
    FPSCR->raw |= exceptions | VX_VX;
}

#endif //GC__FPSCR_H
