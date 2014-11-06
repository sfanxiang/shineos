bits 64

global in32
in32:
	push dx
	mov dx,di
	in eax,dx
	pop dx
	ret

global out32
out32:
	push dx
	mov dx,di
	mov eax,esi
	out dx,eax
	pop dx
	ret
