#define __AS386_64__
__asm__(
	".global _start\n"
	"_start:\n"
	"movq $0x70000,%rsp\n"
	"jmp main");

#include "defines.h"
#include "display.h"
#include "ahci.h"

void main()
{
	puts("Starting ShineOS...\n");
	
	struct pci_device ahci;
	if(findahci(&ahci))
	{
		char buf[20];
		puts("Found AHCI controller at:\nBus: 0x");
		puts(itoa(ahci.bus,buf,16));
		puts("\nSlot: 0x");
		puts(itoa(ahci.slot,buf,16));
		putchar('\n');
	}
	else
		puts("AHCI controller not found.\n");
	
	for(;;);
}
