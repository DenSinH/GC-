#ifndef GC__HELPERS_H
#define GC__HELPERS_H

#include "default.h"
#include <stdbool.h>

#define ADD_OVERFLOW32(x, y, result) ((((x) ^ (result)) & ((y) ^ (result))) >> 31) != 0
#define ADD_CARRY(x, y) (u32)(x) > ~((u32)(y))

#define ROTL32(uval, n) (((uval) << (n)) | ((uval) >> (32 - (n))))
#define ROTR32(uval, n) (((uval) >> (n)) | ((uval) << (32 - (n))))
#define EXTS32(val, len) (((i32)((val) << (32 - (len)))) >> (32 - (len)))

#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#define MAX(a, b) (((a) > (b)) ? (a) : (b))
#define CLAMP(val, min, max) MIN(max, MAX(val, min))

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


#endif //GC__HELPERS_H
