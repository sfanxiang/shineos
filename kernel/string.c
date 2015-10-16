#include "string.h"

char* itoa(int n,char *str,int base)
{
	unsigned int value;
	if(n<0){
		*(str++)='-';
		value=-n;
	}else{
		value=n;
	}
	char *strbackup=str;
	do
	{
		int cur=value%base;
		if(cur<10)
			*str=cur+'0';
		else
			*str=cur-10+'a';
		str++;value/=base;
	}while(value);
	*str='\0';
	char *f;
	for(f=strbackup,str--;f<str;f++,str--)
	{
		char t=*f;
		*f=*str;
		*str=t;
	}
	return strbackup;
}

char* utoa(unsigned int value,char *str,int base)
{
	char *strbackup=str;
	do
	{
		unsigned int cur=value%base;
		if(cur<10)
			*str=cur+'0';
		else
			*str=cur-10+'a';
		str++;value/=base;
	}while(value);
	*str='\0';
	char *f;
	for(f=strbackup,str--;f<str;f++,str--)
	{
		char t=*f;
		*f=*str;
		*str=t;
	}
	return strbackup;
}

char* ltoa(long n,char *str,int base)
{
	unsigned long value;
	if(n<0){
		*(str++)='-';
		value=-n;
	}else{
		value=n;
	}
	char *strbackup=str;
	do
	{
		long cur=value%base;
		if(cur<10)
			*str=cur+'0';
		else
			*str=cur-10+'a';
		str++;value/=base;
	}while(value);
	*str='\0';
	char *f;
	for(f=strbackup,str--;f<str;f++,str--)
	{
		char t=*f;
		*f=*str;
		*str=t;
	}
	return strbackup;
}

char* ultoa(unsigned long value,char *str,int base)
{
	char *strbackup=str;
	do
	{
		unsigned long cur=value%base;
		if(cur<10)
			*str=cur+'0';
		else
			*str=cur-10+'a';
		str++;value/=base;
	}while(value);
	*str='\0';
	char *f;
	for(f=strbackup,str--;f<str;f++,str--)
	{
		char t=*f;
		*f=*str;
		*str=t;
	}
	return strbackup;
}

int strcmp(const char *str1,const char *str2)
{
	while(*str1||*str2)
	{
		if(*str1<*str2)
			return -1;
		if(*str1>*str2)
			return 1;
		str1++;str2++;
	}
	return 0;
}

size_t strlen(const char* str){
	size_t i=0;
	while(*str!=NULL){
		str++;
		i++;
	}
	return i;
}
