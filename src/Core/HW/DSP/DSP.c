#include "DSP.h"

#include "log.h"
#include "DSP_memory.h"
#include "DSP_instructions.h"

#include <memory.h>
#include <stdio.h>


void init_DSP_table(s_DSP* DSP) {
    for (int i = 0; i < DSP_INSTR_TABLE_SIZE; i++) {
        switch (i) {
            case 0x00:
                DSP->instructions[i] = DSP_0000_0000;
                break;
            case 0x12:
                DSP->instructions[i] = DSP_SBSET;
                break;
            default:
                DSP->instructions[i] = DSP_unimplemented;
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

    // todo: strict aliasing?
    DSP->r[0] = &DSP->ar[0];
    DSP->r[1] = &DSP->ar[1];
    DSP->r[2] = &DSP->ar[2];
    DSP->r[3] = &DSP->ar[3];

    DSP->r[4 + 0] = &DSP->ix[0];
    DSP->r[4 + 1] = &DSP->ix[1];
    DSP->r[4 + 2] = &DSP->ix[2];
    DSP->r[4 + 3] = &DSP->ix[3];

    DSP->r[8 + 0] = &DSP->wr[0];
    DSP->r[8 + 1] = &DSP->wr[1];
    DSP->r[8 + 2] = &DSP->wr[2];
    DSP->r[8 + 3] = &DSP->wr[3];

    DSP->r[0xc + 0] = &DSP->st[0];
    DSP->r[0xc + 1] = &DSP->st[1];
    DSP->r[0xc + 2] = &DSP->st[2];
    DSP->r[0xc + 3] = &DSP->st[3];

    DSP->r[0x10 + 0] = (u16*)&DSP->ac[0] + 2;  // h
    DSP->r[0x10 + 1] = (u16*)&DSP->ac[1] + 2;  // h

    DSP->r[0x12] = (u16*)&DSP->config;
    DSP->r[0x13] = (u16*)&DSP->sr;

    DSP->r[0x14] = (u16*)&DSP->prod;
    DSP->r[0x15] = (u16*)&DSP->prod + 1;
    DSP->r[0x16] = (u16*)&DSP->prod + 2;
    DSP->r[0x17] = (u16*)&DSP->prod + 3;

    DSP->r[0x18 + 0] = (u16*)&DSP->ax[0];  // l
    DSP->r[0x18 + 1] = (u16*)&DSP->ax[1];  // l

    DSP->r[0x1a + 0] = (u16*)&DSP->ax[0] + 1;  // h
    DSP->r[0x1a + 1] = (u16*)&DSP->ax[1] + 1;  // h

    DSP->r[0x1c + 0] = (u16*)&DSP->ac[0];  // l
    DSP->r[0x1c + 1] = (u16*)&DSP->ac[1];  // l

    DSP->r[0x1e + 0] = (u16*)&DSP->ac[0] + 1;  // m
    DSP->r[0x1e + 1] = (u16*)&DSP->ac[1] + 1;  // m

    DSP->pc = 0x8000;  // start in IROM
}

void step_DSP(s_DSP* DSP) {
    u16 instr = DSP_read_imem(DSP, DSP->pc++);
    DSP->instructions[instr >> 8](DSP, instr);
}