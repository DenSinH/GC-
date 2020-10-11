#include "DigitalSoundProcessorInterface.h"

#include "log.h"
#include "core_utils.h"

HW_REG_READ_PRECALL(read_DSP_MailboxHiFromDSP, DSPI) {

}

HW_REG_READ_PRECALL(read_DSP_MailboxLoFromDSP, DSPI) {

}

HW_REG_WRITE_CALLBACK(write_DSP_CSR, DSPI) {
    u16 value = READ16(DSPI->regs, DSP_reg_CSR);

    if (value & DSP_CSR_RES) {
        // reset DSP this happens "instantly"
    }
    DSPI->DSPCSR = value & ~(DSP_CSR_RES);
}

HW_REG_READ_PRECALL(read_DSP_CSR, DSPI) {
    WRITE16(DSPI->regs, DSP_reg_CSR, DSPI->DSPCSR);
}

HW_REG_INIT_FUNCTION(DSPI) {
    DSPI->read[DSP_reg_CSR >> DSP_SHIFT] = read_DSP_CSR;

    DSPI->write[DSP_reg_CSR >> DSP_SHIFT] = write_DSP_CSR;
}
