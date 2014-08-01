#include "types.h"

u8 currentdrive;

void main()
{
	__asm{
		push ds
		mov ax,0x7ff
		mov ds,ax
		mov ds:[0],dx
		mov ds:[2],es
		mov ds:[4],di
		pop ds
	}
	
	currentdrive=*((u8 far*)0x7ff0);
}
