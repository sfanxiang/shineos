#ifndef DEFINES_H
#define DEFINES_H

#include <stddef.h>

typedef __u8 u8;
typedef __u16 u16;
typedef __u32 u32;

#define offsetof(st, m) ((size_t)(&((st *)0)->m))

#endif
