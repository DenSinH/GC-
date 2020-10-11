#include "DSP.h"

#include "log.h"

#include <memory.h>
#include <stdio.h>


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
}