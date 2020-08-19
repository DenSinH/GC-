#ifndef GC__INSTRUCTIONS_H
#define GC__INSTRUCTIONS_H

#include "PPC/Gekko.h"
#include "gekko_instruction.h"
#include "default.h"
#include "log.h"

GEKKO_INSTR(unimplemented);

#define B_X_OPCODE 0x48000000
GEKKO_INSTR(b_x);

#define ADDI_OPCODE 0x38000000
GEKKO_INSTR(addi);

#define ADDIS_OPCODE 0x3c000000
GEKKO_INSTR(addis);

#define ORI_OPCODE 0x60000000
GEKKO_INSTR(ori);


GEKKO_INSTR(instr_010011);

#define BCLR_X_OPCODE_EXTENDED 0x020
INLINE_GEKKO_INSTR(bclr_x);

#endif //GC__INSTRUCTIONS_H
