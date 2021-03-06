#ifndef KEYBOARD_H
#define KEYBOARD_H

#include "defines.h"
#include "display.h"

#define GETS_MAX_CHARS 8

#ifdef __AS386_16__

char checkkeystroke()
{
	asm("\
		mov ah,#0x11\n\
		int #0x16\n\
		jnz checkkeystroke_havekey\n\
		mov al,#0\n\
	checkkeystroke_havekey:\
	");
}

char getkeystroke()
{
	asm("\
		mov ah,#0x10\n\
		int #0x16\
	");
}

char* _gets(char *str)
{
	char cur;char *head;head=str;
	while((cur=getkeystroke())!='\r')
	{
		if(cur==8)
		{
			if(str>head)
			{
				_putchar(8);
				_putchar(' ');
				_putchar(8);
				str--;
			}
		}
		else if(cur>=32&&cur<=126)
		{
			if(str-head<GETS_MAX_CHARS)
			{
				_putchar(cur);
				if(head)*str=cur;
				str++;
			}
		}
	}
	if(head)*str=0;
	return head;
}

#endif

#endif
