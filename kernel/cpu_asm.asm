.intel_syntax noprefix
.code64

.section .text

.globl haltcpu
haltcpu:
	hlt
	jmp haltcpu
