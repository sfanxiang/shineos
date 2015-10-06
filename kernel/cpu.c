#include "cpu.h"

void setstack(void* oldstack,void* newstack,size_t oldsize){
	memcpy(newstack-oldsize,oldstack-oldsize,oldsize);
	__asm__ __volatile__("sub %0,%%rsp\n"
	                     "add %1,%%rsp\n"
	                     ::"r"(oldstack),"r"(newstack));
}