.intel_syntax noprefix
.code64

.section .text

.globl loadidt
loadidt:
	lidt [rdi]
	ret

inthandler_base:
inthandler_base_handler:
	.quad 0
inthandler_base_num:
	.word 0
inthandler_base_haserrcode:
	.byte 0
inthandler_base_code:
	cmp byte ptr inthandler_base_haserrcode[rip],0
	jne inthandler_base_skiperrcode
	pushq 0
inthandler_base_skiperrcode:
	push rbp
	mov rbp,rsp
	push rdi
	push rsi
	push rdx
	push rcx
	push r8
	push r9
	push rax
	push r10
	push r11
	mov di,inthandler_base_num[rip]
	mov si,[rbp+48]
	mov rdx,[rbp+40]
	mov rcx,[rbp+32]
	mov r8w,[rbp+24]
	mov r9,[rbp+16]
	push qword ptr [rbp+8]
	mov eax,0
	call qword ptr inthandler_base_handler[rip]
	add rsp,8
	pop r11
	pop r10
	pop rax
	pop r9
	pop r8
	pop rcx
	pop rdx
	pop rsi
	pop rdi
	pop rbp
	add rsp,8
	iretq

.globl getinthandler_base_size
getinthandler_base_size:
	mov rax,getinthandler_base_size-inthandler_base
	ret

.globl getinthandler_base
getinthandler_base:
	lea rax,inthandler_base[rip]
	ret
