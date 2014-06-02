;boot/includes/fs.inc
;shineos

struc sdesc
	.magic resb 2	;should be 'sf'
	.size resb 4	;sectors
	.state resb 2
	.diskalloc resb 4	;sectors
	;.treealloc resb 4	;sectors
	.wtime resb 8
	.name resb 16	;null-terminated string
endstruc
%define FILE_TYPE_ERROR 0
%define FILE_TYPE_DIR 1
%define FILE_TYPE_FILE 2
struc treedesc
	.type resb 1
	.attrib resb 2
	.pfile resb 4	;sector
	.pprev resb 4	;sector
	.pnext resb 4	;sector
	.pparent resb 4	;sector
	.pchild resb 4	;sector
	.name resb (512-1-2-4-4-4-4)
endstruc
struc filesec
	.pprev resb 4	;sector
	.size resb 8
	.data resb (512-4-8-4)
	.pnext resb 4	;sector
endstruc

%macro func_initfs 0
%ifndef _boot_includes_fs_inc_func_initfs_
%define _boot_includes_fs_inc_func_initfs_
;_in_  ecx: point to a sdesc (extend to 1 sector(512 bytes))
;_in_  edx: first section of a partition
;_in_  stack 0-1: drive number
;_out_ eax: point to a sdesc, sdesc.magic=='sf' if succeed 
initfs:
	push bp
	push di
	push ecx
	mov bp,sp

	add edx,64
	push dword 0
	push edx
	mov di,cx
	shr ecx,16
	mov ds,cx
	push ds
	push di
	push dword 0x10010

	mov cl,[ss:bp+10]
	mov dx,ss
	shl edx,16
	mov dx,sp
	call readdrive
	cmp al,0
	jz %%ok

	mov byte [di+sdesc.magic],0

%%ok:
	mov sp,bp	
	pop eax
	pop di
	pop bp
	ret

	func_readdrive
%endif
%endmacro

%macro func_openfile 0
%ifndef _boot_includes_fs_inc_func_readfile_
%define _boot_includes_fs_inc_func_readfile_
;_in_  ecx: section number of partition+64+diskalloc+1 (treedesc of /)
;_in_  edx: point to the path
;_in_  stack 0-1: drive number
;_out_ eax: point to the file (start section-treedesc of /), or 0 if failed
%define TREEDESC 0xf000
%define START (TREEDESC-4)	;section
%define CURRENT (START-4)	;section
%define PATH (CURRENT-4)
%define DRIVE (PATH-1)
%define NEXTSLASH (DRIVE-2)
%define CURNAME (NEXTSLASH-512)
openfile:
	push bp
	mov bp,sp
	
	mov [ss:START],ecx
	dec dword [ss:START]
	mov [ss:CURRENT],ecx
	mov [ss:PATH],edx
	mov dl,[ss:bp+4]
	mov [ss:DRIVE],dl
	
	mov ax,0
	mov [ss:NEXTSLASH],ax
%%startscan:
	cmp word [ss:NEXTSLASH],-1
	jz %%endscan

	mov eax,[ss:CURRENT]
	cmp eax,[ss:START]
	jz %%error
	
	mov ecx,[ss:PATH]
	mov dl,'/'
	call findchar
	mov [ss:NEXTSLASH],ax
	cmp ax,-1
	jnz @f

	mov ecx,[ss:PATH]
	call strlen
	mov cx,ax
	jmp %%copystr
@@
	mov cx,[ss:NEXTSLASH]
%%copystr:
	mov si,[ss:PATH]
	mov ds,[ss:PATH+2]
	mov di,CURNAME
	mov ax,ss
	mov es,ax
	cld
	rep movsb
	mov byte [es:di],0
@@
	push dword 0
	push dword [ss:CURRENT]
	push ss
	push word TREEDESC
	push dword 0x10010

	mov cl,[ss:DRIVE]
	mov dx,ss
	shl edx,16
	mov dx,sp
	call readdrive
	mov sp,bp

	cmp al,0
	jnz %%error

	cmp byte [ss:TREEDESC+treedesc.type],FILE_TYPE_ERROR
	jz %%error

	mov cx,ss
	shl ecx,16
	mov cx,CURNAME
	mov dx,ss
	shl edx,16
	mov dx,TREEDESC+treedesc.name
	call strcmp

	cmp al,0
	jz %%found

	mov ecx,[ss:TREEDESC+treedesc.pnext]
	jecxz %%error
	add ecx,[ss:START]
	mov [ss:CURRENT],ecx
	jmp @b

%%found:
	mov ax,[ss:NEXTSLASH]
	inc ax
	add [ss:PATH],ax
	mov ecx,[ss:TREEDESC+treedesc.pchild]
	add ecx,[ss:START]
	mov [ss:CURRENT],ecx
	jmp %%startscan

%%endscan:
	mov eax,[ss:TREEDESC+treedesc.pfile]
	jmp %%return
%%error:
	mov eax,0
%%return:
	pop bp
	ret

	func_findchar
	func_readdrive
	func_strcmp
	func_strlen
%endif
%endmacro
