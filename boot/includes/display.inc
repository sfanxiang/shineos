;boot/includes/display.inc
;real mode

%macro func_putchar 0
%ifndef _boot_includes_display_inc_func_putchar_
%define _boot_includes_display_inc_func_putchar_
putchar:
	pusha
	mov al,cl
	mov bl,dl
	mov bh,0
	mov ah,0xe
	int 10h
	popa
	ret
%endif
%endmacro

%macro func_printstring 0
%ifndef _boot_includes_display_inc_func_printstring_
%define _boot_includes_display_inc_func_printstring_
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

%macro func_scrollscreenup 0
%ifndef _boot_includes_display_inc_func_scrollscreenup_
%define _boot_includes_display_inc_func_scrollscreenup_
scrollscreenup:
	pusha
	mov al,cl
	mov bh,dl
	mov cx,0
	mov dx,0x1950
	mov ah,6
	int 0x10
	popa
	ret
%endif
%endmacro

%macro func_setcursor 0
%ifndef _boot_includes_display_inc_func_setcursor_
%define _boot_includes_display_inc_func_setcursor_
setcursor:
	pusha
	mov dx,cx
	mov bh,0
	mov ah,2
	int 0x10
	popa
	ret
%endif
%endmacro

%macro func_initscreen 0
%ifndef _boot_includes_display_inc_func_initscreen_
%define _boot_includes_display_inc_func_initscreen_
initscreen:
	pusha
	mov ax,0x500
	int 0x10
	popa
	ret
%endif
%endmacro

%macro func_cls 0
%ifndef _boot_includes_display_inc_func_cls_
%define _boot_includes_display_inc_func_cls_
cls:
	call initscreen
	mov cl,0
	mov dl,7
	call scrollscreenup
	mov cx,0
	call setcursor
	ret

	func_initscreen
	func_scrollscreenup
	func_setcursor
%endif
%endmacro
