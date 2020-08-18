#ifndef CBOY_DEFAULT_H
#define CBOY_DEFAULT_H

#include <stdint.h>

#define MIN(a, b) ((a < b) ? a : b)
#define MAX(a, b) ((a > b) ? a : b)

static inline uint8_t flip_byte(uint8_t b) {
    b = (b & 0xF0) >> 4 | (b & 0x0F) << 4;
    b = (b & 0xCC) >> 2 | (b & 0x33) << 2;
    b = (b & 0xAA) >> 1 | (b & 0x55) << 1;
    return b;
}

#endif //CBOY_DEFAULT_H
