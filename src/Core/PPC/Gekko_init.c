#include "Gekko.h"
#include "instructions.h"

void init_Gekko(s_Gekko* cpu) {
    init_HW_regs(&cpu->HW_regs);

    cpu->IMMU.RAM_ptr = cpu->DMMU.RAM_ptr = cpu->memory;
    cpu->IMMU.SR_ptr = cpu->DMMU.SR_ptr = cpu->SR;
    cpu->IMMU.HW_regs_ptr = cpu->DMMU.HW_regs_ptr = &cpu->HW_regs;

    memset(&cpu->SPR_write_mask, 0xff, sizeof(cpu->SPR_write_mask));

    // todo: full map
    cpu->SPR[SPR_XER] = &cpu->XER;
    cpu->SPR[SPR_LR] = &cpu->LR;
    cpu->SPR[SPR_CTR] = &cpu->CTR;
    cpu->SPR[SPR_SRR0] = &cpu->SRR0;
    cpu->SPR[SPR_SRR1] = &cpu->SRR1;
    cpu->SPR[SPR_GQR0] = &cpu->GQR[0];
    cpu->SPR[SPR_GQR1] = &cpu->GQR[1];
    cpu->SPR[SPR_GQR2] = &cpu->GQR[2];
    cpu->SPR[SPR_GQR3] = &cpu->GQR[3];
    cpu->SPR[SPR_GQR4] = &cpu->GQR[4];
    cpu->SPR[SPR_GQR5] = &cpu->GQR[5];
    cpu->SPR[SPR_GQR6] = &cpu->GQR[6];
    cpu->SPR[SPR_GQR7] = &cpu->GQR[7];
    cpu->SPR[SPR_HID0] = &cpu->HID[0];
    cpu->SPR[SPR_HID1] = &cpu->HID[1];
    cpu->SPR[SPR_HID2] = &cpu->HID2;
    cpu->SPR_write_mask[SPR_HID2] = HID2_WRITE_MASK;

    cpu->SPR[SPR_IBAT0L] = &cpu->IMMU.BAT[BAT0L];
    cpu->SPR[SPR_IBAT0U] = &cpu->IMMU.BAT[BAT0U];
    cpu->SPR[SPR_IBAT1L] = &cpu->IMMU.BAT[BAT1L];
    cpu->SPR[SPR_IBAT1U] = &cpu->IMMU.BAT[BAT1U];
    cpu->SPR[SPR_IBAT2L] = &cpu->IMMU.BAT[BAT2L];
    cpu->SPR[SPR_IBAT2U] = &cpu->IMMU.BAT[BAT2U];
    cpu->SPR[SPR_IBAT3L] = &cpu->IMMU.BAT[BAT3L];
    cpu->SPR[SPR_IBAT3U] = &cpu->IMMU.BAT[BAT3U];

    cpu->SPR[SPR_DBAT0L] = &cpu->DMMU.BAT[BAT0L];
    cpu->SPR[SPR_DBAT0U] = &cpu->DMMU.BAT[BAT0U];
    cpu->SPR[SPR_DBAT1L] = &cpu->DMMU.BAT[BAT1L];
    cpu->SPR[SPR_DBAT1U] = &cpu->DMMU.BAT[BAT1U];
    cpu->SPR[SPR_DBAT2L] = &cpu->DMMU.BAT[BAT2L];
    cpu->SPR[SPR_DBAT2U] = &cpu->DMMU.BAT[BAT2U];
    cpu->SPR[SPR_DBAT3L] = &cpu->DMMU.BAT[BAT3L];
    cpu->SPR[SPR_DBAT3U] = &cpu->DMMU.BAT[BAT3U];

    // require normal reads/writes
    cpu->SPR[SPR_SPRG0] = &cpu->default_SPR[SPR_SPRG0];
    cpu->SPR[SPR_SPRG1] = &cpu->default_SPR[SPR_SPRG1];
    cpu->SPR[SPR_SPRG2] = &cpu->default_SPR[SPR_SPRG2];
    cpu->SPR[SPR_SPRG3] = &cpu->default_SPR[SPR_SPRG3];


    // todo: stubbed
    cpu->SPR[SPR_L2CR] = &cpu->default_SPR[SPR_L2CR];

    // todo: implement this
    cpu->SPR[SPR_MMCR0] = &cpu->default_SPR[SPR_MMCR0];
    cpu->SPR[SPR_MMCR1] = &cpu->default_SPR[SPR_MMCR1];
    cpu->SPR[SPR_PMC1] = &cpu->default_SPR[SPR_PMC1];
    cpu->SPR[SPR_PMC2] = &cpu->default_SPR[SPR_PMC2];
    cpu->SPR[SPR_PMC3] = &cpu->default_SPR[SPR_PMC3];
    cpu->SPR[SPR_PMC4] = &cpu->default_SPR[SPR_PMC4];

    build_instr_table(cpu);
}

void build_instr_table(s_Gekko* cpu) {
    for (u32 i = 0; i < MAIN_INSTR_TABLE_SIZE; i++) {
        switch (i) {
            /* branch and linkage */
            case MAIN_INSTR_HASH(B_X_OPCODE):
                cpu->instructions[i] = b_x;
                break;
            case MAIN_INSTR_HASH(BC_X_OPCODE):
                cpu->instructions[i] = bc_x;
                break;
            /* integer arithmetic */
            case MAIN_INSTR_HASH(ADDI_OPCODE):
                cpu->instructions[i] = addi;
                break;
            case MAIN_INSTR_HASH(ADDIS_OPCODE):
                cpu->instructions[i] = addis;
                break;
            /* integer logical */
            case MAIN_INSTR_HASH(ORI_OPCODE):
                cpu->instructions[i] = ori;
                break;
            case MAIN_INSTR_HASH(ORIS_OPCODE):
                cpu->instructions[i] = oris;
                break;
            case MAIN_INSTR_HASH(ANDI_OPCODE):
                cpu->instructions[i] = andi;
                break;
            case MAIN_INSTR_HASH(ANDIS_OPCODE):
                cpu->instructions[i] = andis;
                break;
            /* integer compare */
            case MAIN_INSTR_HASH(CMPLI_OPCODE):
                cpu->instructions[i] = cmpli;
                break;
            case MAIN_INSTR_HASH(CMPI_OPCODE):
                cpu->instructions[i] = cmpi;
                break;
            /* integer rotate and shift */
            case MAIN_INSTR_HASH(RLWINM_OPCODE):
                cpu->instructions[i] = rlwinm;
                break;
            /* integer load/store */
            case MAIN_INSTR_HASH(STW_OPCODE):
                cpu->instructions[i] = stw;
                break;
            case MAIN_INSTR_HASH(STH_OPCODE):
                cpu->instructions[i] = sth;
                break;
            case MAIN_INSTR_HASH(STWU_OPCODE):
                cpu->instructions[i] = stwu;
                break;
            case MAIN_INSTR_HASH(LWZ_OPCODE):
                cpu->instructions[i] = lwz;
                break;
            case MAIN_INSTR_HASH(LHZ_OPCODE):
                cpu->instructions[i] = lhz;
                break;
            case MAIN_INSTR_HASH(LWZU_OPCODE):
                cpu->instructions[i] = lwzu;
                break;
            case MAIN_INSTR_HASH(STMW_OPCODE):
                cpu->instructions[i] = stmw;
                break;
            case MAIN_INSTR_HASH(LMW_OPCODE):
                cpu->instructions[i] = lmw;
                break;
            /* float single load/store */
            case MAIN_INSTR_HASH(LFD_OPCODE):
                cpu->instructions[i] = lfd;
                break;
            /* paired single load/store */
            case MAIN_INSTR_HASH(PSQ_L_OPCODE):
                cpu->instructions[i] = psq_l;
                break;
            /* aggregate instructions */
            case 0b000100:
                cpu->instructions[i] = instr_000100;
                break;
            case 0b010011:
                cpu->instructions[i] = instr_010011;
                break;
            case 0b011111:
                cpu->instructions[i] = instr_011111;
                break;
            case 0b111111:
                cpu->instructions[i] = instr_111111;
                break;
            default:
                cpu->instructions[i] = unimplemented;
                break;
        }
    }
}
