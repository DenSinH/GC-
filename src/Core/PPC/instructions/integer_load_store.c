#include "instructions.h"
#include "../MMU.h"

#include "byteswap.h"

GEKKO_INSTR(stw) {
    GEKKO_INSTR_HEADER

    log_cpu_verbose("stw %08x", instruction.raw);

    u32 EA = (instruction.general_SAd.A ? cpu->GPR[instruction.general_SAd.A] : 0) + (i32)((i16)(instruction.general_SAd.d));
    write32(&cpu->DMMU, EA, cpu->GPR[instruction.general_SAd.S]);
}

INLINE_GEKKO_INSTR(stwx) {
    GEKKO_INSTR_HEADER

    log_cpu_verbose("stwx %08x", instruction.raw);

    u32 EA = (instruction.general_SAd.A ? cpu->GPR[instruction.general_SAB.A] : 0) + cpu->GPR[instruction.general_SAB.B];
    write32(&cpu->DMMU, EA, cpu->GPR[instruction.general_SAB.S]);
}

INLINE_GEKKO_INSTR(stwux) {
    GEKKO_INSTR_HEADER

    log_cpu_verbose("stwux %08x", instruction.raw);
    u32 EA = (instruction.general_SAd.A ? cpu->GPR[instruction.general_SAB.A] : 0) + cpu->GPR[instruction.general_SAB.B];
    write32(&cpu->DMMU, EA, cpu->GPR[instruction.general_SAB.S]);
    cpu->GPR[instruction.general_SAB.A] = EA;
}

GEKKO_INSTR(sth) {
    GEKKO_INSTR_HEADER

    log_cpu_verbose("sth %08x", instruction.raw);

    u32 EA = (instruction.general_SAd.A ? cpu->GPR[instruction.general_SAd.A] : 0) + (i32)((i16)(instruction.general_SAd.d));
    write16(&cpu->DMMU, EA, (u16)cpu->GPR[instruction.general_SAd.S]);
}

INLINE_GEKKO_INSTR(sthx) {
    GEKKO_INSTR_HEADER

    log_cpu_verbose("sthx %08x", instruction.raw);

    u32 EA = (instruction.general_SAd.A ? cpu->GPR[instruction.general_SAB.A] : 0) + cpu->GPR[instruction.general_SAB.B];
    write16(&cpu->DMMU, EA, (u16)cpu->GPR[instruction.general_SAB.S]);
}

GEKKO_INSTR(stb) {
    GEKKO_INSTR_HEADER

    log_cpu_verbose("stb %08x", instruction.raw);

    u32 EA = (instruction.general_SAd.A ? cpu->GPR[instruction.general_SAd.A] : 0) + (i32)((i16)(instruction.general_SAd.d));
    write8(&cpu->DMMU, EA, (u8)cpu->GPR[instruction.general_SAd.S]);
}

INLINE_GEKKO_INSTR(stbx) {
    GEKKO_INSTR_HEADER

    log_cpu_verbose("stbx %08x", instruction.raw);

    u32 EA = (instruction.general_SAd.A ? cpu->GPR[instruction.general_SAB.A] : 0) + cpu->GPR[instruction.general_SAB.B];
    write8(&cpu->DMMU, EA, (u8)cpu->GPR[instruction.general_SAB.S]);
}

GEKKO_INSTR(stbu) {
    GEKKO_INSTR_HEADER

    log_cpu_verbose("stb %08x", instruction.raw);

    u32 EA = cpu->GPR[instruction.general_SAd.A] + (i32)((i16)(instruction.general_SAd.d));
    write8(&cpu->DMMU, EA, (u8)cpu->GPR[instruction.general_SAd.S]);
    cpu->GPR[instruction.general_SAd.A] = EA;
}

GEKKO_INSTR(stwu) {
    GEKKO_INSTR_HEADER

    log_cpu_verbose("stwu %08x", instruction.raw);

    u32 EA = cpu->GPR[instruction.general_SAd.A] + (i32)((i16)(instruction.general_SAd.d));
    write32(&cpu->DMMU, EA, cpu->GPR[instruction.general_SAd.S]);
    cpu->GPR[instruction.general_SAd.A] = EA;
}

GEKKO_INSTR(lwz) {
    GEKKO_INSTR_HEADER
    log_cpu_verbose("lwz %08x", instruction.raw);

    u32 EA = (instruction.general_DAd.A ? cpu->GPR[instruction.general_DAd.A] : 0) + (i32)((i16)(instruction.general_DAd.d));
    cpu->GPR[instruction.general_DAd.D] = read32(&cpu->DMMU, EA);
}

GEKKO_INSTR(lhz) {
    GEKKO_INSTR_HEADER
    log_cpu_verbose("lhz %08x", instruction.raw);

    u32 EA = (instruction.general_DAd.A ? cpu->GPR[instruction.general_DAd.A] : 0) + (i32)((i16)(instruction.general_DAd.d));
    cpu->GPR[instruction.general_DAd.D] = read16(&cpu->DMMU, EA);
}

INLINE_GEKKO_INSTR(lbzx) {
    GEKKO_INSTR_HEADER
    log_cpu_verbose("lbzx %08x", instruction.raw);

    u32 EA = (instruction.general_DAB.A ? cpu->GPR[instruction.general_DAd.A] : 0) + cpu->GPR[instruction.general_DAB.B];
    cpu->GPR[instruction.general_DAB.D] = read8(&cpu->DMMU, EA);
}

GEKKO_INSTR(lha) {
    GEKKO_INSTR_HEADER
    log_cpu_verbose("lha %08x", instruction.raw);

    u32 EA = (instruction.general_DAd.A ? cpu->GPR[instruction.general_DAd.A] : 0) + (i32)((i16)(instruction.general_DAd.d));
    cpu->GPR[instruction.general_DAd.D] = (i32)((i16)read16(&cpu->DMMU, EA));
}

INLINE_GEKKO_INSTR(lhzx) {
    GEKKO_INSTR_HEADER
    log_cpu_verbose("lhzx %08x", instruction.raw);

    u32 EA = (instruction.general_DAB.A ? cpu->GPR[instruction.general_DAd.A] : 0) + cpu->GPR[instruction.general_DAB.B];
    cpu->GPR[instruction.general_DAB.D] = read16(&cpu->DMMU, EA);
}

GEKKO_INSTR(lbz) {
    GEKKO_INSTR_HEADER
    log_cpu_verbose("lbz %08x", instruction.raw);

    u32 EA = (instruction.general_DAd.A ? cpu->GPR[instruction.general_DAd.A] : 0) + (i32)((i16)(instruction.general_DAd.d));
    cpu->GPR[instruction.general_DAd.D] = read8(&cpu->DMMU, EA);
}

GEKKO_INSTR(lbzu) {
    GEKKO_INSTR_HEADER
    log_cpu_verbose("lbzu %08x", instruction.raw);

    u32 EA = cpu->GPR[instruction.general_DAd.A] + (i32)((i16)(instruction.general_DAd.d));
    cpu->GPR[instruction.general_DAd.D] = read8(&cpu->DMMU, EA);
    cpu->GPR[instruction.general_DAd.A] = EA;
}

GEKKO_INSTR(lwzu) {
    GEKKO_INSTR_HEADER
    log_cpu_verbose("lwzu %08x", instruction.raw);

    u32 EA = cpu->GPR[instruction.general_DAd.A] + (i32)((i16)(instruction.general_DAd.d));
    cpu->GPR[instruction.general_DAd.D] = read32(&cpu->DMMU, EA);
    cpu->GPR[instruction.general_DAd.A] = EA;
    // todo: what if rD == rA? (instruction form invalid)
}

INLINE_GEKKO_INSTR(lwzx) {
    GEKKO_INSTR_HEADER
    log_cpu_verbose("lwzx %08x", instruction.raw);

    u32 EA = (instruction.general_DAB.A ? cpu->GPR[instruction.general_DAB.A] : 0) + cpu->GPR[instruction.general_DAB.B];
    cpu->GPR[instruction.general_DAB.D] = read32(&cpu->DMMU, EA);
}

INLINE_GEKKO_INSTR(lwzux) {
    GEKKO_INSTR_HEADER
    log_cpu_verbose("lwzux %08x", instruction.raw);

    u32 EA = (instruction.general_DAB.A ? cpu->GPR[instruction.general_DAB.A] : 0) + cpu->GPR[instruction.general_DAB.B];
    cpu->GPR[instruction.general_DAB.D] = read32(&cpu->DMMU, EA);
    cpu->GPR[instruction.general_DAB.A] = EA;
}

INLINE_GEKKO_INSTR(lwbrx) {
    GEKKO_INSTR_HEADER
    log_cpu_verbose("lwzbrx %08x", instruction.raw);

    u32 EA = (instruction.general_DAB.A ? cpu->GPR[instruction.general_DAB.A] : 0) + cpu->GPR[instruction.general_DAB.B];
    u32 word = read32(&cpu->DMMU, EA);
    cpu->GPR[instruction.general_DAB.D] = __bswap_32(word);
}

GEKKO_INSTR(stmw) {
    GEKKO_INSTR_HEADER

    log_cpu_verbose("stmw %08x", instruction.raw);

    u32 EA = (instruction.general_SAd.A ? cpu->GPR[instruction.general_SAd.A] : 0) + (i32)((i16)instruction.general_SAd.d);

    for (u8 r = instruction.general_SAd.S; r < 32; r++, EA += 4) {
        write32(&cpu->DMMU, EA, cpu->GPR[r]);
    }

    // todo: misaligned write exception
}

GEKKO_INSTR(lmw) {
    GEKKO_INSTR_HEADER

    log_cpu_verbose("lmw %08x", instruction.raw);

    u32 EA = (instruction.general_SAd.A ? cpu->GPR[instruction.general_SAd.A] : 0) + (i32)((i16)instruction.general_SAd.d);

    for (u8 r = instruction.general_SAd.S; r < 32; r++, EA += 4) {
        cpu->GPR[r] = read32(&cpu->DMMU, EA);
    }

    // todo: misaligned write exception
}