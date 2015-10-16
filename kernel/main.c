#include "apic.h"
#include "interrupt.h"
#include "misc.h"
#include "mp.h"
#include "paging.h"
#include "pmm.h"
#include "stdio.h"

void kmain()
{
	if(!pmm_init())
		error("Failed initializing PMM.",1);
	if(!vmm_init(0))
		error("Failed initializing VMM.",1);
	initmemory(0);
	if(!initinterrupt())
		error("Failed initializing interrupt.",1);
	if(!initapic())
		error("Failed initializing APIC.",1);
	if(!initacpi())
		error("Failed initializing ACPI.",1);
	initmp();

	printf("Initialized %d processor(s).\n",getprocessorcount());
	haltcpu();
}
