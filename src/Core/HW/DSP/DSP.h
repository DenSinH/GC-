#ifndef GC__DSP_H
#define GC__DSP_H

#include "default.h"

#include <stdbool.h>

#define DSP_CLOCK_FREQUENCY 81000000

typedef union s_ax {
    struct {
        unsigned l: 16;
        unsigned h: 16;
    };

    unsigned lh;
} s_ax;

STATIC_ASSERT(sizeof(s_ax) == 4, "DSP.ax union was the wrong size!");

typedef union s_ac {
    struct {
        u64 l: 16;
        u64 m: 16;
        u64 h: 16;
    };

    struct {
        i64 lmh: 40;
    };

    u64 raw;
} s_ac;

typedef union s_prod {
    struct {
        u64 l: 16;
        u64 m: 16;
        u64 h: 16;
        u64 m2: 16;  // I don't know what this does...
    };
    u64 raw;
} s_prod;

typedef union s_sr {
    struct {
        unsigned C:   1;  // carry
        unsigned O:   1;  // overflow
        unsigned Z:   1;  // arithmetic zero
        unsigned S:   1;  // sign
        unsigned AS:  1;  // above s32
        unsigned TT:  1;  // top 2 bits are equal
        unsigned LZ:  1;  // logic zero
        unsigned OS:  1;  // overflow (sticky)
        unsigned   :  1;  // always 0?
        unsigned IE:  1;  // interrupt enable
        unsigned   :  1;
        unsigned EIE: 1;  // external interrupt enable
        unsigned:     1;
        unsigned AM:  1;  // product multiply result by 2 (when AM = 0)
        unsigned SXM: 1;  // sign extension mode (0 = set16, 1 = set40)
        unsigned SU:  1;  // operands are signed (1 = unsigned)
    };

    u16 raw;
} s_sr;

typedef enum e_DSP_exception_vectors {
    DSP_exception_RESET = 0x0000,
    DSP_exception_STOVF = 0x0002,  // stack under/overflow
    DSP_exception_ACCOV = 0x000a,  // accumulator address overflow
    DSP_exception_INT   = 0x000e,  // external interrupt (from CPU)
} e_DSP_exception_vectors;

typedef enum e_DSP_IO {
    DSP_IO_CMBH  = 0xfffe,
    DSP_IO_CMBL  = 0xffff,
    DSP_IO_DMBH  = 0xfffc,
    DSP_IO_DMBL  = 0xfffd,

    DSP_IO_DSMAH = 0xffce,
    DSP_IO_DSMAL = 0xffcf,
    DSP_IO_DSPA  = 0xffcd,
    DSP_IO_DSCR  = 0xffc9,
    DSP_IO_DSBL  = 0xffcb,

    DSP_IO_ACSAH = 0xffd4,
    DSP_IO_ACSAL = 0xffd5,
    DSP_IO_ACEAH = 0xffd6,
    DSP_IO_ACEAL = 0xffd7,
    DSP_IO_ACCAH = 0xffd8,
    DSP_IO_ACCAL = 0xffd9,
    DSP_IO_ACDAT = 0xffdd,

    DSP_IO_DIRQ = 0xfffb,
} e_DSP_IO;

typedef enum e_DSP_regs {
    DSP_reg_ar0 = 0x00,
    DSP_reg_ar1 = 0x01,
    DSP_reg_ar2 = 0x02,
    DSP_reg_ar3 = 0x03,

    DSP_reg_ix0 = 0x04,
    DSP_reg_ix1 = 0x05,
    DSP_reg_ix2 = 0x06,
    DSP_reg_ix3 = 0x07,

    DSP_reg_wr0 = 0x08,
    DSP_reg_wr1 = 0x09,
    DSP_reg_wr2 = 0x0a,
    DSP_reg_wr3 = 0x0b,

    DSP_reg_st0 = 0x0c,
    DSP_reg_st1 = 0x0d,
    DSP_reg_st2 = 0x0e,
    DSP_reg_st3 = 0x0f,

    DSP_reg_ac0h = 0x10,
    DSP_reg_ac1h = 0x11,

    DSP_reg_config = 0x12,
    DSP_reg_sr = 0x13,

    DSP_reg_prodl = 0x14,
    DSP_reg_prodm = 0x15,
    DSP_reg_prodh = 0x16,
    DSP_reg_prodm2 = 0x17,

    DSP_reg_ax0l = 0x18,
    DSP_reg_ax1l = 0x19,
    DSP_reg_ax0h = 0x1a,
    DSP_reg_ax1h = 0x1b,

    DSP_reg_ac0l = 0x1c,
    DSP_reg_ac1l = 0x1d,
    DSP_reg_ac0m = 0x1e,
    DSP_reg_ac1m = 0x1f,
} e_DSP_regs;

typedef enum e_DSP_cond {
    DSP_cond_GE     = 0b0000,
    DSP_cond_L      = 0b0001,
    DSP_cond_G      = 0b0010,
    DSP_cond_LE     = 0b0011,
    DSP_cond_NE     = 0b0100,
    DSP_cond_EQ     = 0b0101,
    DSP_cond_NC     = 0b0110,
    DSP_cond_C      = 0b0111,
    DSP_cond_B32    = 0b1000,
    DSP_cond_A32    = 0b1001,
    DSP_cond_NZ     = 0b1100,
    DSP_cond_ZR     = 0b1101,
    DSP_cond_O      = 0b1110,
    DSP_cond_ALWAYS = 0b1111,
} e_DSP_cond;

typedef enum e_DSP_CR {
    DSP_CR_EXTERNAL_INT = 0x0002,
    DSP_CR_HALT         = 0x0004,
    DSP_CR_INIT         = 0x0400,
} e_DSP_CR;

typedef enum e_DSP_int {
    DSP_int_RESET = 0x0000,
    DSP_int_STOVF = 0x0002,
    DSP_int_ACCOV = 0x0004,
    DSP_int_INT   = 0x000E,  // external
} e_DSP_int;

#define DSP_STACK_SIZE 0x20
#define DSP_STACK_MASK 0x1f
#define DSP_INSTR_TABLE_SIZE 0x100
#define DSP_EXT_INSTR_TABLE_SIZE 0x100

#define DSP_INSTR(_name) int _name(struct s_DSP* DSP, u16 instruction)
#define DSP_EXT_INSTR(_name) void _name(struct s_DSP* DSP, u8 instruction)

typedef struct s_DSP {
    // todo:
    bool started;

    bool halted;

    u8 ARAM[0x1000000];  // 16MB ARAM
    /*
     * Instruction memory:
     * 0x0000
     *  IRAM
     *  mapped through the first 8kB of ARAM
     * 0x0FFF
     * 0x8000
     *  IROM
     * 0x8FFF
     *
     * Data memory:
     * 0x0000
     *  DRAM
     * 0x0FFF
     * 0x8000
     *  DROM
     * 0x8FFF
     * 0xFFFB
     *  HWIO
     * 0xFFFF
     *
     * Memory is addressed as DSP words (16bit)
     *
     * Since this is also Big Endian, we keep it stored as byte arrays anyway
     * */
    u8 DRAM[2 * 0x1000]; // 4Kword data RAM
    u8 DROM[2 * 0x800];  // 2Kword data ROM
    // IRAM is mapped to ARAM start
    u8 IROM[2 * 0x1000]; // 4Kword instruction ROM

    u16 ar[4];     // addressing registers
    u16 ix[4];     // indexing registers
    u16 wr[4];     // wrapping registers
    /*
     * The GameCube DSP has 4 stack pointers:
     *  - st0 : Call stack register
     *  - st1 : Data stack register
     *  - st2 : Loop address stack register
     *  - st3 : Loop counter stack register
     *
     *  Stacks are implemented in hardware and have limited depth. The data stack is limited to four values
     *  and the call stack is limited to eight values. The loop stack is limited to four values. Upon underflow or
     *  overflow of any of the stack registers exception STOVF is raised.
     *  The loop stack is used to control execution of repeated blocks of instructions. Whenever there is a value
     *  in $st2 and the current PC is equal to the value in $st2, then the value in $st3 is decremented. If the
     *  value is not zero, then the PC is modified by the value from call stack $st0. Otherwise values from the
     *  call stack $st0 and both loop stacks, $st2 and $st3, are popped and execution continues at the next
     *  opcode.
     * */
    u16 st[4];     // stack pointers
    s_ac ac[2];    // 40-bit accumulators
    s_ax ax[2];    // 32-bit accumulators
    s_prod prod;   // product register
    u16 config;    // config register (unknown purpose)
    s_sr sr;

    u16* r[0x20];  // pointers to registers to access directly
    u16 stacks[4][DSP_STACK_SIZE];

    u16 pc;

    u16 CMBH, CMBL;  // CPU -> DSP
    u16 DMBH, DMBL;  // DSP -> CPU
    u16 DSMAH, DSMAL;
    u16 DSPA, DSCR, DSBL;

    u16 ACSAH, ACSAL;
    u16 ACEAH, ACEAL;
    u16 ACCAH, ACCAL;
    u16 ACDAT;

    u16 DIRQ;

    DSP_INSTR((*instructions[DSP_INSTR_TABLE_SIZE]));
    DSP_EXT_INSTR((*ext_instructions[DSP_EXT_INSTR_TABLE_SIZE]));

} s_DSP;

void init_DSP(s_DSP* DSP, const char* IROM_file, const char* DROM_file);
void DSP_interrupt(s_DSP* DSP, e_DSP_int interrupt);
bool DSP_halted(s_DSP* DSP);
void halt_DSP(s_DSP* DSP);
void unhalt_DSP(s_DSP* DSP);
int step_DSP(s_DSP* DSP);

#endif //GC__DSP_H
