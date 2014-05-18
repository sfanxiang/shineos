;/boot/includes/hdd.inc
;real mode

struc DriveParametersPacket
	.Size resw 30
	.Flags resw 0
	.Cylinders resd 0
	.Heads resd 0
	.SectorsPerTrack resd 0
	.Sectors resq 0
	.BytesPerSector resw 0
	.EDD_Offset resw 0
	.EDD_Segment resw 0
endstruc

%macro func_GetDriveParameters 0
%ifndef	_boot_includes_hdd_inc_func_GetDriveParameters_
%define _boot_includes_hdd_inc_func_GetDriveParameters_
GetDriveParameters:
	push di	
	mov di,dx
	mov dl,cl
	mov ah,0x48
	int 0x13
	
	jc %%Failed
	mov ax,0
	ret
%%Failed:
	movsx ax,ah
	ret
%endif
%endmacro
