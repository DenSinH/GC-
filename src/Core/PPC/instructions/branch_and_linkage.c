#include "instructions.h"

GEKKO_INSTR(b_x) {
    GEKKO_INSTR_HEADER
    log_cpu("b_x %08x", instruction.raw);
    i32 LI = ((i32)(instruction.branch.LI << 8) >> 6);  // sign extend and ||0b00

    if (instruction.branch.LK) {
        cpu->LR = cpu->PC;
#ifdef DO_CALL_STACK
        cpu->call_stack[cpu->call_stack_pointer++] = cpu->PC - 4;
#endif
    }

    if (instruction.branch.AA) {
        cpu->PC = (u32)LI;
    }
    else {
        cpu->PC += LI - 4;  // correct for PC being 4 bytes ahead from instruction fetch
    }
}

static inline bool conditional_branch_condition(s_Gekko* cpu, s_gekko_instruction instruction) {
    // bottom bit: branch likely
    if (!(instruction.branch_conditional.BO & 0b00100)) {
        // if !BO[2]: CTR <- CTR - 1
        cpu->CTR--;
    }

    bool do_branch = false;
    bool condition = cpu->CR.raw & (0x80000000 >> instruction.branch_conditional.BI);
    switch (instruction.branch_conditional.BO) {
        case 0b00000:
        case 0b00001:
            do_branch = cpu->CTR != 0 && !condition;
            break;
        case 0b00010:
        case 0b00011:
            do_branch = cpu->CTR == 0 && !condition;
            break;
        case 0b00100 ... 0b00111:
            do_branch = !condition;
            break;
        case 0b01000:
        case 0b01001:
            do_branch = cpu->CTR != 0 && condition;
            break;
        case 0b01010:
        case 0b01011:
            do_branch = cpu->CTR == 0 && condition;
            break;
        case 0b01100 ... 0b01111:
            do_branch = condition;
            break;
        case 0b10000:
        case 0b10001:
        case 0b11000:
        case 0b11001:
            do_branch = cpu->CTR != 0;
            break;
        case 0b10010:
        case 0b10011:
        case 0b11010:
        case 0b11011:
            do_branch = cpu->CTR == 0;
            break;
        default:
            do_branch = true;
            break;
    }
    return do_branch;
}

GEKKO_INSTR(bc_x) {
    GEKKO_INSTR_HEADER
    log_cpu("bc_x %08x", instruction.raw);

    bool do_branch = conditional_branch_condition(cpu, instruction);
    if (do_branch) {
        u32 CIA = cpu->PC - 4;  // Current Instruction Address
        cpu->PC = (i32)((i16)(instruction.branch_conditional.BD << 2));
        if (!instruction.branch_conditional.AA) {
            cpu->PC += CIA;
        }

        if (instruction.branch_conditional.LK) {
            cpu->LR = CIA + 4;
#ifdef DO_CALL_STACK
            cpu->call_stack[cpu->call_stack_pointer++] = CIA;
#endif
        }
        log_cpu("branch taken -> %08x", cpu->PC);
    }
}

INLINE_GEKKO_INSTR(bclr_x) {
    GEKKO_INSTR_HEADER
    log_cpu("bclr_x %08x", instruction.raw);

    bool do_branch = conditional_branch_condition(cpu, instruction);
    if (do_branch) {
        log_cpu("branch taken");
        u32 CIA = cpu->PC - 4;  // Current Instruction Address
        cpu->PC = cpu->LR & (~0x3);

#ifdef DO_CALL_STACK
        cpu->call_stack_pointer--;
#endif

        if (instruction.branch_conditional.LK) {
            cpu->LR = CIA;
#ifdef DO_CALL_STACK
            cpu->call_stack[cpu->call_stack_pointer++] = CIA;
#endif
        }
    }
}

INLINE_GEKKO_INSTR(bcctr_x) {
    GEKKO_INSTR_HEADER
    log_cpu("bcctr_x %08x", instruction.raw);

    bool do_branch = conditional_branch_condition(cpu, instruction);
    if (do_branch) {
        log_cpu("branch taken");
        u32 CIA_plus_4 = cpu->PC;  // Current Instruction Address
        cpu->PC = cpu->CTR & (~0x3);

        if (instruction.branch_conditional.LK) {
            cpu->LR = CIA_plus_4;
#ifdef DO_CALL_STACK
            cpu->call_stack[cpu->call_stack_pointer++] = CIA_plus_4 - 4;
#endif
        }
    }
}

// bit 13 is forced to 0
#define RFI_SC_SRR1_MASK 0x87c0ff73
GEKKO_INSTR(sc) {
    GEKKO_INSTR_HEADER
    log_cpu("sc %08x", instruction.raw);
    cpu->SRR0 = cpu->PC;    // PC is 4 ahead already
    cpu->SRR1 = cpu->MSR.raw & RFI_SC_SRR1_MASK;

    // from Dolphin:
    cpu->MSR.LE = cpu->MSR.ILE;
    cpu->MSR.raw &= ~0x04EF36;

    cpu->PC = 0x00000c00;
}

#define RFI_MSR_MASK_INV 0x87c4ff73
INLINE_GEKKO_INSTR(rfi) {
    GEKKO_INSTR_HEADER
    log_cpu("rfi %08x", instruction.raw);
    cpu->MSR.raw = (cpu->MSR.raw & ~RFI_MSR_MASK_INV) | (cpu->SRR1 & RFI_SC_SRR1_MASK);
    cpu->PC = cpu->SRR0 & 0xfffffffc;
}