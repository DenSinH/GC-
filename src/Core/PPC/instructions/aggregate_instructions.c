#include "PPC/instructions.h"

GEKKO_INSTR(instr_010011) {
    switch (instruction.raw & 0x7fe) {
        // bottom 11 - 1 bits
        case BCLR_X_OPCODE_EXTENDED:
            bclr_x(cpu, instruction);
            return;
        default:
            log_fatal("Unimplemented instruction: %08x @%08x", instruction.raw, cpu->PC - 4);

    }
}

GEKKO_INSTR(instr_011111) {
    /* 0x7c000000 */
    switch (instruction.raw & 0x7fe) {
        // bottom 11 - 1 bits
        case MFMSR_OPCODE_EXTENDED:
            mfmsr(cpu, instruction);
            return;
        case MTMSR_OPCODE_EXTENDED:
            mtmsr(cpu, instruction);
            return;
        case MFSPR_OPCODE_EXTENDED:
            mfspr(cpu, instruction);
            return;
        case MTSPR_OPCODE_EXTENDED:
            mtspr(cpu, instruction);
            return;
        case SYNC_OPCODE_EXTENDED:
            sync(cpu, instruction);
            return;
        default:
            log_fatal("Unimplemented instruction: %08x @%08x", instruction.raw, cpu->PC - 4);
    }
}