#ifndef GC_GEKKO_INSTRUCTION_H
#define GC_GEKKO_INSTRUCTION_H

#include "default.h"
#include <stdbool.h>

#define GEKKO_INSTR(name) void name(s_Gekko* cpu, s_gekko_instruction instruction)
#define INSTR_TABLE_SIZE 0x20
#define INSTR_HASH(instruction) instruction & 0x1f

typedef union gekko_instruction {
    u32 raw;

    struct {
        u8 opcode:5;
        unsigned: 27;
    } DEFAULT;

    struct {
        u8 opcode:5;
        u32 LI: 27;
        bool AA: 1;
        bool LK: 1;
    } branch;

} s_gekko_instruction;

#endif //GC_GEKKO_INSTRUCTION_H
