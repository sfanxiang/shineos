#ifndef IO_H
#define IO_H

#ifdef __x86_64__

#include "defines.h"

extern u8 inb(u16 port);
extern void outb(u16 port,u8 val);
extern u16 inw(u16 port);
extern void outw(u16 port,u16 val);
extern u32 inl(u16 port);
extern void outl(u16 port,u32 val);

#endif

#endif
