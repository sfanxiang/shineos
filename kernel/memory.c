#include "memory.h"

struct mat *__memory_mat;

s64 matfind(void *addr)
{
	if(!__memory_mat)return -1;
	u64 i;
	for(i=0;i<__memory_mat->count;i++)
	{
		if(__memory_mat->block[i].addr==addr)
			return i;
		if(__memory_mat->block[i].addr>addr)
			return -1;
	}
	return -1;
}

s64 matinsert(u64 block,struct mat_block *data)
{
	if(!__memory_mat)return -1;
	if(!data)return -1;
	if(block>=__memory_mat->count||block==0)return -1;
	if(__memory_mat->block[block-1].addr+__memory_mat->block[block-1].len
	   >data->addr)return -1;
	if(__memory_mat->block[block].addr<data->addr+data->len)return -1;
	if(data->addr>=__memory_mat->memsize||data->len>__memory_mat->memsize)return -1;
	if(data->len==0)return -1;

	u64 i;
	for(i=__memory_mat->count-1;i>=block;i--)
		memcpy(&(__memory_mat->block[i+1]),&(__memory_mat->block[i]),sizeof(struct mat_block));

	memcpy(&(__memory_mat->block[block]),data,sizeof(struct mat_block));
	__memory_mat->count++;
	
	return block;
}

u8 matremove(u64 block)
{
	if(!__memory_mat)return 0;
	if(block>=__memory_mat->count-1||block==0)return 0;

	u64 i;
	for(i=block+1;i<__memory_mat->count;i++)
		memcpy(&(__memory_mat->block[i-1]),&(__memory_mat->block[i]),sizeof(struct mat_block));

	__memory_mat->count--;
	return 1;
}

s64 kmalloc_find(size_t size,size_t align,void **ret_start)
{
	if(size==0)return -1;
	if(align==0)return -1;

	u64 i;
	for(i=0;i<__memory_mat->count-1;i++)
	{
		void *start=__memory_mat->block[i].addr+__memory_mat->block[i].len;
		if(((size_t)start%4096)&&__memory_mat->block[i].task!=0)
			start=((size_t)start+4095)/4096*4096;
		start=(((size_t)start-1)/align+1)*align;
		void *end=__memory_mat->block[i+1].addr;
		if(((size_t)end%4096)&&
			__memory_mat->block[i+1].type!=MAT_TYPE_END&&
			__memory_mat->block[i+1].task!=0)
			end=(size_t)end/4096*4096;
		if(start>=end)continue;
		if(end-start<size)continue;
		if(ret_start)*ret_start=start;
		return i+1;
	}
}

void* kmalloc_align(size_t size,size_t align)
{
	if(__memory_mat->count>=__memory_mat->maxcount)
	{
		struct mat *prevmat=__memory_mat;
		u64 newcount=prevmat->count*3/2;
		size_t newsize=sizeof(struct mat)
		                  +sizeof(struct mat_block)*newcount;
		s64 newblock=kmalloc_find(newsize,8,&__memory_mat);
		if(newblock==-1)
		{
			newcount=prevmat->count+2;
			newsize=sizeof(struct mat)+sizeof(struct mat_block)*newcount;
			newblock=kmalloc_find(newsize,8,&__memory_mat);
			if(newblock==-1)return NULL;
		}

		memcpy(__memory_mat,prevmat,sizeof(struct mat)
		       +sizeof(struct mat_block)*(prevmat->count));
		__memory_mat->maxcount=newcount;
		
		struct mat_block blockdata;
		blockdata.addr=__memory_mat;
		blockdata.len=newsize;
		blockdata.task=0;
		blockdata.type=MAT_TYPE_USED;
		if(matinsert(newblock,&blockdata)==-1)return NULL;
		
		kfree(prevmat);
	}

	void *start;
	s64 block=kmalloc_find(size,align,&start);
	if(block==-1)return NULL;

	struct mat_block data;
	data.addr=start;
	data.len=size;
	data.task=0;
	data.type=MAT_TYPE_USED;
	
	if(matinsert(block,&data)==-1)return NULL;
	return start;
}

void* kmalloc(size_t size)
{
	return kmalloc_align(size,1);
}

void* kcalloc_align(size_t num,size_t size,size_t align)
{
	void *addr=kmalloc_align(size*num,align);
	if(!addr)return NULL;
	memset(addr,0,sizeof(addr));
	return addr;
}

void* kcalloc(size_t num,size_t size)
{
	return kcalloc_align(num,size,1);
}

void kfree(void* ptr)
{
	s64 block=matfind(ptr);
	if(block==-1)return;
	if(__memory_mat->block[block].type!=MAT_TYPE_USED)return;
	matremove(block);

	u64 maxcount=__memory_mat->count*3/2;
	if(__memory_mat->maxcount>maxcount)
	{
		s64 block=matfind(__memory_mat);
		if(block==-1)return;
		__memory_mat->block[block].len=sizeof(struct mat)
			+sizeof(struct mat_block)*maxcount;
		__memory_mat->maxcount=maxcount;
	}
}

struct mat* getmat()
{
	return __memory_mat;
}

void setmat(struct mat* mat)
{
	__memory_mat=mat;
}
