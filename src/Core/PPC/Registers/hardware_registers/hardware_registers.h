#ifndef GC__HARDWARE_REGISTERS_H
#define GC__HARDWARE_REGISTERS_H

#include "default.h"

#define HW_REG_READ_FUNCTION(_name) u32 _name(struct s_GameCube* system, u32 address, size_t size)
#define HW_REG_WRITE_FUNCTION(_name) void _name(struct s_GameCube* system, u32 address, u32 value, size_t size)

#define HR_INDEX_FROM_ADDRESS(address) (((address) >> 12) & 0xf)

typedef enum {
    HR_CP = 0,
    HR_PE = 1,
    HR_VI = 2,
    HR_PI = 3,
    HR_MI = 4,
    HR_AI = 5,
    HR_DI_SI_EXI_Streaming = 6,
    HR_GX_FIFO = 8,
} e_hardware_registers;

const static size_t hardware_register_block_size[9] = {
        0x80,
        0x100,
        0x100,
        0x100,
        0x80,
        0x200,
        0x1000,
        0,
        4
};

typedef struct s_hardware_registers {
    u8 CP[0x80];
    u8 PE[0x100];
    u8 VI[0x100];
    u8 PI[0x100];
    u8 MI[0x80];
    u8 AI[0x200];
    u8 DI_SI_EXI_Streaming[0x1000];
    u8 GX_FIFO[0x4];

    u8* pointers[9];

    HW_REG_READ_FUNCTION((*read[9]));
    HW_REG_WRITE_FUNCTION((*write[9]));
} s_hardware_registers;

void init_HW_regs(s_hardware_registers* HW_regs);

#endif //GC__HARDWARE_REGISTERS_H
