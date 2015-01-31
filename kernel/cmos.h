#ifndef CMOS_H
#define CMOS_H

#ifdef __x86_64__

#include "cpu.h"
#include "io.h"

extern u8 readcmos(u8 reg);
extern void writecmos(u8 reg,u8 val);

#endif

#endif
