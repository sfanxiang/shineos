.intel_syntax noprefix
.code64

.section .text

.globl setpaging
setpaging:
	mov cr3,rdi
	ret

.globl invlpaging
invlpaging:
	mov rax,cr3
	mov cr3,rax
	ret
