org 0x7c00

%include "std.inc"

%define SEL_CODE32 (gdt_code32-gdt)
%define SEL_DATA32 (gdt_data32-gdt)
%define SEL_CODE64 (gdt_code64-gdt)
%define SEL_DATA64 (gdt_data64-gdt)

bits 16
	cli
	jmp 0:real
real:
	mov ax,cs
	mov ds,ax
	mov ss,ax
	mov sp,real
	
	mov ax,3
	int 0x10
	
	lgdt [gdt_ptr]
	
	in al,0x92
	or al,2
	out 0x92,al
	
	mov al,0xff
	out 0xa1,al
	out 0x21,al
	nop
	nop
	
	lidt [idt]
	
	mov eax,cr0
	or eax,1
	mov cr0,eax
	
	jmp dword SEL_CODE32:protected

gdt:
gdt_null:
	dq 0
gdt_code32:
	dq 0x00c09a00000000ff
gdt_data32:
	dq 0x00c09200000000ff
gdt_code64:
	dq 0x00209a0000000000
gdt_data64:
	dq 0x0020920000000000

gdt_ptr:
	dw $-gdt-1
	dd gdt

idt:
	dw 0
	dd 0
	
bits 32
protected:
	mov ax,SEL_DATA32
	mov ds,ax
	mov ss,ax
	mov esp,real

	mov eax,0x81000|3
	xor edx,edx
	mov edi,0x80000
	mov ecx,0x1000/8
@@
	mov [edi],eax
	mov [edi+4],edx
	add edi,8
	loop @b
	
	mov eax,0x82000|3
	xor edx,edx
	mov edi,0x81000
	mov ecx,0x1000/8
@@
	mov [edi],eax
	mov [edi+4],edx
	add edi,8
	loop @b
	
	mov eax,0x83000|3
	xor edx,edx
	mov edi,0x82000
	mov ecx,0x1000/8
@@
	mov [edi],eax
	mov [edi+4],edx
	add edi,8
	loop @b
	
	mov eax,3
	xor edx,edx
	mov edi,0x83000
	mov ecx,0x200000/0x1000
@@
	mov [edi],eax
	mov [edi+4],edx
	add edi,8
	add eax,0x1000
	loop @b
	
	mov eax,0x80000
	mov cr3,eax
	
	mov eax,cr4
	or eax,10100000b
	mov cr4,eax
	
	mov ecx,0xc0000080
	rdmsr
	or eax,0x100
	wrmsr
	
	mov eax,cr0
	or eax,0x80000000
	mov cr0,eax
	
	jmp SEL_CODE64:long_start

bits 64
long_start:
	mov ax,SEL_DATA64
	mov ds,ax
	mov ss,ax
