#ifndef STRING_H
#define STRING_H

#ifdef __x86_64__

#include "defines.h"

extern void* memcpy(void *destination,const void *source,size_t num);
extern void* memset(void *ptr,u8 value,size_t num);
extern char* itoa(u64 value,char *str,u8 base);
extern s8 strcmp(const char *str1,const char *str2);

#endif

#endif
