#ifndef GC__MMU_H
#define GC__MMU_H

#include "default.h"

typedef enum {
    BAT0L = 0,
    BAT0U = 1,
    BAT1L = 2,
    BAT1U = 3,
    BAT2L = 4,
    BAT2U = 5,
    BAT3L = 6,
    BAT3U = 7,
} e_BAT;

/*
 * There are 2 MMU's, an instruction MMU, and a data MMU
 * the Instruction MMU SR's "Shadow" the Data MMU's SRs
 * */
typedef struct s_MMU {
    u32* SR;      // Segment registers          [SUPERVISOR]
    u64 BAT[8];   // Batch Address Translation  [SUPERVISOR]
    u8* RAM;
} s_MMU;

u8 read8(s_MMU* mmu, u32 address);
u16 read16(s_MMU* mmu, u32 address);
u32 read32(s_MMU* mmu, u32 address);
u64 read64(s_MMU* mmu, u32 address);

void write8(s_MMU* mmu, u32 address, u8 value);
void write16(s_MMU* mmu, u32 address, u16 value);
void write32(s_MMU* mmu, u32 address, u32 value);
void write64(s_MMU* mmu, u32 address, u64 value);

#endif //GC__MMU_H
