.intel_syntax noprefix
.code64

.section .text

.globl haltcpu
haltcpu:
	cli
	mov al,0xff
	out 0xa1,al
	out 0x21,al
	nop
	nop
haltcpu_loop:   
	hlt
	jmp haltcpu_loop

.globl readmsr
readmsr:
	push rdx
	mov ecx,edi
	rdmsr
	shl rdx,32
	mov rdi,0xffffffff
	and rax,rdi
	or rax,rdx
	pop rdx
	ret

.globl writemsr
writemsr:
	push rdx
	mov ecx,edi
	mov eax,esi
	shr rsi,32
	mov edx,esi
	wrmsr
	pop rdx
	ret
