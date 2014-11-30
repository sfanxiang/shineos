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
