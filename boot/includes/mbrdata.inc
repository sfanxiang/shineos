;boot/includes/mbrdata.inc
;boot MBR

;File System
	dq 'SFS'

@comment
;fat12
;OEM name
	db 'Shine OS'
;Bytes per sector
	dw 0x200
;Sectors per cluster
	db 0x1
;Reserved sectors count
	dw 0x1
;Number of FATs
	db 0x2
;Root entries count
	dw 0xe0
;Total sectors(16-bit)
	dw 0
;Media
	db 0xf8
;Sectors per FAT(FAT 16)
	dw 0x9
;Sectors per track
	dw 0x12
;Number of heads
	dw 0x2
;Hidden sectors
	dd 0
;Total sectors(32-bit)
	dd 0xb40
;Drive number
	db 0
;Reserved
	db 0
;Boot sign
	db 0x29
;Volume ID
	dd 0	;Need to configure
;Volume label
	times 11 db 0
;File system type
	dq 'FAT12'
@endcomment
