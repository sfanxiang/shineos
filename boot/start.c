asm("jmpf _main,#0x7c0");

#undef __AS386_16__
#define __AS386_32__
#include "memory.h"
#undef __AS386_32__
#define __AS386_16__

void main()
{
	enable_a20();
	
	for(;;);
}
