#ifndef MEMORY_H
#define MEMORY_H

#include "defines.h"

#ifdef __AS386_16__

typedef u32 pfar;

u8 getfarbyte(pfar pt)
{
	asm("\
		push bp\n\
		mov bp,sp\n\
		push ds\n\
		push si\n\
		sseg\n\
		mov si,[bp+6]\n\
		mov ds,si\n\
		sseg\n\
		mov si,[bp+4]\n\
		dseg\n\
		mov al,[si]\n\
		pop si\n\
		pop ds\n\
		pop bp\
	");
}

u32 getfardword(pfar pt)
{
	asm("\
		push bp\n\
		mov bp,sp\n\
		push ds\n\
		push si\n\
		sseg\n\
		mov si,[bp+6]\n\
		mov ds,si\n\
		sseg\n\
		mov si,[bp+4]\n\
		dseg\n\
		mov ax,[si]\n\
		dseg\n\
		mov dx,[si+2]\n\
		pop si\n\
		pop ds\n\
		pop bp\
	");
}

pfar getpfar(void* pt)
{
	asm("\
		push bp\n\
		mov bp,sp\n\
		mov dx,cs\n\
		sseg\n\
		mov ax,[bp+4]\n\
		pop bp\
	");
}

pfar setfarbyte(pfar ptr,u8 val)
{
	asm("\
		push bp\n\
		mov bp,sp\n\
		push ds\n\
		push si\n\
		sseg\n\
		mov ax,[bp+6]\n\
		mov ds,ax\n\
		sseg\n\
		mov si,[bp+4]\n\
		sseg\n\
		mov cl,[bp+8]\n\
		dseg\n\
		mov [si],cl\n\
		mov dx,ds\n\
		mov ax,si\n\
		pop si\n\
		pop ds\n\
		pop bp\
	");
}

#endif

#ifdef __x86_64__

#pragma pack(push,1)

struct smap_entry{
	u64 base;
	u64 len;
	u32 type;
	u32 acpi;
};

#pragma pack(pop)

#define SMAP_TABLE ((struct smap_entry*)0x500)
#define SMAP_COUNT (*((s16*)0x1000))

#endif

#endif
