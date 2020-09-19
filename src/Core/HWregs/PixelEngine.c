#include "PixelEngine.h"
#include "ProcessorInterface.h"

HW_REG_WRITE_CALLBACK(write_PE_intr_status, PE) {
    u16 old_status = PE->intr_status;
    PE->intr_status &= ~READ16(PE->regs, PE_reg_interrupt_status);
    log_cpu("Wrote to PE intr status (%08x -> %08x)", old_status, PE->intr_status);

    // clear interrupts in processor interface as well
    if (old_status & ~PE->intr_status & PE_intr_DONE) {
        clear_PI_intsr(PE->PI, PI_intr_PE_DONE);
    }

    if (old_status & ~PE->intr_status & PE_intr_TOKEN) {
        clear_PI_intsr(PE->PI, PI_intr_PE_TOKEN);
    }
}

HW_REG_READ_PRECALL(read_PE_intr_status, PE) {
    WRITE16(PE->regs, PE_reg_interrupt_status, PE->intr_status);
}


HW_REG_INIT_FUNCTION(PE) {
    PE->write[PE_reg_interrupt_status >> 1] = write_PE_intr_status;
    PE->read[PE_reg_interrupt_status >> 1] = read_PE_intr_status;
}