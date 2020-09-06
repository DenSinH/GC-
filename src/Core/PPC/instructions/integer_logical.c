#include "PPC/instructions.h"
#include "helpers.h"


GEKKO_INSTR(ori) {
    GEKKO_INSTR_HEADER
    log_cpu("ori %08x", instruction.raw);
    cpu->GPR[instruction.arithmetic_uimm.A] = cpu->GPR[instruction.arithmetic_uimm.S] | (u32)instruction.arithmetic_uimm.UIMM;
}

GEKKO_INSTR(oris) {
    GEKKO_INSTR_HEADER
    log_cpu("oris %08x", instruction.raw);
    cpu->GPR[instruction.arithmetic_uimm.A] = cpu->GPR[instruction.arithmetic_uimm.S] | ((u32)instruction.arithmetic_uimm.UIMM << 16);
}

INLINE_GEKKO_INSTR(orx) {
    GEKKO_INSTR_HEADER
    log_cpu("orx %08x", instruction.raw);
    cpu->GPR[instruction.general_SAB.A] = cpu->GPR[instruction.general_SAB.S] | cpu->GPR[instruction.general_SAB.B];
    if (instruction.general_SAB.Rc) {
        UPDATE_CR0_RESULT32(cpu, cpu->GPR[instruction.general_SAB.A]);
    }
}

INLINE_GEKKO_INSTR(andx) {
    GEKKO_INSTR_HEADER
    log_cpu("andx %08x", instruction.raw);
    cpu->GPR[instruction.general_SAB.A] = cpu->GPR[instruction.general_SAB.S] & cpu->GPR[instruction.general_SAB.B];
    if (instruction.general_SAB.Rc) {
        UPDATE_CR0_RESULT32(cpu, cpu->GPR[instruction.general_SAB.A]);
    }
}

GEKKO_INSTR(andi) {
    GEKKO_INSTR_HEADER
    log_cpu("andi %08x", instruction.raw);
    cpu->GPR[instruction.arithmetic_uimm.A] = cpu->GPR[instruction.arithmetic_uimm.S] & (u32)instruction.arithmetic_uimm.UIMM;
    UPDATE_CR0_RESULT32(cpu, cpu->GPR[instruction.general_SAB.A]);
}

GEKKO_INSTR(andis) {
    GEKKO_INSTR_HEADER
    log_cpu("andis %08x", instruction.raw);
    cpu->GPR[instruction.arithmetic_uimm.A] = cpu->GPR[instruction.arithmetic_uimm.S] & ((u32)instruction.arithmetic_uimm.UIMM << 16);
    UPDATE_CR0_RESULT32(cpu, cpu->GPR[instruction.general_SAB.A]);
}

INLINE_GEKKO_INSTR(nor) {
    GEKKO_INSTR_HEADER
    log_cpu("nor %08x", instruction.raw);
    cpu->GPR[instruction.general_SAB.A] = ~(cpu->GPR[instruction.general_SAB.S] | cpu->GPR[instruction.general_SAB.B]);
    if (instruction.general_SAB.Rc) {
        UPDATE_CR0_RESULT32(cpu, cpu->GPR[instruction.general_SAB.A]);
    }
}

INLINE_GEKKO_INSTR(andc) {
    GEKKO_INSTR_HEADER
    log_cpu("andc %08x", instruction.raw);
    cpu->GPR[instruction.general_SAB.A] = cpu->GPR[instruction.general_SAB.S] & ~cpu->GPR[instruction.general_SAB.B];
    if (instruction.general_SAB.Rc) {
        UPDATE_CR0_RESULT32(cpu, cpu->GPR[instruction.general_SAB.A]);
    }
}

INLINE_GEKKO_INSTR(xorx) {
    GEKKO_INSTR_HEADER
    log_cpu("xorx %08x", instruction.raw);
    cpu->GPR[instruction.general_SAB.A] = cpu->GPR[instruction.general_SAB.S] ^ cpu->GPR[instruction.general_SAB.B];
    if (instruction.general_SAB.Rc) {
        UPDATE_CR0_RESULT32(cpu, cpu->GPR[instruction.general_SAB.A]);
    }
}

GEKKO_INSTR(xori) {
    GEKKO_INSTR_HEADER
    log_cpu("xori %08x", instruction.raw);
    cpu->GPR[instruction.arithmetic_uimm.A] = cpu->GPR[instruction.arithmetic_uimm.S] ^ (u32)instruction.arithmetic_uimm.UIMM;
}

GEKKO_INSTR(xoris) {
    GEKKO_INSTR_HEADER
    log_cpu("oris %08x", instruction.raw);
    cpu->GPR[instruction.arithmetic_uimm.A] = cpu->GPR[instruction.arithmetic_uimm.S] ^ ((u32)instruction.arithmetic_uimm.UIMM << 16);
}

INLINE_GEKKO_INSTR(neg) {
    GEKKO_INSTR_HEADER
    log_cpu("neg %08x", instruction.raw);
    u32 A = cpu->GPR[instruction.arithmetic_uimm.A];

    if (instruction.general_DAB.OE) {
        // only way this overflows is if A == 0x88000000 -> (0x7fffffff + 1)
        UPDATE_XER_OV(cpu->XER, A == 0x80000000);
    }

    cpu->GPR[instruction.general_DAB.D] = (~A) + 1;

    if (instruction.general_DAB.Rc) {
        UPDATE_CR0_RESULT32(cpu, cpu->GPR[instruction.general_DAB.D]);
    }
}

INLINE_GEKKO_INSTR(extsh) {
    GEKKO_INSTR_HEADER
    log_cpu("extsh %08x", instruction.raw);
    cpu->GPR[instruction.general_SAB.A] = (i32)((i16)cpu->GPR[instruction.general_SAB.S]);
    if (instruction.general_SAB.Rc) {
        UPDATE_CR0_RESULT32(cpu, cpu->GPR[instruction.general_SAB.A]);
    }
}

INLINE_GEKKO_INSTR(extsb) {
    GEKKO_INSTR_HEADER
    log_cpu("extsh %08x", instruction.raw);
    cpu->GPR[instruction.general_SAB.A] = (i32)((i8)cpu->GPR[instruction.general_SAB.S]);
    if (instruction.general_SAB.Rc) {
        UPDATE_CR0_RESULT32(cpu, cpu->GPR[instruction.general_SAB.A]);
    }
}

INLINE_GEKKO_INSTR(cntlzw) {
    GEKKO_INSTR_HEADER
    log_cpu("cntlzw %08x", instruction.raw);

    cpu->GPR[instruction.general_SAB.A] = ctlz(cpu->GPR[instruction.general_SAB.S]);

    if (instruction.general_SAB.Rc) {
        UPDATE_CR0_RESULT32(cpu, cpu->GPR[instruction.general_SAB.A]);
    }
}