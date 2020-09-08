#include <inttypes.h>

#include "MMU.h"

#include "default.h"
#include "log.h"

/*
 * I assume the basic "Theory 1" from https://dolphin-emu.org/blog/2016/09/06/booting-the-final-gc-game/
 * With this basic assumption, I only need to check if the address < 0xc800 0000 and I just mirror it down
 * */

u8 read8(s_MMU* mmu, u32 address) {
    mmu->TBR_ptr->raw++;

    if (address < 0xc1800000) {
        log_mmu("Read byte %02x from %08x", READ8(mmu->memory_ptr, MASK_24MB(address)), address);
        return READ8(mmu->memory_ptr, MASK_24MB(address));
    }

    switch (address >> 20) {
        case 0xcc0:
            log_hw_reg("Hardware register byte read access: %x", address);
            return HW_reg_read8(mmu->HW_regs_ptr, address);
        case 0xe00:
            log_fatal("L2 Cache access: %x", address);
        case 0xfff:
            log_fatal("IPL Chip access: %x", address);
        default:
            log_fatal("Unimplemented memory address: %x", address);
    }
}

u16 read16(s_MMU* mmu, u32 address) {
    mmu->TBR_ptr->raw++;

    if (address < 0xc1800000) {
        // remember: the GameCube is BIG ENDIAN!
        address = MASK_24MB(address);
        log_mmu("Read halfword %04x from %08x", READ16(mmu->memory_ptr, address), address);
        return READ16(mmu->memory_ptr, address);
    }

    switch (address >> 20) {
        case 0xcc0:
            log_hw_reg("Hardware register halfword read access: %x", address);
            return HW_reg_read16(mmu->HW_regs_ptr, address);
        case 0xe00:
            log_fatal("L2 Cache access: %x", address);
        case 0xfff:
            log_fatal("IPL Chip access: %x", address);
        default:
            log_fatal("Unimplemented memory address: %x", address);
    }
}

u32 read32(s_MMU* mmu, u32 address) {
    mmu->TBR_ptr->raw++;

    if (address < 0xc1800000) {
        // remember: the GameCube is BIG ENDIAN!
        address = MASK_24MB(address);
        log_mmu("Read word %08x from %08x", READ32(mmu->memory_ptr, address), address);
        return READ32(mmu->memory_ptr, address);
    }

    switch (address >> 20) {
        case 0xcc0:
            log_hw_reg("Hardware register word read access: %x", address);
            return HW_reg_read32(mmu->HW_regs_ptr, address);
        case 0xe00:
            log_fatal("L2 Cache access: %x", address);
        case 0xfff:
            log_fatal("IPL Chip access: %x", address);
        default:
            log_fatal("Unimplemented memory address: %x", address);
    }
}

u64 read64(s_MMU* mmu, u32 address) {
    mmu->TBR_ptr->raw++;

    if (address < 0xc1800000) {
        // remember: the GameCube is BIG ENDIAN!
        address = MASK_24MB(address);
        log_mmu("Read double word %016" PRIx64 " from %08x", READ64(mmu->memory_ptr, address), address);

        return READ64(mmu->memory_ptr, address);
    }

    switch (address >> 20) {
        case 0xcc0:
            log_hw_reg("Hardware register double word read access: %x", address);
            return HW_reg_read64(mmu->HW_regs_ptr, address);
        case 0xe00:
            log_fatal("L2 Cache access: %x", address);
        case 0xfff:
            log_fatal("IPL Chip access: %x", address);
        default:
            log_fatal("Unimplemented memory address: %x", address);
    }
}

void write8(s_MMU* mmu, u32 address, u8 value) {
    mmu->TBR_ptr->raw++;
    log_mmu("Write byte %02x to %08x", value, address);

    if (address < 0xc1800000) {
        WRITE8(mmu->memory_ptr, MASK_24MB(address), value);
        return;
    }

    switch (address >> 20) {
        case 0xcc0:
            log_hw_reg("Hardware register byte write access: %x (%02x)", address, value);
            HW_reg_write8(mmu->HW_regs_ptr, address, value);
            return;
        case 0xe00:
            log_fatal("L2 Cache access: %x", address);
        case 0xfff:
            log_fatal("IPL Chip access: %x", address);
        default:
            log_fatal("Unimplemented memory address: %x", address);
    }
}

void write16(s_MMU* mmu, u32 address, u16 value) {
    mmu->TBR_ptr->raw++;
    log_mmu("Write halfword %04x to %08x", value, address);

    if (address < 0xc1800000) {
        // remember: the GameCube is BIG ENDIAN!
        address = MASK_24MB(address);
        WRITE16(mmu->memory_ptr, address, value);
        return;
    }

    switch (address >> 20) {
        case 0xcc0:
            log_hw_reg("Hardware register halfword write access: %x (%04x)", address, value);
            HW_reg_write16(mmu->HW_regs_ptr, address, value);
            return;
        case 0xe00:
            log_fatal("L2 Cache access: %x", address);
        case 0xfff:
            log_fatal("IPL Chip access: %x", address);
        default:
            log_fatal("Unimplemented memory address: %x", address);
    }
}

void write32(s_MMU* mmu, u32 address, u32 value) {
    mmu->TBR_ptr->raw++;
    log_mmu("Write word %08x to %08x", value, address);

    if (address < 0xc1800000) {
        // remember: the GameCube is BIG ENDIAN!
        address = MASK_24MB(address);
        WRITE32(mmu->memory_ptr, address, value);
        return;
    }

    switch (address >> 20) {
        case 0xcc0:
            log_hw_reg("Hardware register word write access: %x (%08x)", address, value);
            HW_reg_write32(mmu->HW_regs_ptr, address, value);
            return;
        case 0xe00:
            log_fatal("L2 Cache access: %x", address);
        case 0xfff:
            log_fatal("IPL Chip access: %x", address);
        default:
            log_fatal("Unimplemented memory address: %x", address);
    }
}

void write64(s_MMU* mmu, u32 address, u64 value) {
    mmu->TBR_ptr->raw++;
    log_mmu("Write word %016" PRIx64 " to %08x", value, address);

    if (address < 0xc1800000) {
        // remember: the GameCube is BIG ENDIAN!
        address = MASK_24MB(address);
        WRITE64(mmu->memory_ptr, address, value);
        return;
    }

    switch (address >> 20) {
        case 0xcc0:
            log_hw_reg("Hardware register double word write access: %x (%016llx)", address, value);
            HW_reg_write64(mmu->HW_regs_ptr, address, value);
            return;
        case 0xe00:
            log_fatal("L2 Cache access: %x", address);
        case 0xfff:
            log_fatal("IPL Chip access: %x", address);
        default:
            log_fatal("Unimplemented memory address: %x", address);
    }
}

u32 get_word(s_MMU* mmu, u32 address) {
    return READ32(mmu->memory_ptr, MASK_24MB(address));
}

void dump_range(s_MMU* mmu, u32 start, u32 end) {
    for (u32 i = start; i < end; i += 4) {
        if (!(i & 0xf)) {
            printf("\n[%08x] ", i);
        }
        printf("%08x ", READ32(mmu->memory_ptr, MASK_24MB(i)));
    }
}