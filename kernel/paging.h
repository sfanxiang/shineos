#ifndef PAGING_H
#define PAGING_H

#ifdef __x86_64__

#include "memory.h"

extern u64* setpaging();
extern u8 initpaging();
extern void invlpaging();

#endif

#endif
