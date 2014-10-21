global _main
extern _main16

_main:
	mov ax,0x7c0
	mov ds,ax
	mov ss,ax
	xor sp,sp
	jmp 0x7c0:_main16
