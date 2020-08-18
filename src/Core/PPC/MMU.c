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
        return (mmu->RAM[address] << 24) | (mmu->RAM[address + 1] << 16) | (mmu->RAM[address + 2] << 8) | mmu->RAM[address + 3];
    }
    else {
        log_fatal("Unimplemented memory address: %x", address);
    }
}


void write8(s_MMU* mmu, u32 address, u8 value) {
    if (address < 0xc8000000) {
        // remember: the GameCube is BIG ENDIAN!
        mmu->RAM[MASK_24MB(address)] = value;
    }
    else {
        log_fatal("Unimplemented memory address: %x", address);
    }
}

void write16(s_MMU* mmu, u32 address, u16 value) {
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