#include "memory.h"

size_t getmemorysize()
{
	u16 i;size_t memsize=0;
	for(i=0;i<SMAP_COUNT;i++)
		if((SMAP_TABLE+i)->base+(SMAP_TABLE+i)->len>memsize)
			memsize=(SMAP_TABLE+i)->base+(SMAP_TABLE+i)->len;
	return memsize;
}
