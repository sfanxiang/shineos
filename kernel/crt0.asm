.intel_syntax noprefix
.code64

.section .text

.extern _end

.globl kernel_size
kernel_size:
	.quad _end-kernel_size

.globl _start
_start:
	lea rsp,stack_top[rip]
	push rdi
	call _init
	pop rdi
	jmp kmain

.section .stack,"aw",@nobits
.skip 65536
stack_top:
