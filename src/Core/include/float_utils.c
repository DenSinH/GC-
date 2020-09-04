#include "float_utils.h"
#include "log.h"

#include <immintrin.h>
#include <limits.h>

s_float_result float_round_to_int(s_FPSCR* FPSCR, bit_double value) {
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
        result.value.u = (u32)value.d;
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

s_float_result float_round_to_single(s_FPSCR* FPSCR, bit_double value, e_RN_modes rounding_mode) {
    s_float_result result;

    if (IS_SNAN(value)) {
        result.exception = VX_VXSNAN;
        SET_VX_EXCEPTIONS(FPSCR, VX_VXSNAN);
    }

    result.value.d = (float)value.d;
    switch (rounding_mode) {
        case RN_ROUND_TO_NEAREST:
            // todo: ?
        case RN_ROUND_TOWARD_ZERO:
            break;
        case RN_ROUND_TOWARD_NEG_INFTY:
            if (result.value.d > value.d) {
                result.value.d = nextafter(result.value.d, -INFINITY);
            }
        case RN_ROUND_TOWARD_POS_INFTY:
            if (result.value.d < value.d) {
                result.value.d = nextafter(result.value.d, INFINITY);
            }
        default:
            log_fatal("Unimplemented rounding mode: %d", rounding_mode);
    }

    if (value.d == result.value.d) {
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
    s_float_result result = { .value.d = op1.d / op2.d };

    if (isnan(result.value.d)) {
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

s_float_result float_sub(s_FPSCR* FPSCR, bit_double op1, bit_double op2) {
    s_float_result result = { .value.d = op1.d - op2.d };

    if (isnan(result.value.d)) {
        // SNAN exception
        if (IS_SNAN(op1) || IS_SNAN(op2)) {
            result.exception |= VX_VXSNAN;
            SET_VX_EXCEPTIONS(FPSCR, VX_VXSNAN);
        }

        // clear FR,FI on invalid operation exceptions
        FPSCR->FR = 0;
        FPSCR->FI = 0;

        if (isnan(op1.d)) {
            result.value.u = DOUBLE_MAKE_QUIET(op1.u);
        }
        else if (isnan(op2.d)) {
            result.value.u = DOUBLE_MAKE_QUIET(op2.u);
        }
        // ISI exception
        else if (isinf(op1.d) && isinf(op2.d)) {
            result.exception |= VX_VXISI;
            SET_VX_EXCEPTIONS(FPSCR, VX_VXISI);
        }
    }

    return result;
}


s_float_result float_add(s_FPSCR* FPSCR, bit_double op1, bit_double op2) {
    // identical to float_sub, except the operation is different
    s_float_result result = { .value.d = op1.d + op2.d };

    if (isnan(result.value.d)) {
        // SNAN exception
        if (IS_SNAN(op1) || IS_SNAN(op2)) {
            result.exception |= VX_VXSNAN;
            SET_VX_EXCEPTIONS(FPSCR, VX_VXSNAN);
        }

        // clear FR,FI on invalid operation exceptions
        FPSCR->FR = 0;
        FPSCR->FI = 0;

        if (isnan(op1.d)) {
            result.value.u = DOUBLE_MAKE_QUIET(op1.u);
        }
        else if (isnan(op2.d)) {
            result.value.u = DOUBLE_MAKE_QUIET(op2.u);
        }
            // ISI exception
        else if (isinf(op1.d) && isinf(op2.d)) {
            result.exception |= VX_VXISI;
            SET_VX_EXCEPTIONS(FPSCR, VX_VXISI);
        }
    }

    return result;
}


s_float_result float_mul(s_FPSCR* FPSCR, bit_double opa, bit_double opc) {
    s_float_result result = { .value.d = opa.d * opc.d };

    if (isnan(result.value.d)) {
        // SNAN exception
        if (IS_SNAN(opa) || IS_SNAN(opc)) {
            result.exception |= VX_VXSNAN;
            SET_VX_EXCEPTIONS(FPSCR, VX_VXSNAN);
        }

        // clear FR,FI on invalid operation exceptions
        FPSCR->FR = 0;
        FPSCR->FI = 0;

        if (isnan(opa.d)) {
            result.value.u = DOUBLE_MAKE_QUIET(opa.u);
            return result;
        }
        else if (isnan(opc.d)) {
            result.value.u = DOUBLE_MAKE_QUIET(opc.u);
            return result;
        }
        // IMZ exception, only other possibility
        else {
            result.exception |= VX_VXIMZ;
            SET_VX_EXCEPTIONS(FPSCR, VX_VXIMZ);
        }
    }
    return result;
}


s_float_result float_madd(s_FPSCR* FPSCR, bit_double opa, bit_double opb, bit_double opc) {
    s_float_result result = { .value.d = opa.d * opc.d };

    if (isnan(result.value.d)) {
        // SNAN exception
        if (IS_SNAN(opa) || IS_SNAN(opb) || IS_SNAN(opc)) {
            result.exception |= VX_VXSNAN;
            SET_VX_EXCEPTIONS(FPSCR, VX_VXSNAN);
        }

        // clear FR,FI on invalid operation exceptions
        FPSCR->FR = 0;
        FPSCR->FI = 0;

        if (isnan(opa.d)) {
            result.value.u = DOUBLE_MAKE_QUIET(opa.u);
            return result;
        }
        else if (isnan(opb.d)) {
            result.value.u = DOUBLE_MAKE_QUIET(opb.u);
            return result;
        }
        else if (isnan(opc.d)) {
            result.value.u = DOUBLE_MAKE_QUIET(opc.u);
            return result;
        }
        // IMZ exception, only other possibility
        else {
            result.exception |= VX_VXIMZ;
            SET_VX_EXCEPTIONS(FPSCR, VX_VXIMZ);
        }
    }

    result.value.d += opb.d;

    if (isnan(result.value.d)) {
        // result was not NaN before
        if (isnan(opb.d)) {
            result.value.u = DOUBLE_MAKE_QUIET(opb.u);
            return result;
        }

        // clear FR,FI on invalid operation exceptions
        FPSCR->FR = 0;
        FPSCR->FI = 0;

        // only other possibility is ISI
        result.exception |= VX_VXISI;
        SET_VX_EXCEPTIONS(FPSCR, VX_VXISI);
    }

    return result;
}

s_float_result float_msub(s_FPSCR* FPSCR, bit_double opa, bit_double opb, bit_double opc) {
    s_float_result result = { .value.d = opa.d * opc.d };

    if (isnan(result.value.d)) {
        // SNAN exception
        if (IS_SNAN(opa) || IS_SNAN(opb) || IS_SNAN(opc)) {
            result.exception |= VX_VXSNAN;
            SET_VX_EXCEPTIONS(FPSCR, VX_VXSNAN);
        }

        // clear FR,FI on invalid operation exceptions
        FPSCR->FR = 0;
        FPSCR->FI = 0;

        if (isnan(opa.d)) {
            result.value.u = DOUBLE_MAKE_QUIET(opa.u);
            return result;
        }
        else if (isnan(opb.d)) {
            result.value.u = DOUBLE_MAKE_QUIET(opb.u);
            return result;
        }
        else if (isnan(opc.d)) {
            result.value.u = DOUBLE_MAKE_QUIET(opc.u);
            return result;
        }
            // IMZ exception, only other possibility
        else {
            result.exception |= VX_VXIMZ;
            SET_VX_EXCEPTIONS(FPSCR, VX_VXIMZ);
        }
    }

    result.value.d -= opb.d;

    if (isnan(result.value.d)) {
        // result was not NaN before
        if (isnan(opb.d)) {
            result.value.u = DOUBLE_MAKE_QUIET(opb.u);
            return result;
        }

        // clear FR,FI on invalid operation exceptions
        FPSCR->FR = 0;
        FPSCR->FI = 0;

        // only other possibility is ISI
        result.exception |= VX_VXISI;
        SET_VX_EXCEPTIONS(FPSCR, VX_VXISI);
    }

    return result;
}

inline float rsqrt(const float f)
{
    __m128 temp = _mm_set_ss(f);
    temp = _mm_rsqrt_ss(temp);
    return _mm_cvtss_f32(temp);
}

s_float_result float_rsqrte(s_FPSCR* FPSCR, bit_double op) {
    s_float_result result = { .value.d = op.d };

    if (op.d < 0.0) {
        SET_VX_EXCEPTIONS(FPSCR, VX_VXSQRT);
        result.exception |= VX_VXSQRT;

        // clear FR,FI on invalid operation exceptions
        FPSCR->FR = 0;
        FPSCR->FI = 0;

        if (!FPSCR->VE) {
            result.value.d = DOUBLE_QNAN;
        }
    }
    else if (op.d == 0.0) {
        if (!FPSCR->ZX) {
            FPSCR->ZX = 1;
            FPSCR->FX = 1;
        }

        // clear FR,FI on invalid operation exceptions
        FPSCR->FR = 0;
        FPSCR->FI = 0;

        if (!FPSCR->VE) {
            result.value.d = INFINITY;
        }
    }
    else if (IS_SNAN(op)) {
        SET_VX_EXCEPTIONS(FPSCR, VX_VXSNAN);
        result.exception |= VX_VXSNAN;

        // clear FR,FI on invalid operation exceptions
        FPSCR->FR = 0;
        FPSCR->FI = 0;

        if (!FPSCR->VE) {
            result.value.u = DOUBLE_MAKE_QUIET(op.u);
        }
    }
    // in case of QNaN: result is already the right value
    else if (!isnan(op.d)) {
        result.value.d = rsqrt((float)op.d);
    }

    return result;
}