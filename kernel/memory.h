#ifndef MEMORY_H
#define MEMORY_H

#ifdef __x86_64__

#include "defines.h"
#include "vmm.h"

//#define NALLOC 4096

//extern int brk(int n,void *addr);
//extern void *sbrk(int n,size_t increment);

typedef struct memory_header_t{
	struct memory_header_t *ptr;
	size_t size;
}memory_header;

extern void *malloc(int n,size_t bytes);
extern void *calloc(int n,size_t nmemb, size_t size);
extern void free(int n,void *p);
extern int initmemory(int n);

#endif

#endif
