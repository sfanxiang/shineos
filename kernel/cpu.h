#ifndef CPU_H
#define CPU_H

#ifdef __x86_64__

#include "defines.h"

extern void haltcpu();
extern u64 readmsr(u32 reg);
extern void writemsr(u32 reg,u64 num);

#endif

#endif
