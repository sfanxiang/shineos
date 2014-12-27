#include "defines.h"
#include "paging.h"

void kmain(struct mat* mat)
{
	setmat(mat);
	if(!initpaging())
		haltcpu();
	for(;;);
}
