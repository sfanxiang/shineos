bits 64

global getcursorpos
getcursorpos:
	push dx
	
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
	
	pop dx
	ret

global setcursorpos
setcursorpos:
	push cx
	push dx
	
	mov cx,di
	mov al,0xe
	mov dx,0x3d4
	out dx,al
	mov al,ch
	inc dx
	out dx,al
	
	mov al,0xf
	dec dx
	out dx,al
	mov al,cl
	inc dx
	out dx,al
	
	pop dx
	pop cx
	ret

global writechar
writechar:
	mov ax,di
	movzx rsi,si
	mov [0xb8000+rsi*2],al
	ret
