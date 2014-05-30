;boot/includes/string.inc

;btoa, stoa, itoa are used for unsigned integer numbers
%macro func_btoa 0
%ifndef _boot_includes_string_inc_func_btoa_
%define _boot_includes_string_inc_func_btoa_
;_in_  cl: number
;_in_  dl: radix
;_in_  stack 0-3: point to the string
btoa:
	mov bp,sp

	mov si,sp
	mov ax,[ss:si+4]
	mov es,ax
	mov di,[ss:si+2]

	mov sp,0xfffe
	mov byte [ss:0xffff],0

	mov al,cl
	mov cx,1
%%BeginDiv:
	inc cx
	mov ah,0
	div dl
	cmp ah,10
	jb %%Number
	add ah,'a'-'0'-10
%%Number:
	add ah,'0'
	push ax
	inc sp
	cmp al,0
	jne %%BeginDiv
	
	mov ax,ss
	mov ds,ax
	mov si,sp
	rep movsb

	mov sp,bp

	ret
%endif
%endmacro

%macro func_stoa 0
%ifndef _boot_includes_string_inc_func_stoa_
%define _boot_includes_string_inc_func_stoa_
;_in_  cx: number
;_in_  dl: radix
;_in_  stack 0-3: point to the string
stoa:
	mov si,sp
	
	push ebx
	movsx bx,dl
	mov bp,sp

	mov ax,[ss:si+4]
	mov es,ax
	mov di,[ss:si+2]
	
	mov sp,0xfffe
	mov byte [ss:0xffff],0

	mov ax,cx
	mov cx,1
%%BeginDiv:
	inc cx
	mov dx,0
	div bx
	cmp dl,10
	jb %%Number
	add dl,'a'-'0'-10
%%Number:
	add dl,'0'
	mov dh,dl
	push dx
	inc sp
	cmp ax,0
	jne %%BeginDiv
	
	mov ax,ss
	mov ds,ax
	mov si,sp
	rep movsb

	mov sp,bp
	pop ebx
	ret
%endif
%endmacro

%macro func_itoa 0
%ifndef _boot_includes_string_inc_func_itoa_
%define _boot_includes_string_inc_func_itoa_
;_in_  ecx: number
;_in_  dl: radix
;_in_  stack 0-3: point to the string
itoa:
	mov si,sp

	push ebx
	movsx ebx,dl
	mov bp,sp

	mov ax,[ss:si+4]
	mov es,ax
	mov di,[ss:si+2]

	mov sp,0xfffe
	mov byte [ss:0xffff],0

	mov eax,ecx
	mov cx,1
%%BeginDiv:
	inc cx
	mov edx,0
	div ebx
	cmp dl,10
	jb %%Number
	add dl,'a'-'0'-10
%%Number:
	add dl,'0'
	mov dh,dl
	push dx
	inc sp
	cmp eax,0
	jne %%BeginDiv
	
	mov ax,ss
	mov ds,ax
	mov si,sp
	rep movsb

	mov sp,bp
	pop ebx
	ret
%endif
%endmacro

%macro func_findchar 0
%ifndef _boot_includes_string_inc_func_findchar_
%define _boot_includes_string_inc_func_findchar_
;_in_  ecx: point to the string
;_in_  dl: char to find
;_in_  dh: search from
;_out_ ax: index of the first char, or -1 if failed
findchar:
	push si
	push ds
	mov si,cx
	shr ecx,16
	mov ds,cx
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
	jmp %%return
%%failed:
	mov ax,-1
%%return:
	pop ds
	pop si
	ret
%endif
%endmacro

%macro func_strcmp 0
%ifndef _boot_includes_string_inc_func_strcmp_
%define _boot_includes_string_inc_func_strcmp_
;_in_  ecx: point to 1st string
;_in_  edx: point to 2nd string
;_out_ al: <0 for s1<s2, ==0 for s1==s2, >0 for s1>s2
strcmp:
	push di
	push si
	push ds
	push es
	mov di,cx
	mov si,dx
	shr ecx,16
	mov ds,cx
	shr edx,16
	mov es,dx

%%startcmp:
	mov al,[ds:di]
	mov ah,[es:si]

	cmp al,0
	jnz %%cmpahzero
	cmp ah,0
	jnz %%below
	
	mov al,0
	jmp %%return
%%cmpahzero:
	cmp ah,0
	jz %%above

	cmp al,ah
	jb %%below
	ja %%above

	inc di
	inc si
	jmp %%startcmp
	
%%below:
	mov al,-1
	jmp %%return
%%above:
	mov al,1
%%return:
	pop ds
	pop cs
	pop si
	pop di
	ret
%endif
%endmacro

;end of file
