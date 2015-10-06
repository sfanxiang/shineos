.intel_syntax noprefix
.code64

.globl memcpy
memcpy:
	push rcx
	mov rcx,rdx
	mov rax,rdi
	cld
	rep movsb
	pop rcx
	ret

.globl memset
memset:
	mov rax,rdi
	push rax
	push rcx
	mov ax,si
	mov rcx,rdx
	cld
	rep stosb
	pop rcx
	pop rax
	ret
