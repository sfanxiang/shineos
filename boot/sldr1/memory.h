#ifndef MEMORY_H
#define MEMORY_H

#include "defines.h"

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

pfar setfarbyte(pfar ptr,u8 val)
{
	asm("\
		push bp\n\
		mov bp,sp\n\
		push ds\n\
		push si\n\
		sseg\n\
		mov ax,6[bp]\n\
		mov ds,ax\n\
		sseg\n\
		mov si,4[bp]\n\
		sseg\n\
		mov cl,8[bp]\n\
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

