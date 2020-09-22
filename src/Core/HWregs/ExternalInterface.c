#include "ExternalInterface.h"

HW_REG_READ_PRECALL(EXI0CR_read, EXI) {
    EXI->regs[0xc + 3] &= 0xfe;  // clear status bit
}


HW_REG_INIT_FUNCTION(EXI) {
    EXI->read[0xc >> EXI_SHIFT] = EXI0CR_read;
}
