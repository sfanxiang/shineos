;boot/includes/fs.inc
;shineos

struc sdesc
	.magic resb 2	;should be 'sf'
	.size resb 4	;sectors
	.state resb 2
	.diskalloc resb 4
	.treealloc resb 4
	.wtime resb 8
	.name resb 16	;null-terminated string
endstruc

%macro func_initfs 0
%ifndef _boot_includes_fs_inc_func_initfs_
%define _boot_includes_fs_inc_func_initfs_
;_in_  ecx: first section of a partition
;_in_  dl: drive number
;_out_ ax: pointer to a sdesc
initfs:
	pusha
	add ecx,64
	mov [cs:%%readdap+dap.startsector],ecx
	mov cl,dl
	mov ax,sp
	add ax,0x1000
	mov [cs:%%readdap+dap.offset],ax
	mov ax,ss
	mov [cs:%%readdap+dap.segment],ax
	mov dx,%%readdap
	call readdrive
	cmp al,0
	jnz %%error

	mov bp,sp
	add bp,0x1000
	cmp word [ss:bp+sdesc.magic],'sf'
	jnz %%error
	
	popa
	mov ax,sp
	add ax,0x1000
	ret
%%error:
	popa
	mov ax,0
	ret

%%readdap:
istruc dap
	at dap.size,db 0x10
	at dap.zero,db 0
	at dap.sectors,dw 1
	at dap.offset,dw 0
	at dap.segment,dw 0
	at dap.startsector,dq 0
iend
	func_readdrive
%endif
%endmacro
