;boot/booter.asm
;ASM
;ShineOS booter(MBR)

%include "std.inc"
%include "display.inc"
%include "string.inc"
%include "hdd.inc"

[bits 16]
[org 0x7c00]

;jump boot
	jmp Label_StartBoot
	nop

%include "mbrdata.inc"

;Booter
Label_StartBoot:
	
	mov ax,0x2000
	mov ss,ax
	mov sp,0xffff

	mov cx,Msg_Boot
	mov dx,0x700
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
	
Label_ReadFs:

	cli
	hlt

Msg_Boot db 'ShineOS booter v0.01',0xa,0xd
	db 'Booting...',0xa,0xd
Msg_NewLine db 0xa,0xd,0
Msg_Error db 'Error Code: ',0
Msg_Number dq 0
Msg_ErrGetDriveParameters db 'Failed getting drive parameters!',0xa,0xd,0

;used functions
	func_printstring
	;func_putchar
	func_GetDriveParameters
	func_btoa
	func_stoa
	func_itoa

;End of sector
	times 510-($-$$) db 0
	dw 0xaa55
