;boot/part0.asm
;sldr part0

[bits 16]
%include "display.inc"
%include "hd.inc"
%include "std.inc"

code0:
	jmp label_start
	nop
	
readdrivepack:
istruc dap
	at dap.size,db 0x10
	at dap.zero,db 0
	at dap.sectors,dw 1
	at dap.offset,dw 0
	at dap.segment,dw 0x800
	at dap.startsector,dq 1
iend
	
reserved times 32-($-code0) db 0
label_start:
	jmp 0x7c0:label_jump
label_jump:
	cli

	mov ax,0
	mov ss,ax
	mov sp,0xf000

	mov [cs:init_dx],dx
	mov [cs:init_es],es
	mov [cs:init_di],di
	
	mov cx,msg_introduction
	mov dx,0x700
	call printstring

	mov dl,[cs:init_dx]
	mov ah,0
	int 0x13
	jc label_diskerror

	mov cl,[cs:init_dx]
	mov dx,readdrivepack
	call readdrive
	cmp ax,0
	jnz label_diskerror

	mov cx,msg_ok
	mov dx,0x700
	call printstring

	mov dx,[cs:init_dx]
	mov es,[cs:init_es]
	mov di,[cs:init_di]
	jmp 0x800:0

times 218-($-code0) db 0

zero dw 0
orginal_drive db 0x80	;0x80 to 0xff
;timestamp
seconds db 0
minutes db 0
hours db 0

code1:
label_diskerror:
	mov cx,msg_diskerror
	mov dx,0x700
	call printstring

	jmp $

init_dx dw 0
init_es dw 0
init_di dw 0

msg_introduction db 'SLDR(part0):',0xd,0xa,0
msg_ok db 'Ok. Now loading part 1.',0xd,0xa,0
msg_diskerror db 'Something wrong with disk operation.',0xd,0xa,0
;used functions
	func_printstring
	func_readdrive

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
