#ifndef GC__DSP_H
#define GC__DSP_H

#include "DSP_instruction.h"

#include "default.h"

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
        u64 lmh: 40;
    };
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

#define DSP_STACK_SIZE 0x20
#define DSP_STACK_MASK 0x1f
#define DSP_INSTR_TABLE_SIZE 0x100

typedef struct s_DSP {
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
    u8 IRAM[2 * 0x1000]; // 4Kword instruction RAM
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
} s_DSP;

static inline void DSP_PUSH_STACK(s_DSP* DSP, unsigned index, u16 value) {
    DSP->stacks[index][DSP->st[index]++] = value;
}

static inline u16 DSP_POP_STACK(s_DSP* DSP, unsigned index) {
    return DSP->stacks[index][--DSP->st[index]];
}

void init_DSP(s_DSP* DSP, const char* IROM_file, const char* DROM_file);
void step_DSP(s_DSP* DSP);

#endif //GC__DSP_H
