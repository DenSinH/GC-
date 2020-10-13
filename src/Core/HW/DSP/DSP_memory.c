#include "DSP_memory.h"

#include "core_utils.h"

#include "log.h"

u16 DSP_read_imem(s_DSP* DSP, u16 address){
    switch (address >> 12) {
        case 0:
            return READ16(DSP->IRAM, address << 1);
        case 8:
            return READ16(DSP->IROM, (address & 0x0fff) << 1);
        default:
            log_fatal("[DSP] Invalid IMEM read: %04x", address);
    }
}

void DSP_write_imem(s_DSP* DSP, u16 address, u16 value){
    // is it even writeable?
    switch (address >> 12) {
        case 0:
            WRITE16(DSP->IRAM, address << 1, value);
            return;
        default:
            log_fatal("[DSP] Invalid IMEM write: %04x", address);
    }
}

u16 DSP_read_dmem(s_DSP* DSP, u16 address){
    switch (address >> 12) {
        case 0:
            return READ16(DSP->IRAM, address << 1);
        case 8:
            return READ16(DSP->IROM, (address & 0x0fff) << 1);
        case 0xf:
            // IO read
            switch (address) {
                case DSP_IO_CMBH:
                    return DSP->CMBH;
                case DSP_IO_CMBL:
                    return DSP->CMBL;
                case DSP_IO_DMBH:
                    return DSP->DMBH;
                case DSP_IO_DMBL:
                    return DSP->DMBL;
                case DSP_IO_DSMAH:
                    return DSP->DSMAH;
                case DSP_IO_DSMAL:
                    return DSP->DSMAL;
                case DSP_IO_DSPA:
                    return DSP->DSPA;
                case DSP_IO_DSCR:
                    return DSP->DSCR;
                case DSP_IO_DSBL:
                    return DSP->DSBL;
                case DSP_IO_ACSAH:
                    return DSP->ACSAH;
                case DSP_IO_ACSAL:
                    return DSP->ACSAL;
                case DSP_IO_ACEAH:
                    return DSP->ACEAH;
                case DSP_IO_ACEAL:
                    return DSP->ACEAL;
                case DSP_IO_ACCAH:
                    return DSP->ACCAH;
                case DSP_IO_ACCAL:
                    return DSP->ACCAL;
                case DSP_IO_ACDAT:
                    return DSP->ACDAT;
                case DSP_IO_DIRQ:
                    return DSP->DIRQ;
                default:
                    // log fatal
                    break;
            }
        default:
            log_fatal("[DSP] Invalid DMEM read: %04x", address);
    }
}

void DSP_write_dmem(s_DSP* DSP, u16 address, u16 value){
    switch (address >> 12) {
        case 0:
            WRITE16(DSP->IRAM, address << 1, value);
            return;
        case 0xf:
            // IO read
            switch (address) {
                case DSP_IO_CMBH:
                    DSP->CMBH = value;
                    return;
                case DSP_IO_CMBL:
                    DSP->CMBL = value;
                    return;
                case DSP_IO_DMBH:
                    DSP->DMBH = value;
                    return;
                case DSP_IO_DMBL:
                    DSP->DMBL = value;
                    return;
                case DSP_IO_DSMAH:
                    DSP->DSMAH = value;
                    return;
                case DSP_IO_DSMAL:
                    DSP->DSMAL = value;
                    return;
                case DSP_IO_DSPA:
                    DSP->DSPA = value;
                    return;
                case DSP_IO_DSCR:
                    DSP->DSCR = value;
                    return;
                case DSP_IO_DSBL:
                    DSP->DSBL = value;
                    return;
                case DSP_IO_ACSAH:
                    DSP->ACSAH = value;
                    return;
                case DSP_IO_ACSAL:
                    DSP->ACSAL = value;
                    return;
                case DSP_IO_ACEAH:
                    DSP->ACEAH = value;
                    return;
                case DSP_IO_ACEAL:
                    DSP->ACEAL = value;
                    return;
                case DSP_IO_ACCAH:
                    DSP->ACCAH = value;
                    return;
                case DSP_IO_ACCAL:
                    DSP->ACCAL = value;
                    return;
                case DSP_IO_ACDAT:
                    DSP->ACDAT = value;
                    return;
                case DSP_IO_DIRQ:
                    DSP->DIRQ = value;
                    return;
                default:
                    // log fatal
                    break;
            }
        default:
            log_fatal("[DSP] Invalid DMEM write: %04x", address);
    }
}
