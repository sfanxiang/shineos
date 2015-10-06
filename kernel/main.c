#include "pmm.h"
#include "apic.h"
#include "interrupt.h"
#include "misc.h"
#include "mp.h"
#include "paging.h"

void kmain()
{
	if(!pmm_init())
		error("Failed initializing PMM.",1);
	if(!vmm_init(0))
		error("Failed initializing VMM.",1);
	initmemory(0);
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
	puts(" processor(s).\n");

	haltcpu();
}
