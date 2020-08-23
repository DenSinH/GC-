#include "PPC/instructions.h"

GEKKO_INSTR(rlwinm) {
    GEKKO_INSTR_HEADER
    log_cpu("rlwinm %08x", instruction.raw);

    cpu->GPR[instruction.rotate.A] =
            (ROTL32(cpu->GPR[instruction.rotate.S], instruction.rotate.SH)) &
            (MASK(instruction.rotate.MB, instruction.rotate.ME));

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