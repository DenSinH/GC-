#include "PPC/instructions.h"

GEKKO_INSTR(unimplemented) {
    dump_Gekko(cpu);
    log_fatal("Unimplemented instruction: %08x @%08x", instruction.raw, cpu->PC - 4);
}

