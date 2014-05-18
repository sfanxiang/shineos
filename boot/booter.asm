;boot/booter.asm
;ASM
;ShineOS booter(MBR)

%include "std.inc"
%include "display.inc"
%include "string.inc"
%include "hdd.inc"

[bits 16]
[org 7c00h]

;jump boot
	jmp Label_StartBoot
	nop

%include "mbrdata.inc"

;Booter
Label_StartBoot:
	
	mov ax,cs
	mov ss,ax
	mov sp,StackEnd+0x1000

	mov cx,Msg_Boot
	call printstring
	
	mov ax,0x1000
	mov ds,ax
	mov cl,0x80
	mov dx,0
	call GetDriveParameters

	cmp al,0
	jne Label_ErrGetDriveParameters
	mov ax,0x1000
	mov ds,ax
	mov ecx,[DriveParametersPacket.Sectors]
	mov dl,10
	push Msg_Number
	call itoa
	
	mov cx,Msg_Number
	call printstring
	jmp Label_ReadFs

Label_ErrGetDriveParameters:
	mov cl,al
	mov dl,10
	push Msg_Number
	call btoa

	mov cx,Msg_ErrGetDriveParameters
	call printstring
	mov cx,Msg_Error
	call printstring
	mov cx,Msg_Number
	call printstring
	
Label_ReadFs:

	cli
	hlt

Msg_Boot db 'ShineOS booter v0.01',0xa,0xd
	db 'Booting...',0xa,0xd,0xa,0xd,0
Msg_Error db 'Error Code: ',0
Msg_Number dq 0
Msg_ErrGetDriveParameters db 'Failed getting drive parameters!',0xa,0xd,0

;used functions
	func_printstring
	;func_putchar
	func_GetDriveParameters
	func_itoa
	func_btoa

;End of sector
	times 510-($-$$) db 0
	dw 0xaa55

StackEnd:
