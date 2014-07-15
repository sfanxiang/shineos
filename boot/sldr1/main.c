asm("jmp near _main");

#include <string.h>
#include "display.h"
#include "drive.h"
#include "memory.h"

u1 currentdrive;

void error(char* msg,bool halt)
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

u4 findactivepart()
{
	pfar ppartentry;
	for(ppartentry=0x7dbe;ppartentry<=0x7dee;ppartentry+=0x10)
		if(getfarbyte(ppartentry)==0x80)break;
	if(ppartentry>0x7dee)return 0;else return getfardword(ppartentry+8);
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
	
	u4 partlba;
	struct dap drivedata;
	u1 sec[512];
	
	currentdrive=getfarbyte(0x7ff0);
	_puts("\r\n");
	
	partlba=findactivepart();
	if(!partlba)error("No active partition found.",1);
	
	drivedata.size=sizeof(drivedata);
	drivedata.sectors=1;
	drivedata.address=getpfar(sec);
	drivedata.startsector=0;
	drivedata.reserved=0; //drive number detected 0 here after bochs resets, is bochs buggy?
	readdrivesectors(currentdrive,&drivedata);
	
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

