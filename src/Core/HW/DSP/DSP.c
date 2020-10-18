#include "DSP.h"

#include "log.h"
#include "DSP_memory.h"
#include "DSP_instructions.h"
#include "DSP_extended_instructions.h"
#include "DSP_util.h"

#include "files/dsp_coef.h"
#include "files/dsp_rom.h"

#include <memory.h>
#include "default.h"


void init_DSP_table(s_DSP* DSP) {
    for (int i = 0; i < DSP_INSTR_TABLE_SIZE; i++) {
        switch (i) {
            case 0x00:
                DSP->instructions[i] = DSP_0000_0000;
                break;
            case 0x02:
            case 0x03:
                DSP->instructions[i] = DSP_0000_001r;
                break;
            case 0x12:
                DSP->instructions[i] = DSP_SBSET;
                break;
            case 0x16:
                DSP->instructions[i] = DSP_SI;
                break;
            case 0x18:
            case 0x19:
                DSP->instructions[i] = DSP_LRRx;
                break;
            case 0x1a:
            case 0x1b:
                DSP->instructions[i] = DSP_SRRx;
                break;
            case 0x1c ... 0x1f:
                DSP->instructions[i] = DSP_MRR;
                break;
            case 0x20 ... 0x27:
                DSP->instructions[i] = DSP_LRS;
                break;
            case 0x28 ... 0x2f:
                DSP->instructions[i] = DSP_SRS;
                break;
            case 0x82:
                DSP->instructions[i] = DSP_CMP;
                break;
            case 0x81:
            case 0x89:
                DSP->instructions[i] = DSP_CLR;
                break;
            case 0x8a:
                DSP->instructions[i] = DSP_M0;
                break;
            case 0x8b:
                DSP->instructions[i] = DSP_M2;
                break;
            case 0x8c:
                DSP->instructions[i] = DSP_CLR15;
                break;
            case 0x8d:
                DSP->instructions[i] = DSP_SET15;
                break;
            case 0x8e:
                DSP->instructions[i] = DSP_SET16;
                break;
            case 0x8f:
                DSP->instructions[i] = DSP_SET40;
                break;
            default:
                DSP->instructions[i] = DSP_unimplemented;
                break;
        }
    }
}

void init_DSP_ext_table(s_DSP* DSP) {
    for (int i = 0; i < DSP_EXT_INSTR_TABLE_SIZE; i++) {
        switch (i >> 4) {
            case 0b0000:
                switch ((i & 0xc) >> 2) {
                    case 0b00:
                        // actually only 0000 0000 ?
                        DSP->ext_instructions[i] = DSP_EXT_NOP;
                        break;
                    case 0b01:
                        DSP->ext_instructions[i] = DSP_EXT_DR;
                        break;
                    case 0b10:
                        DSP->ext_instructions[i] = DSP_EXT_IR;
                        break;
                    case 0b11:
                        DSP->ext_instructions[i] = DSP_EXT_NR;
                        break;
                    default:
                        log_fatal("What?");
                }
                break;
            case 0b0001:
                DSP->ext_instructions[i] = DSP_EXT_MV;
                break;
            case 0b0010:
            case 0b0011:
                if (i & 0x04) {
                    DSP->ext_instructions[i] = DSP_EXT_SN;
                }
                else {
                    DSP->ext_instructions[i] = DSP_EXT_S;
                }
                break;
            case 0b0100 ... 0b0111:
                if (i & 0x04) {
                    DSP->ext_instructions[i] = DSP_EXT_LN;
                }
                else {
                    DSP->ext_instructions[i] = DSP_EXT_L;
                }
                break;
            case 0b1000 ... 0b1011:
                switch (i & 0x0e) {
                    case 0b0000:
                        DSP->ext_instructions[i] = DSP_EXT_LS;
                        break;
                    case 0b0100:
                        DSP->ext_instructions[i] = DSP_EXT_LSN;
                        break;
                    case 0b1000:
                        DSP->ext_instructions[i] = DSP_EXT_LSM;
                        break;
                    case 0b1100:
                        DSP->ext_instructions[i] = DSP_EXT_LSNM;
                        break;
                    case 0b0010:
                        DSP->ext_instructions[i] = DSP_EXT_SL;
                        break;
                    case 0b0110:
                        DSP->ext_instructions[i] = DSP_EXT_SLN;
                        break;
                    case 0b1010:
                        DSP->ext_instructions[i] = DSP_EXT_SLM;
                        break;
                    case 0b1110:
                        DSP->ext_instructions[i] = DSP_EXT_SLNM;
                        break;
                    default:
                        log_fatal("Excuse me?");
                }
                break;
            case 0b1100 ... 0b1111:
                switch (i & 0x0c) {
                    case 0b0000:
                        if ((i & 3) == 3) {
                            DSP->ext_instructions[i] = DSP_EXT_LDAX;
                        }
                        else {
                            DSP->ext_instructions[i] = DSP_EXT_LD;
                        }
                        break;
                    case 0b0100:
                        if ((i & 3) == 3) {
                            DSP->ext_instructions[i] = DSP_EXT_LDAXN;
                        }
                        else {
                            DSP->ext_instructions[i] = DSP_EXT_LDN;
                        }
                        break;
                    case 0b1000:
                        if ((i & 3) == 3) {
                            DSP->ext_instructions[i] = DSP_EXT_LDAXM;
                        }
                        else {
                            DSP->ext_instructions[i] = DSP_EXT_LDM;
                        }
                        break;
                    case 0b1100:
                        if ((i & 3) == 3) {
                            DSP->ext_instructions[i] = DSP_EXT_LDAXNM;
                        }
                        else {
                            DSP->ext_instructions[i] = DSP_EXT_LDNM;
                        }
                        break;
                    default:
                        log_fatal("Pfft");
                }
                break;
            default:
                DSP->ext_instructions[i] = DSP_EXT_INVALID;
                break;
        }
    }
}

void init_DSP(s_DSP* DSP) {
    memcpy(DSP->IROM, dsp_rom,  sizeof(DSP->IROM));
    log_dsp("Loaded 0x%x bytes of IROM", sizeof(DSP->IROM));
    memcpy(DSP->DROM, dsp_coef,  sizeof(DSP->DROM));
    log_dsp("Loaded 0x%x bytes of DROM", sizeof(DSP->DROM));

    init_DSP_table(DSP);
    init_DSP_ext_table(DSP);

    // todo: strict aliasing?
    DSP->r[DSP_reg_ar0] = &DSP->ar[0];
    DSP->r[DSP_reg_ar1] = &DSP->ar[1];
    DSP->r[DSP_reg_ar2] = &DSP->ar[2];
    DSP->r[DSP_reg_ar3] = &DSP->ar[3];

    DSP->r[DSP_reg_ix0] = &DSP->ix[0];
    DSP->r[DSP_reg_ix1] = &DSP->ix[1];
    DSP->r[DSP_reg_ix2] = &DSP->ix[2];
    DSP->r[DSP_reg_ix3] = &DSP->ix[3];

    DSP->r[DSP_reg_wr0] = &DSP->wr[0];
    DSP->r[DSP_reg_wr1] = &DSP->wr[1];
    DSP->r[DSP_reg_wr2] = &DSP->wr[2];
    DSP->r[DSP_reg_wr3] = &DSP->wr[3];

    DSP->r[DSP_reg_st0] = &DSP->st[0];
    DSP->r[DSP_reg_st1] = &DSP->st[1];
    DSP->r[DSP_reg_st2] = &DSP->st[2];
    DSP->r[DSP_reg_st3] = &DSP->st[3];

    DSP->r[DSP_reg_ac0h] = (u16*)&DSP->ac[0] + 2;  // h
    DSP->r[DSP_reg_ac1h] = (u16*)&DSP->ac[1] + 2;  // h

    DSP->r[DSP_reg_config] = (u16*)&DSP->config;
    DSP->r[DSP_reg_sr] = (u16*)&DSP->sr;

    DSP->r[DSP_reg_prodl] = (u16*)&DSP->prod;
    DSP->r[DSP_reg_prodm] = (u16*)&DSP->prod + 1;
    DSP->r[DSP_reg_prodh] = (u16*)&DSP->prod + 2;
    DSP->r[DSP_reg_prodm2] = (u16*)&DSP->prod + 3;

    DSP->r[DSP_reg_ax0l] = (u16*)&DSP->ax[0];  // l
    DSP->r[DSP_reg_ax1l] = (u16*)&DSP->ax[1];  // l

    DSP->r[DSP_reg_ax0h] = (u16*)&DSP->ax[0] + 1;  // h
    DSP->r[DSP_reg_ax1h] = (u16*)&DSP->ax[1] + 1;  // h

    DSP->r[DSP_reg_ac0l] = (u16*)&DSP->ac[0];  // l
    DSP->r[DSP_reg_ac1l] = (u16*)&DSP->ac[1];  // l

    DSP->r[DSP_reg_ac0m] = (u16*)&DSP->ac[0] + 1;  // m
    DSP->r[DSP_reg_ac1m] = (u16*)&DSP->ac[1] + 1;  // m

    // todo: what does the exception vector region expect the RTI to do?
    DSP_PUSH_STACK(DSP, 0, 0x8000);  // IROM start
    DSP_PUSH_STACK(DSP, 1, 0x1e00);  // bits set by reset routine
}

int step_DSP(s_DSP* DSP) {
    // return the amount of cycles an instruction took
    // this allows us to have "higher order instructions", like BLOOP(I)

    if (!DSP->started) {
        return 1;
    }

    u16 instr = DSP_read_imem(DSP, DSP->pc++);
    return DSP->instructions[instr >> 8](DSP, instr);
}

void DSP_interrupt(s_DSP* DSP, e_DSP_int interrupt) {
    DSP->config &= ~DSP_CR_HALT;
    DSP_PUSH_STACK(DSP, 0, DSP->pc);
    DSP_PUSH_STACK(DSP, 1, DSP->sr.raw);
    DSP->pc = interrupt;
}

bool DSP_halted(s_DSP* DSP) {
    return DSP->halted;
}

void halt_DSP(s_DSP* DSP) {
    DSP->halted = true;
}

void unhalt_DSP(s_DSP* DSP) {
    DSP->halted = false;
}