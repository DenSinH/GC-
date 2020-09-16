#include "ProcessorInterface.h"
#include "core_utils.h"

#include <stdio.h>

HW_REG_WRITE_CALLBACK(write_PI_INTMR, PI) {
    PI->INTMR = READ32(PI->regs, 4);
}

HW_REG_INIT_FUNCTION(PI) {
    PI->write[0x4 >> 2] = write_PI_INTMR;
}