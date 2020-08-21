#include "PPC/instructions.h"

GEKKO_INSTR(instr_000100) {
    // note: for this aggregate instruction, some have 5 bits to signify the instruction, others 10
    // I want to handle the 10bit case in the default case for the statement
    switch (instruction.raw & 0x03e) {
        // bottom 6 - 1 bits
        default:
            switch (instruction.raw & 0x7fe) {
                // bottom 11 bits
                case PS_MR_OPCODE_EXTENDED_10bit:
                    ps_mr(cpu, instruction);
                    return;
                default:
                log_fatal("Unimplemented instruction: %08x @%08x", instruction.raw, cpu->PC - 4);
            }
    }
}

GEKKO_INSTR(instr_010011) {
    switch (instruction.raw & 0x7fe) {
        // bottom 11 - 1 bits
        case BCLR_X_OPCODE_EXTENDED:
            bclr_x(cpu, instruction);
            return;
        case CRXOR_OPCODE_EXTENDED:
            crxor(cpu, instruction);
            return;
        case ISYNC_OPCODE_EXTENDED:
            isync(cpu, instruction);
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

GEKKO_INSTR(instr_111111) {
    // note: for this aggregate instruction, some have 5 bits to signify the instruction, others 10
    // I want to handle the 10bit case in the default case for the statement
    switch (instruction.raw & 0x03e) {
        // bottom 6 - 1 bits
        default:
            switch (instruction.raw & 0x7fe) {
                // bottom 11 bits
                case FMR_OPCODE_EXTENDED_10bit:
                    fmr(cpu, instruction);
                    return;
                case MTFSF_OPCODE_EXTENDED_10bit:
                    mtfsf(cpu, instruction);
                    return;
                default:
                log_fatal("Unimplemented instruction: %08x @%08x", instruction.raw, cpu->PC - 4);
            }
    }
}