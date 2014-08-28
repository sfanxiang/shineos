asm("\
	jmp near _main\
");

#include "display.h"
#include "drive.h"
#include "fs.h"
#include "memory.h"

u8 currentdrive;

void error(char* msg,u8 halt)
{
	_puts("SLDR: Error:\r\n");
	_puts(msg);
	_puts("\r\n");
	if(halt)
	{
		_puts("SLDR: System halted.\r\n");
		for(;;);
	}
}

void processbootcfg(char values[16][512],u8 valcnt)
{
	u8 i;
	for(i=0;i<valcnt;i++)
		_puts(values[i]),
		_puts("\r\n");
}

void readbootcfg(u32 part)
{
	char values[16][512];u8 valcnt;
	{
		u16 blocksize;u32 pfile;
		u8 data[8192];u32 bytesread;
		u16 index;u32 i;
		if(!openfile(currentdrive,part,
			"boot/sldr.cfg",&pfile,
			&blocksize,NULL))
			error("Cannot open \"boot/sldr.cfg\".",1);
		if(!readfile(currentdrive,part,pfile,
			(u32)16,blocksize,data,NULL,
			&bytesread))
			error("Failed reading \"boot/sldr.cfg\".",1);
		for(i=0,valcnt=0,index=0;i<bytesread;i++)
		{
			if(data[i]=='\n')
			{
				values[valcnt][index]=0;
				index=0;
				valcnt++;
			}
			else
			{
				values[valcnt][index]
					=data[i];
				index++;
			}
		}
		if((i>0)&&(data[i-1]!='\n'))valcnt++;
	}
	processbootcfg(values,valcnt);
}

void loadactivepart()
{
	pfar ppartentry;
	for(ppartentry=0x7dbe;ppartentry<=0x7dee;ppartentry+=0x10)
		if(getfarbyte(ppartentry)==0x80)break;
	if(ppartentry>0x7dee)
		error("No active partition found.",1);
	if(getfarbyte(ppartentry+4)!=0x60)
		error("Active partition has an unknown file system.",1);
	readbootcfg(getfardword(ppartentry+8));
}

void main()
{
	asm("\
		push ds\n\
		mov ax,#0x7ff\n\
		mov ds,ax\n\
		dseg\n\
		mov [0],dx\n\
		dseg\n\
		mov [2],es\n\
		dseg\n\
		mov [4],di\n\
		pop ds\
	");

	currentdrive=getfarbyte((pfar)0x7ff0);
	_puts("\r\n");
	
	loadactivepart();
	
	for(;;);
	
	asm("\
		mov dx,#0x7ff\n\
		mov ds,dx\n\
		dseg\n\
		mov dx,[2]\n\
		mov es,dx\n\
		dseg\n\
		mov dx,[0]\n\
		dseg\n\
		mov di,[4]\
	");
}
