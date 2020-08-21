#ifndef GC__LOADER_H
#define GC__LOADER_H

#include "default.h"

#define IPL_HEADER_END 0x100
#define IPL_ROM_END 0x1AFF00
#define IPL_DATA_LENGTH IPL_ROM_END - IPL_HEADER_END
#define IPL_CODE_START 0x720  // offset corrected for missing IPL_HEADER

u8* decrypt_IPL(const char file_name[]);
void decrypt_IPL_to(const char file_name[], u8* target);
void load_DOL_to(const char file_name[], u8* target);
void dump_IPL(u8* IPL, const char file_name[]);
void free_IPL(u8* IPL);

#endif //GC__LOADER_H
