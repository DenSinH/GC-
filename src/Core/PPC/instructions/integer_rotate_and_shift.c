#include "PPC/instructions.h"

GEKKO_INSTR(rlwinm) {
    GEKKO_INSTR_HEADER
    log_cpu("rlwinm %08x", instruction.raw);

    u32 r = ROTL32(cpu->GPR[instruction.rotate.S], instruction.rotate.SH);
    u32 m = MASK(instruction.rotate.MB, instruction.rotate.ME);

    cpu->GPR[instruction.rotate.A] = (r & m);

    if (instruction.general_DAB.Rc) {
        UPDATE_CR0_RESULT32(cpu, cpu->GPR[instruction.rotate.A]);
    }
}

GEKKO_INSTR(rlwimi) {
    GEKKO_INSTR_HEADER
    log_cpu("rlwimi %08x", instruction.raw);

    u32 r = ROTL32(cpu->GPR[instruction.rotate.S], instruction.rotate.SH);
    u32 m = MASK(instruction.rotate.MB, instruction.rotate.ME);

    cpu->GPR[instruction.rotate.A] = (r & m) | (cpu->GPR[instruction.rotate.A] & ~m);

    if (instruction.general_DAB.Rc) {
        UPDATE_CR0_RESULT32(cpu, cpu->GPR[instruction.rotate.A]);
    }
}

INLINE_GEKKO_INSTR(slw) {
    GEKKO_INSTR_HEADER
    log_cpu("slw %08x", instruction.raw);

    u8 n = cpu->GPR[instruction.general_SAB.B] & 0x1f;
    if (n > 31) {
        cpu->GPR[instruction.general_SAB.A] = 0;
    }
    else {
        cpu->GPR[instruction.general_SAB.A] = cpu->GPR[instruction.general_SAB.S] << n;
    }

    if (instruction.general_DAB.Rc) {
        UPDATE_CR0_RESULT32(cpu, cpu->GPR[instruction.general_SAB.A]);
    }
}

INLINE_GEKKO_INSTR(srw) {
    GEKKO_INSTR_HEADER
    log_cpu("srw %08x", instruction.raw);

    u8 n = cpu->GPR[instruction.general_SAB.B] & 0x1f;
    if (n > 31) {
        cpu->GPR[instruction.general_SAB.A] = 0;
    }
    else {
        cpu->GPR[instruction.general_SAB.A] = cpu->GPR[instruction.general_SAB.S] >> n;
    }

    if (instruction.general_DAB.Rc) {
        UPDATE_CR0_RESULT32(cpu, cpu->GPR[instruction.general_SAB.A]);
    }
}

INLINE_GEKKO_INSTR(sraw) {
    GEKKO_INSTR_HEADER
    log_cpu("sraw %08x", instruction.raw);

    u8 n = cpu->GPR[instruction.general_SAB.B] & 0x1f;
    if (n > 31) {
        if (cpu->GPR[instruction.general_SAB.S] & 0x80000000) {
            cpu->GPR[instruction.general_SAB.A] = 0xffffffff;
            cpu->XER.CA = 1;
        }
        else {
            cpu->GPR[instruction.general_SAB.A] = 0;
            cpu->XER.CA = 0;
        }
    }
    else {
        cpu->XER.CA = (cpu->GPR[instruction.general_SAB.S] & ((1 << n) - 1)) != 0;
        cpu->GPR[instruction.general_SAB.A] = ((i32)cpu->GPR[instruction.general_SAB.S]) >> n;
    }

    if (instruction.general_DAB.Rc) {
        UPDATE_CR0_RESULT32(cpu, cpu->GPR[instruction.general_SAB.A]);
    }
}