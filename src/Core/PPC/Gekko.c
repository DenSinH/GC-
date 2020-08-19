#include <stdio.h>

#include "Gekko.h"
#include "instructions.h"
#include "../IPL/loader.h"

#include "log.h"


#define IPL_START_ADDRESS 0x1300000
#define IPL_FILE_NAME "./files/ipl.bin"

void init_Gekko(s_Gekko* cpu) {
    cpu->IMMU.RAM = cpu->DMMU.RAM = cpu->memory;

    cpu->PC = 0x81300000;

    build_instr_table(cpu);
}

void build_instr_table(s_Gekko* cpu) {
    for (u32 i = 0; i < MAIN_INSTR_TABLE_SIZE; i++) {
        switch (i) {
            case MAIN_INSTR_HASH(B_X_OPCODE):
                cpu->instructions[i] = b_x;
                break;
            case MAIN_INSTR_HASH(ADDI_OPCODE):
                cpu->instructions[i] = addi;
                break;
            case MAIN_INSTR_HASH(ADDIS_OPCODE):
                cpu->instructions[i] = addis;
                break;
            case MAIN_INSTR_HASH(ORI_OPCODE):
                cpu->instructions[i] = ori;
                break;
            case 0b010011:
                cpu->instructions[i] = instr_010011;
                break;
            default:
                cpu->instructions[i] = unimplemented;
                break;
        }
    }
}

void load_IPL_to_Gekko(s_Gekko* cpu) {
    decrypt_IPL_to(IPL_FILE_NAME, cpu->memory + IPL_START_ADDRESS);
}

#define GPR_FORMAT "%02d: %08x | "
void format_Gekko(s_Gekko* cpu) {
    int index = 0;
    char format[sizeof(GPR_FORMAT) + 2] = GPR_FORMAT;
    for (int i=0; i<32; i++) {
        if ((i + 1) & 3) {
            format[sizeof(GPR_FORMAT) - 1] = ' ';
        }
        else {
            format[sizeof(GPR_FORMAT) - 1] = '\n';
        }

        index += snprintf(&cpu->log_line[index], LOG_LINE_LENGTH - index, format, i, cpu->GPR[i]);
    }

    index += snprintf(&cpu->log_line[index], LOG_LINE_LENGTH - index, "\nLR: %08x\n", cpu->LR);
    index += snprintf(&cpu->log_line[index], LOG_LINE_LENGTH - index, "SP: %08x\n", cpu->SP);
    index += snprintf(&cpu->log_line[index], LOG_LINE_LENGTH - index, "PC: %08x\n", cpu->PC);
    snprintf(&cpu->log_line[index], LOG_LINE_LENGTH - index, "CR: %08x\n", cpu->CR.raw);
}

void step_Gekko(s_Gekko* cpu) {
    s_gekko_instruction instruction;
    instruction.raw = read32(&cpu->IMMU, cpu->PC);
    cpu->PC += 4;

    cpu->instructions[MAIN_INSTR_HASH(instruction.raw)](cpu, instruction);
    format_Gekko(cpu);
    log_debug("%s", cpu->log_line);
}