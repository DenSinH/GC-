#include "DSP_memory.h"

#include "core_utils.h"

#include "log.h"

u16 DSP_read_imem(s_DSP* DSP, u16 address){
    switch (address >> 12) {
        case 0:
            // IMEM is mapped to the start of ARAM
            return READ16(DSP->ARAM, address << 1);
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
            WRITE16(DSP->ARAM, address << 1, value);
            return;
        default:
            log_fatal("[DSP] Invalid IMEM write: %04x", address);
    }
}

static inline void wrap_ACCA(s_DSP* DSP) {
    if (DSP->ACCA > DSP->ACEA) {
        DSP->ACCA = DSP->ACSA;
        DSP_interrupt(DSP, DSP_int_ACCOV);
    }
}

u16 DSP_read_dmem(s_DSP* DSP, u16 address){
    switch (address >> 12) {
        case 0:
            return READ16(DSP->ARAM, address << 1);
        case 1:
            // COEF
            return READ16(DSP->IROM, (address & 0x0fff) << 1);
        case 0xf:
            // IO read
            log_dsp_verbose("DSP HWIO read <- [%04x]", address);
            switch (address) {
                case DSP_IO_CMBH:
                    return DSP->CMBH;
                case DSP_IO_CMBL:
                    // M bit is cleared on read
                    DSP->CMBH &= 0x7fff;
                    return DSP->CMBL;
                case DSP_IO_DMBH:
                    return DSP->DMBH & 0x8000;
                case DSP_IO_DMBL:
                    return 0;
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
                    return DSP->ACSA >> 16;
                case DSP_IO_ACSAL:
                    return (u16)DSP->ACSA;
                case DSP_IO_ACEAH:
                    return DSP->ACEA >> 16;
                case DSP_IO_ACEAL:
                    return (u16)DSP->ACEA;
                case DSP_IO_ACCAH:
                    return DSP->ACCA >> 16;
                case DSP_IO_ACCAL:
                    return (u16)DSP->ACCA;
                case DSP_IO_ACDAT:
                    u16 value = READ16(DSP->ARAM, (DSP->ACCA++) << 1);
                    wrap_ACCA(DSP);
                    return value;
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
            WRITE16(DSP->DRAM, address << 1, value);
            return;
        case 0xf:
            // IO read
            log_dsp_verbose("DSP HWIO write %04x -> [%04x]", value, address);
            switch (address) {
                case DSP_IO_CMBH:
                case DSP_IO_CMBL:
                    log_dsp("DSP attempted write to CMBH/L");
                    return;
                case DSP_IO_DMBH:
                    DSP->DMBH = value;
                    return;
                case DSP_IO_DMBL:
                    DSP->DMBL = value;
                    DSP->DMBH |= 0x8000; // M bit set on writes to L reg
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
                    DSP->ACSA = (DSP->ACSA & 0xffff) | ((u32)value << 16);
                    wrap_ACCA(DSP);
                    return;
                case DSP_IO_ACSAL:
                    DSP->ACSA = (DSP->ACSA & 0xffff0000) | value;
                    wrap_ACCA(DSP);
                    return;
                case DSP_IO_ACEAH:
                    DSP->ACEA = (DSP->ACEA & 0xffff) | ((u32)value << 16);
                    wrap_ACCA(DSP);
                    return;
                case DSP_IO_ACEAL:
                    DSP->ACEA = (DSP->ACEA & 0xffff0000) | value;
                    wrap_ACCA(DSP);
                    return;
                case DSP_IO_ACCAH:
                    DSP->ACCA = (DSP->ACCA & 0xffff) | ((u32)value << 16);
                    wrap_ACCA(DSP);
                    return;
                case DSP_IO_ACCAL:
                    DSP->ACCA = (DSP->ACEA & 0xffff0000) | value;
                    wrap_ACCA(DSP);
                    return;
                case DSP_IO_ACDAT:
                    // this register can't be written to
                    return;
                case DSP_IO_DIRQ:
                    DSP->DIRQ = value & 1;
                    return;
                default:
                    // log fatal
                    break;
            }
        default:
            log_fatal("[DSP] Invalid DMEM write: %04x", address);
    }
}
