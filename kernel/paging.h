#ifndef PAGING_H
#define PAGING_H

#ifdef __x86_64__

#include "defines.h"

extern u64* buildpagetable();
extern u64* getpagetable(u32 n);
extern void setpagetable(u64 *ptr,u32 n);
extern u8 initpaging();
extern void setpaging(u64 *ptr);
extern void invlpaging();
extern u64* getpageentry(void *addr,u32 n);

#include "memory.h"

#endif

#endif
