bits 64

global getcursorpos
getcursorpos:
	mov al,0xe
	mov dx,0x3d4
	out dx,al
	inc dx
	in al,dx
	mov ah,al
	
	mov al,0xf
	dec dx
	out dx,al
	inc dx
	in al,dx
	
	ret

global _putchar
_putchar:
	;todo
	ret
