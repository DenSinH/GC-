#ifndef GC__TBR_H
#define GC__TBR_H

#include "default.h"

/*
 * Idea: I tick cycles too slow (1 per instruction + 1 per memory access), so I have a 3 bit buffer
 * (time / 8 vs time / 12) causing TBR to tick faster to correct for the timing
 * */
typedef union s_TBR {
    struct {
        u64 buffer: 3;
        u64 time: 61;
    };
    u64 raw;
} s_TBR;

#endif //GC__TBR_H
