.intel_syntax noprefix
.code64

.globl inb
inb:
	push dx
	mov dx,di
	in al,dx
	pop dx
	ret

.globl outb
outb:
	push dx
	mov dx,di
	mov al,sil
	out dx,al
	pop dx
	ret

.globl inw
inw:
	push dx
	mov dx,di
	in ax,dx
	pop dx
	ret

.globl outw
outw:
	push dx
	mov dx,di
	mov ax,si
	out dx,ax
	pop dx
	ret

.globl inl
inl:
	push dx
	mov dx,di
	in eax,dx
	pop dx
	ret

.globl outl
outl:
	push dx
	mov dx,di
	mov eax,esi
	out dx,eax
	pop dx
	ret
