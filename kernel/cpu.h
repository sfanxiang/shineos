#ifndef CPU_H
#define CPU_H

#ifdef __x86_64__

#include "defines.h"

extern void haltcpu();
extern void enable_int();
extern void disable_int();
extern u64 readmsr(u32 reg);
extern void writemsr(u32 reg,u64 num);
extern u64 getcpuflags();

#define CPU_FLAGS_IF (1<<9)

#endif

#endif
