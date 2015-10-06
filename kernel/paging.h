#ifndef PAGING_H
#define PAGING_H

#ifdef __x86_64__

#include "defines.h"

extern void setpaging(u64 *ptr);
extern void invlpaging();
extern u64* getcurpagetable();

#endif

#endif
