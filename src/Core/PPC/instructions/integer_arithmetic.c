#include "PPC/instructions.h"

INLINE_GEKKO_INSTR(add) {
    GEKKO_INSTR_HEADER
    log_cpu("add %08x", instruction.raw);

    u32 result = cpu->GPR[instruction.general_DAB.A] + cpu->GPR[instruction.general_DAB.B];

    if (instruction.general_DAB.OE) {
        UPDATE_XER_OV(cpu->XER, ADD_OVERFLOW32(cpu->GPR[instruction.general_DAB.A], cpu->GPR[instruction.general_DAB.B], result));
    }

    if (instruction.general_DAB.Rc) {
        UPDATE_CR0_RESULT32(cpu, result);
    }

    cpu->GPR[instruction.general_DAB.D] = result;
}

GEKKO_INSTR(addi) {
    GEKKO_INSTR_HEADER
    log_cpu("addi %08x", instruction.raw);
    if (!instruction.arithmetic_simm.A) {
        cpu->GPR[instruction.arithmetic_simm.D] = (u32)((i32)((i16)(instruction.arithmetic_simm.SIMM)));
    }
    else {
        cpu->GPR[instruction.arithmetic_simm.D] = cpu->GPR[instruction.arithmetic_simm.A] + (i32)((i16)(instruction.arithmetic_simm.SIMM));
    }
}

GEKKO_INSTR(addis) {
    GEKKO_INSTR_HEADER
    log_cpu("addis %08x", instruction.raw);
    if (!instruction.arithmetic_simm.A) {
        cpu->GPR[instruction.arithmetic_simm.D] = (u32)(((i32)((i16)(instruction.arithmetic_simm.SIMM))) << 16);
    }
    else {
        cpu->GPR[instruction.arithmetic_simm.D] = cpu->GPR[instruction.arithmetic_simm.A] + (i32)(((i16)(instruction.arithmetic_simm.SIMM)) << 16);
    }
}

GEKKO_INSTR(addic) {
    GEKKO_INSTR_HEADER
    log_cpu("addic %08x", instruction.raw);
    i32 SIMM = (i32)((i16)instruction.arithmetic_simm.SIMM);
    cpu->XER.CA = ADD_CARRY(cpu->GPR[instruction.arithmetic_simm.A], SIMM);
    cpu->GPR[instruction.arithmetic_simm.D] = cpu->GPR[instruction.arithmetic_simm.A] + SIMM;
}

GEKKO_INSTR(addic_) {
    GEKKO_INSTR_HEADER
    log_cpu("addic %08x", instruction.raw);
    i32 SIMM = (i32)((i16)instruction.arithmetic_simm.SIMM);
    cpu->XER.CA = ADD_CARRY(cpu->GPR[instruction.arithmetic_simm.A], SIMM);
    cpu->GPR[instruction.arithmetic_simm.D] = cpu->GPR[instruction.arithmetic_simm.A] + SIMM;
    UPDATE_CR0_RESULT32(cpu, cpu->GPR[instruction.arithmetic_simm.D]);
}

INLINE_GEKKO_INSTR(subf) {
    GEKKO_INSTR_HEADER
    log_cpu("subf %08x", instruction.raw);

    u32 result = ~cpu->GPR[instruction.general_DAB.A] + cpu->GPR[instruction.general_DAB.B] + 1;

    if (instruction.general_DAB.OE) {
        UPDATE_XER_OV(cpu->XER, ADD_OVERFLOW32(~cpu->GPR[instruction.general_DAB.A], cpu->GPR[instruction.general_DAB.B], result));
    }

    if (instruction.general_DAB.Rc) {
        UPDATE_CR0_RESULT32(cpu, result);
    }

    cpu->GPR[instruction.general_DAB.D] = result;
}

INLINE_GEKKO_INSTR(divw) {
    GEKKO_INSTR_HEADER
    log_cpu("divw %08x", instruction.raw);

    bool overflow = cpu->GPR[instruction.general_DAB.B] == 0 ||
                   (cpu->GPR[instruction.general_DAB.A] == 0x80000000 && (i32)cpu->GPR[instruction.general_DAB.B] == -1);

    i32 result;
    if (overflow) {
        result = (i32)cpu->GPR[instruction.general_DAB.A] < 0 ? MAXINT32 : 0;
    }
    else {
        result = (i32)cpu->GPR[instruction.general_DAB.A] / (i32)cpu->GPR[instruction.general_DAB.B];
    }


    if (instruction.general_DAB.OE) {
        UPDATE_XER_OV(cpu->XER, overflow);
    }

    if (instruction.general_DAB.Rc) {
        UPDATE_CR0_RESULT32(cpu, result);
    }

    cpu->GPR[instruction.general_DAB.D] = result;
}

INLINE_GEKKO_INSTR(divwu) {
    GEKKO_INSTR_HEADER
    log_cpu("divwu %08x", instruction.raw);

    bool overflow = cpu->GPR[instruction.general_DAB.B] == 0;

    u32 result;
    if (overflow) {
        result = 0;
    }
    else {
        result = cpu->GPR[instruction.general_DAB.A] / cpu->GPR[instruction.general_DAB.B];
    }

    if (instruction.general_DAB.OE) {
        UPDATE_XER_OV(cpu->XER, overflow);
    }

    if (instruction.general_DAB.Rc) {
        UPDATE_CR0_RESULT32(cpu, result);
    }

    cpu->GPR[instruction.general_DAB.D] = result;
}

GEKKO_INSTR(mulli) {
    GEKKO_INSTR_HEADER
    log_cpu("mulli %08x", instruction.raw);

    i64 result = (i64)((i32)cpu->GPR[instruction.arithmetic_simm.A]) * (i64)((i16)instruction.arithmetic_simm.SIMM);

    cpu->GPR[instruction.arithmetic_simm.D] = (u32)result;
}

INLINE_GEKKO_INSTR(mullw) {
    GEKKO_INSTR_HEADER
    log_cpu("mullw %08x", instruction.raw);

    u64 result = (u64)cpu->GPR[instruction.general_DAB.A] * (u64)cpu->GPR[instruction.general_DAB.B];

    if (instruction.general_DAB.OE) {
        UPDATE_XER_OV(cpu->XER, (result >> 32) != 0);
    }

    if (instruction.general_DAB.Rc) {
        UPDATE_CR0_RESULT32(cpu, (u32)result);
    }

    cpu->GPR[instruction.general_DAB.D] = (u32)result;
}