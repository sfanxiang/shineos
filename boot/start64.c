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
	
	struct pci_device dev;
	u16 bus,vendor;	//u8 can't reach 256	
	char buf[20];
	for(bus=0;bus<256;bus++)
		for(dev.slot=0;dev.slot<32;dev.slot++)
		{
			dev.bus=bus;
			dev.func=0;
			vendor=readpciconfig(&dev,
				offsetof(struct pci_config_std,vendor_id));
			if(vendor==0xffff)continue;
			
			puts("bus=0x");
			puts(itoa(bus,buf,16));
			puts(", slot=0x");
			puts(itoa(dev.slot,buf,16));
			puts(", vendor=0x");
			puts(itoa(vendor,buf,16));
			puts(", class=0x");
			puts(itoa(to8bit(readpciconfig(&dev,
				offsetof(struct pci_config_std,class))),
				buf,16));
			puts(", subclass=0x");
			puts(itoa(to8bit(readpciconfig(&dev,
				offsetof(struct pci_config_std,subclass))),
				buf,16));
			if(testbit(readpciconfig(&dev,
				offsetof(struct pci_config_std,header_type)),
				7))
				puts(", multifunction");
			putchar('\n');
		}
	
	for(;;);
}
