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

STATIC_ASSERT(sizeof(s_GQR) == 4, "GQR union was the wrong size!");

#endif //GC__GQR_H
