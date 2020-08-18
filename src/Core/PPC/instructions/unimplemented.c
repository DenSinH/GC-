#include "instructions.h"

GEKKO_INSTR(unimplemented) {
    log_fatal("Unimplemented instruction: %02x", instruction.raw);
}

