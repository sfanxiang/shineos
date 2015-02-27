#include "misc.h"

u8 checksum(u8* start,u8* end)
{
	u8 sum=0;
	for(;start<end;start++)
		sum+=*start;
	return (sum==0);
}

void message(char *msg)
{
	puts(msg);
	putchar('\n');
}

void error(char *msg,u8 halt)
{
	message("error");
	message(msg);
	if(halt)
	{
		message("stopped");
		haltcpu();
	}
}
