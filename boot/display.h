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

#ifdef __AS386_64__

#include "string.h"

#define SCREEN_XMAX 80
#define SCREEN_YMAX 25

extern u16 getcursorpos();
extern void setcursorpos(u16 pos);
extern void writechar(char chr,u16 pos);

void scrollscreen(u8 num)
{
	memcpy((void*)0xb8000,(void*)(0xb8000+num*SCREEN_XMAX*2),(SCREEN_XMAX*SCREEN_YMAX-num*SCREEN_XMAX)*2);
	u16 *p;
	for(p=(void*)(0xb8000+(SCREEN_XMAX*SCREEN_YMAX-num*SCREEN_XMAX)*2);p<(void*)(0xb8000+(SCREEN_XMAX*SCREEN_YMAX)*2);p++)
		*p=0x720;
}

void _putchar(char chr)
{
	u16 cursor=getcursorpos();
	if(chr=='\n')
		cursor=cursor-cursor%SCREEN_XMAX+SCREEN_XMAX;
	else
	{
		writechar(chr,cursor);
		cursor++;
	}
	if(cursor>=SCREEN_XMAX*SCREEN_YMAX)
	{
		scrollscreen(1);
		cursor-=SCREEN_XMAX;
	}
	setcursorpos(cursor);
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
