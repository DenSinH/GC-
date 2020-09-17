#include "ProcessorInterface.h"
#include "core_utils.h"

#include <stdio.h>

HW_REG_WRITE_CALLBACK(write_PI_INTSR, PI) {
    PI->INTSR &= ~READ32(PI->regs, 0);
}

HW_REG_READ_PRECALL(read_PI_INTSR, PI) {
    WRITE32(PI->regs, 0, PI->INTSR);
}

HW_REG_WRITE_CALLBACK(write_PI_INTMR, PI) {
    PI->INTMR = READ32(PI->regs, 4);
}

HW_REG_INIT_FUNCTION(PI) {
    PI->write[0x0 >> 2] = write_PI_INTSR;
    PI->read[0x0 >> 2] = read_PI_INTSR;
    PI->write[0x4 >> 2] = write_PI_INTMR;
}