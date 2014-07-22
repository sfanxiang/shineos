#ifndef _DRIVE_H
#define _DRIVE_H

#include "types.h"

struct dap{
	u16 size;
	u16 sectors;
	pfar address;
	u32 startsector;
	u32 reserved;	//should be 0
};
struct partentry{
	u8 status;	//0x00:inactive,0x80:active
	u8 firsthead,firstsector,firstcylinder,type,lasthead,lastsector,lastcylinder;
	u32 firstlba,sectors;
};

u8 readdrivesectors(u8 drive,struct dap *data)
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

