#include "early_memory.h"

s64 matfind(void *addr)
{
	if(!MEMORY_MAT)return -1;
	u64 i;
	for(i=0;i<MEMORY_MAT->count;i++)
	{
		if(MEMORY_MAT->block[i].addr==addr)
			return i;
		if(MEMORY_MAT->block[i].addr>addr)
			return -1;
	}
	return -1;
}

s64 matinsert(u64 block,struct mat_block *data)
{
	if(!MEMORY_MAT)return -1;
	if(!data)return -1;
	if(block>=MEMORY_MAT->count||block==0)return -1;
	if(MEMORY_MAT->block[block-1].addr+MEMORY_MAT->block[block-1].len
	   >data->addr)return -1;
	if(MEMORY_MAT->block[block].addr<data->addr+data->len)return -1;
	if(data->addr>=MEMORY_MAT->memsize||data->len>MEMORY_MAT->memsize)return -1;
	if(data->len==0)return -1;

	u64 i;
	for(i=MEMORY_MAT->count-1;i>=block;i--)
		memcpy(&(MEMORY_MAT->block[i+1]),&(MEMORY_MAT->block[i]),sizeof(struct mat_block));

	memcpy(&(MEMORY_MAT->block[block]),data,sizeof(struct mat_block));
	MEMORY_MAT->count++;
	
	return block;
}

u8 matremove(u64 block)
{
	if(!MEMORY_MAT)return 0;
	if(block>=MEMORY_MAT->count-1||block==0)return 0;

	u64 i;
	for(i=block+1;i<MEMORY_MAT->count;i++)
		memcpy(&(MEMORY_MAT->block[i-1]),&(MEMORY_MAT->block[i]),sizeof(struct mat_block));

	MEMORY_MAT->count--;
	return 1;
}

s64 early_malloc_find(size_t size,size_t align,void **ret_start)
{
	if(size==0)return -1;
	if(align==0)return -1;

	u64 i;
	for(i=0;i<MEMORY_MAT->count-1;i++)
	{
		void *start=MEMORY_MAT->block[i].addr+MEMORY_MAT->block[i].len;
		if(((size_t)start%4096)&&MEMORY_MAT->block[i].proc!=0)
			start=((size_t)start+4095)/4096*4096;
		start=(((size_t)start-1)/align+1)*align;
		void *end=MEMORY_MAT->block[i+1].addr;
		if(((size_t)end%4096)&&
			MEMORY_MAT->block[i+1].type!=MAT_TYPE_END&&
			MEMORY_MAT->block[i+1].proc!=0)
			end=(size_t)end/4096*4096;
		if(start>=end)continue;
		if(end-start<size)continue;
		if(ret_start)*ret_start=start;
		return i+1;
	}
}

void* early_malloc_align(size_t size,size_t align)
{
	if(MEMORY_MAT->count>=MEMORY_MAT->maxcount)
	{
		struct mat *prevmat=MEMORY_MAT;
		u64 newcount=prevmat->count*3/2;
		size_t newsize=sizeof(struct mat)
		                  +sizeof(struct mat_block)*newcount;
		s64 newblock=early_malloc_find(newsize,8,&MEMORY_MAT);
		if(newblock==-1)
		{
			newcount=prevmat->count+2;
			newsize=sizeof(struct mat)+sizeof(struct mat_block)*newcount;
			newblock=early_malloc_find(newsize,8,&MEMORY_MAT);
			if(newblock==-1)return NULL;
		}

		memcpy(MEMORY_MAT,prevmat,sizeof(struct mat)
		       +sizeof(struct mat_block)*(prevmat->count));
		MEMORY_MAT->maxcount=newcount;
		
		struct mat_block blockdata;
		blockdata.addr=MEMORY_MAT;
		blockdata.len=newsize;
		blockdata.proc=0;
		blockdata.type=MAT_TYPE_USED;
		if(matinsert(newblock,&blockdata)==-1)return NULL;
		
		early_free(prevmat);
	}

	void *start;
	s64 block=early_malloc_find(size,align,&start);
	if(block==-1)return NULL;

	struct mat_block data;
	data.addr=start;
	data.len=size;
	data.proc=0;
	data.type=MAT_TYPE_USED;
	
	if(matinsert(block,&data)==-1)return NULL;
	return start;
}

void* early_malloc(size_t size)
{
	return early_malloc_align(size,1);
}

void* early_calloc_align(size_t num,size_t size,size_t align)
{
	void *addr=early_malloc_align(size*num,align);
	if(!addr)return NULL;
	memset(addr,0,size*num);
	return addr;
}

void* early_calloc(size_t num,size_t size)
{
	return early_calloc_align(num,size,1);
}

void early_free(void* ptr)
{
	s64 block=matfind(ptr);
	if(block==-1)return;
	if(MEMORY_MAT->block[block].type!=MAT_TYPE_USED)return;
	matremove(block);

	u64 maxcount=MEMORY_MAT->count*3/2;
	if(MEMORY_MAT->maxcount>maxcount)
	{
		s64 block=matfind(MEMORY_MAT);
		if(block==-1)return;
		MEMORY_MAT->block[block].len=sizeof(struct mat)
			+sizeof(struct mat_block)*maxcount;
		MEMORY_MAT->maxcount=maxcount;
	}
}
