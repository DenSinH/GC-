#include <stdio.h>
#include <stdlib.h>

#include "loader.h"

#include "default.h"
#include "log.h"


u8* load_IPL_file(const char *file_name) {
    FILE* file;
    fopen_s(&file, file_name, "rb");
    if (!file) log_fatal("Failed IPL file read");

    fseek(file, 0, SEEK_END);
    int file_size = ftell(file);
    rewind(file);

    if (file_size < IPL_ROM_END) {
        log_fatal("Invalid IPL file: too short");
    }

    fseek(file, IPL_HEADER_END, SEEK_SET);

    u8* IPL = malloc(IPL_DATA_LENGTH);

    fread(IPL, 1, IPL_ROM_END - IPL_HEADER_END, file);
    fclose(file);
    log_info("[IPL] Loaded %x bytes", file_size);

    return IPL;
}

u8* decrypt_IPL(const char file_name[]) {
    u8* IPL = load_IPL_file(file_name);

    /*
     * bootrom descrambler reversed by segher
     * Copyright 2008 Segher Boessenkool <segher@kernel.crashing.org>
     * */

    u8 acc = 0;
    u8 nacc = 0;

    u16 t = 0x2953;
    u16 u = 0xd9c2;
    u16 v = 0x3ff1;

    u8 x = 1;
    u32 it = 0;

    while (it < IPL_DATA_LENGTH) {
        int t0 = t & 1;
        int t1 = (t >> 1) & 1;
        int u0 = u & 1;
        int u1 = (u >> 1) & 1;
        int v0 = v & 1;

        x ^= t1 ^ v0;
        x ^= (u0 | u1);
        x ^= (t0 ^ u1 ^ v0) & (t0 ^ u0);

        if (t0 == u0)
        {
            v >>= 1;
            if (v0)
                v ^= 0xb3d0;
        }

        if (t0 == 0)
        {
            u >>= 1;
            if (u0)
                u ^= 0xfb10;
        }

        t >>= 1;
        if (t0)
            t ^= 0xa740;

        nacc++;
        acc = 2*acc + x;
        if (nacc == 8)
        {
            IPL[it++] ^= acc;
            nacc = 0;
        }
    }

    return IPL;
}


void dump_IPL(u8* IPL, const char file_name[]) {
    FILE* file;
    fopen_s(&file, file_name, "wb");

    unsigned int written_length = fwrite(IPL + IPL_CODE_START, 1, IPL_DATA_LENGTH - IPL_CODE_START, file);
    if (written_length != IPL_DATA_LENGTH - IPL_CODE_START) {
        log_fatal("Error writing to file: wrote %x bytes, should have written %x", written_length, IPL_DATA_LENGTH - IPL_CODE_START);
    }
    fclose(file);
}

void free_IPL(u8* IPL) {
    free(IPL);
}

void decrypt_IPL_to(const char file_name[], u8* target) {
    u8* IPL = decrypt_IPL(file_name);
    memcpy_s(target, IPL_DATA_LENGTH - IPL_CODE_START, IPL + IPL_CODE_START, IPL_DATA_LENGTH - IPL_CODE_START);
    free_IPL(IPL);
}