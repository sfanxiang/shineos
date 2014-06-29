;boot/file.inc
;shineos

%macro func_filereadline 0
%ifndef _boot_file_inc_func_filereadline_
%define _boot_file_inc_func_filereadline_
;_in_  ecx: current index of the file
;_in_  dx: current offset of the file
;_in_  stack 0-1: drive number
;_in_  stack 2-5: point to drive buffer (preserve 512 bytes, do not write during file operations)
;_in_  stack 6-9: point to output buffer (at big as you can)
;_in_  stack 10-13: physical sector number of 1st block index (treedesc of /)
;_out  al: 0 if failed, 1 if not eof, 2 if eof
;_out_ ecx: current index of the file	;special
;_out_ dx: current offset of the file	;special
%define _FUNC_FILEREADLINE_STARTSEC (0xf000-4)
%define _FUNC_FILEREADLINE_INDEX (_FUNC_FILEREADLINE_STARTSEC-4)
%define _FUNC_FILEREADLINE_OFFSET (_FUNC_FILEREADLINE_INDEX-2)
%define _FUNC_FILEREADLINE_DRIVE (_FUNC_FILEREADLINE_OFFSET-2)
%define _FUNC_FILEREADLINE_DRIVEBUFFER (_FUNC_FILEREADLINE_DRIVE-4)
%define _FUNC_FILEREADLINE_OUTBUFFER (_FUNC_FILEREADLINE_DRIVEBUFFER-4)
%define _FUNC_FILEREADLINE_LFFLAG (_FUNC_FILEREADLINE_OUTBUFFER-1)

filereadline:
	push bp
	mov bp,sp
	push bx
	push si
	push di
	
	mov [ss:_FUNC_FILEREADLINE_INDEX],ecx
	mov [ss:_FUNC_FILEREADLINE_OFFSET],dx
	mov ax,[ss:bp+2+2]
	mov [ss:_FUNC_FILEREADLINE_DRIVE],ax
	mov eax,[ss:bp+2+2+2]
	mov [ss:_FUNC_FILEREADLINE_DRIVEBUFFER],eax
	mov eax,[ss:bp+2+2+2+4]
	mov [ss:_FUNC_FILEREADLINE_OUTBUFFER],eax
	mov eax,[ss:bp+2+2+2+4+4]
	mov [ss:_FUNC_FILEREADLINE_STARTSEC],eax
	xor al,al
	mov [ss:_FUNC_FILEREADLINE_LFFLAG],al
	
%%startread:
	cmp word [ss:_FUNC_FILEREADLINE_OFFSET],0
	jnz %%skipread

	mov ecx,[ss:_FUNC_FILEREADLINE_INDEX]
	add ecx,[ss:_FUNC_FILEREADLINE_STARTSEC]
	dec ecx
	mov dl,[ss:_FUNC_FILEREADLINE_DRIVE]
	push dword [ss:_FUNC_FILEREADLINE_DRIVEBUFFER]
	call readfile
	add sp,4
	cmp al,0
	jz %%errret

%%skipread:
	mov si,[ss:_FUNC_FILEREADLINE_DRIVEBUFFER+2]
	mov ds,si
	mov si,[ss:_FUNC_FILEREADLINE_DRIVEBUFFER]
	cmp dword [si+fileblock.pnext],0
	jnz @f
	
	mov al,2
	mov bx,[si+fileblock.eof]
	cmp [ss:_FUNC_FILEREADLINE_OFFSET],bx
	jnb %%return

@@
	add si,fileblock.data
	add si,[ss:_FUNC_FILEREADLINE_OFFSET]
	mov al,[si]
	cmp al,0xa
	sete byte [ss:_FUNC_FILEREADLINE_LFFLAG]
	
	mov di,[ss:_FUNC_FILEREADLINE_OUTBUFFER+2]
	mov es,di
	mov di,[ss:_FUNC_FILEREADLINE_OUTBUFFER]
	mov [es:di],al

	inc word [ss:_FUNC_FILEREADLINE_OUTBUFFER]
	
	inc word [ss:_FUNC_FILEREADLINE_OFFSET]
	cmp word [ss:_FUNC_FILEREADLINE_OFFSET],FILE_BYTES_PER_BLOCK
	jb %%prestartread
	
	mov si,[ss:_FUNC_FILEREADLINE_DRIVEBUFFER+2]
	mov ds,si
	mov si,[ss:_FUNC_FILEREADLINE_DRIVEBUFFER]
	mov al,2
	cmp dword [si+fileblock.pnext],0
	jz %%return
	
	mov eax,[si+fileblock.pnext]
	mov [ss:_FUNC_FILEREADLINE_INDEX],eax
	mov ax,0
	mov [ss:_FUNC_FILEREADLINE_OFFSET],ax
	
%%prestartread:
	cmp byte [ss:_FUNC_FILEREADLINE_LFFLAG],0
	jz %%startread
	
	mov al,1
	dec word [ss:_FUNC_FILEREADLINE_OUTBUFFER]
	
%%return:
	mov ds,[ss:_FUNC_FILEREADLINE_OUTBUFFER+2]
	mov si,[ss:_FUNC_FILEREADLINE_OUTBUFFER]
	mov byte [si],0
	mov ecx,[ss:_FUNC_FILEREADLINE_INDEX]
	mov edx,[ss:_FUNC_FILEREADLINE_OFFSET]
	
%%errret:
	pop di
	pop si
	pop bx
	pop bp
	ret

;used functions
	func_readfile
%endif
%endmacro
