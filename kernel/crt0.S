.intel_syntax noprefix
.code64

.section .text

.extern _end

.globl kernel_size,kernel_start
kernel_start:
kernel_size:
	.quad _end-kernel_start

.globl _start
_start:
	lea rsp,stack_top[rip]
	push rdi
	call _init
	pop rdi
	jmp kmain

.section .stack,"aw",@nobits
.skip 8192*1024
stack_top:
