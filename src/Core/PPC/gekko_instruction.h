#ifndef GC_GEKKO_INSTRUCTION_H
#define GC_GEKKO_INSTRUCTION_H

#include "default.h"
#include <stdbool.h>
#include <assert.h>

#define GEKKO_INSTR(name) void name(s_Gekko* cpu, s_gekko_instruction instruction)
#define INLINE_GEKKO_INSTR(name) static inline void name(s_Gekko* cpu, s_gekko_instruction instruction)
#define GEKKO_INSTR_HEADER
#define ASSERT_SUPERVISOR assert(!cpu->MSR.PR /* supervisor instruction attempted in user mode */);
#define MAIN_INSTR_TABLE_SIZE 1 << 6
#define MAIN_INSTR_HASH(instruction) instruction >> 26

typedef union gekko_instruction {
    u32 raw;

    struct {
        unsigned: 26;
        unsigned opcode:6;
    } DEFAULT;

    struct {
        unsigned Rc: 1;
        unsigned: 9;
        unsigned OE: 1;
        unsigned B: 5;
        unsigned A: 5;
        unsigned D: 5;
        unsigned opcode:6;
    } general_DAB;

    struct {
        unsigned Rc: 1;
        unsigned: 9;
        unsigned OE: 1;
        unsigned B: 5;
        unsigned A: 5;
        unsigned S: 5;
        unsigned opcode:6;
    } general_SAB;

    struct {
        unsigned d: 16;
        unsigned A: 5;
        unsigned S: 5;
        unsigned opcode: 6;
    } general_SAd;

    struct {
        unsigned d: 16;
        unsigned A: 5;
        unsigned D: 5;
        unsigned opcode: 6;
    } general_DAd;

    struct {
        unsigned LK: 1;
        unsigned AA: 1;
        unsigned LI: 24;
        unsigned opcode:6;
    } branch;

    struct {
        unsigned LK: 1;
        unsigned AA: 1;
        unsigned BD: 14;
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

    struct {
        unsigned UIMM: 16;
        unsigned A: 5;
        unsigned L: 1;
        unsigned: 1;
        unsigned crfD: 3;
        unsigned opcode: 6;
    } cmp_uimm;

    struct {
        unsigned SIMM: 16;
        unsigned A: 5;
        unsigned L: 1;
        unsigned: 1;
        unsigned crfD: 3;
        unsigned opcode: 6;
    } cmp_simm;

    struct {
        unsigned: 1;
        unsigned: 10;
        unsigned B: 5;
        unsigned A: 5;
        unsigned L: 1;
        unsigned: 1;
        unsigned crfD: 3;
        unsigned opcode: 6;
    } cmp;

    struct {
        unsigned Rc: 1;
        unsigned ME: 5;
        unsigned MB: 5;
        unsigned SH: 5;
        unsigned A: 5;
        unsigned S: 5;
        unsigned opcode: 6;
    } rotate;

    struct {
        unsigned d: 12;
        unsigned I: 3;
        unsigned W: 1;
        unsigned A: 5;
        unsigned D: 5;
        unsigned opcode: 6;
    } paired_single_load;

    // SPR field is split
    struct {
        unsigned Rc: 1;
        unsigned: 10;
        unsigned SPR_hi: 5;
        unsigned SPR_lo: 5;
        unsigned D: 5;
        unsigned opcode:6;
    } SPR_D;

    struct {
        unsigned Rc: 1;
        unsigned: 10;
        unsigned SPR_hi: 5;
        unsigned SPR_lo: 5;
        unsigned S: 5;
        unsigned opcode:6;
    } SPR_S;

    struct {
        unsigned Rc: 1;
        unsigned: 10;
        unsigned B: 5;
        unsigned:1;
        unsigned FM: 8;
        unsigned: 1;
        unsigned opcode:6;
    } mtfsf;

    struct {
        unsigned: 1;
        unsigned: 10;
        unsigned B: 5;
        unsigned SR: 4;
        unsigned: 1;
        unsigned DS: 5;
        unsigned opcode:6;
    } mxsr;

} s_gekko_instruction;

static_assert(sizeof(s_gekko_instruction) == 4, "union was the wrong size!");

#endif //GC_GEKKO_INSTRUCTION_H
