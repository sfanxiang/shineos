#ifndef STRING_H
#define STRING_H

#ifdef __x86_64__

#include "defines.h"

extern void* memcpy(void *destination,const void *source,size_t num);
extern void* memset(void *ptr,u8 value,size_t num);
extern char* itoa(int value,char *str,int base);
extern char* utoa(unsigned int value,char *str,int base);
extern char* ltoa(long value,char *str,int base);
extern int strcmp(const char *str1,const char *str2);
extern size_t strlen(const char* str);

#endif

#endif
