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
