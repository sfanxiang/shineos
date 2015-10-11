#include "pmm.h"

size_t *pmm_init(){
	size_t memsize=EARLY_MEMORY_MAT->memsize;
	size_t pt_size=memsize/4096;
	size_t *page_stack=early_calloc_align(pt_size,1,4096);
	if(page_stack==NULL)return NULL;
	
	*page_stack=0;*(page_stack+1)=pt_size;
	size_t *p=page_stack+2;
	void *kernel_start=get_phy_kernel_start();
	void *kernel_end=get_phy_kernel_end();
	int i;
	for(i=0;i<SMAP_COUNT;i++){
		struct smap_entry *se=SMAP_TABLE+i;
		if(se->type!=SMAP_TYPE_USABLE)continue;
		size_t addr=se->base/4096*4096;
		size_t end=(se->base+se->len+4095)/4096*4096;
		if(addr<0xfffff+1)addr=0xfffff+1;
		while(addr<end){
			if((addr<(size_t)page_stack||addr>(size_t)(page_stack+pt_size))
			   &&(addr<(size_t)kernel_start||addr>(size_t)kernel_end)){
				*(p++)=addr;
				(*page_stack)++;
			}
			addr+=4096;
		}
	}
	*PMM_STACK=page_stack;
	return page_stack;
}

int pmm_lock;

void *pmm_alloc(){
	spinlock_acquire(&pmm_lock);
	size_t *page_stack=*PMM_STACK;
	if(*page_stack!=0){
		void *ret=page_stack[((*page_stack)--)+1];
		spinlock_release(&pmm_lock);
		return ret;
	}else{
		spinlock_release(&pmm_lock);
		return NULL;
	}
}

void pmm_free(void *p){
	spinlock_acquire(&pmm_lock);
	if(p==NULL)return;
	
	size_t pt=p;
	pt=pt/4096*4096;
	size_t *page_stack=*PMM_STACK;
	(*page_stack)++;
	page_stack[(*page_stack)+1]=pt;
	
	spinlock_release(&pmm_lock);
}
