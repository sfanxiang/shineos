#ifndef STRING_H
#define STRING_H

#include "defines.h"

#ifdef __AS386_64__

extern void* memcpy(void *destination,const void *source,size_t num);

#endif

#endif
