#ifndef GC__DIGITALSOUNDPROCESSORINTERFACE_H
#define GC__DIGITALSOUNDPROCESSORINTERFACE_H

#include "DSP/DSP.h"

#include "ProcessorInterface.h"

#include "default.h"
#include "hwreg_utils.h"

#include "../Scheduler/scheduler.h"

typedef enum e_DSPI_regs {
    DSPI_reg_MailboxHiToDSP    = 0x0,
    DSPI_reg_MailboxLoToDSP    = 0x2,
    DSPI_reg_MailboxHiFromDSP  = 0x4,
    DSPI_reg_MailboxLoFromDSP  = 0x6,
    DSPI_reg_CSR               = 0xa,
    DSPI_reg_AR_DMA_MMADDR_H   = 0x20,
    DSPI_reg_AR_DMA_MMADDR_L   = 0x22,
    DSPI_reg_AR_DMA_ARADDR_H   = 0x24,
    DSPI_reg_AR_DMA_ARADDR_L   = 0x26,
    DSPI_reg_AR_DMA_CNT_H      = 0x28,
    DSPI_reg_AR_DMA_CNT_L      = 0x2a,
    DSPI_reg_AI_DMA_START      = 0x30,
    DSPI_reg_AI_DMA_CNT        = 0x36,
    DSPI_reg_AI_DMA_bytes_left = 0x3a,
} e_DSPI_regs;

typedef enum e_DSPI_CSR {
    DSPI_CSR_RES          = 0x0001,
    DSPI_CSR_PIINT        = 0x0002,
    DSPI_CSR_HALT         = 0x0004,
    DSPI_CSR_AIDINT       = 0x0008,
    DSPI_CSR_AIDINTMSK    = 0x0010,
    DSPI_CSR_ARINT        = 0x0020,
    DSPI_CSR_ARINTMSK     = 0x0040,
    DSPI_CSR_DSPINT       = 0x0080,
    DSPI_CSR_DSPINTMSK    = 0x0100,
    DSPI_CSR_DSPDMASTATUS = 0x0200,
} e_DSPI_CSR;

#define DSP_AR_DMA_DIR 0x80000000
#define DSP_AR_DMA_LEN 0x7fffffff
#define DSP_AI_DMA_START 0x8000

#define DSP_SHIFT 1

typedef struct s_DSPI {
    u8 regs[0x200];
    HW_REG_WRITE_CALLBACK((*write[0x200 >> DSP_SHIFT]), DSPI);
    HW_REG_READ_PRECALL((*read[0x200 >> DSP_SHIFT]), DSPI);
    struct s_GameCube* system;

    s_PI* PI;

    s_DSP DSP;
    s_event DSP_step_event;
    s_event DSP_AR_DMA_done_event;
    s_event DSP_AI_DMA_done_event;

#ifdef DO_DEBUGGER
    bool step_DSP;
#endif

    /* internal function */
    u16 DSPCSR;

    u32 AR_DMA_MMADDR, AR_DMA_ARADDR, AR_DMA_CNT;
    u32 AI_DMA_LEN;
} s_DSPI;


HW_REG_INIT_FUNCTION(DSPI);

#endif //GC__DIGITALSOUNDPROCESSORINTERFACE_H
