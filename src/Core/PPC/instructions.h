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

#define SC_OPCODE 0x44000000
GEKKO_INSTR(sc);

/* integer arithmetic */
#define ADDI_OPCODE 0x38000000
GEKKO_INSTR(addi);

#define ADDIS_OPCODE 0x3c000000
GEKKO_INSTR(addis);

#define ADDIC_OPCODE 0x30000000
GEKKO_INSTR(addic);

#define SUBFIC_OPCODE 0x20000000
GEKKO_INSTR(subfic);

#define ADDIC__OPCODE 0x34000000
GEKKO_INSTR(addic_);

#define MULLI_OPCODE 0x1c000000
GEKKO_INSTR(mulli);

/* integer logical */
#define ORI_OPCODE 0x60000000
GEKKO_INSTR(ori);

#define ORIS_OPCODE 0x64000000
GEKKO_INSTR(oris);

#define ANDI_OPCODE 0x70000000
GEKKO_INSTR(andi);

#define ANDIS_OPCODE 0x74000000
GEKKO_INSTR(andis);

#define XORI_OPCODE 0x68000000
GEKKO_INSTR(xori);

/* integer compare */
#define CMPLI_OPCODE 0x28000000
GEKKO_INSTR(cmpli);

#define CMPI_OPCODE 0x2c000000
GEKKO_INSTR(cmpi);

/* integer rotate and shift */
#define RLWINM_OPCODE 0x54000000
GEKKO_INSTR(rlwinm);

#define RLWIMI_OPCODE 0x50000000
GEKKO_INSTR(rlwimi);

/* integer load/store */
#define STW_OPCODE 0x90000000
GEKKO_INSTR(stw);

#define STH_OPCODE 0xb0000000
GEKKO_INSTR(sth);

#define STB_OPCODE 0x98000000
GEKKO_INSTR(stb);

#define STBU_OPCODE 0x9c000000
GEKKO_INSTR(stbu);

#define STWU_OPCODE 0x94000000
GEKKO_INSTR(stwu);

#define LWZ_OPCODE 0x80000000
GEKKO_INSTR(lwz);

#define LHZ_OPCODE 0xa0000000
GEKKO_INSTR(lhz);

#define LHA_OPCODE 0xa8000000
GEKKO_INSTR(lha);

#define LBZ_OPCODE 0x88000000
GEKKO_INSTR(lbz);

#define LBZU_OPCODE 0x8c000000
GEKKO_INSTR(lbzu);

#define LWZU_OPCODE 0x84000000
GEKKO_INSTR(lwzu);

#define STMW_OPCODE 0xbc000000
GEKKO_INSTR(stmw);

#define LMW_OPCODE 0xb8000000
GEKKO_INSTR(lmw);

/* float load/store */
#define STFD_OPCODE 0xd8000000
GEKKO_INSTR(stfd);

#define LFD_OPCODE 0xc8000000
GEKKO_INSTR(lfd);

#define LFS_OPCODE 0xc0000000
GEKKO_INSTR(lfs);

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

#define BCCTR_X_OPCODE_EXTENDED 0x420
INLINE_GEKKO_INSTR(bcctr_x);

#define MCRF_OPCODE_EXTENDED 0x000
INLINE_GEKKO_INSTR(mcrf);

#define ISYNC_OPCODE_EXTENDED 0x12c
INLINE_GEKKO_INSTR(isync);

#define CRXOR_OPCODE_EXTENDED 0x182
INLINE_GEKKO_INSTR(crxor);

#define RFI_OPCODE_EXTENDED 0x064
INLINE_GEKKO_INSTR(rfi);

GEKKO_INSTR(instr_011111);

#define ADD_OPCODE_EXTENDED 0x214
INLINE_GEKKO_INSTR(add);

#define ADDC_OPCODE_EXTENDED 0x014
INLINE_GEKKO_INSTR(addc);

#define ADDE_OPCODE_EXTENDED 0x114
INLINE_GEKKO_INSTR(adde);

#define ADDZE_OPCODE_EXTENDED 0x194
INLINE_GEKKO_INSTR(addze);

#define SUBF_OPCODE_EXTENDED 0x050
INLINE_GEKKO_INSTR(subf);

#define SUBFE_OPCODE_EXTENDED 0x110
INLINE_GEKKO_INSTR(subfe);

#define SUBFC_OPCODE_EXTENDED 0x010
INLINE_GEKKO_INSTR(subfc);

#define CMP_OPCODE_EXTENDED 0x000
INLINE_GEKKO_INSTR(cmp);

#define CMPL_OPCODE_EXTENDED 0x040
INLINE_GEKKO_INSTR(cmpl);

#define DIVW_OPCODE_EXTENDED 0x3d6
INLINE_GEKKO_INSTR(divw);

#define DIVWU_OPCODE_EXTENDED 0x396
INLINE_GEKKO_INSTR(divwu);

#define MULLW_OPCODE_EXTENDED 0x1d6
INLINE_GEKKO_INSTR(mullw);

#define ORX_OPCODE_EXTENDED 0x378
INLINE_GEKKO_INSTR(orx);

#define ANDX_OPCODE_EXTENDED 0x038
INLINE_GEKKO_INSTR(andx);

#define XORX_OPCODE_EXTENDED 0x278
INLINE_GEKKO_INSTR(xorx);

#define NOR_OPCODE_EXTENDED 0x0f8
INLINE_GEKKO_INSTR(nor);

#define ANDC_OPCODE_EXTENDED 0x078
INLINE_GEKKO_INSTR(andc);

#define NEG_OPCODE_EXTENDED 0x0d0
INLINE_GEKKO_INSTR(neg);

#define EXTSH_OPCODE_EXTENDED 0x734
INLINE_GEKKO_INSTR(extsh);

#define CNTLZW_OPCODE_EXTENDED 0x034
INLINE_GEKKO_INSTR(cntlzw);

#define SLW_OPCODE_EXTENDED 0x030
INLINE_GEKKO_INSTR(slw);

#define SRW_OPCODE_EXTENDED 0x430
INLINE_GEKKO_INSTR(srw);

#define SRAW_OPCODE_EXTENDED 0x630
INLINE_GEKKO_INSTR(sraw);

#define SRAWI_OPCODE_EXTENDED 0x670
INLINE_GEKKO_INSTR(srawi);

#define STWX_OPCODE_EXTENDED 0x12e
INLINE_GEKKO_INSTR(stwx);

#define LWZX_OPCODE_EXTENDED 0x02e
INLINE_GEKKO_INSTR(lwzx);

#define LHZX_OPCODE_EXTENDED 0x22e
INLINE_GEKKO_INSTR(lhzx);

#define MFMSR_OPCODE_EXTENDED 0x0a6
INLINE_GEKKO_INSTR(mfmsr);

#define MTMSR_OPCODE_EXTENDED 0x124
INLINE_GEKKO_INSTR(mtmsr);

#define MFSPR_OPCODE_EXTENDED 0x2a6
INLINE_GEKKO_INSTR(mfspr);

#define MTSPR_OPCODE_EXTENDED 0x3a6
INLINE_GEKKO_INSTR(mtspr);

#define MFCR_OPCODE_EXTENDED 0x026
INLINE_GEKKO_INSTR(mfcr);

#define MTCRF_OPCODE_EXTENDED 0x120
INLINE_GEKKO_INSTR(mtcrf);

#define MFSR_OPCODE_EXTENDED 0x4a6
INLINE_GEKKO_INSTR(mfsr);

#define MTSR_OPCODE_EXTENDED 0x1a4
INLINE_GEKKO_INSTR(mtsr);

#define MFTB_OPCODE_EXTENDED 0x2e6
INLINE_GEKKO_INSTR(mftb);

#define SYNC_OPCODE_EXTENDED 0x4ac
INLINE_GEKKO_INSTR(sync);

#define DCBF_OPCODE_EXTENDED 0x0ac
INLINE_GEKKO_INSTR(dcbf);

#define DCBI_OPCODE_EXTENDED 0x3ac
INLINE_GEKKO_INSTR(dcbi);

#define ICBI_OPCODE_EXTENDED 0x7ac
INLINE_GEKKO_INSTR(icbi);

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
