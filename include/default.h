#ifndef GC__DEFAULT_H
#define GC__DEFAULT_H

#include <stdint.h>
#include <float.h>

#ifdef _MSC_VER
#define STRCPY(dest, len, src) strcpy_s(dest, len, src)
#define SSCANF sscanf_s
#define SPRINTF sprintf_s
#define FOPEN(stream, fname, mode) fopen_s(stream, fname, mode)
#else
#include <stddef.h>
#define STRCPY(dest, len, src) strcpy(dest, src)
#define SSCANF sscanf
#define SPRINTF(dest, len, src, ...) sprintf(dest, src, ##__VA_ARGS__)
#define FOPEN(stream, fname, mode) *stream = fopen(fname, mode)
#endif

#ifdef __has_builtin
#define HAS_BUILTIN(builtin) __has_builtin(builtin)
#else
// todo
#define HAS_BUILTIN(builtin) 0
#endif

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;

#endif //GC__DEFAULT_H
