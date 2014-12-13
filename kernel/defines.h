#ifndef DEFINES_H
#define DEFINES_H

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
