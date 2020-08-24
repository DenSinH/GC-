#include <stdio.h>
#include <inttypes.h>
#include <memory.h>

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
    memset(cpu->log_line, 0x00, sizeof(cpu->log_line));
    int index = 0;
    char r32_format[sizeof(R32_FORMAT) + 1] = R32_FORMAT;
    char r64_format[sizeof(R64_FORMAT) + 1] = R64_FORMAT;

    for (int i=0; i<32; i++) {
        if (i & 1) {
            r64_format[0] = ' ';
        }
        else {
            r64_format[0] = '\n';
        }

        index += snprintf(&cpu->log_line[index], LOG_LINE_LENGTH - index, r64_format, i, GET_FPR(cpu, i));
    }

    for (int i=0; i<32; i++) {
        if (i & 3) {
            r32_format[0] = ' ';
        }
        else {
            r32_format[0] = '\n';
        }

        index += snprintf(&cpu->log_line[index], LOG_LINE_LENGTH - index, r32_format, i, cpu->GPR[i]);
    }

    index += snprintf(&cpu->log_line[index], LOG_LINE_LENGTH - index, "\nLR: %08x | ", cpu->LR);
    index += snprintf(&cpu->log_line[index], LOG_LINE_LENGTH - index, "SP: %08x | ", cpu->SP);
    index += snprintf(&cpu->log_line[index], LOG_LINE_LENGTH - index, "PC: %08x | ", cpu->PC);
    index += snprintf(&cpu->log_line[index], LOG_LINE_LENGTH - index, "CR: %08x\n", cpu->CR.raw);

    index += snprintf(&cpu->log_line[index], LOG_LINE_LENGTH - index, "MSR: %08x ", cpu->MSR.raw);
    index += snprintf(&cpu->log_line[index], LOG_LINE_LENGTH - index, "CTR: %08x ", cpu->CTR);
    index += snprintf(&cpu->log_line[index], LOG_LINE_LENGTH - index, "FPSCR: %08x ", cpu->FPSCR.raw);
    index += snprintf(&cpu->log_line[index], LOG_LINE_LENGTH - index, "XER: %08x ", cpu->XER.raw);
}

void dump_Gekko(s_Gekko* cpu) {
    format_Gekko(cpu);
    log_warn("CPU dump:\n%s", cpu->log_line);
}

void dump_Gekko_mem_range(s_Gekko* cpu, u32 start, u32 end) {
    FILE* file;
    fopen_s(&file, GEKKO_DUMP_FILE_NAME, "wb");

    u32 written_length = fwrite(cpu->memory + start, 1, end - start, file);
    if (written_length != end - start) {
        log_fatal("Error writing to file: wrote %x bytes, should have written %x", written_length, end - start);
    }
    fclose(file);
}

void dump_Gekko_stack_trace(s_Gekko* cpu) {
#ifdef DO_CALL_STACK
    memset(cpu->log_line, 0x00, sizeof(cpu->log_line));
    char r32_format[sizeof(R32_FORMAT) + 1] = R32_FORMAT;
    r32_format[sizeof(R32_FORMAT) - 1] = '\n';

    int index = 0;
    for (int i=0; i < cpu->call_stack_pointer; i++) {
        index += snprintf(&cpu->log_line[index], LOG_LINE_LENGTH - index, r32_format, i, cpu->GPR[i]);
    }
#endif
}

void step_Gekko(s_Gekko* cpu) {
    s_gekko_instruction instruction;
    instruction.raw = read32(&cpu->IMMU, cpu->PC);
    cpu->PC += 4;
    cpu->TBR++;

    cpu->instructions[MAIN_INSTR_HASH(instruction.raw)](cpu, instruction);
}