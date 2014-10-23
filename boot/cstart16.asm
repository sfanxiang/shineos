global _main
global _thispart
extern _main16

_main:
	jmp short continue
	
_thispart:
	db 0

continue:
	mov ax,0x7c0
	mov ds,ax
	mov ss,ax
	xor sp,sp
	jmp 0x7c0:_main16
