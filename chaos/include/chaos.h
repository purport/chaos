#pragma once

typedef _Bool bool;
typedef int bool32;
typedef char c8;
typedef short c16;
typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long long u64;
typedef char i8;
typedef int i32;
typedef long long i64;
typedef float f32;
typedef double f64;

#define ASSERT(cond) ((cond) || (__debugbreak(), 0))

extern u32 chaos_main(void);
extern u32 string_length(c8 *message);
extern void print(c8 *message);
