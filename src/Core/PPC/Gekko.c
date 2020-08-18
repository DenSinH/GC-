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
    for (u32 i = 0; i < INSTR_TABLE_SIZE; i++) {
        cpu->instructions[i] = unimplemented;
    }
}

void load_IPL_to_Gekko(s_Gekko* cpu) {
    decrypt_IPL_to(IPL_FILE_NAME, cpu->memory + IPL_START_ADDRESS);
}

void format_Gekko(s_Gekko* cpu) {
    int index = 0;
    for (int i=0; i<32; i++)
        index += snprintf(&cpu->log_line[index], LOG_LINE_LENGTH - index, "%02d: %08x\n", i, cpu->GPR[i]);

    index += snprintf(&cpu->log_line[index], LOG_LINE_LENGTH - index, "LR: %08x\n", cpu->LR);
    index += snprintf(&cpu->log_line[index], LOG_LINE_LENGTH - index, "PC: %08x\n", cpu->PC);
    snprintf(&cpu->log_line[index], LOG_LINE_LENGTH - index, "CR: %08x\n", cpu->CR);
}

void step_Gekko(s_Gekko* cpu) {
    s_gekko_instruction instruction;
    instruction.raw = read32(&cpu->IMMU, cpu->PC);
    cpu->PC += 4;

    cpu->instructions[INSTR_HASH(instruction.raw)](cpu, instruction);
    format_Gekko(cpu);
    log_debug("%s", cpu->log_line);
}