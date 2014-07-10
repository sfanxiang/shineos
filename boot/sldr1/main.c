asm("jmp near _main");
#include <string.h>
#include "io.h"
void main()
{
	int a=strlen("1234");
	if(a==4)_puts("\r\n");
l:	goto l;
}

