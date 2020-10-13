#ifndef GC__DSP_INSTRUCTION_H
#define GC__DSP_INSTRUCTION_H

#include "default.h"

#define DSP_INSTR(_name) void _name(struct s_DSP* DSP, s_dsp_instruction instruction)

typedef union s_dsp_instruction {
    u16 raw;
} s_dsp_instruction;

#endif //GC__DSP_INSTRUCTION_H
