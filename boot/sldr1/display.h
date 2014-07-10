
void _putchar(char chr)
{
	/*int a=1,b=2,c=3,d=4;
	a=5;*/
	asm(
		"pusha\n\
		mov bx,4[bp]\n\
		mov al,[bx]\n\
		mov bx,#7\n\
		mov ah,#0xe\n\
		int #0x10\n\
		popa"
	);
	/*if(a==5&&b==2&&c==3&&d==4)
		return;
	else
	{
l:		goto l;
	}*/
}

void _puts(char* str)
{
	while(*str)
	{
		_putchar(*str);
		str++;
	}
}

