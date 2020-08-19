#ifndef GC__MSR_H
#define GC__MSR_H

#include "default.h"

#define MSR_MASK 0xeeffa000

typedef union s_MSR {
    struct {
        unsigned LE: 1;   // Little Endian mode (always 0 for GameCube?)
        unsigned RI: 1;   // System reset or machine check exception recoverable?
        unsigned PM: 1;   // Performance monitor marked mode
        unsigned: 1;      // RESERVED
        unsigned DR: 1;   // Data address translation
        unsigned IR: 1;   // Instruction address translation
        unsigned IP: 1;   // Exception prefix (vectored to 0xfffn_nnnn if true, otherwise 0x000n_nnnn)
        unsigned: 1;      // RESERVED
        unsigned FE1: 1;  // IEEE floating-point exception mode 1
        unsigned BE: 1;   // Branch trace enabled
        unsigned SE: 1;   // Single step trace enabled
        unsigned FE0: 1;  // IEEE floating-point exception mode 0
        unsigned ME: 1;   // Machine Check (exceptions) enable
        unsigned FP: 1;   // Floating-point available
        unsigned PR: 1;   // Privilege level (user = true)
        unsigned EE: 1;   // External interrupt enable
        unsigned ILE: 1;  // Exception little endian mode
        unsigned: 1;      // RESERVED
        unsigned POW: 1;  // Power management enable
        unsigned :13;       // RESERVED
    };
    unsigned raw;
} s_MSR;


#endif //GC__MSR_H
