#ifndef _MEMORY_H
#define _MEMORY_H

#include "types.h"

typedef u32 pfar;

u8 getfarbyte(pfar pt)
{
	asm("\
		push bp\n\
		mov bp,sp\n\
		push ds\n\
		push si\n\
		sseg\n\
		mov si,6[bp]\n\
		mov ds,si\n\
		sseg\n\
		mov si,4[bp]\n\
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
		mov si,6[bp]\n\
		mov ds,si\n\
		sseg\n\
		mov si,4[bp]\n\
		dseg\n\
		mov ax,[si]\n\
		dseg\n\
		mov dx,2[si]\n\
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
		mov ax,4[bp]\n\
		pop bp\
	");
}
#endif

