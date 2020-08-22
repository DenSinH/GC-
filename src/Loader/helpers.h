#ifndef GC__HELPERS_H
#define GC__HELPERS_H

#include "default.h"

#define IPL_HEADER_END 0x100
#define IPL_ROM_END 0x1AFF00
#define IPL_DATA_LENGTH IPL_ROM_END - IPL_HEADER_END
#define IPL_CODE_START 0x720  // offset corrected for missing IPL_HEADER

#define BIG_ENDIAN32(array, address) ((array[(address)] << 24) | (array[(address) + 1] << 16) | (array[(address) + 2] << 8) | (array[(address) + 3]))

typedef struct s_DOLData {
    u32 TextOffset[7];
    u32 DataOffset[11];
    u32 TextAddress[7];
    u32 DataAddress[11];
    u32 TextSize[7];
    u32 DataSize[11];
    u32 BSSAddress;
    u32 BSSSize;
    u32 EntryPoint;
} s_DOLData;

s_DOLData parse_DOL_header(u8* DOL);

#endif //GC__HELPERS_H
