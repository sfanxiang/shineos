;boot/includes/hdd.inc
;real mode

struc DriveParametersPacket
	.Size resb 2
	.Flags resb 2
	.Cylinders resb 4
	.Heads resb 4
	.SectorsPerTrack resb 4
	.Sectors resb 8
	.BytesPerSector resb 2
	.EDD_Offset resb 2
	.EDD_Segment resb 2
endstruc

%macro func_GetDriveParameters 0
%ifndef	_boot_includes_hdd_inc_func_GetDriveParameters_
%define _boot_includes_hdd_inc_func_GetDriveParameters_
GetDriveParameters:
	push di	
	mov di,dx
	mov word [di],30
	mov dl,cl
	mov ah,0x48
	int 0x13
	
	jc %%Failed
	mov ax,0
	jmp %%Return
%%Failed:
	movsx ax,ah

%%Return:
	pop di
	ret
%endif
%endmacro
