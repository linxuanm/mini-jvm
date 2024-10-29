#pragma once

#include <cstdint>
#include <vector>

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;
typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;

typedef u32 pc_t;

typedef std::vector<u8> ByteArray;
typedef const ByteArray *ByteArrayRef;