#include "SPR.h"


void init_SPRs(s_Gekko* cpu) {
    memset(&cpu->SPR_write_mask, 0xff, sizeof(cpu->SPR_write_mask));

    // todo: full map
    cpu->SPR[SPR_XER] = &cpu->XER;
    cpu->SPR[SPR_LR] = &cpu->LR;
    cpu->SPR[SPR_CTR] = &cpu->CTR;
    cpu->SPR[SPR_SRR0] = &cpu->SRR0;
    cpu->SPR[SPR_SRR1] = &cpu->SRR1;
    cpu->SPR[SPR_GQR0] = &cpu->GQR[0];
    cpu->SPR[SPR_GQR1] = &cpu->GQR[1];
    cpu->SPR[SPR_GQR2] = &cpu->GQR[2];
    cpu->SPR[SPR_GQR3] = &cpu->GQR[3];
    cpu->SPR[SPR_GQR4] = &cpu->GQR[4];
    cpu->SPR[SPR_GQR5] = &cpu->GQR[5];
    cpu->SPR[SPR_GQR6] = &cpu->GQR[6];
    cpu->SPR[SPR_GQR7] = &cpu->GQR[7];
    cpu->SPR[SPR_HID0] = &cpu->HID[0];
    cpu->SPR[SPR_HID1] = &cpu->HID[1];
    cpu->SPR[SPR_HID2] = &cpu->HID2;
    cpu->SPR_write_mask[SPR_HID2] = HID2_WRITE_MASK;

    cpu->SPR[SPR_IBAT0L] = ((void*)&cpu->IMMU.BAT[BAT0L >> 1]);
    cpu->SPR[SPR_IBAT0U] = ((void*)&cpu->IMMU.BAT[BAT0U >> 1]) + 4;
    cpu->SPR[SPR_IBAT1L] = ((void*)&cpu->IMMU.BAT[BAT1L >> 1]);
    cpu->SPR[SPR_IBAT1U] = ((void*)&cpu->IMMU.BAT[BAT1U >> 1]) + 4;
    cpu->SPR[SPR_IBAT2L] = ((void*)&cpu->IMMU.BAT[BAT2L >> 1]);
    cpu->SPR[SPR_IBAT2U] = ((void*)&cpu->IMMU.BAT[BAT2U >> 1]) + 4;
    cpu->SPR[SPR_IBAT3L] = ((void*)&cpu->IMMU.BAT[BAT3L >> 1]);
    cpu->SPR[SPR_IBAT3U] = ((void*)&cpu->IMMU.BAT[BAT3U >> 1]) + 4;

    cpu->SPR[SPR_DBAT0L] = ((void*)&cpu->DMMU.BAT[BAT0L >> 1]);
    cpu->SPR[SPR_DBAT0U] = ((void*)&cpu->DMMU.BAT[BAT0U >> 1]) + 4;
    cpu->SPR[SPR_DBAT1L] = ((void*)&cpu->DMMU.BAT[BAT1L >> 1]);
    cpu->SPR[SPR_DBAT1U] = ((void*)&cpu->DMMU.BAT[BAT1U >> 1]) + 4;
    cpu->SPR[SPR_DBAT2L] = ((void*)&cpu->DMMU.BAT[BAT2L >> 1]);
    cpu->SPR[SPR_DBAT2U] = ((void*)&cpu->DMMU.BAT[BAT2U >> 1]) + 4;
    cpu->SPR[SPR_DBAT3L] = ((void*)&cpu->DMMU.BAT[BAT3L >> 1]);
    cpu->SPR[SPR_DBAT3U] = ((void*)&cpu->DMMU.BAT[BAT3U >> 1]) + 4;

    // todo: implement functionality
    cpu->SPR[SPR_WPAR] = &cpu->WPAR;
    cpu->SPR_write_mask[SPR_WPAR] = WPAR_WRITE_MASK;

    // require normal reads/writes
    cpu->SPR[SPR_SPRG0] = &cpu->default_SPR[SPR_SPRG0];
    cpu->SPR[SPR_SPRG1] = &cpu->default_SPR[SPR_SPRG1];
    cpu->SPR[SPR_SPRG2] = &cpu->default_SPR[SPR_SPRG2];
    cpu->SPR[SPR_SPRG3] = &cpu->default_SPR[SPR_SPRG3];


    // todo: stubbed
    cpu->SPR[SPR_L2CR] = &cpu->default_SPR[SPR_L2CR];
    cpu->SPR[SPR_DEC] = &cpu->DEC;

    // todo: implement this
    cpu->SPR[SPR_MMCR0] = &cpu->default_SPR[SPR_MMCR0];
    cpu->SPR[SPR_MMCR1] = &cpu->default_SPR[SPR_MMCR1];
    cpu->SPR[SPR_PMC1] = &cpu->default_SPR[SPR_PMC1];
    cpu->SPR[SPR_PMC2] = &cpu->default_SPR[SPR_PMC2];
    cpu->SPR[SPR_PMC3] = &cpu->default_SPR[SPR_PMC3];
    cpu->SPR[SPR_PMC4] = &cpu->default_SPR[SPR_PMC4];
}
