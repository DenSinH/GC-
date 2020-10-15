#include "DSP.h"

#include "log.h"
#include "DSP_memory.h"
#include "DSP_instructions.h"
#include "DSP_extended_instructions.h"

#include <memory.h>
#include <stdio.h>


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

void init_DSP(s_DSP* DSP, const char* IROM_file, const char* DROM_file) {
    // open file
    FILE* file;

    size_t size[2] = { sizeof(DSP->IROM), sizeof(DSP->DROM) };
    const char* file_name[2] = { IROM_file, DROM_file };
    u8* dest[2] = { DSP->IROM, DSP->IRAM };

    for (int i = 0; i < 2; i++) {
        FOPEN(&file, file_name[i], "rb");
        if (!file) log_fatal("Failed reading %cROM_file (%s)", "ID"[i], file_name[i]);
        else {
            // find size
            fseek(file, 0, SEEK_END);
            size_t file_size = ftell(file);
            rewind(file);

            // checks
            if (file_size != size[i]) {
                log_fatal("Invalid %cROM file: expected %x bytes, got %x", "ID"[i], size[i], file_size);
            }

            // copy file into memory
            fseek(file, 0, SEEK_SET);

            fread(dest[i], 1, size[i], file);
            fclose(file);
            log_dsp("Loaded 0x%x bytes of %cROM", file_size, "ID"[i]);
        }
    }

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

    DSP->pc = 0x8000;  // start in IROM
}

void step_DSP(s_DSP* DSP) {
    u16 instr = DSP_read_imem(DSP, DSP->pc++);
    DSP->instructions[instr >> 8](DSP, instr);
}