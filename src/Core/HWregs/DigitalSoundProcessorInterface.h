#ifndef GC__DIGITALSOUNDPROCESSORINTERFACE_H
#define GC__DIGITALSOUNDPROCESSORINTERFACE_H

#include "default.h"
#include "hwreg_utils.h"

typedef enum e_DSP_regs {
    DSP_MailboxHiToDSP = 0x0,
    DSP_MailboxLoToDSP = 0x2,
    DSP_MailboxHiFromDSP = 0x4,
    DSP_MailboxLoFromDSP = 0x6,
    DSP_CSR = 0xa
} e_DSP_regs;

typedef struct s_DSP {
    u8 regs[0x200];
    HW_REG_WRITE_CALLBACK((*write[0x100]), DSP);
    HW_REG_READ_PRECALL((*read[0x100]), DSP);
    struct s_GameCube* system;
} s_DSP;


HW_REG_INIT_FUNCTION(DSP);

#endif //GC__DIGITALSOUNDPROCESSORINTERFACE_H
