;boot/sldr.asm
;sldr

[bits 16]
%include "display.inc"
%include "fs.inc"
%include "hd.inc"
%include "std.inc"
%include "string.inc"

@@
;enable @f

label_code0:
	jmp short label_code0_start
	nop
	
readdrivepack0:
istruc dap
	at dap.size,db 0x10
	at dap.zero,db 0
	at dap.sectors,dw 1
	at dap.offset,dw label_code2
	at dap.segment,dw 0x7c0
	at dap.startsector,dq 1
iend
	
reserved times 32-($-label_code0) db 0
label_code0_start:
	jmp 0x7c0:label_jump
label_jump:
	cli

	mov ax,0x7c0
	mov ss,ax
	mov sp,0xe000

	mov [cs:init_dx],dx
	mov [cs:init_es],es
	mov [cs:init_di],di

	call cls

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

	jmp label_code2

;used functions
	func_cls

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
	sti
	jmp $
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
	at part_entry.sectors,dd 1
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

;------------------------------------------------
label_code2:
	jmp short label_code2_start
	nop

readdrivepack1:
istruc dap
	at dap.size,db 0x10
	at dap.zero,db 0
	at dap.sectors,dw 0x63
	at dap.offset,dw label_code3
	at dap.segment,dw 0x7c0
	at dap.startsector,dq 2
iend
label_code2_start:
	mov cl,[cs:init_dx]
	mov dx,cs
	shl edx,16
	mov dx,readdrivepack1
	call readdrive
	cmp al,0
	jnz label_diskerror

	jmp label_code3
times 510-($-label_code2) db 0
dw 0xaa55

;-------------------------------------------------
label_code3:
	jmp near label_code3_start
	nop
bootpart db 0xff	;0xff automatically detect bootable partition
bootcfgfile db '/boot/boot.cfg'
times 512-($-bootcfgfile) db 0

label_code3_start:
	cmp byte [cs:bootpart],0xff
	jz @f

	mov al,[cs:bootpart]
	mov bl,part1-part0
	mul bl
	add ax,part0
	mov si,ax
	jmp foundbootable
@@
	mov si,part0+part_entry.status
	mov cx,4
@@
	test byte [cs:si+part_entry.status],0x80
	jz searchnext
	cmp byte [cs:si+part_entry.type],0x60
	jz foundbootable
searchnext:
	add si,part1-part0
	loop @b

	mov cx,cs
	shl ecx,16
	mov cx,msg_nobootable
	mov dl,7
	call printstring
	jmp label_syshalt
msg_nobootable db 'No bootable partition with supported file system.',0xd,0xa,0

foundbootable:
	mov edx,[cs:si+part_entry.first_lba]
	mov [cs:pfs],edx
	mov cx,cs
	shl ecx,16
	mov cx,isdesc
	movsx ax,byte [cs:init_dx]
	push ax
	call initfs
	add sp,2

	cmp word [cs:isdesc+sdesc.magic],'sf'
	jz @f

	mov cx,cs
	shl ecx,16
	mov cx,msg_fserror
	mov dl,7
	call printstring
	jmp label_syshalt
msg_fserror db 'Error occurred when trying to read file system.',0xd,0xa,0
pfs dd 0

@@
	mov ecx,[cs:pfs]
	add ecx,[cs:isdesc+sdesc.diskalloc]
	add ecx,65
	mov dx,cs
	shl edx,16
	mov dx,bootcfgfile
	movsx ax,byte [cs:init_dx]
	sub sp,8-2
	push ax
	call openfile
	pop ebx
	pop ecx

	cmp eax,0
	jnz @f

openfileerror:
	mov cx,cs
	shl ecx,16
	mov cx,msg_openfileerror
	mov dl,7
	call printstring
	jmp label_syshalt
msg_openfileerror db 'Failed opening file(s).',0xd,0xa,0

@@
	jmp $

;used functions
	func_initfs
	func_openfile

;big stuffs
isdesc:

