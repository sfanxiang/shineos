#define __AS386_64__
__asm__(
	".global _start\n"
	"_start:\n"
	"movq $0x70000,%rsp\n"
	"jmp main");

#include "defines.h"
#include "display.h"

void main()
{
	//test
	_puts("start");
	for(;;);
}
