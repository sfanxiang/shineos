#include "stdio.h"

//todo: define stdout stuff

int putchar_lock;

void putchar_unlocked(char chr)
{
	u16 cursor=getcursorpos();
	if(chr=='\n')
		cursor=cursor-cursor%SCREEN_XMAX+SCREEN_XMAX;
	else
	{
		writechar(chr,cursor);
		cursor++;
	}
	if(cursor>=SCREEN_XMAX*SCREEN_YMAX)
	{
		scrollscreen(1);
		cursor-=SCREEN_XMAX;
	}
	setcursorpos(cursor);
}

void putchar(char chr)
{
	int ifstate=((getcpuflags()&CPU_FLAGS_IF)!=0);
	if(ifstate)disable_int();
	spinlock_acquire(&putchar_lock);
	putchar_unlocked(chr);
	spinlock_release(&putchar_lock);
	if(ifstate)enable_int();
}

void puts(char* str)
{
	//adding a lock may cause deadlock
	while(*str)
	{
		putchar(*str);
		str++;
	}
}

int vsnprintf(char *str, size_t size, const char *format, va_list ap){
	int curlen=0;
	while(*format!=NULL){
		if(*format!='%'){
			if(size>1){
				*(str++)=*format;
				size--;
			}
			curlen++;
		}else{
			format++;
			if(*format=='%'){
				if(size>1){
					*(str++)='%';
					size--;
				}
				curlen++;
			}else if(*format=='d'){
				char str2[30];
				itoa(va_arg(ap,int),str2,10);
				size_t sz=strlen(str2);
				curlen+=sz;
				if(sz>=size)sz=size-1;
				memcpy(str,str2,sizeof(char)*sz);
				str+=sz;
				size-=sz;
			}else if(*format=='x'){
				char str2[30];
				utoa(va_arg(ap,unsigned int),str2,16);
				size_t sz=strlen(str2);
				curlen+=sz;
				if(sz>=size)sz=size-1;
				memcpy(str,str2,sizeof(char)*sz);
				str+=sz;
				size-=sz;
			}else if(*format=='u'){
				char str2[30];
				utoa(va_arg(ap,unsigned int),str2,10);
				size_t sz=strlen(str2);
				curlen+=sz;
				if(sz>=size)sz=size-1;
				memcpy(str,str2,sizeof(char)*sz);
				str+=sz;
				size-=sz;
			}else if(*format=='l'){
				format++;
				if(*format==NULL)break;
				if(*format=='d'){
					char str2[30];
					ltoa(va_arg(ap,long),str2,10);
					size_t sz=strlen(str2);
					curlen+=sz;
					if(sz>=size)sz=size-1;
					memcpy(str,str2,sizeof(char)*sz);
					str+=sz;
					size-=sz;
				}else if(*format=='x'){
					char str2[30];
					ultoa(va_arg(ap,unsigned long),str2,16);
					size_t sz=strlen(str2);
					curlen+=sz;
					if(sz>=size)sz=size-1;
					memcpy(str,str2,sizeof(char)*sz);
					str+=sz;
					size-=sz;
				}else if(*format=='u'){
					char str2[30];
					ultoa(va_arg(ap,unsigned long),str2,10);
					size_t sz=strlen(str2);
					curlen+=sz;
					if(sz>=size)sz=size-1;
					memcpy(str,str2,sizeof(char)*sz);
					str+=sz;
					size-=sz;
				}
			}else if(*format=='s'){
				char *str2=va_arg(ap,char*);
				size_t sz=strlen(str2);
				curlen+=sz;
				if(sz>=size)sz=size-1;
				memcpy(str,str2,sizeof(char)*sz);
				str+=sz;
				size-=sz;
			}else if(*format==NULL){
				break;
			}
		}
		format++;
	}
	*str='\0';
	return curlen;
}

int snprintf(char *str, size_t size, const char *format, ...){
	va_list ap;
	va_start(ap,format);
	int ret=vsnprintf(str,size,format,ap);
	va_end(ap);
	return ret;
}

int vprintf(const char *format, va_list ap){
	char str[20];
	va_list ap2;
	va_copy(ap2,ap);
	size_t size=vsnprintf(str,20,format,ap);
	if(size>19){
		char *str2=malloc((size+1)*sizeof(char));
		if(str2!=NULL){
			size=vsnprintf(str2,size+1,format,ap2);
			puts(str2);
			free(str2);
		}else{
			va_end(ap2);
			return -1;
		}
	}else{
		puts(str);
	}
	va_end(ap2);
	return size;
}

int printf(const char *format, ...){
	va_list ap;
	va_start(ap,format);
	int ret=vprintf(format,ap);
	va_end(ap);
	return ret;
}
