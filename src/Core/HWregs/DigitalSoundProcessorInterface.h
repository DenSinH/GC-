#ifndef GC__DIGITALSOUNDPROCESSORINTERFACE_H
#define GC__DIGITALSOUNDPROCESSORINTERFACE_H

#include "default.h"
#include "hwreg_utils.h"

typedef enum e_DSP_regs {
    DSP_reg_MailboxHiToDSP   = 0x0,
    DSP_reg_MailboxLoToDSP   = 0x2,
    DSP_reg_MailboxHiFromDSP = 0x4,
    DSP_reg_MailboxLoFromDSP = 0x6,
    DSP_reg_CSR              = 0xa
} e_DSP_regs;

typedef enum e_DSP_CSR {
    DSP_CSR_RES        = 0x0001,
    DSP_CSR_PIINT      = 0x0002,
    DSP_CSR_HALT       = 0x0004,
    DSP_CSR_AIDINT     = 0x0008,
    DSP_CSR_AIDINTMSK  = 0x0010,
    DSP_CSR_ARINT      = 0x0020,
    DSP_CSR_ARINTMSK   = 0x0040,
    DSP_CSR_DSPINT     = 0x0080,
    DSP_CSR_DSPINTMSK  = 0x0100,
    DSP_CSR_DSPDMAINTS = 0x0200,
} e_DSP_CSR;

#define DSP_SHIFT 1

typedef struct s_DSP {
    u8 regs[0x200];
    HW_REG_WRITE_CALLBACK((*write[0x200 >> DSP_SHIFT]), DSP);
    HW_REG_READ_PRECALL((*read[0x200 >> DSP_SHIFT]), DSP);
    struct s_GameCube* system;

    /* internal function */
    u8 memory[0x10000];

    u16 DSPCSR;
    u16 DSPDMACSR;
} s_DSP;


HW_REG_INIT_FUNCTION(DSP);

#endif //GC__DIGITALSOUNDPROCESSORINTERFACE_H
