;boot/fs.inc
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
struc treedesc
	.present resb 1	;bool
	.pfiledesc resb 4
	.pprev resb 4
	.pnext resb 4
	.pparent resb 4
	.pchild resb 4
	.name resb (512-1-2-4-4-4-4)
endstruc
%define FILE_TYPE_ERROR 0
%define FILE_TYPE_DIR 1
%define FILE_TYPE_FILE 2
struc filedesc		;may need to add time & date
	.type resb 1
	.mode resb 4
	.size resb 8
	.pfile resb 4
endstruc
%define FILE_BYTES_PER_BLOCK (512-4-4-4-2)
struc fileblock
	.pdesc resb 4
	.pprev resb 4
	.pnext resb 4
	.eof resb 2	;eof=1 means 1 byte
	.data resb FILE_BYTES_PER_BLOCK
endstruc

;block index starts from 1,
;physical sector of 1st block = 1st sector number of partition + 64 + diskalloc + 1
;block size = 512 bytes

%macro func_initfs 0
%ifndef _boot_fs_inc_func_initfs_
%define _boot_fs_inc_func_initfs_
;_in_  ecx: point to a sdesc (extend to 1 sector(512 bytes))
;_in_  edx: first sector of the partition
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
%ifndef _boot_fs_inc_func_openfile_
%define _boot_fs_inc_func_openfile_
;_in_  ecx: physical sector number of 1st block index (treedesc of /)
;_in_  edx: point to the path
;_in_  stack 0-1: drive number
;_out_ eax: index of the file, or 0 if failed
;_out_ stack 0-7: file size	;special, preserve space for 8 bytes
%define _FUNC_OPENFILE_DISKTMP 0xf000
%define _FUNC_OPENFILE_START (_FUNC_OPENFILE_DISKTMP-4)	;section
%define _FUNC_OPENFILE_CURRENT (_FUNC_OPENFILE_START-4)	;section
%define _FUNC_OPENFILE_PATH (_FUNC_OPENFILE_CURRENT-4)
%define _FUNC_OPENFILE_DRIVE (_FUNC_OPENFILE_PATH-1)
%define _FUNC_OPENFILE_NEXTSLASH (_FUNC_OPENFILE_DRIVE-2)
%define _FUNC_OPENFILE_CURNAME (_FUNC_OPENFILE_NEXTSLASH-512)
openfile:
	push bp
	push di
	push si
	mov bp,sp
	
	mov [ss:_FUNC_OPENFILE_CURRENT],ecx
	dec ecx
	mov [ss:_FUNC_OPENFILE_START],ecx
	mov [ss:_FUNC_OPENFILE_PATH],edx
	mov dl,[ss:bp+2+2+2+2]
	mov [ss:_FUNC_OPENFILE_DRIVE],dl
	
	mov ax,0
	mov [ss:_FUNC_OPENFILE_NEXTSLASH],ax
%%startscan:
	cmp word [ss:_FUNC_OPENFILE_NEXTSLASH],-1
	jz %%endscan

	mov eax,[ss:_FUNC_OPENFILE_CURRENT]
	cmp eax,[ss:_FUNC_OPENFILE_START]
	jz %%error
	
	mov ecx,[ss:_FUNC_OPENFILE_PATH]
	mov dl,'/'
	call findchar
	mov [ss:_FUNC_OPENFILE_NEXTSLASH],ax
	cmp ax,-1
	jnz @f

	mov ecx,[ss:_FUNC_OPENFILE_PATH]
	call strlen
	mov cx,ax
	jmp %%copystr
@@
	mov cx,[ss:_FUNC_OPENFILE_NEXTSLASH]
%%copystr:
	mov si,[ss:_FUNC_OPENFILE_PATH]
	mov ds,[ss:_FUNC_OPENFILE_PATH+2]
	mov di,_FUNC_OPENFILE_CURNAME
	mov ax,ss
	mov es,ax
	cld
	rep movsb
	mov byte [es:di],0
@@
	push dword 0
	push dword [ss:_FUNC_OPENFILE_CURRENT]
	push ss
	push word _FUNC_OPENFILE_DISKTMP
	push dword 0x10010

	mov cl,[ss:_FUNC_OPENFILE_DRIVE]
	mov dx,ss
	shl edx,16
	mov dx,sp
	call readdrive
	mov sp,bp

	cmp al,0
	jnz %%error

	cmp byte [ss:_FUNC_OPENFILE_DISKTMP+treedesc.present],0
	jz %%error

	mov cx,ss
	shl ecx,16
	mov cx,_FUNC_OPENFILE_CURNAME
	mov dx,ss
	shl edx,16
	mov dx,_FUNC_OPENFILE_DISKTMP+treedesc.name
	call strcmp

	cmp al,0
	jz %%found

	mov ecx,[ss:_FUNC_OPENFILE_DISKTMP+treedesc.pnext]
	jecxz %%error
	add ecx,[ss:_FUNC_OPENFILE_START]
	mov [ss:_FUNC_OPENFILE_CURRENT],ecx
	jmp @b

%%found:
	mov ax,[ss:_FUNC_OPENFILE_NEXTSLASH]
	inc ax
	add [ss:_FUNC_OPENFILE_PATH],ax
	mov ecx,[ss:_FUNC_OPENFILE_DISKTMP+treedesc.pchild]
	add ecx,[ss:_FUNC_OPENFILE_START]
	mov [ss:_FUNC_OPENFILE_CURRENT],ecx
	jmp %%startscan

%%endscan:
	jmp @f
%%error:
	mov eax,0
	jmp %%return

@@
	mov eax,[ss:_FUNC_OPENFILE_DISKTMP+treedesc.pfiledesc]
	add eax,[ss:_FUNC_OPENFILE_START]
	push dword 0
	push eax
	push ss
	push word _FUNC_OPENFILE_DISKTMP
	push dword 0x10010

	mov cl,[ss:_FUNC_OPENFILE_DRIVE]
	mov dx,ss
	shl edx,16
	mov dx,sp
	call readdrive
	mov sp,bp

	cmp al,0
	jnz %%error

	cmp byte [ss:_FUNC_OPENFILE_DISKTMP+filedesc.type],FILE_TYPE_ERROR
	jz %%error

	mov si,bp
	mov ecx,[ss:_FUNC_OPENFILE_DISKTMP+filedesc.size]
	mov edx,[ss:_FUNC_OPENFILE_DISKTMP+filedesc.size+4]
	mov [ss:si+2+2+2+2],ecx
	mov [ss:si+2+2+2+2+4],edx

	mov eax,[ss:_FUNC_OPENFILE_DISKTMP+filedesc.pfile]

%%return:
	pop si
	pop di
	pop bp
	ret

%undef _FUNC_OPENFILE_DISKTMP
%undef _FUNC_OPENFILE_START
%undef _FUNC_OPENFILE_CURRENT
%undef _FUNC_OPENFILE_PATH
%undef _FUNC_OPENFILE_DRIVE
%undef _FUNC_OPENFILE_NEXTSLASH
%undef _FUNC_OPENFILE_CURNAME

	func_findchar
	func_readdrive
	func_strcmp
	func_strlen
%endif
%endmacro

%macro func_readfile 0
%ifndef _boot_fs_inc_func_readfile_
%define _boot_fs_inc_func_readfile_
;reads 1 block of specified file
;_in_  ecx: block to read
;_in_  dl: drive number
;_in_  stack 0-3: point to the buffer
;_out  al: 0 if failed
;buffer size==512 bytes
%define _FUNC_READFILE_DRIVE 0xf000
%define _FUNC_READFILE_SECTOR (_FUNC_READFILE_DRIVE-4)
readfile:
	push bp
	mov bp,sp
	
	push dword 0
	push ecx
	mov cl,dl
	push dword [ss:bp+2+2]
	push dword 0x10010
	mov dx,ss
	shl edx,16
	mov dx,sp
	call readdrive

	cmp al,0
	sete al

	mov sp,bp
	pop bp
	ret

	func_readdrive
%endif
%endmacro
