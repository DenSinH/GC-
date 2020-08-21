#ifndef GC__HID_H
#define GC__HID_H

#define ASSERT_FLOATING_POINT assert(cpu->HID2.PSE /* HID2 does not allow floating point operations */);

// todo: unused bits?
#define HID2_WRITE_MASK 0xf0ff0000

typedef union s_HID2 {
    // integer:
    struct {
        unsigned: 16;
        unsigned DQOEE: 1;  // DMA queue overflow error enable
        unsigned DCMEE: 1;  // DMA cache miss error enable
        unsigned DNCEE: 1;  // DMA normal cache error enable
        unsigned DCHEE: 1;  // dcbz_l cache hit error enable
        unsigned DQOERR: 1; // DMA queue overflow error (sticky)
        unsigned DCMERR: 1; // DMA cache miss error (sticky)
        unsigned DNCERR: 1; // DMA normal cache error (sticky)
        unsigned DCHERR: 1; // dcbz_l cache hit error (sticky)
        unsigned DMAQL: 4;  // DMA queue length (read only)
        unsigned LCE: 1;    // Locked cache enable
        unsigned PSE: 1;    // Paired single enable
        unsigned WPE: 1;    // Write pipe enable
        unsigned LSQE: 1;   // load/store quantized enable for non-indexed format instructions
    };
    unsigned raw;
} s_HID2;

#endif //GC__HID_H
