#ifndef MEMORY_H
#define MEMORY_H

#ifdef __x86_64__

#include "defines.h"
#include "vmm.h"

typedef struct memory_header_t{
	struct memory_header_t *ptr;
	size_t size;
}memory_header;

extern void *malloc(size_t bytes);
extern void *calloc(size_t nmemb, size_t size);
extern void free(void *p);
extern int initmemory(int n);

#endif

#endif
