#define __AS386_64__
__asm__(
	".global _start\n"
	"_start:\n"
	"movq $0x70000,%rsp\n"
	"jmp main");

#include "defines.h"
#include "display.h"
#include "pci.h"

void main()
{
	puts("Starting ShineOS...\n");
	
	u16 bus;	//u8 can't reach 256
	u8 device;
	for(bus=0;bus<256;bus++)
		for(device=0;device<32;device++)
		{
			u16 vendor=readpciconfig(bus,device,0,
				offsetof(struct pci_std,vendor_id));
			if(vendor==0xffff)continue;
			
			char buf[20];
			puts("bus=0x");
			puts(itoa(bus,buf,16));
			puts(", slot=0x");
			puts(itoa(device,buf,16));
			puts(", vendor=0x");
			puts(itoa(vendor,buf,16));
			puts(", class=0x");
			puts(itoa(to8bit(readpciconfig(bus,device,0,
				offsetof(struct pci_std,class))),
				buf,16));
			puts(", subclass=0x");
			puts(itoa(to8bit(readpciconfig(bus,device,0,
				offsetof(struct pci_std,subclass))),
				buf,16));
			if(testbit(readpciconfig(bus,device,0,
				offsetof(struct pci_std,header_type)),
				7))
				puts(", multifunction");
			putchar('\n');
		}
	
	for(;;);
}
