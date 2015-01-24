#ifndef DRIVE_H
#define DRIVE_H

#ifdef __AS386_16__

#include "memory.h"

struct dap{
	u16 size;
	u16 sectors;
	pfar address;
	u32 startsector;
	u32 reserved;	//should be 0
};

u8 readdrivesectors(u8 drive,struct dap *data)
{
	asm("\
		push bp\n\
		mov bp,sp\n\
		push ds\n\
		mov si,cs\n\
		mov ds,si\n\
		sseg\n\
		mov si,[bp+6]\n\
		sseg\n\
		mov dl,[bp+4]\n\
		mov ah,#0x42\n\
		int #0x13\n\
		jc readdrivesectors_error\n\
		mov al,#0\n\
		jmp readdrivesectors_return\n\
	readdrivesectors_error:\n\
		mov al,ah\n\
	readdrivesectors_return:\n\
		pop ds\n\
		pop bp\
	");
}

#endif

#ifdef __x86_64__

#include "ahci.h"

u8 __useahci;
s8 driveinit()
{
	s8 drivescnt=ahciinit();
	if(drivescnt==-1)
	{
		__useahci=0;
		//todo
		return -1;
	}
	__useahci=1;
	return drivescnt;
}

void drivestop()
{
	if(__useahci)ahcistop();
}

u8 readdrive(u8 drive,u64 start,u32 count,void *buf)
{
	if(__useahci)return ahciread(drive,start,count,buf);
	else return 0;
}

#endif

#endif
