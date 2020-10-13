#ifndef GC__DSP_MEMORY_H
#define GC__DSP_MEMORY_H

#include "DSP.h"

#include "default.h"

u16 DSP_read_imem(s_DSP* DSP, u16 address);
void DSP_write_imem(s_DSP* DSP, u16 address, u16 value);
u16 DSP_read_dmem(s_DSP* DSP, u16 address);
void DSP_write_dmem(s_DSP* DSP, u16 address, u16 value);

#endif //GC__DSP_MEMORY_H
