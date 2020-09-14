#include "Gekko.h"

#include "../system.h"
#include "instructions.h"
#include "Registers/SPR.h"


void init_Gekko(s_Gekko* cpu) {
    cpu->IMMU.memory_ptr = cpu->DMMU.memory_ptr = cpu->system->memory;
    cpu->IMMU.system_ptr = cpu->DMMU.system_ptr = cpu->system;
    cpu->IMMU.SR_ptr = cpu->DMMU.SR_ptr = cpu->SR;
    cpu->IMMU.TBR_ptr = cpu->DMMU.TBR_ptr = &cpu->TBR;

    init_SPRs(cpu);
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
            case MAIN_INSTR_HASH(SC_OPCODE):
                cpu->instructions[i] = sc;
                break;
            /* integer arithmetic */
            case MAIN_INSTR_HASH(ADDI_OPCODE):
                cpu->instructions[i] = addi;
                break;
            case MAIN_INSTR_HASH(ADDIS_OPCODE):
                cpu->instructions[i] = addis;
                break;
            case MAIN_INSTR_HASH(ADDIC_OPCODE):
                cpu->instructions[i] = addic;
                break;
            case MAIN_INSTR_HASH(ADDIC__OPCODE):
                cpu->instructions[i] = addic_;
                break;
            case MAIN_INSTR_HASH(SUBFIC_OPCODE):
                cpu->instructions[i] = subfic;
                break;
            case MAIN_INSTR_HASH(MULLI_OPCODE):
                cpu->instructions[i] = mulli;
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
            case MAIN_INSTR_HASH(XORI_OPCODE):
                cpu->instructions[i] = xori;
                break;
            case MAIN_INSTR_HASH(XORIS_OPCODE):
                cpu->instructions[i] = xoris;
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
            case MAIN_INSTR_HASH(RLWIMI_OPCODE):
                cpu->instructions[i] = rlwimi;
                break;
            /* integer load/store */
            case MAIN_INSTR_HASH(STW_OPCODE):
                cpu->instructions[i] = stw;
                break;
            case MAIN_INSTR_HASH(STH_OPCODE):
                cpu->instructions[i] = sth;
                break;
            case MAIN_INSTR_HASH(STB_OPCODE):
                cpu->instructions[i] = stb;
                break;
            case MAIN_INSTR_HASH(STBU_OPCODE):
                cpu->instructions[i] = stbu;
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
            case MAIN_INSTR_HASH(LHA_OPCODE):
                cpu->instructions[i] = lha;
                break;
            case MAIN_INSTR_HASH(LBZ_OPCODE):
                cpu->instructions[i] = lbz;
                break;
            case MAIN_INSTR_HASH(LBZU_OPCODE):
                cpu->instructions[i] = lbzu;
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
            /* float load/store */
            case MAIN_INSTR_HASH(STFD_OPCODE):
                cpu->instructions[i] = stfd;
                break;
            case MAIN_INSTR_HASH(LFD_OPCODE):
                cpu->instructions[i] = lfd;
                break;
            case MAIN_INSTR_HASH(STFS_OPCODE):
                cpu->instructions[i] = stfs;
                break;
            case MAIN_INSTR_HASH(LFS_OPCODE):
                cpu->instructions[i] = lfs;
                break;
            /* paired single load/store */
            case MAIN_INSTR_HASH(PSQ_L_OPCODE):
                cpu->instructions[i] = psq_l;
                break;
            case MAIN_INSTR_HASH(PSQ_ST_OPCODE):
                cpu->instructions[i] = psq_st;
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
            case 0b111011:
                cpu->instructions[i] = instr_111011;
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
