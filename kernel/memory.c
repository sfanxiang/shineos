#include "memory.h"

memory_header base[255];
memory_header* freep[255];

int alloc_block(int n,memory_header* h){
	size_t p=h+1;
	p=GET_PAGE(p+4095);
	size_t op=p;
	for(;p<((size_t)h)+h->size;p+=4096){
		if(vm_getphypage(n,p)!=NULL){
			char str[30];
			puts("memory.c: warning: page already has physical address: ");
			puts(itoa(p,str,16));
			putchar('\n');
			op=p+4096;
			continue;
		}
		size_t newpage=pmm_alloc();
		if(newpage==NULL){
			for(;op<p;op+=4096){
				pmm_free(vm_getphypage(n,op));
				vm_unmap(n,op);
			}
			return 0;
		}
		if(vm_map(n,p,newpage)==NULL){
			for(;op<=p;op+=4096){
				pmm_free(vm_getphypage(n,op));
				vm_unmap(n,op);
			}
			return 0;
		}
	}
	return 1;
}

void *malloc(int n,size_t bytes){
	if(bytes==0)return NULL;

	size_t alloc=bytes+sizeof(memory_header);
	alloc=((alloc-1)/sizeof(memory_header)+1)*sizeof(memory_header);

	memory_header *p,*prevp=freep[n];
	for(p=prevp->ptr;;prevp=p,p=p->ptr){
		if(p->size>alloc){
			size_t orgsize=p->size;
			p->size=alloc+sizeof(memory_header);
			if(alloc_block(n,p)){
				p->size-=sizeof(memory_header);
				prevp->ptr=(memory_header*)(((void*)p)+alloc);
				((memory_header*)(((void*)p)+alloc))->size=orgsize-alloc;
				((memory_header*)(((void*)p)+alloc))->ptr=p->ptr;
				freep[n]=((memory_header*)(((void*)p)+alloc));
				return (void*)(p+1);
			}else{
				p->size=orgsize;
			}
		}else if(p->size==alloc){
			if(((void*)p)+p->size<=VM_KERNEL*2){
				if(alloc_block(n,p)){
					prevp->ptr=p->ptr;
					freep[n]=prevp;
					return (void*)(p+1);
				}
			}
		}
		if(p==freep[n])return NULL;
	}
}

void *calloc(int n,size_t nmemb, size_t size){
	size_t s=nmemb*size;
	if(s==0)return NULL;
	void *ptr=malloc(n,s);
	if(ptr==NULL)
		return NULL;
	else{
		memset(ptr,0,s);
		return ptr;
	}
}

void free_block(int n,memory_header *h){
	size_t p=h+1;
	p=(p+4095)/4096*4096;
	for(;p<((size_t)h)+h->size-4095;p+=4096){
		pmm_free(vm_getphypage(n,p));
		vm_unmap(n,p);
	}
}

void free(int n,void *pt){
	if(pt==NULL)return;
	
	memory_header *h=pt;
	h--;

	memory_header *p;
	for(p=freep[n];;p=p->ptr){
		if(h>p&&h<p->ptr){
			if(((void*)h)+h->size==p->ptr){
				size_t size=h->size+p->ptr->size;
				memory_header *ptr=p->ptr->ptr;
				if(p->ptr->size>=4096)h->size+=4096;
				else h->size+=p->ptr->size;
				free_block(n,h);
				h->size=size;
				h->ptr=ptr;
			}else{
				h->ptr=p->ptr;
				free_block(n,h);
			}
			if(((void*)p)+p->size==h){
				size_t size=p->size+h->size;
				memory_header *ptr=h->ptr;
				p->size+=sizeof(memory_header);
				free_block(n,p);
				p->size=size;
				p->ptr=ptr;
			}else{
				p->ptr=h;
			}
			freep[n]=p;
			break;
		}
	}
}

int initmemory(int n){
	freep[n]=get_phy_kernel_end()-get_phy_kernel_start()+VM_KERNEL;
	void *p=pmm_alloc();
	if(p==NULL)return 0;
	if(vm_map(n,freep[n],p)==0)return 0;
	freep[n]->size=0;
	freep[n]->ptr=freep[n]+1;
	(freep[n]+1)->size=VM_KERNEL;
	(freep[n]+1)->ptr=freep[n];
	return 1;
}
