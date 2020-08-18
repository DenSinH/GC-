#ifndef GC__MMU_H
#define GC__MMU_H

#include "default.h"

/*
 * There are 2 MMU's, an instruction MMU, and a data MMU
 * the Instruction MMU SR's "Shadow" the Data MMU's SRs
 * */
typedef struct s_MMU {
    u32 SR[16];   // Segment registers          [SUPERVISOR]
    u64 BAT[8];   // Batch Address Translation  [SUPERVISOR]
    u8* RAM;
} s_MMU;

u8 read8(s_MMU* mmu, u32 address);
u16 read16(s_MMU* mmu, u32 address);
u32 read32(s_MMU* mmu, u32 address);

void write8(s_MMU* mmu, u32 address, u8 value);
void write16(s_MMU* mmu, u32 address, u16 value);
void write32(s_MMU* mmu, u32 address, u32 value);

#endif //GC__MMU_H
