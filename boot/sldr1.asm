;boot/sldr1.asm
;sldr

[bits 16]

%include "display.inc"
%include "file.inc"
%include "fs.inc"
%include "hd.inc"
%include "std.inc"
%include "string.inc"

@@
;enable @f

%define part0 446
%define part1 462
%define part2 478
%define part3 494

;symbols for free space

%define isdesc freespace
%define pfs (isdesc+512)
%define fileindex (pfs+4)
%define fileoffset (fileindex+4)
%define drivebuffer (fileoffset+2)
%define itemcount (drivebuffer+512)
%define itembuffer (itemcount+1)

%define itemindex freespace
%define strbuffer (itemindex+1)

label_code2:
	jmp short label_code2_start
	nop

readdrivepack1:
istruc dap
	at dap.size,db 0x10
	at dap.zero,db 0
	at dap.sectors,dw 0x63
	at dap.offset,dw label_code3
	at dap.segment,dw 0x800
	at dap.startsector,dq 2
iend
custom_dx db 0
custom_es db 0
custom_di db 0
init_dx db 0
init_es db 0
init_di db 0

label_code2_start:
	cli

	cmp byte [cs:custom_dx],0
	jnz @f
	mov [cs:init_dx],dx
@@
	cmp byte [cs:custom_es],0
	jnz @f
	mov [cs:init_es],es
@@
	cmp byte [cs:custom_di],0
	jnz @f
	mov [cs:init_di],di
@@
	
	mov sp,0x800
	mov ss,sp
	mov sp,0xe000

	mov cl,[cs:init_dx]
	mov dx,cs
	shl edx,16
	mov dx,readdrivepack1
	call readdrive
	cmp al,0
	jnz label_diskerror

	jmp label_code3

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

;used functions
	func_printstring
	func_readdrive

times 510-($-label_code2) db 0
dw 0xaa55

;-------------------------------------------------
label_code3:
	jmp near label_code3_start
	nop
bootpart db 0xff	;0xff automatically detect bootable partition
bootdefault db 0
boottimeout db 10
bootcfgfile db '/boot/sldr.cfg'
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
	mov ax,0x7c0
	mov ds,ax
@@
	test byte [si+part_entry.status],0x80
	jz searchnext
	cmp byte [si+part_entry.type],0x60
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
	mov edx,[si+part_entry.first_lba]
	mov [cs:pfs],edx
	mov cx,cs
	shl ecx,16
	mov cx,isdesc
	mov al,byte [cs:init_dx]
	xor ah,ah
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
msg_fserror db 'Bad file system.',0xd,0xa,0

@@
	mov ecx,[cs:pfs]
	add ecx,[cs:isdesc+sdesc.diskalloc]
	add ecx,65
	mov [cs:pfs],ecx

	mov dx,cs
	shl edx,16
	mov dx,bootcfgfile
	mov al,byte [cs:init_dx]
	xor ah,ah
	sub sp,8-2
	push ax
	call openfile
	pop ebx
	pop ecx

	cmp eax,0
	jnz @f

opencfgfileerror:
	mov cx,cs
	shl ecx,16
	mov cx,msg_openfileerror0
	mov dl,7
	call printstring
	mov cx,cs
	shl ecx,16
	mov cx,bootcfgfile
	mov dl,7
	call printstring
	mov cx,cs
	shl ecx,16
	mov cx,msg_openfileerror1
	mov dl,7
	call printstring
	jmp label_syshalt
msg_openfileerror0 db 'Cannot open "',0
msg_openfileerror1 db '".',0xd,0xa,0

@@
	mov [cs:fileindex],eax
	mov ax,0
	mov [cs:fileoffset],ax
	mov [cs:itemcount],al

	mov bl,1
	mov bp,itembuffer
	
%macro cfgfilereadline 0
	mov ecx,[cs:fileindex]
	mov dx,[cs:fileoffset]
	push dword [cs:pfs]
	push cs
	push bp
	push cs
	push word drivebuffer
	mov al,[cs:init_dx]
	mov ah,0
	push ax
	call filereadline
	add sp,14
	cmp al,0
	jz readcfgfileerror
	mov [cs:fileindex],ecx
	mov [cs:fileoffset],dx
	mov bl,al
%endmacro

@@
	cmp bl,2
	jz printprompt
	
	cfgfilereadline
	
	cmp byte [cs:bp],0
	jz @b
	cmp byte [cs:bp],'#'
	jz @b
	
	cmp byte [cs:bp],'$'
	jnz additem
	
	;test symbol_default
	mov cx,cs
	shl ecx,16
	mov cx,symbol_default
	mov dx,cs
	shl edx,16
	mov dx,bp
	inc dx
	call strcmp
	cmp al,0
	jnz test_timeout
	
	cfgfilereadline
	mov cx,cs
	shl ecx,16
	mov cx,bp
	mov dl,10
	call atob
	mov [cs:bootdefault],al
	jmp @b
	
test_timeout:
	mov cx,cs
	shl ecx,16
	mov cx,symbol_timeout
	mov dx,cs
	shl edx,16
	mov dx,bp
	inc dx
	call strcmp
	cmp al,0
	jnz @b
	
	cfgfilereadline
	mov cx,cs
	shl ecx,16
	mov cx,bp
	mov dl,10
	call atob
	mov [cs:boottimeout],al
	jmp @b
	
additem:
	add bp,512
	cfgfilereadline
	inc byte [cs:itemcount]
	add bp,512
	jmp @b
	
msg_crlf db 0xd,0xa,0
symbol_default db 'default',0
symbol_timeout db 'timeout',0

readcfgfileerror:
	mov cx,cs
	shl ecx,16
	mov cx,msg_readfileerror0
	mov dl,7
	call printstring
	mov cx,cs
	shl ecx,16
	mov cx,bootcfgfile
	mov dl,7
	call printstring
	mov cx,cs
	shl ecx,16
	mov cx,msg_readfileerror1
	mov dl,7
	call printstring
	jmp label_syshalt
msg_readfileerror0 db 'Bad file "',0
msg_readfileerror1 db '".',0xd,0xa,0

;used functions
	func_atob
	func_btoa
	func_cls
	func_initfs
	func_openfile
	func_putchar
	func_readfile
	func_filereadline
	
printprompt:
	mov cx,cs
	shl ecx,16
	mov cx,msg_crlf
	mov dl,7
	call printstring
	
	mov al,1
	mov [cs:itemindex],al
	mov bp,itembuffer
	
@@
	mov al,[cs:itemindex]
	cmp al,[cs:itemcount]
	ja @f

	mov cx,cs
	shl ecx,16
	mov cx,strbuffer
	mov dl,[cs:itemindex]
	push word 10
	call btoa
	add sp,2
	
	mov cx,cs
	shl ecx,16
	mov cx,strbuffer
	mov dl,7
	call printstring
	
	mov cl,' '
	mov dl,7
	call putchar
	
	mov cx,cs
	shl ecx,16
	mov cx,bp
	mov dl,7
	call printstring
	
	add bp,512*2
	inc byte [cs:itemindex]
	
	mov cx,cs
	shl ecx,16
	mov cx,msg_crlf
	mov dl,7
	call printstring
	
	jmp @b

@@
	mov cx,cs
	shl ecx,16
	mov cx,msg_promptchoose
	mov dl,7
	call printstring
	
	mov cx,cs
	shl ecx,16
	mov cx,strbuffer
	mov dl,[cs:bootdefault]
	push word 10
	call btoa
	add sp,2
	
	mov cx,cs
	shl ecx,16
	mov cx,strbuffer
	mov dl,7
	call writestring
	
	jmp $
	;todo
	
msg_promptchoose db 'Choose one to boot: ',0

;used functions
	func_writestring

freespace:

