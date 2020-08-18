#ifndef GC__MSR_H
#define GC__MSR_H

#include <stdbool.h>
#include "default.h"

enum MSR_flags {
    MSR_POW = 1 << 13,
    MSR_ILE = 1 << 15,
    MSR_EE  = 1 << 16,
    MSR_PR  = 1 << 17,
    MSR_FP  = 1 << 18,
    MSR_ME  = 1 << 19,
    MSR_FE0 = 1 << 20,
    MSR_SE  = 1 << 21,
    MSR_BE  = 1 << 22,
    MSR_FE1 = 1 << 23,
    MSR_IP  = 1 << 25,
    MSR_IR  = 1 << 26,
    MSR_DR  = 1 << 27,
    MSR_PM  = 1 << 29,
    MSR_RI  = 1 << 30,
    MSR_LE  = 1 << 31,
};
#define MSR_MASK 0xeeffa000

typedef union s_MSR {
    struct {
        u16:13;       // RESERVED
        bool POW: 1;  // Power management enable
        bool: 1;      // RESERVED
        bool ILE: 1;  // Exception little endian mode
        bool EE: 1;   // External interrupt enable
        bool PR: 1;   // Privilege level (user = true)
        bool FP: 1;   // Floating-point available
        bool ME: 1;   // Machine Check (exceptions) enable
        bool FE0: 1;  // IEEE floating-point exception mode 0
        bool SE: 1;   // Single step trace enabled
        bool BE: 1;   // Branch trace enabled
        bool FE1: 1;  // IEEE floating-point exception mode 1
        bool: 1;      // RESERVED
        bool IP: 1;   // Exception prefix (vectored to 0xfffn_nnnn if true, otherwise 0x000n_nnnn)
        bool IR: 1;   // Instruction address translation
        bool DR: 1;   // Data address translation
        bool: 1;      // RESERVED
        bool PM: 1;   // Performance monitor marked mode
        bool RI: 1;   // System reset or machine check exception recoverable?
        bool LE: 1;   // Little Endian mode (always 0 for GameCube?)
    };
    u32 raw;
} s_MSR;


#endif //GC__MSR_H
