#ifndef _DISPLAY_H
#define _DISPLAY_H
void _putchar(char chr)
{
	asm("\
		push bp\n\
		mov bp,sp\n\
		sseg\n\
		mov al,4[bp]\n\
		mov bx,#7\n\
		mov ah,#0xe\n\
		int #0x10\n\
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

