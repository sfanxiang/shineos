#ifndef STRING_H
#define STRING_H

#include "defines.h"

#ifdef __AS386_64__

extern void* memcpy(void *destination,const void *source,size_t num);

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

#endif

#endif
