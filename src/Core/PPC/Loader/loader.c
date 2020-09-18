#include <stdio.h>
#include <stdlib.h>

#include "loader.h"
#include "helpers.h"

#include "default.h"
#include "log.h"

#ifndef _MSC_VER
#include <string.h>
#endif


u8* load_IPL_file(const char *file_name) {
    // open file
    FILE* file;
    FOPEN(&file, file_name, "rb");
    if (!file) log_fatal("Failed Loader file read");

    // find size
    fseek(file, 0, SEEK_END);
    size_t file_size = ftell(file);
    rewind(file);

    // checks
    if (file_size < IPL_ROM_END) {
        log_fatal("Invalid Loader file: too short");
    }

    // copy file into memory
    fseek(file, IPL_HEADER_END, SEEK_SET);

    u8* IPL = malloc(IPL_DATA_LENGTH);

    fread(IPL, 1, IPL_ROM_END - IPL_HEADER_END, file);
    fclose(file);
    log_info("[Loader] Loaded %x bytes", file_size);

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
    // dump decrypted IPL (from memory location) to file
    FILE* file;
    FOPEN(&file, file_name, "wb");

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
    // decrypt IPL file to a memory location
    u8* IPL = decrypt_IPL(file_name);
    memcpy(target, IPL + IPL_CODE_START, IPL_DATA_LENGTH - IPL_CODE_START);
    free_IPL(IPL);
}


u32 load_DOL_to(const char file_name[], u8* target) {
    // parse and load a DOL file to a memory location
    // todo: don't assume memory location >= 80000000
    // open the file
    FILE* file;
    FOPEN(&file, file_name, "rb");
    if (!file) log_fatal("Failed DOL file read");

    fseek(file, 0, SEEK_END);
    size_t file_size = ftell(file);
    rewind(file);

    u8* DOL = malloc(file_size);

    // read the file
    fread(DOL, 1, file_size, file);
    fclose(file);
    log_info("[DOL] read 0x%x bytes", file_size);

    // parse it
    s_DOLData data = parse_DOL_header(DOL);

    // copy the relevant sections
    for (int i = 0; i < 7; i++) {
        if (!data.TextSize[i]) continue;

        if (data.TextAddress[i] < 0x80000000 || data.TextAddress[i] >= 0x81800000) {
            log_fatal("[DOL] unimplemented text section offset: %08x (+ %08x)", data.TextAddress[i], data.TextSize[i]);
        }
        data.TextAddress[i] -= 0x80000000;
        log_info("[DOL] copying text section from %08x to %08x of length %08x", data.TextOffset[i], data.TextAddress[i], data.TextSize[i])

        memcpy(target + data.TextAddress[i], DOL + data.TextOffset[i], data.TextSize[i]);
    }

    for (int i = 0; i < 11; i++) {
        if (!data.DataSize[i]) continue;

        if (data.DataAddress[i] < 0x80000000 || data.DataAddress[i] + data.DataSize[i] >= 0x81800000) {
            log_fatal("[DOL] unimplemented data section offset: %08x (+ %08x)", data.DataAddress[i], data.DataSize[i]);
        }
        data.DataAddress[i] -= 0x80000000;
        log_info("[DOL] copying data section from %08x to %08x of length %08x", data.DataOffset[i], data.DataAddress[i], data.DataSize[i])

        memcpy(target + data.DataAddress[i], DOL + data.DataOffset[i], data.DataSize[i]);
    }

    // clear the BSS section
    if (data.BSSAddress < 0x80000000 || data.BSSAddress + data.BSSSize >= 0x81800000) {
        log_fatal("[DOL] unimplemented BSS section offset: %08x (+ %08x)", data.BSSAddress, data.BSSSize);
    }
    data.BSSAddress -= 0x80000000;
    log_info("[DOL] zeroing at %08x with length %08x", data.BSSAddress, data.BSSSize);
    memset(target + data.BSSAddress, 0, data.BSSSize);

    log_info("[DOL] entry point %08x", data.EntryPoint);
    return data.EntryPoint;
}