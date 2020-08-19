#include "PPC/instructions.h"

GEKKO_INSTR(instr_010011) {
    switch (instruction.raw & 0x7fe) {
        // bottom 11 - 1 bits
        case BCLR_X_OPCODE_EXTENDED:
            bclr_x(cpu, instruction);
            break;
        default:
            log_fatal("Unimplemented instruction: %08x @%08x", instruction.raw, cpu->PC - 4);

    }
}