#include "defines.h"
#include "display.h"
#include "fs.h"

void error(char *msg,u8 halt)
{
	puts("Error:\n");
	puts(msg);
	putchar('\n');
	if(halt)
	{
		puts("System halted.\n");
		__asm__ __volatile__(
		                     "error_halt:\n\t"
		                     "hlt\n\t"
		                     "jmp error_halt"
		                     );
	}
}

void main()
{
	puts("Starting ShineOS...\n");

	u32 disksign=*(u32*)(0x7c00+5);
	
	s8 drivecnt;
	if((drivecnt=initdrive())==-1)
		error("Failed initializing drive.",1);

	s8 i;
	for(i=0;i<drivecnt;i++)
	{
		u8 diskbuf[512];
		if(!readdrive(i,0,1,diskbuf))
		{
			u8 buf[20];
			puts("Error:\nFailed reading drive #");
			puts(itoa(i,buf,10));
			puts(".\n");
			continue;
		}
		if(((struct mbr*)diskbuf)->disksign==disksign)
		{
			//todo
		}
	}
	
	while(1);
}
