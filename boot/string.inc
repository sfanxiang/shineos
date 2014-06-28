;boot/string.inc

;btoa, stoa, itoa are used for unsigned integer numbers
%macro func_btoa 0
%ifndef _boot_includes_string_inc_func_btoa_
%define _boot_includes_string_inc_func_btoa_
;_in_  ecx: point to the string
;_in_  dl: number
;_in_  stack 0-1: radix
;_out_ eax: point to the string
btoa:
	push ecx
	push bp
	push di
	push si

	mov bp,sp

	mov di,cx
	shr ecx,16
	mov es,cx

	mov sp,0xfffe
	mov byte [ss:0xffff],0

	mov al,dl
	mov dl,[ss:bp+12]
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
	cld
	rep movsb

	mov sp,bp

	pop si
	pop di
	pop bp
	pop eax
	ret
%endif
%endmacro

%macro func_stoa 0
%ifndef _boot_includes_string_inc_func_stoa_
%define _boot_includes_string_inc_func_stoa_
;_in_  ecx: point to the string
;_in_  dx: number
;_in_  stack 0-1: radix
;_out_ eax: point to the string
stoa:
	push bx
	push ecx
	push bp
	push di
	push si

	mov bp,sp

	mov di,cx
	shr ecx,16
	mov es,cx
	
	mov sp,0xfffe
	mov byte [ss:0xffff],0

	mov ax,dx
	mov bx,[ss:bp+14]
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
	cld
	rep movsb

	mov sp,bp

	pop si
	pop di
	pop bp
	pop eax
	pop bx
	ret
%endif
%endmacro

%macro func_itoa 0
%ifndef _boot_includes_string_inc_func_itoa_
%define _boot_includes_string_inc_func_itoa_
;_in_  ecx: point to the string
;_in_  edx: number
;_in_  stack 0-1: radix
;_out_ eax: point to the string
itoa:
	push ebx
	push ecx
	push bp
	push di
	push si

	mov bp,sp

	mov di,cx
	shr ecx,16
	mov es,cx

	mov sp,0xfffe
	mov byte [ss:0xffff],0

	mov eax,edx
	mov bx,[ss:bp+16]
	and ebx,0xffff
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
	cld
	rep movsb

	mov sp,bp

	pop si
	pop di
	pop bp
	pop eax
	pop ebx
	ret
%endif
%endmacro

;todo: atob, atos, atoi
%macro func_atob 0
%ifndef _boot_includes_string_inc_func_atob_
%define _boot_includes_string_inc_func_atob_
;_in_  ecx: point to the string
;_in_  dl: radix
;_out_ al: number
atob:
	push bx
	push si
	mov si,cx
	shr ecx,16
	mov ds,cx
	xor cx,cx
	xor bl,bl
	mov dh,1
	
@@
	cmp byte [si],0
	jz @f
	inc si
	inc cx
	jmp @b

@@
	cmp cx,0
	jz @f
	dec cx
	dec si
	mov al,[si]
	cmp al,'9'
	jna %%number
	cmp al,'Z'
	jna %%cap
	sub al,'a'-'A'
%%cap:
	sub al,'A'-'9'-1
%%number:
	sub al,'0'
	
	mul dh
	add bl,al
	mov al,dh
	mul dl
	mov dh,al
	jmp @b
	
@@
	mov al,bl
	pop si
	pop bx
	ret
%endif
%endmacro

%macro func_findchar 0
%ifndef _boot_includes_string_inc_func_findchar_
%define _boot_includes_string_inc_func_findchar_
;_in_  ecx: point to the string
;_in_  dl: char to find
;_out_ ax: index of the first char, or -1 if failed
findchar:
	push si
	push ds
	mov si,cx
	shr ecx,16
	mov ds,cx
	mov cx,si
	cld
%%search:
	lodsb
	cmp al,0
	jz %%failed
	cmp al,dl
	jnz %%search
	sub si,cx
	dec si
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
	pop es
	pop ds
	pop si
	pop di
	ret
%endif
%endmacro

%macro func_strlen 0
%ifndef _boot_includes_string_inc_func_strlen_
%define _boot_includes_string_inc_func_strlen_
;_in_  ecx: point to the string
;_out_ ax: length
strlen:
	push si
	mov si,cx
	shr ecx,16
	mov ds,cx
	xor ax,ax

@@
	cmp byte [si],0
	jz @f
	inc ax
	inc si
	jmp @b

@@
	pop si
	ret
%endif
%endmacro

%macro func_strcpy 0
%ifndef _boot_includes_string_inc_func_strcpy_
%define _boot_includes_string_inc_func_strcpy_
;_in_  ecx: point to source
;_in_  edx: point to destination
strcpy:
	push si
	push di
	
	mov si,cx
	shr ecx,16
	mov ds,cx
	mov di,dx
	shr edx,16
	mov es,dx
	
@@
	cmp byte [si],0
	jz @f
	mov al,[si]
	mov [es:di],al
	inc si
	inc di
	jmp @b
	
@@
	pop di
	pop si
	ret
%endif
%endmacro

;end of file

