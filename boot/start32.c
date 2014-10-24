asm("\
	mov ax,#(8*2)\n\
	mov ds,ax\n\
	mov ss,ax\n\
	mov esp,8192\n\
	mov ax,#(8*3)\n\
	mov gs,ax\n\
	jmp _main\
");

#include "defines.h"

u8 thispart;

void main()
{
	asm("mov [_thispart],cl");
	
	//test
	asm("\
		mov al,#80\n\
		gseg\n\
		mov [0],al\
	");
	
	for(;;);
}
