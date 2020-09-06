#ifndef GC__GEKKO_H
#define GC__GEKKO_H

#include <stdbool.h>
#include <memory.h>
#include <math.h>

#include "MMU.h"
#include "Registers/CR.h"
#include "Registers/FPSCR.h"
#include "Registers/MSR.h"
#include "Registers/XER.h"
#include "Registers/GQR.h"
#include "Registers/HID.h"
#include "Registers/FPR.h"
#include "Registers/TBR.h"
#include "Registers/WPAR.h"
#include "HWregs/hardware_registers.h"
#include "gekko_instruction.h"

#include "default.h"
#include "log.h"
#include "float_utils.h"

#define FULL_WRITE_MASK 0xffffffff

#define LOG_LINE_LENGTH 0x800
#define CALL_STACK_LENGTH 0x100
#define DOL_FILE_NAME_LENGTH 0x100

#define SP GPR[1]
#define GET_TBL(cpu_ptr) (u32)(cpu_ptr)->TBR.time
#define GET_TBU(cpu_ptr) (u32)((cpu_ptr)->TBR.time >> 32)

typedef struct s_Gekko {
    struct s_GameCube* system;

    u32 GPR[32];      // General purpose Registers
    s_FPR FPR[32];    // Floating-point Registers

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

    u32 DEC;        // Decrement register
    s_TBR TBR;      // Time base register

    s_GQR GQR[8];
    u32 HID[2];     // hardware dependent Registers (todo: stubbed)
    s_HID2 HID2;    // hardware dependent Registers (todo: stubbed)
    s_WPAR WPAR;

    u32 SR[16];

    u32 PC;         // Program counter
    u32 LR;         // Link register

    void (*instructions[MAIN_INSTR_TABLE_SIZE])(struct s_Gekko* cpu, s_gekko_instruction instruction);

    /* Debugging stuff */
    char log_line[LOG_LINE_LENGTH];
#ifdef DO_CALL_STACK
    u32 call_stack[CALL_STACK_LENGTH];
    u8 call_stack_pointer;
#endif
    char dol_file_name[DOL_FILE_NAME_LENGTH];
} s_Gekko;

//                                 LT    , EQ    , GT
const static u8 CR_settings[3] = { 0b1000, 0b0010, 0b0100 };
#define UPDATE_CR0_RESULT32(cpu, result) cpu->CR.direct.CR0 = cpu->XER.SO | CR_settings[((i32)(result) >= 0) + ((i32)(result) > 0)]
#define GET_CRn_CMP_I32(cpu, op1, op2) (cpu->XER.SO | CR_settings[((i32)(op1) >= (i32)(op2)) + ((i32)(op1) > (i32)(op2))])
#define GET_CRn_CMP_U32(cpu, op1, op2) (cpu->XER.SO | CR_settings[((op1) >= (op2)) + ((op1) > (op2))])

#define UPDATE_CR1_RESULT_FLOAT(cpu, result) cpu->CR.direct.CR1 = ((isnan(op1) || isnan(op2)) ? 0b0001 : CR_settings[((op1) >= (op2)) + ((op1) > (op2))])
#define GET_CRn_CMP_FLOAT(op1, op2) ((isnan(op1) || isnan(op2)) ? 0b0001 : CR_settings[((op1) >= (op2)) + ((op1) > (op2))])

static inline void UPDATE_FPRF_RESULT_BIT_DOUBLE(s_Gekko* cpu, bit_double result) {
    cpu->FPSCR.FPRF = (isnan(result.d) || isinf(result.d)) | CR_settings[((result.d) >= 0) + ((result.d) > 0)];
    if (isnan(result.d) || (!(result.u & DOUBLE_QBIT) && result.u != 0)) {
        // NaN, denormalized or -0
        cpu->FPSCR.FPRF |= 0b10000;
    }
}

static inline u64 GET_FPR(s_Gekko* cpu, unsigned int index) {
    if (cpu->HID2.PSE && cpu->HID2.LSQE) {
        return (cpu->FPR[index].PS0.u << 32) | cpu->FPR[index].PS1.u;
    }
    return cpu->FPR[index].PS0.u;
}

#define GEKKO_PC_INIT_IPL 0x81300000
void init_Gekko(s_Gekko* cpu);
void build_instr_table(s_Gekko* cpu);

void dump_Gekko(s_Gekko* cpu);
void dump_Gekko_stack_trace(s_Gekko* cpu);
#define GEKKO_DUMP_FILE_NAME "./files/gekko.dump"
void dump_Gekko_mem_range(s_Gekko* cpu, u32 start, u32 end);
#define IPL_START_ADDRESS 0x1300000
void load_IPL_to_Gekko(s_Gekko* cpu);

void load_DOL_to_Gekko(s_Gekko* cpu, const char file_name[]);
void format_Gekko(s_Gekko* cpu);
void step_Gekko(s_Gekko* cpu);


#endif //GC__GEKKO_H
