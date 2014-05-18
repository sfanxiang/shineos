;boot/includes/string.inc
;for booting

%macro func_btoa 0
%ifndef _boot_includes_string_inc_func_btoa_
%define _boot_includes_string_inc_func_btoa_
btoa:
	mov si,sp
	mov di,[ss:si+2]
	push byte 0
	mov al,cl
	mov cx,1
%%BeginDiv:
	inc cx
	mov ah,0
	div dl
	cmp ah,10
	jb %%Number
	add ah,'a'-'0'
%%Number:
	add ah,'0'
	push ax
	inc sp
	cmp al,0
	jne %%BeginDiv
	
	mov ax,ss
	mov ds,ax
	mov es,ax
	mov si,sp
	add sp,cx

	cld
	rep movsb
	ret
%endif
%endmacro

%macro func_stoa 0
%ifndef _boot_includes_string_inc_func_stoa_
%define _boot_includes_string_inc_func_stoa_
stoa:
	mov si,sp
	mov di,[ss:si+2]
	push ebx
	movsx bx,dl
	push 0
	mov ax,cx
	mov cx,1
%%BeginDiv:
	inc cx
	mov dx,0
	div bx
	cmp dl,10
	jb %%Number
	add dl,'a'-'0'
%%Number:
	add dl,'0'
	mov dh,dl
	push dx
	inc sp
	cmp ax,0
	jne %%BeginDiv
	
	mov ax,ss
	mov ds,ax
	mov es,ax
	mov si,sp
	add sp,cx

	cld
	rep movsb
	pop ebx
	ret
%endif
%endmacro

%macro func_itoa 0
%ifndef _boot_includes_string_inc_func_itoa_
%define _boot_includes_string_inc_func_itoa_
itoa:
	mov si,sp
	mov di,[ss:si+2]
	push ebx
	movsx ebx,dl
	push 0
	mov eax,ecx
	mov cx,1
%%BeginDiv:
	inc cx
	mov edx,0
	div ebx
	cmp dl,10
	jb %%Number
	add dl,'a'-'0'
%%Number:
	add dl,'0'
	mov dh,dl
	push dx
	inc sp
	cmp eax,0
	jne %%BeginDiv
	
	mov ax,ss
	mov ds,ax
	mov es,ax
	mov si,sp
	add sp,cx

	cld
	rep movsb
	pop ebx
	ret
%endif
%endmacro

;end of file
