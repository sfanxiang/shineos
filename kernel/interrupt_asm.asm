.intel_syntax noprefix
.code64

.section .text

.globl loadidt
loadidt:
	lidt [rdi]
	ret

inthandler_base:
inthandler_base_num:
	.word 0
inthandler_base_handler:
	.quad 0
inthandler_base_code:
	push rbp
	mov rbp,rsp
	push di
	push si
	push rdx
	push rcx
	push r8
	push r9
	mov di,inthandler_base_num[rip]
	mov si,[rbp+40]
	mov rdx,[rbp+32]
	mov ecx,[rbp+24]
	mov r8w,[rbp+16]
	mov r9,[rbp+8]
	call inthandler_base_handler[rip]
	pop r9
	pop r8
	pop rcx
	pop rdx
	pop si
	pop di
	pop rbp
	iretq

.globl getinthandler_base_size
getinthandler_base_size:
	mov rax,getinthandler_base_size-inthandler_base
	ret

.globl getinthandler_base
getinthandler_base:
	lea rax,inthandler_base[rip]
	ret
