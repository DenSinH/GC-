#include "helpers.h"


s_DOLData parse_DOL_header(u8* DOL) {
    // info on DOL format: https://wiibrew.org/wiki/DOL
    s_DOLData data;
    for (int i = 0; i < 7; i++) {
        data.TextOffset[i] = BIG_ENDIAN32(DOL, 4  * i);
        data.TextAddress[i] = BIG_ENDIAN32(DOL, 0x48 + 4  * i);
        data.TextSize[i] = BIG_ENDIAN32(DOL, 0x90 + 4  * i);
    }

    for (int i = 0; i < 11; i++) {
        data.DataOffset[i] = BIG_ENDIAN32(DOL, 0x1c + 4  * i);
        data.DataAddress[i] = BIG_ENDIAN32(DOL, 0x64 + 4  * i);
        data.DataSize[i] = BIG_ENDIAN32(DOL, 0xac + 4  * i);
    }

    data.BSSAddress = BIG_ENDIAN32(DOL, 0xd8);
    data.BSSSize = BIG_ENDIAN32(DOL, 0xdc);
    data.EntryPoint = BIG_ENDIAN32(DOL, 0xe0);
    return data;
}
