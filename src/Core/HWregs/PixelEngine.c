#include "PixelEngine.h"

HW_REG_WRITE_CALLBACK(write_PE_intr_status, PE) {
    PE->intr_status &= ~READ16(PE->regs, PE_reg_interrupt_status);
}

HW_REG_READ_PRECALL(read_PE_intr_status, PE) {
    WRITE16(PE->regs, PE_reg_interrupt_status, PE->intr_status);
}


HW_REG_INIT_FUNCTION(PE) {
    PE->write[PE_reg_interrupt_status >> 1] = write_PE_intr_status;
    PE->read[PE_reg_interrupt_status >> 1] = read_PE_intr_status;
}