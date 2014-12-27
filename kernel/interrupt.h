#ifndef INTERRUPT_H
#define INTERRUPT_H

#ifdef __x86_64__

#include "memory.h"

#pragma pack(push,1)

struct idt_ptr{
	u16 limit;
	void *base;
};

struct idt_desc{
	u16 off_lo;
	u16 sel;
	u8 zero0;
	u8 attr;
	u64 off_hi:48;
	u32 zero1;
};

#pragma pack(pop)

#define INTERRUPT_ATTR 0x8e

extern void loadidt(struct idt_ptr *idtr);
extern u8 registerinterrupt(u16 num,void(*handler)
                            (u16 num,u16 ss,u64 rsp,u32 eflags,u16 cs,u64 rip),
                            u8 attr);
extern u8 initinterrupt();

#endif

#endif
