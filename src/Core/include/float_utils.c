#include "float_utils.h"
#include "log.h"
#include <limits.h>

s_float_result float_round(s_FPSCR* FPSCR, bit_double value, e_RN_modes rounding_mode) {
    s_float_result result;

    if (isnan(value.d)) {
        if (IS_SNAN(value)) {
            SET_VX_EXCEPTIONS(FPSCR, VX_VXSNAN | VX_VXCVI);
            result.exception = VX_VXSNAN | VX_VXCVI;
        }
        else {
            SET_VX_EXCEPTIONS(FPSCR, VX_VXCVI);
            result.exception = VX_VXCVI;
        }
    }
    else if (value.d > INT32_MAX) {
        SET_VX_EXCEPTIONS(FPSCR, VX_VXCVI);
        result.exception = VX_VXCVI;
        result.value.u = 0x7fffffff;
    }
    else if (value.d < INT32_MIN) {
        SET_VX_EXCEPTIONS(FPSCR, VX_VXCVI);
        result.exception = VX_VXCVI;
        result.value.u = 0x80000000;
    }
    else {
        switch (rounding_mode) {
            case RN_ROUND_TOWARD_ZERO:
                result.value.u = (u32)value.d;
                break;
            default:
                log_fatal("Unimplemented rounding mode: %d", rounding_mode);
        }
    }

    if (value.d == result.value.u) {
        // exact rounding
        FPSCR->FR = 0;
        FPSCR->FI = 0;
    }
    else {
        // inexact rounding
        FPSCR->FI = 1;
        FPSCR->FR = result.value.u > value.d;  // fraction incremented
    }

    return result;
}


s_float_result float_div(s_FPSCR* FPSCR, bit_double op1, bit_double op2) {
    s_float_result result;
    result.value.d = op1.d / op2.d;

    if (isnan(result.value.d) || isinf(result.value.d)) {
        // SNAN exception
        if (IS_SNAN(op1) || IS_SNAN(op2)) {
            result.exception |= VX_VXSNAN;
            SET_VX_EXCEPTIONS(FPSCR, VX_VXSNAN);
        }

        // clear FR,FI on invalid operation exceptions
        FPSCR->FR = 0;
        FPSCR->FI = 0;

        // propagate NANs
        if (isnan(op1.d)) {
            result.value.u = DOUBLE_MAKE_QUIET(op1.u);
        }
        else if (isnan(op2.d)) {
            result.value.u = DOUBLE_MAKE_QUIET(op2.u);
        }
        // ZDZ/ZX exceptions
        else if (op2.d == 0.0) {
            if (op1.d == 0.0) {
                result.exception |= VX_VXZDZ;
                SET_VX_EXCEPTIONS(FPSCR, VX_VXZDZ);
            }
            else {
                if (!FPSCR->ZX) {
                    FPSCR->ZX = 1;
                    FPSCR->FX = 1;
                }
            }
        }
        // IDI exception
        else if (isinf(op1.d) && isinf(op2.d)) {
            result.exception |= VX_VXIDI;
            SET_VX_EXCEPTIONS(FPSCR, VX_VXIDI);
        }
    }
    return result;
}
