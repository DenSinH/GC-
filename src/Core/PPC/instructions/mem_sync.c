#include "PPC/instructions.h"


INLINE_GEKKO_INSTR(sync) {
    ASSERT_BITFIELD_SIZE
    log_cpu("sync %08x", instruction.raw);
}