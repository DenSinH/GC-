#ifndef GC_GEKKO_INSTRUCTION_H
#define GC_GEKKO_INSTRUCTION_H

#include "default.h"
#include <stdbool.h>
#include <assert.h>

#define GEKKO_INSTR(name) void name(s_Gekko* cpu, s_gekko_instruction instruction)
#define INLINE_GEKKO_INSTR(name) static inline void name(s_Gekko* cpu, s_gekko_instruction instruction)
#define STATIC_ASSERT _Static_assert(sizeof(instruction) == 4, "union was the wrong size!");
#define MAIN_INSTR_TABLE_SIZE 1 << 6
#define MAIN_INSTR_HASH(instruction) instruction >> 26

typedef union gekko_instruction {
    u32 raw;

    struct {
        unsigned: 26;
        unsigned opcode:6;
    } DEFAULT;

    struct {
        unsigned LK: 1;
        unsigned AA: 1;
        unsigned LI: 24;
        unsigned opcode:6;
    } branch;

    struct {
        unsigned LK: 1;
        unsigned: 15;
        unsigned BI: 5;
        unsigned BO: 5;
        unsigned opcode:6;
    } branch_conditional;

    struct {
        unsigned SIMM: 16;
        unsigned A: 5;
        unsigned D: 5;
        unsigned opcode: 6;
    } arithmetic_simm;

    struct {
        unsigned UIMM: 16;
        unsigned A: 5;
        unsigned S: 5;
        unsigned opcode: 6;
    } arithmetic_uimm;

} s_gekko_instruction;

#endif //GC_GEKKO_INSTRUCTION_H
