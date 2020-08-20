#ifndef GC__DEFAULT_H
#define GC__DEFAULT_H

#include <stdint.h>
#include <float.h>

#define u8 uint8_t
#define u16 uint16_t
#define u32 uint32_t
#define u64 uint64_t

#define i8 int8_t
#define i16 int16_t
#define i32 int32_t
#define i64 int64_t

#define ROTL32(uval, n) (((uval) << n) | ((uval) >> (32 - n)))
#define ROTR32(uval, n) (((uval) >> n) | ((uval) << (32 - n)))
#define EXTS32(val, len) (((i32)(val << (32 - len))) >> (32 - len))

#define MIN(a, b) ((a < b) ? a : b)
#define MAX(a, b) ((a > b) ? a : b)

static inline uint8_t flip_byte(uint8_t b) {
    b = (b & 0xF0) >> 4 | (b & 0x0F) << 4;
    b = (b & 0xCC) >> 2 | (b & 0x33) << 2;
    b = (b & 0xAA) >> 1 | (b & 0x55) << 1;
    return b;
}

static inline u32 MASK(x, y) {
    int32_t mask_x = x ? (int32_t)0x80000000 >> (x - 1) : 0;
    int32_t mask_y = y == 31 ? 0 : 0xffffffffu >> (y + 1);
    return ((int32_t)(x - y - 1) >> 31) ^ (mask_x ^ mask_y);
}

#endif //GC__DEFAULT_H
