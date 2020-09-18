#ifndef GC__FLOAT_UTILS_H
#define GC__FLOAT_UTILS_H

#include "flags.h"
#include "default.h"

#include "../PPC/Registers/FPSCR.h"

#include <math.h>
#include <memory.h>
#include <stdbool.h>

#ifdef DO_ASSERT_PAIRED_SINGLE
#define ASSERT_PAIRED_SINGLE assert(cpu->HID2.PSE /* HID2 does not allow floating point operations */);
#else
#define ASSERT_PAIRED_SINGLE
#endif

#define DOUBLE_SIGN 0x8000000000000000ULL
#define DOUBLE_EXP  0x7ff0000000000000ULL
#define DOUBLE_FRAC 0x000fffffffffffffULL
#define DOUBLE_ZERO 0x0000000000000000ULL
#define DOUBLE_QBIT 0x0008000000000000ULL
#define DOUBLE_QNAN (DOUBLE_EXP | DOUBLE_QBIT)

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

typedef union {
    u64 u;
    i64 i;
    double d;
} bit_double;

#ifdef _MSC_VER
static_assert(sizeof(bit_float) == 4, "bit_float union was the wrong size!");
static_assert(sizeof(bit_double) == 8, "bit_double union was the wrong size!");
#else
// todo
#endif

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

s_float_result float_round_to_int(s_FPSCR* FPSCR, bit_double value);
s_float_result float_round_to_single(s_FPSCR* FPSCR, bit_double value, e_RN_modes rounding_mode);
s_float_result float_sub(s_FPSCR* FPSCR, bit_double op1, bit_double op2);
s_float_result float_add(s_FPSCR* FPSCR, bit_double op1, bit_double op2);
s_float_result float_mul(s_FPSCR* FPSCR, bit_double opa, bit_double opc);
s_float_result float_madd(s_FPSCR* FPSCR, bit_double opa, bit_double opb, bit_double opc);
s_float_result float_msub(s_FPSCR* FPSCR, bit_double opa, bit_double opb, bit_double opc);
s_float_result float_div(s_FPSCR* FPSCR, bit_double op1, bit_double op2);
s_float_result float_rsqrte(s_FPSCR* FPSCR, bit_double op);

#ifndef QUANTIZATION_SCALE_TABLES
#define QUANTIZATION_SCALE_TABLES
/* borrowed from Dolphin: */
// dequantize table
const static float dequantize_scale[] = {
        1.0 / (1ULL << 0),  1.0 / (1ULL << 1),  1.0 / (1ULL << 2),  1.0 / (1ULL << 3),
        1.0 / (1ULL << 4),  1.0 / (1ULL << 5),  1.0 / (1ULL << 6),  1.0 / (1ULL << 7),
        1.0 / (1ULL << 8),  1.0 / (1ULL << 9),  1.0 / (1ULL << 10), 1.0 / (1ULL << 11),
        1.0 / (1ULL << 12), 1.0 / (1ULL << 13), 1.0 / (1ULL << 14), 1.0 / (1ULL << 15),
        1.0 / (1ULL << 16), 1.0 / (1ULL << 17), 1.0 / (1ULL << 18), 1.0 / (1ULL << 19),
        1.0 / (1ULL << 20), 1.0 / (1ULL << 21), 1.0 / (1ULL << 22), 1.0 / (1ULL << 23),
        1.0 / (1ULL << 24), 1.0 / (1ULL << 25), 1.0 / (1ULL << 26), 1.0 / (1ULL << 27),
        1.0 / (1ULL << 28), 1.0 / (1ULL << 29), 1.0 / (1ULL << 30), 1.0 / (1ULL << 31),
        (1ULL << 32),       (1ULL << 31),       (1ULL << 30),       (1ULL << 29),
        (1ULL << 28),       (1ULL << 27),       (1ULL << 26),       (1ULL << 25),
        (1ULL << 24),       (1ULL << 23),       (1ULL << 22),       (1ULL << 21),
        (1ULL << 20),       (1ULL << 19),       (1ULL << 18),       (1ULL << 17),
        (1ULL << 16),       (1ULL << 15),       (1ULL << 14),       (1ULL << 13),
        (1ULL << 12),       (1ULL << 11),       (1ULL << 10),       (1ULL << 9),
        (1ULL << 8),        (1ULL << 7),        (1ULL << 6),        (1ULL << 5),
        (1ULL << 4),        (1ULL << 3),        (1ULL << 2),        (1ULL << 1),
};

// quantize table
const static float quantize_scale[] = {
        (1ULL << 0),        (1ULL << 1),        (1ULL << 2),        (1ULL << 3),
        (1ULL << 4),        (1ULL << 5),        (1ULL << 6),        (1ULL << 7),
        (1ULL << 8),        (1ULL << 9),        (1ULL << 10),       (1ULL << 11),
        (1ULL << 12),       (1ULL << 13),       (1ULL << 14),       (1ULL << 15),
        (1ULL << 16),       (1ULL << 17),       (1ULL << 18),       (1ULL << 19),
        (1ULL << 20),       (1ULL << 21),       (1ULL << 22),       (1ULL << 23),
        (1ULL << 24),       (1ULL << 25),       (1ULL << 26),       (1ULL << 27),
        (1ULL << 28),       (1ULL << 29),       (1ULL << 30),       (1ULL << 31),
        1.0 / (1ULL << 32), 1.0 / (1ULL << 31), 1.0 / (1ULL << 30), 1.0 / (1ULL << 29),
        1.0 / (1ULL << 28), 1.0 / (1ULL << 27), 1.0 / (1ULL << 26), 1.0 / (1ULL << 25),
        1.0 / (1ULL << 24), 1.0 / (1ULL << 23), 1.0 / (1ULL << 22), 1.0 / (1ULL << 21),
        1.0 / (1ULL << 20), 1.0 / (1ULL << 19), 1.0 / (1ULL << 18), 1.0 / (1ULL << 17),
        1.0 / (1ULL << 16), 1.0 / (1ULL << 15), 1.0 / (1ULL << 14), 1.0 / (1ULL << 13),
        1.0 / (1ULL << 12), 1.0 / (1ULL << 11), 1.0 / (1ULL << 10), 1.0 / (1ULL << 9),
        1.0 / (1ULL << 8),  1.0 / (1ULL << 7),  1.0 / (1ULL << 6),  1.0 / (1ULL << 5),
        1.0 / (1ULL << 4),  1.0 / (1ULL << 3),  1.0 / (1ULL << 2),  1.0 / (1ULL << 1),
};
#endif

#endif //GC__FLOAT_UTILS_H
