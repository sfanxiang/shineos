;boot/includes/fs.inc
;shineos

struc sdesc
	.magic resb 2	;should be 'sf'
	.size resb 4	;sectors
	.state resb 2
	.diskalloc resb 4	;sectors
	.treealloc resb 4	;sectors
	.wtime resb 8
	.name resb 16	;null-terminated string
endstruc
@define FILE_TYPE_DIR 1
@define FILE_TYPE_FILE 2
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
;_in_  ecx: point to a sdesc
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
	mov ds,cx
	shr ecx,16
	mov di,cx
	push ds
	push cx
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
;_in_  ecx: section number of partition + diskalloc (=start of treealloc)
;_in_  edx: point to the path
;_in_  stack 0-1: drive number	;!!!todo!!!
;_out_ eax: point to the file (start section), or 0 if failed
readfile:
	push bp
	mov bp,sp
	
	push dword 0
	push ecx
	push ss
	push word 0xf000
	push dword 0x10010
	;todo
%endif
%endmacro
