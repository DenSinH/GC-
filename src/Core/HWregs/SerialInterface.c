#include "SerialInterface.h"

#include "core_utils.h"

#include <stdio.h>

HW_REG_WRITE_CALLBACK(write_SI_buffer, SI) {
#ifndef NDEBUG
    printf("SI buffer data: %08x\n", READ32(SI->regs, 0x80));
#endif
}


HW_REG_INIT_FUNCTION(SI) {
    SI->write[0x80 >> SI_SHIFT] = write_SI_buffer;
}