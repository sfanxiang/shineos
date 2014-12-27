#ifndef STRING_H
#define STRING_H

#ifdef __x86_64__

#include "defines.h"

extern void* memcpy(void *destination,const void *source,size_t num);
extern void* memset(void *ptr,u8 value,size_t num);

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

#endif

#endif
