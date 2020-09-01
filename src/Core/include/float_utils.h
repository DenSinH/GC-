#ifndef GC__FLOAT_UTILS_H
#define GC__FLOAT_UTILS_H

#include "default.h"
#include "PPC/registers/FPSCR.h"

#include <math.h>
#include <memory.h>
#include <stdbool.h>

#define DOUBLE_SIGN 0x8000000000000000ULL
#define DOUBLE_EXP  0x7ff0000000000000ULL
#define DOUBLE_FRAC 0x000fffffffffffffULL
#define DOUBLE_ZERO 0x0000000000000000ULL
#define DOUBLE_QBIT 0x0008000000000000ULL

#define DOUBLE_MAKE_QUIET(value) ((value) | DOUBLE_QBIT)

#define SINGLE_SIGN  0x80000000
#define SINGLE_EXP   0x7f800000
#define SINGLE_FRAC  0x007fffff
#define SINGLE_ZERO  0x00000000
#define SINGLE_QBIT  0x00400000

#define SINGLE_MAKE_QUIET(value) ((value) | SINGLE_QBIT)

#define PPC_DOUBLE_P_INFTY 0x7ff0000000000000
#define PPC_DOUBLE_N_INFTY 0xfff0000000000000
typedef union {
    u32 u;
    i32 i;
    float f;
} bit_float;

static_assert(sizeof(bit_float) == 4, "bit_float union was the wrong size!");

typedef union {
    u64 u;
    i64 i;
    double d;
} bit_double;

static_assert(sizeof(bit_double) == 8, "bit_double union was the wrong size!");

typedef struct s_float_result {
    bit_double value;
    e_VX exception;
} s_float_result;

static inline bool IS_QNAN(bit_double value) {
    return ((value.u & DOUBLE_EXP) == DOUBLE_EXP) && ((value.u & DOUBLE_QBIT) == DOUBLE_QBIT);
}

static inline bool IS_SNAN(bit_double value) {
    return ((value.u & DOUBLE_EXP) == DOUBLE_EXP) && ((value.u & DOUBLE_FRAC) != DOUBLE_ZERO) &&
           ((value.u & DOUBLE_QBIT) == DOUBLE_ZERO);
}

static inline bit_double CONVERT_TO_DOUBLE(bit_float single) {
    // todo: is this correct?
    bit_double result;
    if (!isnan(single.f)) {
        result.d = (double)single.f;
    }
    else {
        result.u = (single.u & SINGLE_QBIT) ? DOUBLE_EXP | DOUBLE_QBIT : DOUBLE_EXP | 1;
    }
    return result;
}

static inline bit_double TO_SINGLE(bit_double value) {
    value.d = (float)value.d;
    return value;
}

s_float_result float_round(s_FPSCR* FPSCR, bit_double value, e_RN_modes rounding_mode);
s_float_result float_div(s_FPSCR* FPSCR, bit_double op1, bit_double op2);

#endif //GC__FLOAT_UTILS_H
