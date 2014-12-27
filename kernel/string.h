#ifndef STRING_H
#define STRING_H

#ifdef __x86_64__

#include "defines.h"

extern void* memcpy(void *destination,const void *source,size_t num);
extern void* memset(void *ptr,u8 value,size_t num);

#endif

#endif
