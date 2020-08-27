#include <inttypes.h>

#include "MMU.h"

#include "default.h"
#include "log.h"

#define MASK_24MB(_address) ((_address & 0x01000000) | (_address & 0x7fffff))
/*
 * I assume the basic "Theory 1" from https://dolphin-emu.org/blog/2016/09/06/booting-the-final-gc-game/
 * With this basic assumption, I only need to check if the address < 0xc800 0000 and I just mirror it down
 * */
u32 stubber = 0xffffffff;

u8 read8(s_MMU* mmu, u32 address) {
    (*mmu->TBR_ptr)++;
    switch (address >> 20) {
        case 0x000 ... 0x017:
        case 0x800 ... 0x817:
        case 0xc00 ... 0xc17:
            log_mmu("Read byte %02x from %08x", READ8(mmu->RAM_ptr, MASK_24MB(address)), address);
            return READ8(mmu->RAM_ptr, MASK_24MB(address));
        case 0xcc0:
            log_warn("Hardware register byte read access: %x", address);
            ASSERT_HR_ACCESS(HR_INDEX_FROM_ADDRESS(address), address & 0xfff)
            u8* HW_regs_section = mmu->HW_regs_ptr->pointers[HR_INDEX_FROM_ADDRESS(address)];
            return READ8(HW_regs_section, address & 0xfff);
        case 0xe00:
            log_fatal("L2 Cache access: %x", address);
        case 0xfff:
            log_fatal("IPL Chip access: %x", address);
        default:
            log_fatal("Unimplemented memory address: %x", address);
    }
}

u16 read16(s_MMU* mmu, u32 address) {
    (*mmu->TBR_ptr)++;
    switch (address >> 20) {
        case 0x000 ... 0x017:
        case 0x800 ... 0x817:
        case 0xc00 ... 0xc17:
            // remember: the GameCube is BIG ENDIAN!
            address = MASK_24MB(address);
            log_mmu("Read halfword %04x from %08x", READ16(mmu->RAM_ptr, address), address);
            return READ16(mmu->RAM_ptr, address);
        case 0xcc0:
            log_warn("Hardware register halfword read access: %x", address);
            if (address == 0xcc00500a) {
                // todo: implement
                // for now: prevent infinite poll (reset "reset" bit)
                u8* HW_regs_section = mmu->HW_regs_ptr->pointers[HR_INDEX_FROM_ADDRESS(address)];
                return (READ16(HW_regs_section, address & 0xfff)) & 0xffe;
            }
            else if (address == 0xcc005004 || address == 0xcc005006) {
                // todo: implement
                // return finished status (bit 0 (31 LE)) to prevent polling
                stubber ^= 0xffffffff;
                return (u16)stubber;
            }
            ASSERT_HR_ACCESS(HR_INDEX_FROM_ADDRESS(address), address & 0xfff)
            u8* HW_regs_section = mmu->HW_regs_ptr->pointers[HR_INDEX_FROM_ADDRESS(address)];
            return READ16(HW_regs_section, address & 0xfff);
        case 0xe00:
            log_fatal("L2 Cache access: %x", address);
        case 0xfff:
            log_fatal("IPL Chip access: %x", address);
        default:
            log_fatal("Unimplemented memory address: %x", address);
    }
}

u32 read32(s_MMU* mmu, u32 address) {
    (*mmu->TBR_ptr)++;
    switch (address >> 20) {
        case 0x000 ... 0x017:
        case 0x800 ... 0x817:
        case 0xc00 ... 0xc17:
            // remember: the GameCube is BIG ENDIAN!
            address = MASK_24MB(address);
            log_mmu("Read word %08x from %08x", READ32(mmu->RAM_ptr, address), address);
            return READ32(mmu->RAM_ptr, address);
        case 0xcc0:
            log_warn("Hardware register word read access: %x", address);
            if (address == 0xcc00680c) {
                // todo: add EXI devices
                // exit(1);
                return READ32(mmu->HW_regs_ptr->DI_SI_EXI_Streaming, 0x80c) & 0xfffffffe;
            }
            ASSERT_HR_ACCESS(HR_INDEX_FROM_ADDRESS(address), address & 0xfff)
            u8* HW_regs_section = mmu->HW_regs_ptr->pointers[HR_INDEX_FROM_ADDRESS(address)];
            return READ32(HW_regs_section, address & 0xfff);
        case 0xe00:
            log_fatal("L2 Cache access: %x", address);
        case 0xfff:
            log_fatal("IPL Chip access: %x", address);
        default:
            log_fatal("Unimplemented memory address: %x", address);
    }
}

u64 read64(s_MMU* mmu, u32 address) {
    (*mmu->TBR_ptr)++;
    switch (address >> 20) {
        case 0x000 ... 0x017:
        case 0x800 ... 0x817:
        case 0xc00 ... 0xc17:
            // remember: the GameCube is BIG ENDIAN!
            // remember: the GameCube is BIG ENDIAN!
            address = MASK_24MB(address);
            log_mmu("Read double word %016" PRIx64 " from %08x", READ64(mmu->RAM_ptr, address), address);

            return READ64(mmu->RAM_ptr, address);
        case 0xcc0:
            log_warn("Hardware register double word read access: %x", address);
            ASSERT_HR_ACCESS(HR_INDEX_FROM_ADDRESS(address), address & 0xfff)
            u8* HW_regs_section = mmu->HW_regs_ptr->pointers[HR_INDEX_FROM_ADDRESS(address)];
            return READ64(HW_regs_section, address & 0xfff);
        case 0xe00:
            log_fatal("L2 Cache access: %x", address);
        case 0xfff:
            log_fatal("IPL Chip access: %x", address);
        default:
            log_fatal("Unimplemented memory address: %x", address);
    }
}

void write8(s_MMU* mmu, u32 address, u8 value) {
    (*mmu->TBR_ptr)++;
    log_mmu("Write byte %02x to %08x", value, address);
    switch (address >> 20) {
        case 0x000 ... 0x017:
        case 0x800 ... 0x817:
        case 0xc00 ... 0xc17:
            WRITE8(mmu->RAM_ptr, MASK_24MB(address), value);
            return;
        case 0xcc0:
            log_warn("Hardware register byte write access: %x (%02x)", address, value);
            ASSERT_HR_ACCESS(HR_INDEX_FROM_ADDRESS(address), address & 0xfff)
            u8* HW_regs_section = mmu->HW_regs_ptr->pointers[HR_INDEX_FROM_ADDRESS(address)];
            WRITE8(HW_regs_section, address & 0xfff, value);
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
    (*mmu->TBR_ptr)++;
    log_mmu("Write halfword %04x to %08x", value, address);
    switch (address >> 20) {
        case 0x000 ... 0x017:
        case 0x800 ... 0x817:
        case 0xc00 ... 0xc17:
            // remember: the GameCube is BIG ENDIAN!
            address = MASK_24MB(address);
            WRITE16(mmu->RAM_ptr, address, value);
            return;
        case 0xcc0:
            log_warn("Hardware register halfword write access: %x (%04x)", address, value);
            ASSERT_HR_ACCESS(HR_INDEX_FROM_ADDRESS(address), address & 0xfff)
            u8* HW_regs_section = mmu->HW_regs_ptr->pointers[HR_INDEX_FROM_ADDRESS(address)];
            WRITE16(HW_regs_section, address & 0xfff, value);
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
    (*mmu->TBR_ptr)++;
    log_mmu("Write word %08x to %08x", value, address);
    switch (address >> 20) {
        case 0x000 ... 0x017:
        case 0x800 ... 0x817:
        case 0xc00 ... 0xc17:
            // remember: the GameCube is BIG ENDIAN!
            address = MASK_24MB(address);
            WRITE32(mmu->RAM_ptr, address, value);
            return;
        case 0xcc0:
            log_warn("Hardware register word write access: %x (%08x)", address, value);
            ASSERT_HR_ACCESS(HR_INDEX_FROM_ADDRESS(address), address & 0xfff)
            u8* HW_regs_section = mmu->HW_regs_ptr->pointers[HR_INDEX_FROM_ADDRESS(address)];
            WRITE32(HW_regs_section, address & 0xfff, value);
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
    (*mmu->TBR_ptr)++;
    log_mmu("Write word %016" PRIx64 " to %08x", value, address);
    switch (address >> 20) {
        case 0x000 ... 0x017:
        case 0x800 ... 0x817:
        case 0xc00 ... 0xc17:
            // remember: the GameCube is BIG ENDIAN!
            address = MASK_24MB(address);
            WRITE64(mmu->RAM_ptr, address, value);
            return;
        case 0xcc0:
            log_warn("Hardware register double word write access: %x (%016llx)", address, value);
            ASSERT_HR_ACCESS(HR_INDEX_FROM_ADDRESS(address), address & 0xfff)
            u8* HW_regs_section = mmu->HW_regs_ptr->pointers[HR_INDEX_FROM_ADDRESS(address)];
            WRITE64(HW_regs_section, address & 0xfff, value);
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
    return READ32(mmu->RAM_ptr, MASK_24MB(address));
}

void dump_range(s_MMU* mmu, u32 start, u32 end) {
    for (u32 i = start; i < end; i += 4) {
        if (!(i & 0xf)) {
            printf("\n[%08x] ", i);
        }
        printf("%08x ", READ32(mmu->RAM_ptr, MASK_24MB(i)));
    }
}