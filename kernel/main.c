#include "apic.h"
#include "interrupt.h"
#include "mp.h"
#include "paging.h"

void error(char *msg,u8 halt)
{
	puts("Error:\n");
	puts(msg);
	putchar('\n');
	if(halt)
	{
		puts("Stopped.\n");
		haltcpu();
	}
}

void kmain(struct mat* mat)
{
	setmat(mat);
	if(!initpaging())
		error("Failed initializing paging.",1);
	if(!initinterrupt())
		error("Failed initializing interrupt.",1);
	if(!initapic())
		error("Failed initializing APIC.",1);
	if(!initacpi())
		error("Failed initializing ACPI.",1);
	if(!initmp())
		error("Failed initializing multiprocessor.",0);

	for(;;);
}
