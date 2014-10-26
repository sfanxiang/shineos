#ifndef TIME_H
#define TIME_H

#include "defines.h"

#define CLOCK_TICKS_PER_DAY 0x1800B0

#ifdef __AS386_16__

u32 getclockticks(u8 *midnight)
{
	asm("\
		push bp\n\
		mov bp,sp\n\
		xor ah,ah\n\
		int #0x1a\n\
		mov bp,[bp+4]\n\
		cmp bp,#0\n\
		jz getclockticks_skipmidnight\n\
		mov [bp],al\n\
	getclockticks_skipmidnight:\n\
		mov ax,dx\n\
		mov dx,cx\n\
		pop bp\
	");
}

#endif

#endif
