#include "PPC/instructions.h"
#include "PPC/registers/GQR.h"
#include "PPC/registers/HID.h"
#include "PPC/MMU.h"

#define SINGLE_1 0x3f800000
#define CAST_U32_SINGLE(val) (*(float*)&val)
#define CAST_SINGLE_U32(val) (*(u32*)&val)

GEKKO_INSTR(psq_l) {
    ASSERT_BITFIELD_SIZE
    ASSERT_FLOATING_POINT
    log_cpu("psq_l %08x", instruction.raw);

    u32 EA = (cpu->GPR[instruction.paired_single_load.A] ? cpu->GPR[instruction.paired_single_load.A] : 0) +
             EXTS32(instruction.paired_single_load.d, 12);

    s_GQR GQR = cpu->GQR[instruction.paired_single_load.I];
    float scale = dequantize_scale[GQR.LD_SCALE];
    float PS0;
    float PS1 = 1.0f;

    switch (GQR.LD_TYPE) {
        case GQR_TYPE_single_precision:
            PS0 = read32(&cpu->DMMU, EA);
            if (!instruction.paired_single_load.W) PS1 = read32(&cpu->DMMU, EA + 4);
            break;
        case GQR_TYPE_unsigned_8bit_int:
            PS0 = (float)read8(&cpu->DMMU, EA) * scale;
            if (!instruction.paired_single_load.W) PS1 = (float)read8(&cpu->DMMU, EA + 1) * scale;
            break;
        case GQR_TYPE_unsigned_16_bit_int:
            PS0 = (float)read16(&cpu->DMMU, EA) * scale;
            if (!instruction.paired_single_load.W) PS1 = (float)read16(&cpu->DMMU, EA + 2) * scale;
            break;
        case GQR_TYPE_signed_8bit_int:
            PS0 = (float)((i8)read8(&cpu->DMMU, EA)) * scale;
            if (!instruction.paired_single_load.W) PS1 = (float)((i8)read8(&cpu->DMMU, EA + 1)) * scale;
            break;
        case GQR_TYPE_signed_16bit_int:
            PS0 = (float)((i16)read16(&cpu->DMMU, EA)) * scale;
            if (!instruction.paired_single_load.W) PS1 = (float)((i16)read16(&cpu->DMMU, EA + 2)) * scale;
            break;
        default:
            log_fatal("Invalid GQR_TYPE for single dequantization: %d", GQR.ST_TYPE);
    }

    cpu->FPR[instruction.paired_single_load.D] = (((u64)CAST_SINGLE_U32(PS0)) << 32) | CAST_SINGLE_U32(PS1);
}
