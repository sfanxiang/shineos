asm("jmp near _main");

#include <string.h>
#include "display.h"
#include "drive.h"
#include "memory.h"
#include "types.h"

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

void loadpartfs(u32 fsstart)
{
}

void loadactivepart()
{
	pfar ppartentry;
	for(ppartentry=0x7dbe;ppartentry<=0x7dee;ppartentry+=0x10)
		if(getfarbyte(ppartentry)==0x80)break;
	if(ppartentry>0x7dee)error("No active partition found.",1);
	if(getfarbyte(ppartentry+4)!=0x60)error("Active partition has an unknown file system.",1);
	loadpartfs(getfardword(ppartentry+8));
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

