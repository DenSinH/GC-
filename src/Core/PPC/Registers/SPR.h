#ifndef GC__SPR_H
#define GC__SPR_H

#include "../Gekko.h"

typedef enum {
    SPR_XER = 1,
    SPR_LR = 8,
    SPR_CTR = 9,
    SPR_DSISR = 18,
    SPR_DAR = 19,
    SPR_DEC = 22,
    SPR_SDR1 = 25,
    SPR_SRR0 = 26,
    SPR_SRR1 = 27,
    SPR_TBL= 268,
    SPR_TBU = 269,
    SPR_SPRG0 = 272,
    SPR_SPRG1 = 273,
    SPR_SPRG2 = 274,
    SPR_SPRG3 = 275,
    SPR_EAR = 282,
    SPR_PVR = 287,
    SPR_IBAT0U = 528,
    SPR_IBAT0L = 529,
    SPR_IBAT1U = 530,
    SPR_IBAT1L = 531,
    SPR_IBAT2U = 532,
    SPR_IBAT2L = 533,
    SPR_IBAT3U = 534,
    SPR_IBAT3L = 535,
    SPR_DBAT0U = 536,
    SPR_DBAT0L = 537,
    SPR_DBAT1U = 538,
    SPR_DBAT1L = 539,
    SPR_DBAT2U = 540,
    SPR_DBAT2L = 541,
    SPR_DBAT3U = 542,
    SPR_DBAT3L = 543,
    SPR_GQR0 = 912,
    SPR_GQR1 = 913,
    SPR_GQR2 = 914,
    SPR_GQR3 = 915,
    SPR_GQR4 = 916,
    SPR_GQR5 = 917,
    SPR_GQR6 = 918,
    SPR_GQR7 = 919,
    SPR_HID2 = 920,
    SPR_WPAR = 921,
    SPR_DMAU = 922,
    SPR_DMAL = 923,
    SPR_UMMCR0 = 936,
    SPR_UPMC1 = 937,
    SPR_UPMC2 = 938,
    SPR_USIA = 939,
    SPR_UMMCR1 = 940,
    SPR_UPMC3 = 941,
    SPR_UPMC4 = 942,
    SPR_MMCR0 = 952,
    SPR_PMC1 = 953,
    SPR_PMC2 = 954,
    SPR_SIA = 955,
    SPR_MMCR1 = 956,
    SPR_PMC3 = 957,
    SPR_PMC4 = 958,
    SPR_HID0 = 1008,
    SPR_HID1 = 1009,
    SPR_IABR = 1010,
    SPR_DABR = 1013,
    SPR_L2CR = 1017,
    SPR_ICTC = 1019,
    SPR_THRM1 = 1020,
    SPR_THRM2 = 1021,
    SPR_THRM3 = 1022
} e_SPR;

#define SPR_SIZE_BYTES 4
static inline void SET_SPR(struct s_Gekko* cpu, e_SPR SPR, const u32* value) {
    if (!cpu->SPR_write_fn[SPR]) {
        u32 written = *value & cpu->SPR_write_mask[SPR];
        memcpy((void*)cpu->SPR[SPR], (void*)&written, SPR_SIZE_BYTES);
    }
    else {
        cpu->SPR_write_fn[SPR](cpu, *value);
    }
}

static inline void GET_SPR(struct s_Gekko* cpu, e_SPR SPR, u32* target) {
    if (!cpu->SPR_read_fn[SPR]) {
        memcpy((void*)target, (void*)cpu->SPR[SPR], SPR_SIZE_BYTES);
    }
    else {
        *target = cpu->SPR_read_fn[SPR](cpu);
    }
}

void init_SPRs(struct s_Gekko* cpu);

#endif //GC__SPR_H
