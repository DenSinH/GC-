#include <inttypes.h>

#include "MMU.h"

#include "default.h"
#include "log.h"

#define MASK_24MB(_address) ((_address & 0x01000000) | (_address & 0x7fffff))
/*
 * I assume the basic "Theory 1" from https://dolphin-emu.org/blog/2016/09/06/booting-the-final-gc-game/
 * With this basic assumption, I only need to check if the address < 0xc800 0000 and I just mirror it down
 * */

u8 read8(s_MMU* mmu, u32 address) {
    if (address < 0xc8000000) {
        log_mmu("Read byte %02x from %08x", mmu->RAM[MASK_24MB(address)], address);
        return mmu->RAM[MASK_24MB(address)];
    }
    else {
        log_fatal("Unimplemented memory address: %x", address);
    }
}

u16 read16(s_MMU* mmu, u32 address) {
    if (address < 0xc8000000) {
        // remember: the GameCube is BIG ENDIAN!
        address = MASK_24MB(address);
        log_mmu("Read halfword %04x from %08x", (mmu->RAM[address] << 8) | mmu->RAM[address + 1], address);
        return (mmu->RAM[address] << 8) | mmu->RAM[address + 1];
    }
    else {
        log_fatal("Unimplemented memory address: %x", address);
    }
}

u32 read32(s_MMU* mmu, u32 address) {
    if (address < 0xc8000000) {
        // remember: the GameCube is BIG ENDIAN!
        address = MASK_24MB(address);
        log_mmu("Read word %08x from %08x", (mmu->RAM[address] << 24) | (mmu->RAM[address + 1] << 16) | (mmu->RAM[address + 2] << 8) | mmu->RAM[address + 3], address);
        return (mmu->RAM[address] << 24) | (mmu->RAM[address + 1] << 16) | (mmu->RAM[address + 2] << 8) | mmu->RAM[address + 3];
    }
    else {
        log_fatal("Unimplemented memory address: %x", address);
    }
}

u64 read64(s_MMU* mmu, u32 address) {
    if (address < 0xc8000000) {
        // remember: the GameCube is BIG ENDIAN!
        address = MASK_24MB(address);
        log_mmu("Read double word %016" PRIx64 " from %08x",
                ((u64)mmu->RAM[address] << 56) | ((u64)mmu->RAM[address + 1] << 48) | ((u64)mmu->RAM[address + 2] << 40) | ((u64)mmu->RAM[address + 3] << 32) |
                ((u64)mmu->RAM[address + 4] << 24) | ((u64)mmu->RAM[address + 5] << 16) | ((u64)mmu->RAM[address + 6] << 8) | (u64)mmu->RAM[address + 7], address);

        return ((u64)mmu->RAM[address] << 56) |
                ((u64)mmu->RAM[address + 1] << 48) |
                ((u64)mmu->RAM[address + 2] << 40) |
                ((u64)mmu->RAM[address + 3] << 32) |
                ((u64)mmu->RAM[address + 4] << 24) |
                ((u64)mmu->RAM[address + 5] << 16) |
                ((u64)mmu->RAM[address + 6] << 8) |
                 (u64)mmu->RAM[address + 7];
    }
    else {
        log_fatal("Unimplemented memory address: %x", address);
    }
}

void write8(s_MMU* mmu, u32 address, u8 value) {
    log_mmu("Write byte %02x to %08x", value, address);
    if (address < 0xc8000000) {
        // remember: the GameCube is BIG ENDIAN!
        mmu->RAM[MASK_24MB(address)] = value;
    }
    else {
        log_fatal("Unimplemented memory address: %x", address);
    }
}

void write16(s_MMU* mmu, u32 address, u16 value) {
    log_mmu("Write halfword %04x to %08x", value, address);
    if (address < 0xc8000000) {
        // remember: the GameCube is BIG ENDIAN!
        address = MASK_24MB(address);
        mmu->RAM[address] = value >> 8;
        mmu->RAM[address + 1] = value & 0xff;
    }
    else {
        log_fatal("Unimplemented memory address: %x", address);
    }
}

void write32(s_MMU* mmu, u32 address, u32 value) {
    log_mmu("Write word %08x to %08x", value, address);
    if (address < 0xc8000000) {
        // remember: the GameCube is BIG ENDIAN!
        address = MASK_24MB(address);
        mmu->RAM[address] = value >> 24;
        mmu->RAM[address + 1] = (value >> 16) & 0xff;
        mmu->RAM[address + 2] = (value >> 8) & 0xff;
        mmu->RAM[address + 3] = value & 0xff;
    }
    else {
        log_fatal("Unimplemented memory address: %x", address);
    }
}

void write64(s_MMU* mmu, u32 address, u64 value) {
    log_mmu("Write word %016" PRIx64 " to %08x", value, address);
    if (address < 0xc8000000) {
        // remember: the GameCube is BIG ENDIAN!
        address = MASK_24MB(address);
        mmu->RAM[address] = value >> 56;
        mmu->RAM[address + 1] = (value >> 48) & 0xff;
        mmu->RAM[address + 2] = (value >> 40) & 0xff;
        mmu->RAM[address + 3] = (value >> 32) & 0xff;
        mmu->RAM[address + 4] = (value >> 24) & 0xff;
        mmu->RAM[address + 5] = (value >> 16) & 0xff;
        mmu->RAM[address + 6] = (value >> 8) & 0xff;
        mmu->RAM[address + 7] = value & 0xff;
    }
    else {
        log_fatal("Unimplemented memory address: %x", address);
    }
}