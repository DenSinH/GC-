#include "Gekko.h"
#include "instructions.h"

void init_Gekko(s_Gekko* cpu) {
    cpu->IMMU.RAM = cpu->DMMU.RAM = cpu->memory;

    cpu->PC = 0x81300000;

    // todo: full map
    cpu->SPR[SPR_XER] = (u32*)&cpu->XER;
    cpu->SPR[SPR_LR] = &cpu->LR;
    cpu->SPR[SPR_CTR] = &cpu->CTR;
    cpu->SPR[SPR_GQR0] = (u32*)&cpu->GQR[0];
    cpu->SPR[SPR_GQR1] = (u32*)&cpu->GQR[1];
    cpu->SPR[SPR_GQR2] = (u32*)&cpu->GQR[2];
    cpu->SPR[SPR_GQR3] = (u32*)&cpu->GQR[3];
    cpu->SPR[SPR_GQR4] = (u32*)&cpu->GQR[4];
    cpu->SPR[SPR_GQR5] = (u32*)&cpu->GQR[5];
    cpu->SPR[SPR_GQR6] = (u32*)&cpu->GQR[6];
    cpu->SPR[SPR_GQR7] = (u32*)&cpu->GQR[7];
    cpu->SPR[SPR_HID0] = &cpu->HID[0];
    cpu->SPR[SPR_HID1] = &cpu->HID[1];
    cpu->SPR[SPR_HID2] = &cpu->HID[2];

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
            /* integer rotate and shift */
            case MAIN_INSTR_HASH(RLWINM_OPCODE):
                cpu->instructions[i] = rlwinm;
                break;
            /* integer load/store */
            case MAIN_INSTR_HASH(STW_OPCODE):
                cpu->instructions[i] = stw;
                break;
            case MAIN_INSTR_HASH(STWU_OPCODE):
                cpu->instructions[i] = stwu;
                break;
            case MAIN_INSTR_HASH(LWZ_OPCODE):
                cpu->instructions[i] = lwz;
                break;
            /* paired single load/store */
            case MAIN_INSTR_HASH(PSQ_L_OPCODE):
                cpu->instructions[i] = psq_l;
                break;
            /* aggregate instructions */
            case 0b010011:
                cpu->instructions[i] = instr_010011;
                break;
            case 0b011111:
                cpu->instructions[i] = instr_011111;
                break;
            default:
                cpu->instructions[i] = unimplemented;
                break;
        }
    }
}
