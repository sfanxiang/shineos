#ifndef IO_H
#define IO_H

#ifdef __x86_64__

#include "defines.h"

extern u32 in32(u16 port);
extern void out32(u16 port,u32 val);

#endif

#endif
