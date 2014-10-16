#ifndef DISPLAY_H
#define DISPLAY_H

#ifdef __AS386_16__

void _putchar(char chr)
{
	asm("\
		push bp\n\
		mov bp,sp\n\
		push bx\n\
		sseg\n\
		mov al,4[bp]\n\
		mov bx,#7\n\
		mov ah,#0xe\n\
		int #0x10\n\
		pop bx\n\
		pop bp\
	");
}

void _puts(char* str)
{
	while(*str)
	{
		_putchar(*str);
		str++;
	}
}

#endif

#ifdef __AS386_32__
#endif

#endif
