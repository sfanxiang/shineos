#ifndef DEFINES_H
#define DEFINES_H

#ifdef __AS386_16__

#include <stddef.h>

typedef __u8 u8;
typedef __u16 u16;
typedef __s16 s16;
typedef __u32 u32;
typedef __s32 s32;

#endif

#ifdef __x86_64__

typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef signed short s16;
typedef unsigned int u32;
typedef signed int s32;
typedef unsigned long u64;
typedef signed long s64;

typedef u64 size_t;

#define NULL ((void*)0)

#endif

#define offsetof(st,m) ((size_t)(&((st *)0)->m))

#endif
