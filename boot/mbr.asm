;boot/mbr.asm
;shineos mbr

[org 0x7c00]
[bits 16]
%include "display.inc"
%include "hd.inc"
%include "std.inc"

code0:
	jmp label_start
	nop
reserved times 32-($-code0) db 0
label_start:
	cli

	mov [cs:init_dx],dx
	mov [cs:init_es],es
	mov [cs:init_di],di
	
	mov ah,0
	int 0x13
	jc label_diskerror

	mov di,part0+part_entry.status
	mov cx,0
label_searchbootpart:
	cmp cx,4
	jnb label_nobootpart
	cmp byte [cs:di],0x80
	jnz label_searchbootnext
	jmp label_foundbootpart
label_searchbootnext:
	inc cx
	jmp label_searchbootpart

label_foundbootpart:
	jmp $


times 218-($-code0) db 0

zero dw 0
orginal_drive db 0x80	;0x80 to 0xff
;timestamp
seconds db 0
minutes db 0
hours db 0

code1:
label_error:
	sti
	mov cx,msg_introduction
	mov dx,0x700
	call printstring
	ret
label_diskerror:
	call label_error
	mov cx,msg_diskerror
	mov dx,0x700
	call printstring
	jmp $
label_nobootpart:
	call label_error
	mov cx,msg_nobootpart
	mov dx,0x700
	call printstring
	jmp $

init_dx dw 0
init_es dw 0
init_di dw 0

msg_introduction db '(shineos,mbr):',0xd,0xa,0
msg_diskerror db 'Something wrong with disk operation.',0xd,0xa,0
msg_nobootpart db 'No bootable partition.',0xd,0xa,0
;used functions
	func_printstring

times 216-($-code1) db 0
sign dd 0
copy_protect dw 0	;0x5a5a if protected

;partition table
part0:
istruc part_entry
	at part_entry.status,db 0x0	;0x00:inactive,0x80:active
	at part_entry.first_head,db 0
	at part_entry.first_sector,db 0
	at part_entry.first_cylinder,db 0
	at part_entry.type,db 0x60
	at part_entry.last_head,db 0
	at part_entry.last_sector,db 0
	at part_entry.last_cylinder,db 0
	at part_entry.first_lba,dd 0
	at part_entry.sectors,dd 0
iend
part1:
istruc part_entry
	at part_entry.status,db 0
	at part_entry.first_head,db 0
	at part_entry.first_sector,db 0
	at part_entry.first_cylinder,db 0
	at part_entry.type,db 0
	at part_entry.last_head,db 0
	at part_entry.last_sector,db 0
	at part_entry.last_cylinder,db 0
	at part_entry.first_lba,dd 0
	at part_entry.sectors,dd 0
iend
part2:
istruc part_entry
	at part_entry.status,db 0
	at part_entry.first_head,db 0
	at part_entry.first_sector,db 0
	at part_entry.first_cylinder,db 0
	at part_entry.type,db 0
	at part_entry.last_head,db 0
	at part_entry.last_sector,db 0
	at part_entry.last_cylinder,db 0
	at part_entry.first_lba,dd 0
	at part_entry.sectors,dd 0
iend
part3:
istruc part_entry
	at part_entry.status,db 0
	at part_entry.first_head,db 0
	at part_entry.first_sector,db 0
	at part_entry.first_cylinder,db 0
	at part_entry.type,db 0
	at part_entry.last_head,db 0
	at part_entry.last_sector,db 0
	at part_entry.last_cylinder,db 0
	at part_entry.first_lba,dd 0
	at part_entry.sectors,dd 0
iend
;boot signature
dw 0xaa55
