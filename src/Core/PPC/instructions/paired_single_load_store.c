#include "instructions.h"
#include "Registers/GQR.h"
#include "Registers/HID.h"
#include "MMU.h"

#include "helpers.h"
#include "float_utils.h"

#include <limits.h>

GEKKO_INSTR(psq_l) {
    GEKKO_INSTR_HEADER
    ASSERT_PAIRED_SINGLE
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

GEKKO_INSTR(psq_st) {
    GEKKO_INSTR_HEADER
    ASSERT_PAIRED_SINGLE
    log_cpu("psq_st %08x", instruction.raw);

    u32 EA = (instruction.paired_single_store.A ? cpu->GPR[instruction.paired_single_store.A] : 0) +
             EXTS32(instruction.paired_single_store.d, 12);

    s_GQR GQR = cpu->GQR[instruction.paired_single_store.I];
    float scale = quantize_scale[GQR.ST_SCALE];
    double PS0 = cpu->FPR[instruction.paired_single_store.S].PS0.d * scale;
    double PS1 = cpu->FPR[instruction.paired_single_store.S].PS1.d * scale;

    bit_float PS0quantized, PS1quantized;

    switch (GQR.ST_TYPE) {
        case GQR_TYPE_single_precision:
            PS0quantized.f = (float)PS0;
            write32(&cpu->DMMU, EA, PS0quantized.u);
            if (!instruction.paired_single_store.W) {
                PS1quantized.f = (float)PS1;
                write32(&cpu->DMMU, EA + 4, PS1quantized.u);
            }
            break;
        case GQR_TYPE_unsigned_8bit_int:
            write8(&cpu->DMMU, EA, (u8)(CLAMP(PS0, 0, UINT8_MAX)));
            if (!instruction.paired_single_store.W) write8(&cpu->DMMU, EA + 1, (u8)(CLAMP(PS1, 0, UINT8_MAX)));
            break;
        case GQR_TYPE_unsigned_16_bit_int:
            write16(&cpu->DMMU, EA, (u8)(CLAMP(PS0, 0, UINT16_MAX)));
            if (!instruction.paired_single_store.W) write16(&cpu->DMMU, EA + 2, (u16)(CLAMP(PS1, 0, UINT16_MAX)));
            break;
        case GQR_TYPE_signed_8bit_int:
            write8(&cpu->DMMU, EA, (i8)(CLAMP(PS0, INT8_MIN, INT8_MAX)));
            if (!instruction.paired_single_store.W) write8(&cpu->DMMU, EA + 1, (i8)(CLAMP(PS1, INT8_MIN, INT8_MAX)));
            break;
        case GQR_TYPE_signed_16bit_int:
            write16(&cpu->DMMU, EA, (i16)(CLAMP(PS0, INT16_MIN, INT16_MAX)));
            if (!instruction.paired_single_store.W) write16(&cpu->DMMU, EA + 2, (i16)(CLAMP(PS1, INT16_MIN, INT16_MAX)));
            break;
        default:
            log_fatal("Invalid GQR_TYPE for single quantization: %d", GQR.LD_TYPE);
    }
}

INLINE_GEKKO_INSTR(ps_mr) {
    GEKKO_INSTR_HEADER
    ASSERT_PAIRED_SINGLE
    log_cpu("ps_mr %x", instruction.raw);

    cpu->FPR[instruction.general_DAB.D].PS0.u = cpu->FPR[instruction.general_DAB.B].PS0.u;
    cpu->FPR[instruction.general_DAB.D].PS1.u = cpu->FPR[instruction.general_DAB.B].PS1.u;

    if (instruction.general_DAB.Rc) {
        UPDATE_CR1_FROM_FPSCR(cpu->CR, cpu->FPSCR);
    }
}