#ifndef PAGING_H
#define PAGING_H

#ifdef __x86_64__

#include "defines.h"

extern u64* buildpaging();
extern void setpaging(u64 *ptr);
extern u8 initpaging();
extern void invlpaging();

#include "memory.h"

#endif

#endif
