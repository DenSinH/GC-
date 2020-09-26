#include "SerialInterface.h"

#include "core_utils.h"
#include "log.h"

#include <stdio.h>

HW_REG_READ_PRECALL(read_SICOMCSR, SI) {
    log_si("Read SI COMCSR");
    WRITE32(SI->regs, SI_reg_SICOMCSR, SI->COMCSR);
}

HW_REG_WRITE_CALLBACK(write_SICOMCSR, SI) {
    log_si("Write SI COMCSR");
    u32 value = READ32(SI->regs, SI_reg_SICOMCSR);
    SI->COMCSR = (SI->COMCSR & ~(value & 0x80000000)) | (value & 0x7fffffff);  // clear interrupt if enabled
}

HW_REG_READ_PRECALL(read_SISR, SI) {
    // we don't care about this register, as all transfers are instant and perfect (no errors)
    log_si("Read SI SISR");
    WRITE32(SI->regs, SI_reg_SISR, 0);
}

void read_SI_Joy1x(s_SI* SI, int index) {
    log_si("Polled gamepad %d buttons 1", index);
    WRITE32(SI->regs, SI_reg_Joy1_base + (index * 0xc), SI->joypad_buttons_1[index]);
}

HW_REG_READ_PRECALL(read_SI_Joy11, SI) { read_SI_Joy1x(SI, 0); }
HW_REG_READ_PRECALL(read_SI_Joy12, SI) { read_SI_Joy1x(SI, 1); }
HW_REG_READ_PRECALL(read_SI_Joy13, SI) { read_SI_Joy1x(SI, 2); }
HW_REG_READ_PRECALL(read_SI_Joy14, SI) { read_SI_Joy1x(SI, 3); }

void read_SI_Joy2x(s_SI* SI, int index) {
    log_si("Polled gamepad %d buttons 2", index);
    WRITE32(SI->regs, SI_reg_Joy2_base + (index * 0xc), SI->joypad_buttons_2[index]);
}

HW_REG_READ_PRECALL(read_SI_Joy21, SI) { read_SI_Joy2x(SI, 0); }
HW_REG_READ_PRECALL(read_SI_Joy22, SI) { read_SI_Joy2x(SI, 1); }
HW_REG_READ_PRECALL(read_SI_Joy23, SI) { read_SI_Joy2x(SI, 2); }
HW_REG_READ_PRECALL(read_SI_Joy24, SI) { read_SI_Joy2x(SI, 3); }


HW_REG_WRITE_CALLBACK(write_SI_buffer, SI) {
    log_si("SI buffer data: %08x", READ32(SI->regs, 0x80));
}


HW_REG_INIT_FUNCTION(SI) {
    SI->write[0x80 >> SI_SHIFT] = write_SI_buffer;

    SI->read[SI_reg_SICOMCSR >> SI_SHIFT] = read_SICOMCSR;
    SI->read[SI_reg_SISR >> SI_SHIFT] = read_SISR;

    SI->read[(SI_reg_Joy1_base) >> SI_SHIFT] = read_SI_Joy11;
    SI->read[(SI_reg_Joy2_base) >> SI_SHIFT] = read_SI_Joy21;
    SI->read[(SI_reg_Joy1_base + 0xc) >> SI_SHIFT] = read_SI_Joy12;
    SI->read[(SI_reg_Joy2_base + 0xc) >> SI_SHIFT] = read_SI_Joy22;
    SI->read[(SI_reg_Joy1_base + 0x18) >> SI_SHIFT] = read_SI_Joy13;
    SI->read[(SI_reg_Joy2_base + 0x18) >> SI_SHIFT] = read_SI_Joy23;
    SI->read[(SI_reg_Joy1_base + 0x24) >> SI_SHIFT] = read_SI_Joy14;
    SI->read[(SI_reg_Joy2_base + 0x24) >> SI_SHIFT] = read_SI_Joy24;
}