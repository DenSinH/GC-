#include "PPC/instructions.h"
#include "PPC/registers/GQR.h"
#include "PPC/registers/HID.h"
#include "PPC/MMU.h"

GEKKO_INSTR(psq_l) {
    GEKKO_INSTR_HEADER
    ASSERT_FLOATING_POINT
    log_cpu("psq_l %08x", instruction.raw);

    u32 EA = (instruction.paired_single_load.A ? cpu->GPR[instruction.paired_single_load.A] : 0) +
             EXTS32(instruction.paired_single_load.d, 12);

    s_GQR GQR = cpu->GQR[instruction.paired_single_load.I];
    float scale = dequantize_scale[GQR.LD_SCALE];
    bit_float PS0, PS1;
    PS1.f = 1.0f;

    switch (GQR.LD_TYPE) {
        case GQR_TYPE_single_precision:
            PS0.u = read32(&cpu->DMMU, EA);
            if (!instruction.paired_single_load.W) PS1.u = read32(&cpu->DMMU, EA + 4);
            break;
        case GQR_TYPE_unsigned_8bit_int:
            PS0.f = (float)read8(&cpu->DMMU, EA) * scale;
            if (!instruction.paired_single_load.W) PS1.f = (float)read8(&cpu->DMMU, EA + 1) * scale;
            break;
        case GQR_TYPE_unsigned_16_bit_int:
            PS0.f = (float)read16(&cpu->DMMU, EA) * scale;
            if (!instruction.paired_single_load.W) PS1.f = (float)read16(&cpu->DMMU, EA + 2) * scale;
            break;
        case GQR_TYPE_signed_8bit_int:
            PS0.f = (float)((i8)read8(&cpu->DMMU, EA)) * scale;
            if (!instruction.paired_single_load.W) PS1.f = (float)((i8)read8(&cpu->DMMU, EA + 1)) * scale;
            break;
        case GQR_TYPE_signed_16bit_int:
            PS0.f = (float)((i16)read16(&cpu->DMMU, EA)) * scale;
            if (!instruction.paired_single_load.W) PS1.f = (float)((i16)read16(&cpu->DMMU, EA + 2)) * scale;
            break;
        default:
            log_fatal("Invalid GQR_TYPE for single dequantization: %d", GQR.LD_TYPE);
    }

    cpu->FPR[instruction.paired_single_load.D].PS0.d = CONVERT_TO_DOUBLE(PS0).d;
    cpu->FPR[instruction.paired_single_load.D].PS1.d = CONVERT_TO_DOUBLE(PS1).d;
}

INLINE_GEKKO_INSTR(ps_mr) {
    GEKKO_INSTR_HEADER
    ASSERT_FLOATING_POINT
    log_cpu("ps_mr %x", instruction.raw);

    cpu->FPR[instruction.general_DAB.D].PS0.u = cpu->FPR[instruction.general_DAB.B].PS0.u;
    cpu->FPR[instruction.general_DAB.D].PS1.u = cpu->FPR[instruction.general_DAB.B].PS1.u;

    // todo: I don't think floating point exceptions can be caused from moving
    if (instruction.general_DAB.Rc) {
        UPDATE_CR1_FROM_FPSCR(cpu->CR, cpu->FPSCR);
    }
}