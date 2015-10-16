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

	if(SMAP_COUNT==-1)
	{
		error("Cannot fetch memory layout, detecting memory...",0);
		SMAP_COUNT=1;
		(SMAP_TABLE+0)->base=0;
		(SMAP_TABLE+0)->len=0x9fc00;
		(SMAP_TABLE+0)->type=SMAP_TYPE_USABLE;
		(SMAP_TABLE+0)->acpi=0;
		volatile u64 *i;
		volatile u64 dummy;
		u8 iscurusable=0;
		size_t reservedcnt=0;
		for(i=0x100000;;i+=0x1000)
		{
			u64 origin=*i;
			u64 reverse=~origin;
			*i=reverse;
			asm("":::"memory");
			dummy=origin;
			__asm__ __volatile__("wbinvd":::"memory");
			if(*i==reverse)
			{
				reservedcnt=0;
				if(!iscurusable)
				{
					iscurusable=1;
					(SMAP_TABLE+SMAP_COUNT)->base=i;
					(SMAP_TABLE+SMAP_COUNT)->len=0x1000;
					(SMAP_TABLE+SMAP_COUNT)->type=SMAP_TYPE_USABLE;
					(SMAP_TABLE+SMAP_COUNT)->acpi;
					SMAP_COUNT++;
				}
				else
					(SMAP_TABLE+SMAP_COUNT-1)->len+=0x1000;
			}
			else
			{
				iscurusable=0;
				reservedcnt+=0x1000;
				if(reservedcnt>0x40000000)break;
			}
			*i=origin;
		}
	}
	{
		puts("Memory layout:\n");
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

	if(!matbuild())
		error("Failed building MAT.",1);

	s8 drivecnt;
	if((drivecnt=driveinit())==-1)
		error("Failed initializing drive.",1);

	u32 disksign=*(u32*)(0x7c00+5);
	u8 diskpart=*(u8*)(0x7c00+sizeof(u32));

	s8 i;
	for(i=0;i<drivecnt;i++)
	{
		u8 diskbuf[512];
		if(!readdrive(i,0,1,diskbuf))
		{
			char buf[20];
			puts("Error:\nFailed reading drive #");
			puts(itoa(i,buf,10));
			puts(".\n");
			continue;
		}
		if(((struct mbr*)diskbuf)->disksign==disksign)
		{
			struct mbr *diskmbr=(struct mbr*)diskbuf;
			u32 part=diskmbr->parttable[diskpart].firstlba;
			u16 blocksize;u32 pfile;
			if(!openfile(i,part,"/sys/kernel",&pfile,
			             &blocksize,NULL))
			{
				char buf[20];
				puts("Error:\nCannot open \"/sys/kernel\" in drive #");
				puts(itoa(i,buf,10));
				puts(".\n");
				continue;
			}

			void *ksize=malloc(blocksize*512);
			if(!ksize)error("Cannot allocate memory for kernel.",1);
			if(!readfile(i,part,pfile,1,blocksize,ksize,NULL,NULL))
			{
				char buf[20];
				puts("Error:\nFailed reading \"/sys/kernel\" in drive #");
				puts(itoa(i,buf,10));
				puts(".\n");
				free(ksize);
				continue;
			}

			void *kernel=calloc_align(1,*(size_t*)ksize,2*1024*1024);
			*(((u64*)0x50000)+VM_KERNEL/0x8000000000)=0x51000|3;
			*((u64*)(0x52000+8))=((size_t)kernel)|0x83;
			free(ksize);
			if(!kernel)error("Cannot allocate memory for kernel.",1);
			
			if(!readfile(i,part,pfile,0x7fffffff,blocksize,
			             kernel,NULL,NULL))
			{
				char buf[20];
				puts("Error:\nFailed reading \"/sys/kernel\" in drive #");
				puts(itoa(i,buf,10));
				puts(".\n");
				free(kernel);
				continue;
			}

			drivestop();
			*((u64*)kernel)=kernel;
			((void(*)())(VM_KERNEL+4096*512+sizeof(size_t)))();
		}
	}

	drivestop();
	error("No kernel.",1);
}
