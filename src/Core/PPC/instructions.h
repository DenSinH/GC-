#ifndef GC__INSTRUCTIONS_H
#define GC__INSTRUCTIONS_H

#include "PPC/Gekko.h"
#include "gekko_instruction.h"
#include "default.h"
#include "helpers.h"
#include "log.h"

GEKKO_INSTR(unimplemented);

/* branch and linkage */
#define B_X_OPCODE 0x48000000
GEKKO_INSTR(b_x);

#define BC_X_OPCODE 0x40000000
GEKKO_INSTR(bc_x);

/* integer arithmetic */
#define ADDI_OPCODE 0x38000000
GEKKO_INSTR(addi);

#define ADDIS_OPCODE 0x3c000000
GEKKO_INSTR(addis);

/* integer logical */
#define ORI_OPCODE 0x60000000
GEKKO_INSTR(ori);

#define ORIS_OPCODE 0x64000000
GEKKO_INSTR(oris);

/* integer compare */
#define CMPLI_OPCODE 0x28000000
GEKKO_INSTR(cmpli);

/* integer rotate and shift */
#define RLWINM_OPCODE 0x54000000
GEKKO_INSTR(rlwinm);

/* integer load/store */
#define STW_OPCODE 0x90000000
GEKKO_INSTR(stw);

#define STWU_OPCODE 0x94000000
GEKKO_INSTR(stwu);

#define LWZ_OPCODE 0x80000000
GEKKO_INSTR(lwz);

#define STMW_OPCODE 0xbc000000
GEKKO_INSTR(stmw);

#define LMW_OPCODE 0xb8000000
GEKKO_INSTR(lmw);

/* float load/store */
#define LFD_OPCODE 0xc8000000
GEKKO_INSTR(lfd);

/* paired single load/store */
#define PSQ_L_OPCODE 0xe0000000
GEKKO_INSTR(psq_l);

/* aggregate instructions */
GEKKO_INSTR(instr_000100);

#define PS_MR_OPCODE_EXTENDED_10bit 0x090
INLINE_GEKKO_INSTR(ps_mr);

GEKKO_INSTR(instr_010011);

#define BCLR_X_OPCODE_EXTENDED 0x020
INLINE_GEKKO_INSTR(bclr_x);

#define ISYNC_OPCODE_EXTENDED 0x12c
INLINE_GEKKO_INSTR(isync);

#define CRXOR_OPCODE_EXTENDED 0x182
INLINE_GEKKO_INSTR(crxor);

#define RFI_OPCODE_EXTENDED 0x064
INLINE_GEKKO_INSTR(rfi);

GEKKO_INSTR(instr_011111);

#define SUBF_OPCODE_EXTENDED 0x050
INLINE_GEKKO_INSTR(subf);

#define CMP_OPCODE_EXTENDED 0x000
INLINE_GEKKO_INSTR(cmp);

#define ORX_OPCODE_EXTENDED 0x378
INLINE_GEKKO_INSTR(orx);

#define MFMSR_OPCODE_EXTENDED 0x0a6
INLINE_GEKKO_INSTR(mfmsr);

#define MTMSR_OPCODE_EXTENDED 0x124
INLINE_GEKKO_INSTR(mtmsr);

#define MFSPR_OPCODE_EXTENDED 0x2a6
INLINE_GEKKO_INSTR(mfspr);

#define MTSPR_OPCODE_EXTENDED 0x3a6
INLINE_GEKKO_INSTR(mtspr);

#define MFSR_OPCODE_EXTENDED 0x4a6
INLINE_GEKKO_INSTR(mfsr);

#define MTSR_OPCODE_EXTENDED 0x1a4
INLINE_GEKKO_INSTR(mtsr);

#define SYNC_OPCODE_EXTENDED 0x4ac
INLINE_GEKKO_INSTR(sync);

GEKKO_INSTR(instr_111111);

#define FMR_OPCODE_EXTENDED_10bit 0x090
INLINE_GEKKO_INSTR(fmr);

#define MTFSF_OPCODE_EXTENDED_10bit 0x58e
INLINE_GEKKO_INSTR(mtfsf);

#define MTFSB0_OPCODE_EXTENDED_10bit 0x08c
INLINE_GEKKO_INSTR(mtfsb0);
#define MTFSB1_OPCODE_EXTENDED_10bit 0x04c
INLINE_GEKKO_INSTR(mtfsb1);

#endif //GC__INSTRUCTIONS_H
