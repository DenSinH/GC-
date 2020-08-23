#include "PPC/instructions.h"

GEKKO_INSTR(unimplemented) {
    log_fatal("Unimplemented instruction: %08x @0x%08x", instruction.raw, cpu->PC - 4);
}

