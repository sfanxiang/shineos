#define __AS386_64__

#include "defines.h"
#include "display.h"
#include "ahci.h"

void main()
{
	puts("Starting ShineOS...\n");
	
	puts("Searching for available AHCI port...\n");
	volatile struct hba_mem *abar=getabar();
	if(!abar){
		puts("Cannot find ABAR.\n");
		for(;;);
	}
	
	char buf[20];s8 port=0;
	while((port=findahciport(abar,port,AHCI_DEV_SATA))!=-1)
	{
		puts("Port: ");
		puts(itoa(port,buf,10));
		puts("\nCommand list base: 0x");
		puts(itoa((size_t)(abar->ports[port].cmd_list),buf,16));
		puts("\nFIS base: 0x");
		puts(itoa((size_t)(abar->ports[port].fis),buf,16));
		puts("\nFirst free command list slot: ");
		puts(itoa(findahcicmdslot(&(abar->ports[port])),buf,10));
		putchar('\n');
		port++;
	}
	
	for(;;);
}
