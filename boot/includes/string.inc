;boot/includes/string.inc
;for booting

%macro func_btoa 0
%ifndef _boot_includes_string_inc_func_btoa_
%define _boot_includes_string_inc_func_btoa_
btoa:
	mov si,sp
	mov di,[ss:si+2]
	
	dec sp
	mov byte [ss:si-1],0

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
	mov ax,cs
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
	
	dec sp
	mov si,sp
	mov byte [ss:si],0

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
	mov ax,cs
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

	dec sp
	mov si,sp
	mov byte [ss:si],0

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
	mov ax,cs
	mov es,ax
	mov si,sp
	add sp,cx

	cld
	rep movsb
	pop ebx
	ret
%endif
%endmacro

%macro func_findchar 0
%ifndef _boot_includes_string_inc_func_findchar_
%define _boot_includes_string_inc_func_findchar_
;_in_  cx: point to the string
;_in_  dl: char to find
;_in_  dh: search from
;_out_ ax: index of the first char, or -1 if failed
findchar:
	push si
	mov ax,cs
	mov ds,ax
	mov si,cx
	movsx ax,dh
	add si,ax
	cld
%%search:
	lodsb
	cmp al,0
	jz %%failed
	cmp al,dl
	jnz %%search
	sub si,cx
	sub si,1
	mov ax,si
	pop si
	ret
%%failed:
	mov ax,-1
	pop si
	ret
%endif
%endmacro

;end of file
