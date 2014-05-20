;boot/booter.asm
;ASM
;ShineOS booter(MBR)

%include "std.inc"
%include "display.inc"

[bits 16]
[org 0x7c00]

;jump boot
	jmp label_startboot
	nop

%include "mbrdata.inc"

;Booter
label_startboot:
	cli

	xor ax,ax
	mov ss,ax
	mov sp,0xf000

	push dx

	mov ah,0
	int 0x13
	jc error
	
	xor ax,ax
	mov es,ax
	mov bx,0x8000
	mov ax,0x0203
	mov cx,2
	pop dx
	push dx
	mov dh,0
	int 0x13
	jc error
	
	jmp 0:0x8000

error:
	mov dx,0x700
	mov cx,msg_error
	call printstring
	sti
@@
	hlt
	jmp @b

msg_error db '(shineos booter,mbr):',0xd,0xa
	db 'Something wrong happend.',0xd,0xa
	db 'System halted.',0xd,0xa,0

;used functions
	func_printstring
;End of sector
	times 510-($-$$) db 0
	dw 0xaa55
