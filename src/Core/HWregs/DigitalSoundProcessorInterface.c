#include "DigitalSoundProcessorInterface.h"

#include <stdio.h>

HW_REG_READ_PRECALL(read_DSP_MailboxHiFromDSP, DSP) {
    // clear some bits to signify transfer is done

    // todo: implement
    DSP->regs[DSP_MailboxHiFromDSP] ^= 0x80;

    HW_REG_READ_OVERFLOW_RECURSE(DSP, 2);
}

HW_REG_READ_PRECALL(read_DSP_MailboxLoFromDSP, DSP) {
    // clear some bits to signify transfer is done

    // todo: implement
    DSP->regs[DSP_MailboxLoFromDSP] ^= 0x80;

    HW_REG_READ_OVERFLOW_RECURSE(DSP, 2);
}

HW_REG_WRITE_CALLBACK(write_DSP_CSR, DSP) {
    // clear some bits to signify transfer is done
    DSP->regs[DSP_CSR] &= 0x0f;
    DSP->regs[DSP_CSR + 1] &= 0xfe;
    DSP->regs[DSP_CSR + 1] |= 0x20;
}

HW_REG_INIT_FUNCTION(DSP) {
    DSP->regs[DSP_MailboxHiFromDSP] = 0x80;

    DSP->read[DSP_MailboxHiFromDSP >> DSP_SHIFT] = read_DSP_MailboxHiFromDSP;
    DSP->read[DSP_MailboxLoFromDSP >> DSP_SHIFT] = read_DSP_MailboxLoFromDSP;

    DSP->write[DSP_CSR >> DSP_SHIFT] = write_DSP_CSR;
}
