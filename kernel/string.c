#include "string.h"

char* itoa(u64 value,char *str,u8 base)
{
	char *strbackup=str;
	do
	{
		u8 cur=value%base;
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

s8 strcmp(const char *str1,const char *str2)
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
