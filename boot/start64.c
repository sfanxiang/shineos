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
	
	char buf[20];u8 port=0;
	while((port=findahciport(abar,port,AHCI_DEV_SATA))!=0xff)
	{
		puts("Port: ");
		puts(itoa(port,buf,10));
		puts("\nCommand list base: 0x");
		puts(itoa((size_t)(abar->ports[port].cmd_list_base),buf,16));
		puts("\nFIS base: 0x");
		puts(itoa((size_t)(abar->ports[port].fis_base),buf,16));
		putchar('\n');
		port++;
	}
	
	for(;;);
}
