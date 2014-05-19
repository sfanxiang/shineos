;boot/includes/display.inc
;real mode

%macro func_putchar 0
%ifndef _boot_includes_display_inc_func_putchar_
%define _boot_includes_display_inc_func_putchar_
putchar:
	push bx
	mov al,cl
	mov bh,dl
	mov bl,dh
	mov ah,0xe
	int 10h
	pop bx
	ret
%endif
%endmacro

%macro func_printstring 0
%ifndef _boot_includes_display_inc_func_printstring_
%define _boot_includes_display_inc_func_printstring_
printstring:
	push bx
	push dx
	mov bx,cx
	
%%begin:
	cmp byte [cs:bx],0
	je %%exit
	
	mov cl,[cs:bx]
	pop dx
	push dx
	call putchar
	inc bx
	jmp %%begin
%%exit:
	pop dx
	pop bx
	ret
	
	func_putchar
%endif
%endmacro
