[bits 16]
%include "display.inc"
%include "drive.inc"
%include "std.inc"

@@
;enable @f

label_code0:
	jmp short label_code0_start
	nop
	
readdrivepack0:
istruc dap
	at dap.size,db 0x10
	at dap.zero,db 0
	at dap.sectors,dw 64
	at dap.offset,dw 0
	at dap.segment,dw 0x8000
	at dap.startsector,dq 1
iend
	
reserved times 32-($-label_code0) db 0
label_code0_start:
	jmp 0x7c0:label_jump
label_jump:
	mov ax,0x7c0
	mov ss,ax
	mov sp,0xe000

	mov [cs:init_dx],dx
	mov [cs:init_es],es
	mov [cs:init_di],di

	mov cl,3
	call setvideomode

	mov dl,[cs:init_dx]
	mov ah,0
	int 0x13
	jc label_diskerror

	mov cl,[cs:init_dx]
	mov dx,cs
	shl edx,16
	mov dx,readdrivepack0
	call readdrive
	cmp al,0
	jnz label_diskerror

	mov dx,[cs:init_dx]
	mov di,[cs:init_es]
	mov es,di
	mov di,[cs:init_di]
	mov ax,[cs:readdrivepack0+dap.segment]
	mov ds,ax
	mov ss,ax
	xor sp,sp
	
	push word [cs:readdrivepack0+dap.segment]
	push word [cs:readdrivepack0+dap.offset]
	retf

;used functions
	func_setvideomode

times 218-($-label_code0) db 0

zero dw 0
orginal_drive db 0x80	;0x80 to 0xff

;timestamp
seconds db 0
minutes db 0
hours db 0

label_code1:
label_syshalt:
	mov cx,cs
	shl ecx,16
	mov cx,msg_syshalt
	mov dl,7
	call printstring
@@
	hlt
	jmp @b
label_diskerror:
	mov cx,cs
	shl ecx,16
	mov cx,msg_diskerror
	mov dl,7
	call printstring
	jmp label_syshalt

msg_syshalt db 'SLDR: System halted.',0xd,0xa,0
msg_diskerror db 'Something wrong with disk operation.',0xd,0xa,0

init_dx dw 0
init_es dw 0
init_di dw 0

;used functions
	func_printstring
	func_readdrive

times 216-($-label_code1) db 0
sign dd 0
copy_protect dw 0	;0x5a5a if protected

;partition table
part0:
istruc part_entry
	at part_entry.status,db 0x80	;0x00:inactive,0x80:active
	at part_entry.first_head,db 0
	at part_entry.first_sector,db 0
	at part_entry.first_cylinder,db 0
	at part_entry.type,db 0x60
	at part_entry.last_head,db 0
	at part_entry.last_sector,db 0
	at part_entry.last_cylinder,db 0
	at part_entry.first_lba,dd 0x100
	at part_entry.sectors,dd 64
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
