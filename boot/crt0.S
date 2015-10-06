.intel_syntax noprefix
.code64

.section .text

.globl _start
_start:
	mov rsp,0x70000
	jmp main
