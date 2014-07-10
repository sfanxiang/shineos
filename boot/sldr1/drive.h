#ifndef _DRIVE_H
#define _DRIVE_H

#include "memory.h"
struct dap{
	u2 size;
	u2 sectors;
	pfar address;
	u4 startsector;
	u4 reserved;
};
struct partentry{
	u1 status;	//0x00:inactive,0x80:active
	u1 firsthead,firstsector,firstcylinder,type,lasthead,lastsector,lastcylinder;
	u4 firstlba,sectors;
};

u1 readdrivesectors(u1 drive,struct dap arg)
{
	asm("\
		push bp\n\
		mov bp,sp\n\
		push ds\n\
		pusha\n\
		mov si,cs\n\
		mov ds,si\n\
		sseg\n\
		mov si,6[bp]\n\
		sseg\n\
		mov dl,4[bp]\n\
		mov ah,#0x42\n\
		int #0x13\n\
		popa\n\
		jc readdrivesectors_error\n\
		mov al,0\n\
		jmp readdrivesectors_return\n\
	readdrivesectors_error:\n\
		mov al,ah\n\
	readdrivesectors_return:\n\
		pop ds\n\
		pop bp\
	");
}
#endif

