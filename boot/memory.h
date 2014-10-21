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

void enable_a20()
{
	asm("\
		in al,#0x92\n\
		or al,#2\n\
		out #0x92,al\
	");
}

void lgdt(void *gdtptr)
{
	asm("\
		push bp\n\
		mov bp,sp\n\
		mov bp,4[bp]\n\
		sseg\n\
		lgdt [bp]\n\
		pop bp\
	");
}

u16 getsegaddr()
{
	asm("mov ax,cs");
}

#endif

#ifdef __AS386_32__
#endif

#define SEG_DESC_LO(base,limit) (((u32)(limit)&0xffff)|(((u32)(base)&0xffff)<<16))
#define SEG_DESC_HI(base,limit,attr) ((((u32)(base)>>16)&0xff)|(((u32)(attr)&0xf0ff)<<8)|((u32)(limit)&0xf0000)|((u32)(base)&0xff000000))

#define SEG_DESC_ATTR_A 1
#define SEG_DESC_ATTR_RW 2
#define SEG_DESC_ATTR_CO (u8)4
#define SEG_DESC_ATTR_C 8
#define SEG_DESC_ATTR_S 0x10
#define SEG_DESC_ATTR_DPL 0x20
#define SEG_DESC_ATTR_P 0x80
#define SEG_DESC_ATTR_D 0x4000
#define SEG_DESC_ATTR_G 0x8000

#define SEG_DESC_ATTR_FLAT_C (SEG_DESC_ATTR_G|SEG_DESC_ATTR_D|SEG_DESC_ATTR_P|SEG_DESC_ATTR_S|SEG_DESC_ATTR_C|SEG_DESC_ATTR_RW)
#define SEG_DESC_ATTR_FLAT_D (SEG_DESC_ATTR_G|SEG_DESC_ATTR_D|SEG_DESC_ATTR_P|SEG_DESC_ATTR_S|SEG_DESC_ATTR_RW)

#define GDT_PTR_LO(base,limit) (((u32)(u16)(limit)-1)|(((u32)(base)&0xffff)<<16))
#define GDT_PTR_HI(base) ((u16)((u32)(base)>>16))

#define SEG_SEL_RPL 1
#define SEG_SEL_TI 4
#define SEG_SEL_INDEX 8

#endif
