#include "PPC/instructions.h"

GEKKO_INSTR(b_x) {
    STATIC_ASSERT
    log("b_x %08x", instruction.raw);
    i32 LI = ((i32)(instruction.branch.LI << 6) >> 4);  // sign extend and ||0b00
    log("%x, %x: %x", LI, instruction.branch.LI, instruction.raw);
    if (instruction.branch.LK) {
        cpu->LR = cpu->PC;
    }

    if (instruction.branch.AA) {
        cpu->PC = (u32)LI;
    }
    else {
        cpu->PC += LI - 4;  // correct for PC being 4 bytes ahead from instruction fetch
    }
}

INLINE_GEKKO_INSTR(bclr_x) {
    STATIC_ASSERT
    log("bclr_x %08x", instruction.raw);

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
    if (do_branch) {
        u32 CIA = cpu->PC - 4;  // Current Instruction Address
        cpu->PC = cpu->LR & (~0x3);
        if (instruction.branch_conditional.LK) {
            cpu->LR = CIA;
        }
    }
}