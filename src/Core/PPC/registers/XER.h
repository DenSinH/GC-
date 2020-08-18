#ifndef GC__XER_H
#define GC__XER_H

#include <stdbool.h>

#include "default.h"

#define XER_MASK 0xff00000f

typedef union s_XER {
    struct {
        bool SO: 1;  // Summary overflow (set when instruction sets OV)
        bool OV: 1;  // Overflow (set when overflow happened, and OE was set)
        bool CA: 1;  // Carry
        u32:22;      // MUST BE 0
        u8 byte_count: 8;  // number of bytes transferred by a lswx or stswx instruction
    };
    u32 raw;
} s_XER;

#endif //GC__XER_H
