#include "hardware_registers.h"


void init_HW_regs(s_hardware_registers* HW_regs) {
    HW_regs->pointers[HR_CP] = HW_regs->CP;
    HW_regs->pointers[HR_PE] = HW_regs->PE;
    HW_regs->pointers[HR_VI] = HW_regs->VI;
    HW_regs->pointers[HR_PI] = HW_regs->PI;
    HW_regs->pointers[HR_MI] = HW_regs->MI;
    HW_regs->pointers[HR_AI] = HW_regs->AI;
    HW_regs->pointers[HR_DI_SI_EXI_Streaming] = NULL;
    HW_regs->pointers[7] = NULL;
    HW_regs->pointers[HR_GX_FIFO] = HW_regs->GX_FIFO;
}
