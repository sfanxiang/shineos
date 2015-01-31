#ifndef MP_H
#define MP_H

#ifdef __x86_64__

#include "apic.h"
#include "cpu.h"
#include "io.h"

extern void initmp();

#endif

#endif
