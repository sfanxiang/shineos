bits 64

global memcpy
memcpy:
	push rcx
	mov rcx,rdx
	mov rax,rdi
	cld
	rep movsb
	pop rcx
	ret
