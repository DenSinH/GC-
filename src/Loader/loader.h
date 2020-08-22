#ifndef GC__LOADER_H
#define GC__LOADER_H

#include "default.h"

void decrypt_IPL_to(const char file_name[], u8* target);
// returns entry point:
u32 load_DOL_to(const char file_name[], u8* target);

#endif //GC__LOADER_H
