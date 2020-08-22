#include <stdio.h>
#include <inttypes.h>

#include "Gekko.h"
#include "../Loader/loader.h"

#include "log.h"


#define IPL_FILE_NAME "files/ipl.bin"
void load_IPL_to_Gekko(s_Gekko* cpu) {
    decrypt_IPL_to(IPL_FILE_NAME, cpu->memory + IPL_START_ADDRESS);
    cpu->PC = GEKKO_PC_INIT_IPL;
}

void load_DOL_to_Gekko(const char file_name[], s_Gekko* cpu) {
    cpu->PC = load_DOL_to(file_name, cpu->memory);
}

#define R32_FORMAT " %02d: %08"  PRIx32 " | "
#define R64_FORMAT " %02d: %016" PRIx64 " | "
void format_Gekko(s_Gekko* cpu) {
    int index = 0;
    char r32_format[sizeof(R32_FORMAT) + 1] = R32_FORMAT;
    char r64_format[sizeof(R64_FORMAT) + 1] = R64_FORMAT;

    for (int i=0; i<32; i++) {
        if (i & 3) {
            r32_format[0] = ' ';
        }
        else {
            r32_format[0] = '\n';
        }

        index += snprintf(&cpu->log_line[index], LOG_LINE_LENGTH - index, r32_format, i, cpu->GPR[i]);
    }

    for (int i=0; i<32; i++) {
        if (i & 1) {
            r64_format[0] = ' ';
        }
        else {
            r64_format[0] = '\n';
        }

        index += snprintf(&cpu->log_line[index], LOG_LINE_LENGTH - index, r64_format, i, GET_FPR(cpu, i));
    }

    index += snprintf(&cpu->log_line[index], LOG_LINE_LENGTH - index, "\nLR: %08x | ", cpu->LR);
    index += snprintf(&cpu->log_line[index], LOG_LINE_LENGTH - index, "SP: %08x | ", cpu->SP);
    index += snprintf(&cpu->log_line[index], LOG_LINE_LENGTH - index, "PC: %08x | ", cpu->PC);
    index += snprintf(&cpu->log_line[index], LOG_LINE_LENGTH - index, "CR: %08x\n", cpu->CR.raw);

    index += snprintf(&cpu->log_line[index], LOG_LINE_LENGTH - index, "MSR: %08x ", cpu->MSR.raw);
    index += snprintf(&cpu->log_line[index], LOG_LINE_LENGTH - index, "CTR: %08x ", cpu->CTR);
    index += snprintf(&cpu->log_line[index], LOG_LINE_LENGTH - index, "FPSCR: %08x ", cpu->FPSCR.raw);
}

void step_Gekko(s_Gekko* cpu) {
    s_gekko_instruction instruction;
    instruction.raw = read32(&cpu->IMMU, cpu->PC);
    cpu->PC += 4;

    cpu->instructions[MAIN_INSTR_HASH(instruction.raw)](cpu, instruction);
    format_Gekko(cpu);
    log_debug("%s", cpu->log_line);
}