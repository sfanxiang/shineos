;boot/loader.asm
;ASM
;ShineOS loader

%include "std.inc"
%include "display.inc"
%include "string.inc"
%include "hdd.inc"


[bits 16]
[org 0x8000]

	pop word [cs:m_drive]
	mov cx,msg_load
	mov dx,0x700
	call printstring

	cli
	hlt

m_drive db 0,0
msg_load db 'ShineOS loader v0.01',0xa,0xd
	db 'Looding...',0xa,0xd
	db 0xa,0xd,0

;used functions
	func_printstring

;end of loader
	times 0x600-($-$$) db 0
