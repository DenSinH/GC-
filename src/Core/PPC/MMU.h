#ifndef GC__MMU_H
#define GC__MMU_H

#include "default.h"
#include "registers/hardware_registers.h"

#include "flags.h"

#define MASK_24MB(_address) ((_address & 0x01000000) | (_address & 0x7fffff))

#ifdef CHECK_HR_ACCESS

    #define ASSERT_HR_ACCESS(section, index) \
        if (mmu->HW_regs_ptr->pointers[section] == NULL || (index) > hardware_register_block_size[section]) {   \
            log_fatal("Invalid hardware register access: %08x", address);                                     \
        }
#else
    #define ASSERT_HR_ACCESS(section, index)
#endif

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
    u32* SR_ptr;      // Segment registers          [SUPERVISOR]
    u64 BAT[8];       // Batch Address Translation  [SUPERVISOR]
    u8* RAM_ptr;
    u64* TBR_ptr;
    s_hardware_registers* HW_regs_ptr;
} s_MMU;

#define READ8(array, address) array[(address)]
#define READ16(array, address) (array[(address)] << 8) | array[(address) + 1]
#define READ32(array, address)      \
    (array[(address)] << 24)     | \
    (array[(address) + 1] << 16) | \
    (array[(address) + 2] << 8)  | \
    array[(address) + 3]

#define READ64(array, address)           \
    ((u64)array[(address)] << 56)     | \
    ((u64)array[(address) + 1] << 48) | \
    ((u64)array[(address) + 2] << 40) | \
    ((u64)array[(address) + 3] << 32) | \
    ((u64)array[(address) + 4] << 24) | \
    ((u64)array[(address) + 5] << 16) | \
    ((u64)array[(address) + 6] << 8)  | \
    (u64)array[(address) + 7]

#define WRITE8(array, address, value) array[address] = (value)
#define WRITE16(array, address, value)       \
    array[(address)] = (value) >> 8;         \
    array[(address) + 1] = (value) & 0xff
#define WRITE32(array, address, value)              \
    array[(address)] = (value) >> 24;               \
    array[(address) + 1] = ((value) >> 16) & 0xff;  \
    array[(address) + 2] = ((value) >> 8) & 0xff;   \
    array[(address) + 3] = (value) & 0xff
#define WRITE64(array, address, value)              \
    array[(address)] = (value) >> 56;               \
    array[(address) + 1] = ((value) >> 48) & 0xff;  \
    array[(address) + 2] = ((value) >> 40) & 0xff;  \
    array[(address) + 3] = ((value) >> 32) & 0xff;  \
    array[(address) + 4] = ((value) >> 24) & 0xff;  \
    array[(address) + 5] = ((value) >> 16) & 0xff;  \
    array[(address) + 6] = ((value) >> 8) & 0xff;   \
    array[(address) + 7] = (value) & 0xff

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
