;boot/includes/hdd.inc
;real mode

struc part_entry
	.status resb 1	;0x00:inactive,0x80:active
	.first_head resb 1
	.first_sector resb 1
	.first_cylinder resb 1
	.type resb 1	;0x60 for SFS
	.last_head resb 1
	.last_sector resb 1
	.last_cylinder resb 1
	.first_lba resb 4
	.sectors resb 4
endstruc

struc dap
	.size resb 1
	.zero resb 1
	.sectors resb 2
	.offset resb 2
	.segment resb 2
	.startsector resb 8
endstruc

%macro func_readdrive 0
%ifndef _boot_includes_hdd_inc_func_readdrive
%define _boot_includes_hdd_inc_func_readdrive
readdrive:
	pusha
	mov si,dx
	mov dl,cl
	mov ax,cs
	mov ds,ax
	mov ah,0x42
	int 0x13
	jc %%error

	popa
	mov al,0
	jmp %%return
%%error:
	popa
	mov al,0xff
	jmp %%return
%%return:
	ret
%endif
%endmacro
