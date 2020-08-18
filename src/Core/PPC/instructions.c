#include "instructions.h"
#include "Gekko.h"

#include "default.h"
#include "log.h"

GEKKO_INSTR(unimplemented) {
    log_fatal("Unimplemented instruction: %02x", instruction.raw);
}