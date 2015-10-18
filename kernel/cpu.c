#include "cpu.h"

void setstack(void* oldstack,void* newstack,size_t oldsize){
	memcpy(newstack-oldsize,oldstack-oldsize,oldsize);
	size_t temp;
	__asm__ __volatile__("mov %%rsp,%2\n"
	                     "sub %0,%2\n"
	                     "add %1,%2\n"
	                     "mov %2,%%rsp\n"
	                     ::"r"(oldstack),"r"(newstack),"r"(temp));
}
