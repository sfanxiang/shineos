;boot/booter.asm
;ASM
;ShineOS booter(MBR)

%include "std.inc"
%include "display.inc"
%include "hdd.inc"

[bits 16]
[org 0x7c00]

;jump boot
	jmp label_startboot
	nop

%include "mbrdata.inc"

;Booter
label_startboot:
	cli
	push dx

	xor ax,ax
	mov ss,ax
	mov sp,0xffff

	mov cx,msg_boot
	mov dx,0x700
	call printstring
	
@comment
	mov ax,0x1000
	mov ds,ax
	mov cl,0x80
	mov dx,0
	call GetDriveParameters

	cmp al,0
	jne Label_ErrGetDriveParameters

	mov ax,0x1000
	mov ds,ax
	mov cx,[DriveParametersPacket.BytesPerSector]
	mov dl,10
	push Msg_Number
	call stoa

	mov cx,Msg_Number
	mov dx,0x700
	call printstring

	mov cx,Msg_NewLine
	mov dx,0x700
	call printstring

	mov ax,0x1000
	mov ds,ax
	mov ecx,[DriveParametersPacket.Sectors]
	mov dl,10
	push Msg_Number
	call itoa
	
	mov cx,Msg_Number
	mov dx,0x700
	call printstring

	jmp Label_ReadFs

Label_ErrGetDriveParameters:
	mov cx,Msg_ErrGetDriveParameters
	mov dx,0x700
	call printstring
	mov cx,Msg_Error
	mov dx,0x700
	call printstring

	mov cl,al
	mov dl,10
	push Msg_Number
	call btoa

	mov cx,Msg_Number
	mov dx,0x700
	call printstring
@endcomment

	pop cx
	push cx
	mov dx,loader
	call readdrive
	cmp ax,0
	je ok
	
	mov cx,msg_failedread
	mov dx,0x700
	call printstring
	hlt

ok:	jmp 0:0x8000

msg_boot db 'ShineOS booter v0.01',0xa,0xd
	db 'Booting...',0xa,0xd
	db 0xa,0xd,0
msg_failedread db 'Failed reading drive!',0xa,0xd

loader:
istruc dap
	at dap.size, db 0x10
	at dap.zero, db 0
	at dap.sectors, dw 3
	at dap.offset, dw 0x8000
	at dap.segment, dw 0
	at dap.startsector, dq 1
iend

@comment
msg_error db 'Error Code: ',0
Msg_Number dq 0
Msg_ErrGetDriveParameters db 'Failed getting drive parameters!',0xa,0xd,0
@endcomment

;used functions
	func_printstring
	func_readdrive
	;func_putchar
@comment
	func_GetDriveParameters
	func_btoa
	func_stoa
	func_itoa
@endcomment

;End of sector
	times 510-($-$$) db 0
	dw 0xaa55
