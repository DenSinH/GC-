#include "DigitalSoundProcessorInterface.h"

#include "log.h"
#include "core_utils.h"

HW_REG_READ_PRECALL(read_DSP_MailboxHiFromDSP, DSP) {

}

HW_REG_READ_PRECALL(read_DSP_MailboxLoFromDSP, DSP) {

}

HW_REG_WRITE_CALLBACK(write_DSP_CSR, DSP) {
    u16 value = READ16(DSP->regs, DSP_reg_CSR);

    if (value & DSP_CSR_RES) {
        // reset DSP this happens "instantly"
    }
    DSP->DSPCSR = value & ~(DSP_CSR_RES);
}

HW_REG_READ_PRECALL(read_DSP_CSR, DSP) {
    WRITE16(DSP->regs, DSP_reg_CSR, DSP->DSPCSR);
}

HW_REG_INIT_FUNCTION(DSP) {
    DSP->read[DSP_reg_CSR >> DSP_SHIFT] = read_DSP_CSR;

    DSP->write[DSP_reg_CSR >> DSP_SHIFT] = write_DSP_CSR;
}
