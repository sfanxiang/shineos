#include "apic.h"
#include "interrupt.h"
#include "misc.h"
#include "mp.h"
#include "paging.h"

void kmain()
{
	if(!initpaging())
		error("Failed initializing paging.",1);
	if(!initinterrupt(0))
		error("Failed initializing interrupt.",1);
	if(!initapic(0))
		error("Failed initializing APIC.",1);
	if(!initacpi())
		error("Failed initializing ACPI.",1);
	initmp();

	char buf[20];
	puts("Initialized ");
	puts(itoa(getprocessorcount(),buf,10));
	puts(" processors.\n");

	haltcpu();
}
