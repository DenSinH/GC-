#ifndef GC__HARDWARE_REGISTERS_H
#define GC__HARDWARE_REGISTERS_H

#include "AudioStreamingInterface.h"
#include "CommandProcessor.h"
#include "DigitalSoundProcessorInterface.h"
#include "DVDInterface.h"
#include "ExternalInterface.h"
#include "GX_FIFO.h"
#include "MemoryInterface.h"
#include "PixelEngine.h"
#include "ProcessorInterface.h"
#include "SerialInterface.h"
#include "VideoInterface.h"

#include "hwreg_utils.h"
#include "default.h"

typedef struct s_hardware_registers {
    s_CP CP;
    s_PE PE;
    s_VI VI;
    s_PI PI;
    s_MI MI;
    s_DSPI DSPI;
    s_DI DI;
    s_SI SI;
    s_EXI EXI;
    s_AI AI;
    s_GX_FIFO GX_FIFO;

    struct s_GameCube* system;
} s_hardware_registers;

void init_HW_regs(s_hardware_registers* HW_regs);

#define HW_REG_READ_TEMPLATE_SIGNATURE(_size) u ## _size HW_reg_read ## _size(s_hardware_registers* HW_regs, u32 address)
#define HW_REG_WRITE_TEMPLATE_SIGNATURE(_size) void HW_reg_write ## _size(s_hardware_registers* HW_regs, u32 address, u ## _size value)

HW_REG_READ_TEMPLATE_SIGNATURE(8);
HW_REG_READ_TEMPLATE_SIGNATURE(16);
HW_REG_READ_TEMPLATE_SIGNATURE(32);
HW_REG_READ_TEMPLATE_SIGNATURE(64);

HW_REG_WRITE_TEMPLATE_SIGNATURE(8);
HW_REG_WRITE_TEMPLATE_SIGNATURE(16);
HW_REG_WRITE_TEMPLATE_SIGNATURE(32);
HW_REG_WRITE_TEMPLATE_SIGNATURE(64);

#endif //GC__HARDWARE_REGISTERS_H
