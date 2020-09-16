#include "instructions.h"

INLINE_GEKKO_INSTR(add) {
    GEKKO_INSTR_HEADER
    log_cpu_verbose("add %08x", instruction.raw);

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
    log_cpu_verbose("addi %08x", instruction.raw);
    if (!instruction.arithmetic_simm.A) {
        cpu->GPR[instruction.arithmetic_simm.D] = (u32)((i32)((i16)(instruction.arithmetic_simm.SIMM)));
    }
    else {
        cpu->GPR[instruction.arithmetic_simm.D] = cpu->GPR[instruction.arithmetic_simm.A] + (i32)((i16)(instruction.arithmetic_simm.SIMM));
    }
}

INLINE_GEKKO_INSTR(addc) {
    GEKKO_INSTR_HEADER
    log_cpu_verbose("addc %08x", instruction.raw);

    cpu->XER.CA = ADD_CARRY(cpu->GPR[instruction.general_DAB.A], cpu->GPR[instruction.general_DAB.B]);
    u32 result = cpu->GPR[instruction.general_DAB.A] + cpu->GPR[instruction.general_DAB.B];

    if (instruction.general_DAB.OE) {
        UPDATE_XER_OV(cpu->XER, ADD_OVERFLOW32(cpu->GPR[instruction.general_DAB.A], cpu->GPR[instruction.general_DAB.B], result));
    }

    if (instruction.general_DAB.Rc) {
        UPDATE_CR0_RESULT32(cpu, result);
    }

    cpu->GPR[instruction.arithmetic_simm.D] = result;
}

INLINE_GEKKO_INSTR(adde) {
    GEKKO_INSTR_HEADER
    log_cpu_verbose("adde %08x", instruction.raw);

    u32 carry = cpu->XER.CA;
    u32 A = cpu->GPR[instruction.general_DAB.A];
    u32 B = cpu->GPR[instruction.general_DAB.B];
    cpu->XER.CA = ADD_CARRY(A, B) || ADD_CARRY(A + carry, B);
    u32 result = A + B + carry;

    if (instruction.general_DAB.OE) {
        UPDATE_XER_OV(cpu->XER, ADD_OVERFLOW32(cpu->GPR[instruction.general_DAB.A], carry, result));
    }

    if (instruction.general_DAB.Rc) {
        UPDATE_CR0_RESULT32(cpu, result);
    }

    cpu->GPR[instruction.general_DAB.D] = result;
}

INLINE_GEKKO_INSTR(addze) {
    GEKKO_INSTR_HEADER
    log_cpu_verbose("addze %08x", instruction.raw);

    u32 carry = cpu->XER.CA;
    cpu->XER.CA = ADD_CARRY(cpu->GPR[instruction.general_DAB.A], carry);
    u32 result = cpu->GPR[instruction.general_DAB.A] + carry;

    if (instruction.general_DAB.OE) {
        UPDATE_XER_OV(cpu->XER, ADD_OVERFLOW32(cpu->GPR[instruction.general_DAB.A], carry, result));
    }

    if (instruction.general_DAB.Rc) {
        UPDATE_CR0_RESULT32(cpu, result);
    }

    cpu->GPR[instruction.general_DAB.D] = result;
}

GEKKO_INSTR(addis) {
    GEKKO_INSTR_HEADER
    log_cpu_verbose("addis %08x", instruction.raw);
    if (!instruction.arithmetic_simm.A) {
        cpu->GPR[instruction.arithmetic_simm.D] = (u32)(((i32)((i16)(instruction.arithmetic_simm.SIMM))) << 16);
    }
    else {
        cpu->GPR[instruction.arithmetic_simm.D] = cpu->GPR[instruction.arithmetic_simm.A] + (i32)(((i16)(instruction.arithmetic_simm.SIMM)) << 16);
    }
}

GEKKO_INSTR(addic) {
    GEKKO_INSTR_HEADER
    log_cpu_verbose("addic %08x", instruction.raw);
    i32 SIMM = (i32)((i16)instruction.arithmetic_simm.SIMM);
    cpu->XER.CA = ADD_CARRY(cpu->GPR[instruction.arithmetic_simm.A], SIMM);
    cpu->GPR[instruction.arithmetic_simm.D] = cpu->GPR[instruction.arithmetic_simm.A] + SIMM;
}

GEKKO_INSTR(addic_) {
    GEKKO_INSTR_HEADER
    log_cpu_verbose("addic %08x", instruction.raw);
    i32 SIMM = (i32)((i16)instruction.arithmetic_simm.SIMM);
    cpu->XER.CA = ADD_CARRY(cpu->GPR[instruction.arithmetic_simm.A], SIMM);
    cpu->GPR[instruction.arithmetic_simm.D] = cpu->GPR[instruction.arithmetic_simm.A] + SIMM;
    UPDATE_CR0_RESULT32(cpu, cpu->GPR[instruction.arithmetic_simm.D]);
}

INLINE_GEKKO_INSTR(subf) {
    GEKKO_INSTR_HEADER
    log_cpu_verbose("subf %08x", instruction.raw);

    u32 result = ~cpu->GPR[instruction.general_DAB.A] + cpu->GPR[instruction.general_DAB.B] + 1;

    if (instruction.general_DAB.OE) {
        UPDATE_XER_OV(cpu->XER, ADD_OVERFLOW32(~cpu->GPR[instruction.general_DAB.A], cpu->GPR[instruction.general_DAB.B], result));
    }

    if (instruction.general_DAB.Rc) {
        UPDATE_CR0_RESULT32(cpu, result);
    }

    cpu->GPR[instruction.general_DAB.D] = result;
}

GEKKO_INSTR(subfic) {
    GEKKO_INSTR_HEADER
    log_cpu_verbose("subfic %08x", instruction.raw);
    i32 SIMM = (i32)((i16)instruction.arithmetic_simm.SIMM);
    u32 A = ~cpu->GPR[instruction.arithmetic_simm.A];

    cpu->XER.CA = ADD_CARRY(A, SIMM) || ((A + SIMM) == 0xffffffff);

    cpu->GPR[instruction.arithmetic_simm.D] = A + SIMM + 1;
}

INLINE_GEKKO_INSTR(subfe) {
    GEKKO_INSTR_HEADER
    log_cpu_verbose("subfe %08x", instruction.raw);

    u32 carry = cpu->XER.CA;
    u32 A = ~cpu->GPR[instruction.general_DAB.A];
    u32 result = A + cpu->GPR[instruction.general_DAB.B] + carry;

    cpu->XER.CA = ADD_CARRY(A, cpu->GPR[instruction.general_DAB.B]) || ADD_CARRY(A + cpu->GPR[instruction.general_DAB.B], carry);

    if (instruction.general_DAB.OE) {
        UPDATE_XER_OV(cpu->XER, ADD_OVERFLOW32(A, cpu->GPR[instruction.general_DAB.B], result));
    }

    if (instruction.general_DAB.Rc) {
        UPDATE_CR0_RESULT32(cpu, result);
    }

    cpu->GPR[instruction.general_DAB.D] = result;
}

INLINE_GEKKO_INSTR(subfc) {
    GEKKO_INSTR_HEADER
    log_cpu_verbose("subfe %08x", instruction.raw);

    u32 A = ~cpu->GPR[instruction.general_DAB.A];
    u32 B = cpu->GPR[instruction.general_DAB.B];
    u32 result = A + cpu->GPR[instruction.general_DAB.B] + 1;

    cpu->XER.CA = ADD_CARRY(A, B) || (A + B == 0xffffffff);

    if (instruction.general_DAB.OE) {
        UPDATE_XER_OV(cpu->XER, ADD_OVERFLOW32(A, cpu->GPR[instruction.general_DAB.B], result));
    }

    if (instruction.general_DAB.Rc) {
        UPDATE_CR0_RESULT32(cpu, result);
    }

    cpu->GPR[instruction.general_DAB.D] = result;
}

INLINE_GEKKO_INSTR(divw) {
    GEKKO_INSTR_HEADER
    log_cpu_verbose("divw %08x", instruction.raw);

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
    log_cpu_verbose("divwu %08x", instruction.raw);

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
    log_cpu_verbose("mulli %08x", instruction.raw);

    i64 result = (i64)((i32)cpu->GPR[instruction.arithmetic_simm.A]) * (i64)((i16)instruction.arithmetic_simm.SIMM);

    cpu->GPR[instruction.arithmetic_simm.D] = (u32)result;
}

INLINE_GEKKO_INSTR(mullw) {
    GEKKO_INSTR_HEADER
    log_cpu_verbose("mullw %08x", instruction.raw);

    u64 result = (u64)cpu->GPR[instruction.general_DAB.A] * (u64)cpu->GPR[instruction.general_DAB.B];

    if (instruction.general_DAB.OE) {
        UPDATE_XER_OV(cpu->XER, (result >> 32) != 0);
    }

    if (instruction.general_DAB.Rc) {
        UPDATE_CR0_RESULT32(cpu, (u32)result);
    }

    cpu->GPR[instruction.general_DAB.D] = (u32)result;
}