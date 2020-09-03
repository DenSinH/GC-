#include "hardware_registers.h"
#include "core_utils.h"
#include "log.h"

#define HW_REGS_READ_ARGS(_section) &HW_regs->_section, address & 0x3ff, size
#define HW_REGS_WRITE_ARGS(_section) &HW_regs->_section, address & 0x3ff, value, size

void init_HW_regs(s_hardware_registers* HW_regs) {
    HW_regs->CP.system = HW_regs->system;
    HW_regs->PE.system = HW_regs->system;
    HW_regs->VI.system = HW_regs->system;
    HW_regs->PI.system = HW_regs->system;
    HW_regs->MI.system = HW_regs->system;
    HW_regs->DSP.system = HW_regs->system;
    HW_regs->DI.system = HW_regs->system;
    HW_regs->SI.system = HW_regs->system;
    HW_regs->EXI.system = HW_regs->system;
    HW_regs->AI.system = HW_regs->system;
    HW_regs->GX_FIFO.system = HW_regs->system;

    init_CP(&HW_regs->CP);
    init_PE(&HW_regs->PE);
    init_VI(&HW_regs->VI);
    init_PI(&HW_regs->PI);
    init_MI(&HW_regs->MI);
    init_DSP(&HW_regs->DSP);
    init_DI(&HW_regs->DI);
    init_SI(&HW_regs->SI);
    init_EXI(&HW_regs->EXI);
    init_AI(&HW_regs->AI);
    init_GX_FIFO(&HW_regs->GX_FIFO);
}

#define SECTION_READ_TEMPLATE(_section, _size, _section_shift) \
if (HW_regs->_section.read[masked_address >> (_section_shift)]) \
    HW_regs->_section.read[masked_address >> (_section_shift)](&HW_regs->_section, masked_address, _size);                                              \
return READ ## _size(HW_regs->_section.regs, masked_address)

#define HW_REG_READ_TEMPLATE(_size) \
HW_REG_READ_TEMPLATE_SIGNATURE(_size) { \
    u32 masked_address = address & 0x3ff; \
    switch (address & 0x0000fc00) { \
        case 0x0000: \
            SECTION_READ_TEMPLATE(CP, _size, 1); \
        case 0x1000: \
            SECTION_READ_TEMPLATE(PE, _size, 1); \
        case 0x2000: \
            SECTION_READ_TEMPLATE(VI, _size, 2); \
        case 0x3000: \
            SECTION_READ_TEMPLATE(PI, _size, 2); \
        case 0x4000: \
            SECTION_READ_TEMPLATE(MI, _size, 2); \
        case 0x5000: \
            SECTION_READ_TEMPLATE(DSP, _size, 1); \
        case 0x6000: \
            SECTION_READ_TEMPLATE(DI, _size, 2); \
        case 0x6400: \
            SECTION_READ_TEMPLATE(SI, _size, 2); \
        case 0x6800: \
            SECTION_READ_TEMPLATE(EXI, _size, 2); \
        case 0x6c00: \
            SECTION_READ_TEMPLATE(AI, _size, 2); \
        case 0x8000: \
            /* GX FIFO reads */ \
            return 0; \
        default: \
            log_fatal("Invalid hardware register address region: %08x", address); \
    } \
}

HW_REG_READ_TEMPLATE(8)
HW_REG_READ_TEMPLATE(16)
HW_REG_READ_TEMPLATE(32)
HW_REG_READ_TEMPLATE(64)


#define SECTION_WRITE_TEMPLATE(_section, _size, _section_shift) \
WRITE ## _size(HW_regs->_section.regs, masked_address, value); \
if (HW_regs->_section.write[masked_address >> (_section_shift)]) HW_regs->_section.write[masked_address >> (_section_shift)](&HW_regs->_section, masked_address, _size)

#define HW_REG_WRITE_TEMPLATE(_size) \
HW_REG_WRITE_TEMPLATE_SIGNATURE(_size) { \
    u32 masked_address = address & 0x3ff; \
    switch (address & 0x0000fc00) { \
        case 0x0000: \
            SECTION_WRITE_TEMPLATE(CP, 8, 1); \
            break; \
        case 0x1000: \
            SECTION_WRITE_TEMPLATE(PE, 8, 1); \
            break; \
        case 0x2000: \
            SECTION_WRITE_TEMPLATE(VI, 8, 2); \
            break; \
        case 0x3000: \
            SECTION_WRITE_TEMPLATE(PI, 8, 2); \
            break; \
        case 0x4000: \
            SECTION_WRITE_TEMPLATE(MI, 8, 2); \
            break; \
        case 0x5000: \
            SECTION_WRITE_TEMPLATE(DSP, 8, 1); \
            break; \
        case 0x6000: \
            SECTION_WRITE_TEMPLATE(DI, 8, 2); \
            break; \
        case 0x6400: \
            SECTION_WRITE_TEMPLATE(SI, 8, 2); \
            break; \
        case 0x6800: \
            SECTION_WRITE_TEMPLATE(EXI, 8, 2); \
            break; \
        case 0x6c00: \
            SECTION_WRITE_TEMPLATE(AI, 8, 2); \
            break; \
        case 0x8000: \
            WRITE ## _size(HW_regs->GX_FIFO.buffer, masked_address, value); \
            HW_regs->GX_FIFO.write(&HW_regs->GX_FIFO, masked_address, _size); \
            break; \
        default: \
            log_fatal("Invalid hardware register address region: %08x", address); \
    } \
}

HW_REG_WRITE_TEMPLATE(8)
HW_REG_WRITE_TEMPLATE(16)
HW_REG_WRITE_TEMPLATE(32)
HW_REG_WRITE_TEMPLATE(64)