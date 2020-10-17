#include "hardware_registers.h"
#include "core_utils.h"
#include "log.h"

#ifdef CHECK_HR_ACCESS_ALIGNMENT
#include <assert.h>
#endif

#define HW_REGS_READ_ARGS(_section) &HW_regs->_section, address & 0x3ff, size
#define HW_REGS_WRITE_ARGS(_section) &HW_regs->_section, address & 0x3ff, value, size

void init_HW_regs(s_hardware_registers* HW_regs) {
    HW_regs->CP.system   = HW_regs->system;
    HW_regs->PE.system   = HW_regs->system;
    HW_regs->VI.system   = HW_regs->system;
    HW_regs->PI.system   = HW_regs->system;
    HW_regs->MI.system   = HW_regs->system;
    HW_regs->DSPI.system = HW_regs->system;
    HW_regs->DI.system   = HW_regs->system;
    HW_regs->SI.system   = HW_regs->system;
    HW_regs->EXI.system  = HW_regs->system;
    HW_regs->AI.system   = HW_regs->system;
    HW_regs->GX_FIFO.system = HW_regs->system;

    HW_regs->GX_FIFO.CP_ptr = &HW_regs->CP;
    HW_regs->CP.PI = &HW_regs->PI;
    HW_regs->CP.PE = &HW_regs->PE;
    HW_regs->CP.VI = &HW_regs->VI;
    HW_regs->VI.PI = &HW_regs->PI;
    HW_regs->PE.PI = &HW_regs->PI;
    HW_regs->SI.PI = &HW_regs->PI;
    HW_regs->AI.PI = &HW_regs->PI;

    init_CP(&HW_regs->CP);
    init_PE(&HW_regs->PE);
    init_VI(&HW_regs->VI);
    init_PI(&HW_regs->PI);
    init_MI(&HW_regs->MI);
    init_DSPI(&HW_regs->DSPI);
    init_DI(&HW_regs->DI);
    init_SI(&HW_regs->SI);
    init_EXI(&HW_regs->EXI);
    init_AI(&HW_regs->AI);
    init_GX_FIFO(&HW_regs->GX_FIFO);
}

#define SECTION_READ_TEMPLATE(_section, _size, _section_shift) \
if (HW_regs->_section.read[masked_address >> (_section_shift)]) \
    HW_regs->_section.read[masked_address >> (_section_shift)](&HW_regs->_section, masked_address, _size); \
/* misaligned reads */ \
if ((_size) > (8 << (_section_shift)) && HW_regs->_section.read[1 + (masked_address >> (_section_shift))]) \
    HW_regs->_section.read[1 + (masked_address >> (_section_shift))](&HW_regs->_section, masked_address, _size); \
if((masked_address & ((_size >> 3)) - 1) || (_size > 8 * (1 << _section_shift))) \
    log_warn("Misaligned or too large read access at %x (size %d)", address, _size); \
return READ ## _size(HW_regs->_section.regs, masked_address)

#define HW_REG_READ_TEMPLATE(_size) \
HW_REG_READ_TEMPLATE_SIGNATURE(_size) { \
    u32 masked_address = address & 0x3ff; \
    switch (address & 0x0000fc00) { \
        case 0x0000: \
            log_cp("Reading CP %x", masked_address); \
            SECTION_READ_TEMPLATE(CP, _size, CP_SHIFT); \
        case 0x1000: \
            SECTION_READ_TEMPLATE(PE, _size, PE_SHIFT); \
        case 0x2000: \
            SECTION_READ_TEMPLATE(VI, _size, VI_SHIFT); \
        case 0x3000: \
            SECTION_READ_TEMPLATE(PI, _size, PI_SHIFT); \
        case 0x4000: \
            SECTION_READ_TEMPLATE(MI, _size, MI_SHIFT); \
        case 0x5000: \
            log_dsp("Reading DSP %x (%dbit)", masked_address, _size); \
            SECTION_READ_TEMPLATE(DSPI, _size, DSP_SHIFT); \
        case 0x6000: \
            SECTION_READ_TEMPLATE(DI, _size, DI_SHIFT); \
        case 0x6400: \
            log_si("Reading SI %x", masked_address); \
            SECTION_READ_TEMPLATE(SI, _size, SI_SHIFT); \
        case 0x6800: \
            SECTION_READ_TEMPLATE(EXI, _size, EXI_SHIFT); \
        case 0x6c00: \
            log_dsp("Reading AI %x (%dbit)", masked_address, _size); \
            SECTION_READ_TEMPLATE(AI, _size, AI_SHIFT); \
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
if (HW_regs->_section.write[masked_address >> (_section_shift)]) \
    HW_regs->_section.write[masked_address >> (_section_shift)](&HW_regs->_section, masked_address, _size); \
/* misaligned writes */ \
if ((_size) > (8 << (_section_shift)) && HW_regs->_section.write[1 + (masked_address >> (_section_shift))]) \
    HW_regs->_section.write[1 + (masked_address >> (_section_shift))](&HW_regs->_section, masked_address, _size) \

#define HW_REG_WRITE_TEMPLATE(_size) \
HW_REG_WRITE_TEMPLATE_SIGNATURE(_size) { \
    u32 masked_address = address & 0x3ff; \
    switch (address & 0x0000fc00) { \
        case 0x0000: \
            log_cp("Writing CP %x (%x)", masked_address, value); \
            SECTION_WRITE_TEMPLATE(CP, _size, CP_SHIFT); \
            break; \
        case 0x1000: \
            SECTION_WRITE_TEMPLATE(PE, _size, PE_SHIFT); \
            break; \
        case 0x2000: \
            SECTION_WRITE_TEMPLATE(VI, _size, VI_SHIFT); \
            break; \
        case 0x3000: \
            SECTION_WRITE_TEMPLATE(PI, _size, PI_SHIFT); \
            break; \
        case 0x4000: \
            SECTION_WRITE_TEMPLATE(MI, _size, MI_SHIFT); \
            break; \
        case 0x5000: \
            log_dsp("Writing DSP %x (%x) (%dbit)", masked_address, value, _size); \
            SECTION_WRITE_TEMPLATE(DSPI, _size, DSP_SHIFT); \
            break; \
        case 0x6000: \
            SECTION_WRITE_TEMPLATE(DI, _size, DI_SHIFT); \
            break; \
        case 0x6400: \
            log_si("Writing SI %x (%x)", masked_address, value); \
            SECTION_WRITE_TEMPLATE(SI, _size, SI_SHIFT); \
            break; \
        case 0x6800: \
            SECTION_WRITE_TEMPLATE(EXI, _size, EXI_SHIFT); \
            break; \
        case 0x6c00: \
            log_dsp("Writing AI %x (%x) (%dbit)", masked_address, value, _size); \
            SECTION_WRITE_TEMPLATE(AI, _size, AI_SHIFT); \
            break; \
        case 0x8000: \
            /* Write to the end of the buffer of the GX FIFO and always call the write callback */ \
            WRITE ## _size(HW_regs->GX_FIFO.buffer, HW_regs->GX_FIFO.buffer_size, value); \
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
