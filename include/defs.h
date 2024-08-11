#pragma once

#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef int8_t s8;
typedef int16_t s16;
typedef int32_t s32;
typedef int64_t s64;

//ERROR CODES
#define WRONG_EXE_USAGE -1
#define FAILED_ROM_LOAD -2
#define CPU_STOPPED -3
#define UNDEFINED_BEHAVIOUR -4
#define NOT_IMPLEMENTED -5
#define UNKNOWN_INSTRUCTION -7
#define MEMORY_OUT_OF_RANGE -8
#define TEST_PASSED -9


#define BIT(a, n) ((a & (1 << n)) ? 1 : 0)
#define BIT_SET(a, n, on) { if (on) a |= (1 << n); else a &= ~(1 << n); }
#define BETWEEN(a, b, c) ((a >= b) && (a <= c))

void delay(u32 ms);

#define NO_IMPL { fprintf(stderr, "NOT YET IMPLEMENTED\n"); exit(NOT_IMPLEMENTED); }