#ifndef GC__TBR_H
#define GC__TBR_H

#include "default.h"

typedef union s_TBR {
    // integer:
    struct {
        u64 buffer: 3;
        u64 time: 61;
    };
    u64 raw;
} s_TBR;

#endif //GC__TBR_H
