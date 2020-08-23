#ifndef GC__GEKKO_H
#define GC__GEKKO_H

#include <stdbool.h>
#include <memory.h>

#include "MMU.h"
#include "registers/condition_register.h"
#include "registers/floating_point_status_condition_register.h"
#include "registers/MSR.h"
#include "registers/XER.h"
#include "registers/GQR.h"
#include "registers/HID.h"
#include "registers/FPR.h"
#include "registers/SPR_general.h"
#include "registers/hardware_registers.h"
#include "gekko_instruction.h"

#include "default.h"
#include "log.h"

#define FULL_WRITE_MASK 0xffffffff

#define LOG_LINE_LENGTH 0x800
#define SP GPR[1]

typedef struct s_Gekko {
    u8 memory[0x1800000];
    s_hardware_registers HW_regs;

    u32 GPR[32];      // General purpose registers
    s_FPR FPR[32];    // Floating-point registers

    void* SPR[1024];  // pointers to SPRs (encoding in manual)
    u32 SPR_write_mask[1024]; // SPRs write masks
    u32 default_SPR[1024];    // some SPRs with default reads/writes

    s_MMU IMMU;     // Instruction MMU
    s_MMU DMMU;     // Data MMU

    u32 CTR;        // Count register
    s_CR CR;        // Condition register
    s_FPSCR FPSCR;  // Floating point status and condition register
    s_MSR MSR;      // Machine status register [SUPERVISOR]
    s_XER XER;      // fixed point exception register

    u32 SRR0;       // Save/restore register for address on interrupt         [SUPERVISOR]
    u32 SRR1;       // Save/restore register for machine status on interrupt  [SUPERVISOR]

    s_GQR GQR[8];
    u32 HID[0];     // hardware dependent registers (todo: stubbed)
    s_HID2 HID2;     // hardware dependent registers (todo: stubbed)

    u32 SR[16];

    u32 PC;         // Program counter
    u32 LR;         // Link register

    void (*instructions[MAIN_INSTR_TABLE_SIZE])(struct s_Gekko* cpu, s_gekko_instruction instruction);

    /* Debugging stuff */
    char log_line[LOG_LINE_LENGTH];
} s_Gekko;

//                                 LT    , EQ    , GT
const static u8 CR_settings[3] = { 0b1000, 0b0010, 0b0100 };
#define UPDATE_CR0_RESULT32(cpu, result) cpu->CR.direct.CR0 = cpu->XER.SO | CR_settings[((i32)result >= 0) + ((i32)result > 0)]
#define GET_CRn_CMP_I32(cpu, op1, op2) (cpu->XER.SO | CR_settings[((i32)(op1) >= (i32)(op2)) + ((i32)(op1) > (i32)(op2))])
#define GET_CRn_CMP_U32(cpu, op1, op2) (cpu->XER.SO | CR_settings[((op1) >= (op2)) + ((op1) > (op2))])

static inline void LOAD_PAIRED_SINGLE(s_FPR* FPR, void* PS0, void* PS1) {
    memcpy((void*)&(FPR->PS1), PS1, 4);
    memcpy((void*)&(FPR->PS0), PS0, 4);
}

static inline u64 GET_FPR(s_Gekko* cpu, unsigned int index) {
    if (cpu->HID2.PSE && cpu->HID2.LSQE) {
        return (cpu->FPR[index].PS0 << 32) | cpu->FPR[index].PS1;
    }
    return cpu->FPR[index].PS0;
}

#define SPR_SIZE_BYTES 4
static inline void SET_SPR(s_Gekko* cpu, e_SPR SPR, u32* value) {
    *value &= cpu->SPR_write_mask[SPR];
    memcpy((void*)cpu->SPR[SPR], (void*)value, SPR_SIZE_BYTES);
}

static inline void GET_SPR(s_Gekko* cpu, e_SPR SPR, void* target) {
    memcpy(target, (void*)cpu->SPR[SPR], SPR_SIZE_BYTES);
}

#define GEKKO_PC_INIT_IPL 0x81300000
void init_Gekko(s_Gekko* cpu);
void build_instr_table(s_Gekko* cpu);

void dump_Gekko(s_Gekko* cpu);
#define IPL_START_ADDRESS 0x1300000
void load_IPL_to_Gekko(s_Gekko* cpu);

#define DOL_FILE_OFFSET 0x3000  // DOL file loaded into 80003000
void load_DOL_to_Gekko(const char file_name[], s_Gekko* cpu);
void format_Gekko(s_Gekko* cpu);
void step_Gekko(s_Gekko* cpu);


#endif //GC__GEKKO_H
