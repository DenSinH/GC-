#ifndef GC__XER_H
#define GC__XER_H

#include "default.h"

#define XER_MASK 0xff00000f
#define UPDATE_XER_OV(xer, ov) xer.SO |= (xer.SO = ov)

typedef union s_XER {
    struct {
        unsigned byte_count: 8;  // number of bytes transferred by a lswx or stswx instruction
        unsigned:21;      // MUST BE 0
        unsigned CA: 1;  // Carry
        unsigned OV: 1;  // Overflow (set when overflow happened, and OE was set)
        unsigned SO: 1;  // Summary overflow (set when instruction sets OV)
    };
    unsigned raw;
} s_XER;

#endif //GC__XER_H
