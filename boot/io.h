#ifndef IO_H
#define IO_H

#ifdef __AS386_64__

#include "defines.h"

extern void in32(u16 port);
extern void out32(u16 port,u32 val);

#endif

#endif
