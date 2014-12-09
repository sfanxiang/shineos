#include "display.h"
#include "fs.h"
#include "memory.h"

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

	if(SMAP_COUNT!=-1)
	{
		u16 i;
		for(i=0;i<SMAP_COUNT;i++)
		{
			char buf[20];
			puts("base=0x");
			puts(itoa((SMAP_TABLE+i)->base,buf,16));
			puts(", length=0x");
			puts(itoa((SMAP_TABLE+i)->len,buf,16));
			puts(", type=");
			puts(itoa((SMAP_TABLE+i)->type,buf,10));
			putchar('\n');
		}
	}
	else
		error("Failed detecting memory.",1);

	{
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
	}
	
	__asm__ __volatile__(
                     "end_halt:\n\t"
                     "hlt\n\t"
                     "jmp end_halt"
                     );
}
