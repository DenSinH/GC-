#include "ProcessorInterface.h"
#include "core_utils.h"

#include "../PPC/interrupts.h"

// todo: unncessary, only for debugging
#include "../system.h"

#include <stdio.h>

void set_PI_intsr(s_PI* PI, e_PI_interrupt interrupt, int delay) {
    log_cpu("Set PI interrupt %x", interrupt);
    PI->INTSR |= interrupt;

    if (PI->INTMR & PI->INTSR) {
        log_cpu("Requesting interrupt poll for PI interrupt %04x", interrupt);
        start_interrupt_poll(&PI->system->cpu, delay);
    }
}

HW_REG_WRITE_CALLBACK(write_PI_INTSR, PI) {
    PI->INTSR &= ~READ32(PI->regs, 0);
}

HW_REG_READ_PRECALL(read_PI_INTSR, PI) {
    WRITE32(PI->regs, 0, PI->INTSR);
}

HW_REG_WRITE_CALLBACK(write_PI_INTMR, PI) {
    PI->INTMR = READ32(PI->regs, 4);
}

HW_REG_WRITE_CALLBACK(write_PI_unknown, PI) {
    // we use this for debugging purposes to write to in our programs
    printf("PI unknown write: %08x (@%08x)\n", READ32(PI->regs, 0x18), PI->system->cpu.PC);
}

HW_REG_INIT_FUNCTION(PI) {
    PI->write[PI_reg_INTSR >> PI_SHIFT] = write_PI_INTSR;
    PI->read[PI_reg_INTSR >> PI_SHIFT] = read_PI_INTSR;
    PI->write[PI_reg_INTMR >> PI_SHIFT] = write_PI_INTMR;

    PI->write[0x18 >> PI_SHIFT] = write_PI_unknown;
}