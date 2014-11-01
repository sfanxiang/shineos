#ifndef DISPLAY_H
#define DISPLAY_H

#include "defines.h"

#ifdef __AS386_16__

void _putchar(char chr)
{
	asm("\
		push bp\n\
		mov bp,sp\n\
		push bx\n\
		sseg\n\
		mov al,[bp+4]\n\
		mov bx,#7\n\
		mov ah,#0xe\n\
		int #0x10\n\
		pop bx\n\
		pop bp\
	");
}

#endif

/*
#ifdef __AS386_32__

u16 getcursorpos()
{
	asm("\
		mov al,#0xe\n\
		mov dx,#0x3d4\n\
		out dx,al\n\
		inc dx\n\
		in al,dx\n\
		mov ch,al\n\
		\n\
		mov al,#0xf\n\
		dec dx\n\
		out dx,al\n\
		inc dx\n\
		in al,dx\n\
		mov ah,ch\
	");
}

void setcursorpos(u16 pos)
{
	asm("\
		push ebp\n\
		mov ebp,esp\n\
		\n\
		mov al,#0xe\n\
		mov dx,#0x3d4\n\
		out dx,al\n\
		sseg\n\
		mov al,[ebp+8+1]\n\
		inc dx\n\
		out dx,al\n\
		\n\
		mov al,#0xf\n\
		dec dx\n\
		out dx,al\n\
		sseg\n\
		mov al,[ebp+8]\n\
		inc dx\n\
		out dx,al\n\
		\n\
		pop ebp\
	");
}

void writechar(char chr,u16 pos)
{
	asm("\
		push ebp\n\
		mov ebp,esp\n\
		mov al,[ebp+8]\n\
		mov bp,[ebp+8+4]\n\
		shl bp,1\n\
		gseg\n\
		mov [bp],al\n\
		pop ebp\
	");
}

void _putchar(char chr)
{
	u16 pos=getcursorpos();
	writechar(chr,pos);
	setcursorpos(pos+1);
}

#endif
*/

#ifdef __AS386_64__

extern u16 getcursorpos();
extern void setcursorpos(u16 pos);
extern void writechar(char chr,u16 pos);

void _putchar(char chr)
{
	u16 cursor=getcursorpos();
	writechar(chr,cursor);
	setcursorpos(cursor+1);
	//todo
}

#endif

void _puts(char* str)
{
	while(*str)
	{
		_putchar(*str);
		str++;
	}
}

#endif
