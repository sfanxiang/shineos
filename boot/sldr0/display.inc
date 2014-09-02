%ifndef _DISPLAY_INC
%define _DISPLAY_INC

%macro func_putchar 0
%ifndef _boot_display_inc_func_putchar_
%define _boot_display_inc_func_putchar_
putchar:
	push bx
	mov al,cl
	mov bl,dl
	mov bh,0
	mov ah,0xe
	int 10h
	pop bx
	ret
%endif
%endmacro

%macro func_printstring 0
%ifndef _boot_display_inc_func_printstring_
%define _boot_display_inc_func_printstring_
printstring:
	push bx
	push ds

	mov bx,cx
	shr ecx,16
	mov ds,cx
	
%%begin:
	cmp byte [ds:bx],0
	je %%exit
	
	mov cl,[ds:bx]
	call putchar
	inc bx
	jmp %%begin
%%exit:
	pop ds
	pop bx
	ret
	
	func_putchar
%endif
%endmacro

%macro func_writestring 0
%ifndef _boot_display_inc_func_writestring_
%define _boot_display_inc_func_writestring_
writestring:
	push bx

	push ecx
	push dx
	
	mov bh,0
	mov ah,3
	int 0x10
	
	mov bx,dx
	pop dx
	pop ecx
	
	call printstring
	
	mov dx,bx
	mov bh,0
	mov ah,2
	int 0x10
	
	pop bx
	ret
	
	func_printstring
%endif
%endmacro

%macro func_setvideomode 0
%ifndef _boot_display_inc_func_setvideomode_
%define _boot_display_inc_func_setvideomode_
setvideomode:
	mov ah,0
	mov al,cl
	int 0x10
	ret
%endif
%endmacro

%endif
