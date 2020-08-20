#ifndef GC__HID_H
#define GC__HID_H

#define ASSERT_FLOATING_POINT assert(cpu->HID[2] & HID2_PSE /* HID2 does not allow floating point operations */);

enum {
    HID2_PSE = 0x20000000
} HID2_flags;

#endif //GC__HID_H
