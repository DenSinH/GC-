#include "PPC/instructions.h"
#include "../MMU.h"

GEKKO_INSTR(stw) {
    GEKKO_INSTR_HEADER

    log_cpu("stw %08x", instruction.raw);

    u32 EA = (instruction.general_SAd.A ? cpu->GPR[instruction.general_SAd.A] : 0) + (i32)((i16)(instruction.general_SAd.d));
    write32(&cpu->DMMU, EA, cpu->GPR[instruction.general_SAd.S]);
}

INLINE_GEKKO_INSTR(stwx) {
    GEKKO_INSTR_HEADER

    log_cpu("stwx %08x", instruction.raw);

    u32 EA = (instruction.general_SAd.A ? cpu->GPR[instruction.general_SAB.A] : 0) + cpu->GPR[instruction.general_SAB.B];
    write32(&cpu->DMMU, EA, cpu->GPR[instruction.general_SAB.S]);
}

GEKKO_INSTR(sth) {
    GEKKO_INSTR_HEADER

    log_cpu("sth %08x", instruction.raw);

    u32 EA = (instruction.general_SAd.A ? cpu->GPR[instruction.general_SAd.A] : 0) + (i32)((i16)(instruction.general_SAd.d));
    write16(&cpu->DMMU, EA, (u16)cpu->GPR[instruction.general_SAd.S]);
}

GEKKO_INSTR(stb) {
    GEKKO_INSTR_HEADER

    log_cpu("stb %08x", instruction.raw);

    u32 EA = (instruction.general_SAd.A ? cpu->GPR[instruction.general_SAd.A] : 0) + (i32)((i16)(instruction.general_SAd.d));
    write8(&cpu->DMMU, EA, (u8)cpu->GPR[instruction.general_SAd.S]);
}

GEKKO_INSTR(stbu) {
    GEKKO_INSTR_HEADER

    log_cpu("stb %08x", instruction.raw);

    u32 EA = cpu->GPR[instruction.general_SAd.A] + (i32)((i16)(instruction.general_SAd.d));
    write8(&cpu->DMMU, EA, (u8)cpu->GPR[instruction.general_SAd.S]);
    cpu->GPR[instruction.general_SAd.A] = EA;
}

GEKKO_INSTR(stwu) {
    GEKKO_INSTR_HEADER

    log_cpu("stwu %08x", instruction.raw);

    u32 EA = cpu->GPR[instruction.general_SAd.A] + (i32)((i16)(instruction.general_SAd.d));
    write32(&cpu->DMMU, EA, cpu->GPR[instruction.general_SAd.S]);
    cpu->GPR[instruction.general_SAd.A] = EA;
}

GEKKO_INSTR(lwz) {
    GEKKO_INSTR_HEADER
    log_cpu("lwz %08x", instruction.raw);

    u32 EA = (instruction.general_DAd.A ? cpu->GPR[instruction.general_DAd.A] : 0) + (i32)((i16)(instruction.general_DAd.d));
    cpu->GPR[instruction.general_DAd.D] = read32(&cpu->DMMU, EA);
}

GEKKO_INSTR(lhz) {
    GEKKO_INSTR_HEADER
    log_cpu("lhz %08x", instruction.raw);

    u32 EA = (instruction.general_DAd.A ? cpu->GPR[instruction.general_DAd.A] : 0) + (i32)((i16)(instruction.general_DAd.d));
    cpu->GPR[instruction.general_DAd.D] = read16(&cpu->DMMU, EA);
}

GEKKO_INSTR(lha) {
    GEKKO_INSTR_HEADER
    log_cpu("lha %08x", instruction.raw);

    u32 EA = (instruction.general_DAd.A ? cpu->GPR[instruction.general_DAd.A] : 0) + (i32)((i16)(instruction.general_DAd.d));
    cpu->GPR[instruction.general_DAd.D] = (i32)((i16)read16(&cpu->DMMU, EA));
}

INLINE_GEKKO_INSTR(lhzx) {
    GEKKO_INSTR_HEADER
    log_cpu("lhzx %08x", instruction.raw);

    u32 EA = (instruction.general_DAB.A ? cpu->GPR[instruction.general_DAd.A] : 0) + cpu->GPR[instruction.general_DAB.B];
    cpu->GPR[instruction.general_DAB.D] = read16(&cpu->DMMU, EA);
}

GEKKO_INSTR(lbz) {
    GEKKO_INSTR_HEADER
    log_cpu("lbz %08x", instruction.raw);

    u32 EA = (instruction.general_DAd.A ? cpu->GPR[instruction.general_DAd.A] : 0) + (i32)((i16)(instruction.general_DAd.d));
    cpu->GPR[instruction.general_DAd.D] = read8(&cpu->DMMU, EA);
}

GEKKO_INSTR(lbzu) {
    GEKKO_INSTR_HEADER
    log_cpu("lbzu %08x", instruction.raw);

    u32 EA = cpu->GPR[instruction.general_DAd.A] + (i32)((i16)(instruction.general_DAd.d));
    cpu->GPR[instruction.general_DAd.D] = read8(&cpu->DMMU, EA);
    cpu->GPR[instruction.general_DAd.A] = EA;
}

GEKKO_INSTR(lwzu) {
    GEKKO_INSTR_HEADER
    log_cpu("lwzu %08x", instruction.raw);

    u32 EA = cpu->GPR[instruction.general_DAd.A] + (i32)((i16)(instruction.general_DAd.d));
    cpu->GPR[instruction.general_DAd.D] = read32(&cpu->DMMU, EA);
    cpu->GPR[instruction.general_DAd.A] = EA;
    // todo: what if rD == rA? (instruction form invalid)
}

INLINE_GEKKO_INSTR(lwzx) {
    GEKKO_INSTR_HEADER
    log_cpu("lwzx %08x", instruction.raw);

    u32 EA = (instruction.general_DAB.A ? cpu->GPR[instruction.general_DAB.A] : 0) + cpu->GPR[instruction.general_DAB.B];
    cpu->GPR[instruction.general_DAB.D] = read32(&cpu->DMMU, EA);
}

GEKKO_INSTR(stmw) {
    GEKKO_INSTR_HEADER

    log_cpu("stmw %08x", instruction.raw);

    u32 EA = (instruction.general_SAd.A ? cpu->GPR[instruction.general_SAd.A] : 0) + (i32)((i16)instruction.general_SAd.d);

    for (u8 r = instruction.general_SAd.S; r < 32; r++, EA += 4) {
        write32(&cpu->DMMU, EA, cpu->GPR[r]);
    }

    // todo: misaligned write exception
}

GEKKO_INSTR(lmw) {
    GEKKO_INSTR_HEADER

    log_cpu("lmw %08x", instruction.raw);

    u32 EA = (instruction.general_SAd.A ? cpu->GPR[instruction.general_SAd.A] : 0) + (i32)((i16)instruction.general_SAd.d);

    for (u8 r = instruction.general_SAd.S; r < 32; r++, EA += 4) {
        cpu->GPR[r] = read32(&cpu->DMMU, EA);
    }

    // todo: misaligned write exception
}