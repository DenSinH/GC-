#ifndef GC__GEKKO_H
#define GC__GEKKO_H

#include <stdbool.h>

#include "MMU.h"
#include "registers/condition_register.h"
#include "registers/floating_point_status_condition_register.h"
#include "registers/MSR.h"
#include "registers/XER.h"
#include "gekko_instruction.h"

#include "default.h"


#define LOG_LINE_LENGTH 511


typedef struct s_Gekko {
    u8 memory[0x1800000];

    u32 GPR[32];    // General purpose registers
    u64 FPR[32];    // Floating-point registers

    s_MMU IMMU;     // Instruction MMU
    s_MMU DMMU;     // Data MMU

    u32 CTR;        // Count register
    s_CR CR;        // Condition register
    s_FPSCR FPSCR;  // Floating point status and condition register
    s_MSR MSR;      // Machine status register [SUPERVISOR]
    s_XER XER;      // fixed point exception register

    u32 SRR0;       // Save/restore register for address on interrupt         [SUPERVISOR]
    u32 SRR1;       // Save/restore register for machine status on interrupt  [SUPERVISOR]

    u32 GQR[8];

    u32 PC;         // Program counter
    u32 LR;         // Link register

    void (*instructions[INSTR_TABLE_SIZE])(struct s_Gekko* cpu, s_gekko_instruction instruction);

    /* Debugging stuff */
    char log_line[LOG_LINE_LENGTH];
} s_Gekko;

void init_Gekko(s_Gekko* cpu);
void build_instr_table(s_Gekko* cpu);
void load_IPL_to_Gekko(s_Gekko* cpu);
void format_Gekko(s_Gekko* cpu);
void step_Gekko(s_Gekko* cpu);


#endif //GC__GEKKO_H
