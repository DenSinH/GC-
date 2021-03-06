#ifndef GC__DSP_INSTRUCTIONS_H
#define GC__DSP_INSTRUCTIONS_H

#include "DSP.h"

DSP_INSTR(DSP_unimplemented);

DSP_INSTR(DSP_0000_0000);
DSP_INSTR(DSP_0000_001r);

DSP_INSTR(DSP_SBSET);
DSP_INSTR(DSP_SET16);
DSP_INSTR(DSP_SET40);
DSP_INSTR(DSP_SET15);
DSP_INSTR(DSP_CLR15);
DSP_INSTR(DSP_M0);
DSP_INSTR(DSP_M2);
DSP_INSTR(DSP_SI);
DSP_INSTR(DSP_CLR);

DSP_INSTR(DSP_LRS);
DSP_INSTR(DSP_SRS);
DSP_INSTR(DSP_CMP);
DSP_INSTR(DSP_MRR);
DSP_INSTR(DSP_LRRx);
DSP_INSTR(DSP_SRRx);

#endif //GC__DSP_INSTRUCTIONS_H
