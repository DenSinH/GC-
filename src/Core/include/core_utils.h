#ifndef GC__CORE_UTILS_H
#define GC__CORE_UTILS_H

static void memcpy_rev(void* dest, void* src, size_t size) {
    char *d = dest;
    const char *s = src + size - 1;

    while (size--) {
        *d++ = *s--;
    }
}

#define MASK_24MB(_address) (((_address) & 0x01000000) | ((_address) & 0x7fffff))

// todo: builtins/intrinsics
#define READ8(array, address) array[(address)]
#define READ16(array, address) ((array[(address)] << 8) | array[(address) + 1])
// #define READ16LE(array, address) ((array[(address) + 1] << 8) | array[(address)])

#define READ32(array, address)      \
    ((array[(address)] << 24)    | \
    (array[(address) + 1] << 16) | \
    (array[(address) + 2] << 8)  | \
     array[(address) + 3])

//#define READ32LE(array, address)      \
//    ((array[(address) + 3] << 24)    | \
//    (array[(address) + 2] << 16) | \
//    (array[(address) + 1] << 8)  | \
//     array[(address)])

#define READ64(array, address)           \
   (((u64)array[(address)] << 56)     | \
    ((u64)array[(address) + 1] << 48) | \
    ((u64)array[(address) + 2] << 40) | \
    ((u64)array[(address) + 3] << 32) | \
    ((u64)array[(address) + 4] << 24) | \
    ((u64)array[(address) + 5] << 16) | \
    ((u64)array[(address) + 6] << 8)  | \
    (u64) array[(address) + 7])

#define WRITE8(array, address, value) array[address] = (value)

#define WRITE16(array, address, value)       \
    array[(address)] = (value) >> 8;         \
    array[(address) + 1] = (value) & 0xff
//#define WRITE16LE(array, address, value)       \
//    array[(address) + 1] = (value) >> 8;         \
//    array[(address)] = (value) & 0xff

#define WRITE32(array, address, value)              \
    array[(address)] = (value) >> 24;               \
    array[(address) + 1] = ((value) >> 16) & 0xff;  \
    array[(address) + 2] = ((value) >> 8) & 0xff;   \
    array[(address) + 3] = (value) & 0xff
//#define WRITE32LE(array, address, value)              \
//    array[(address) + 3] = (value) >> 24;               \
//    array[(address) + 2] = ((value) >> 16) & 0xff;  \
//    array[(address) + 1] = ((value) >> 8) & 0xff;   \
//    array[(address)] = (value) & 0xff

#define WRITE64(array, address, value)              \
    array[(address)] = (value) >> 56;               \
    array[(address) + 1] = ((value) >> 48) & 0xff;  \
    array[(address) + 2] = ((value) >> 40) & 0xff;  \
    array[(address) + 3] = ((value) >> 32) & 0xff;  \
    array[(address) + 4] = ((value) >> 24) & 0xff;  \
    array[(address) + 5] = ((value) >> 16) & 0xff;  \
    array[(address) + 6] = ((value) >> 8) & 0xff;   \
    array[(address) + 7] = (value) & 0xff

#endif //GC__CORE_UTILS_H
