#ifndef GC__GQR_H
#define GC__GQR_H

typedef enum {
    GQR_TYPE_single_precision = 0,
    GQR_TYPE_unsigned_8bit_int = 4,
    GQR_TYPE_unsigned_16_bit_int = 5,
    GQR_TYPE_signed_8bit_int = 6,
    GQR_TYPE_signed_16bit_int = 7
} GQR_TYPE;

typedef union s_GQR {
    // integer:
    struct {
        unsigned ST_TYPE: 3;  // Type of operand from conversion by store instruction
        unsigned: 5;
        unsigned ST_SCALE: 6; // Scale value used by store instruction
        unsigned: 2;
        unsigned LD_TYPE: 3;  // Type of operand to be converted by load
        unsigned: 5;
        unsigned LD_SCALE: 6; // Scale value used by load instruction
        unsigned: 2;
    };
    unsigned raw;
} s_GQR;

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

#endif //GC__GQR_H
