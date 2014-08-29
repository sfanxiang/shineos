asm("\
	sti\n\
	jmp near _main\
");

#include "display.h"
#include "drive.h"
#include "fs.h"
#include "keyboard.h"
#include "memory.h"
#include "time.h"
#include <stdlib.h>

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

void loadimg(char *fullpath)
{
	_putchar('\n');
	_puts(fullpath);
	for(;;);
}

void processbootcfg(char values[16][512],u8 valcnt)
{
	u8 choice,timeout,ok[16],itemcnt,i;
	{
		u8 okcnt;
		for(i=0,okcnt=-1;i<valcnt;i++)
			if(values[i][0])ok[++okcnt]=i;
		choice=0xff;timeout=10;
		for(i=itemcnt=0;i<okcnt;i+=2)
		{
			if(values[ok[i]][0]=='$')
			{
				if(!strcmp(&values[ok[i]][1],"default"))
					choice=atoi(values[ok[i+1]]);
				else if(!strcmp(&values[ok[i]][1],"timeout"))
					timeout=atoi(values[ok[i+1]]);
			}
			else
			{
				_puts(itoa(itemcnt));
				_putchar(':');
				_puts(values[ok[i]]);
				_puts("\r\n");
				ok[itemcnt++]=ok[i+1];
			}
		}
	}
	if(choice<itemcnt)
	{
		u32 beginticks,curticks;
		u32 waitticks;
		beginticks=curticks=getclockticks(NULL);
		waitticks=timeout*CLOCK_TICKS_PER_DAY/86400;
		while(curticks-beginticks<waitticks)
		{
			curticks=getclockticks(NULL);
			if(curticks<beginticks)curticks+=CLOCK_TICKS_PER_DAY;
			_puts("Default:");
			_puts(itoa(choice));
			_puts("    Timeout:");
			_puts(itoa((u16)(timeout-(curticks-beginticks)*86400/CLOCK_TICKS_PER_DAY)));
			_puts("    \r");
			if(checkkeystroke())
				break;
		}
		if(curticks-beginticks>=waitticks)
			loadimg(values[ok[choice]]);
	}
	{
		char str[GETS_MAX_CHARS+1];
		do{
			u8 i;
			for(i=0;i<79;i++)_putchar(' ');
			_puts("\rChoose one to load:");
			_gets(str);
			_putchar('\r');
		}while(str[0]<'0'||str[0]>'9'
			||(choice=atoi(str))>=itemcnt);
	}
	loadimg(values[ok[choice]]);
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
