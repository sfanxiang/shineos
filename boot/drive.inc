%ifndef DRIVE_INC
%define DRIVE_INC

struc dap
	.size resb 1
	.zero resb 1
	.sectors resb 2
	.offset resb 2
	.segment resb 2
	.startsector resb 8
endstruc
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

%macro func_readdrive 0
%ifndef _boot_hdd_inc_func_readdrive
%define _boot_hdd_inc_func_readdrive
;_in_  cl: drive number
;_in_  edx: point to the dap
;_out_ al: 0 if succeed
readdrive:
	push ds
	push si
	
	mov si,dx
	shr edx,16
	mov ds,dx
	mov dl,cl
	mov ah,0x42
	int 0x13
	jc %%error

	mov al,0
	jmp %%return
%%error:
	mov al,0xff
%%return:
	pop si
	pop ds
	ret
%endif
%endmacro

%endif
