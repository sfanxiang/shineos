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

#ifdef __AS386_64__

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;
typedef unsigned int u32;
typedef int s32;
typedef unsigned long u64;
typedef long s64;

typedef u64 size_t;

#endif

#define offsetof(st,m) ((size_t)(&((st *)0)->m))

#define bitsrange(val,h,r) ((val<<(sizeof(val)*8-r-1))>>(sizeof(val)*8-r-1+h))

#define to8bit(n) (bitsrange(n,0,7))
#define to16bit(n) (bitsrange(n,0,15))
#define to32bit(n) (bitsrange(n,0,31))
#define to64bit(n) (bitsrange(n,0,63))
#define testbit(n,b) (bitsrange(n,b,b))

#endif
