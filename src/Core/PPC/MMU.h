#ifndef GC__MMU_H
#define GC__MMU_H

#include "Registers/hardware_registers/hardware_registers.h"
#include "Registers/TBR.h"

#include "default.h"
#include "core_utils.h"
#include "flags.h"

#define MASK_24MB(_address) (((_address) & 0x01000000) | ((_address) & 0x7fffff))

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
 * the Instruction MMU SR_ptr's "Shadow" the Data MMU's SRs
 * */
typedef struct s_MMU {
    struct s_GameCube* system_ptr;
    u32* SR_ptr;      // Segment Registers          [SUPERVISOR]
    u64 BAT[8];       // Batch Address Translation  [SUPERVISOR]
    u8* memory_ptr;
    s_TBR* TBR_ptr;
    s_hardware_registers* HW_regs_ptr;
} s_MMU;

u8 read8(s_MMU* mmu, u32 address);
u16 read16(s_MMU* mmu, u32 address);
u32 read32(s_MMU* mmu, u32 address);
u64 read64(s_MMU* mmu, u32 address);

void write8(s_MMU* mmu, u32 address, u8 value);
void write16(s_MMU* mmu, u32 address, u16 value);
void write32(s_MMU* mmu, u32 address, u32 value);
void write64(s_MMU* mmu, u32 address, u64 value);

u32 get_word(s_MMU* mmu, u32 address);
void dump_range(s_MMU* mmu, u32 start, u32 end);

#endif //GC__MMU_H
