#ifndef MEMORY_H
#define MEMORY_H

#include "defines.h"

#ifdef __AS386_16__

typedef u32 pfar;

u8 getfarbyte(pfar pt)
{
	asm("\
		push bp\n\
		mov bp,sp\n\
		push ds\n\
		push si\n\
		sseg\n\
		mov si,[bp+6]\n\
		mov ds,si\n\
		sseg\n\
		mov si,[bp+4]\n\
		dseg\n\
		mov al,[si]\n\
		pop si\n\
		pop ds\n\
		pop bp\
	");
}

u32 getfardword(pfar pt)
{
	asm("\
		push bp\n\
		mov bp,sp\n\
		push ds\n\
		push si\n\
		sseg\n\
		mov si,[bp+6]\n\
		mov ds,si\n\
		sseg\n\
		mov si,[bp+4]\n\
		dseg\n\
		mov ax,[si]\n\
		dseg\n\
		mov dx,[si+2]\n\
		pop si\n\
		pop ds\n\
		pop bp\
	");
}

pfar getpfar(void* pt)
{
	asm("\
		push bp\n\
		mov bp,sp\n\
		mov dx,cs\n\
		sseg\n\
		mov ax,[bp+4]\n\
		pop bp\
	");
}

pfar setfarbyte(pfar ptr,u8 val)
{
	asm("\
		push bp\n\
		mov bp,sp\n\
		push ds\n\
		push si\n\
		sseg\n\
		mov ax,[bp+6]\n\
		mov ds,ax\n\
		sseg\n\
		mov si,[bp+4]\n\
		sseg\n\
		mov cl,[bp+8]\n\
		dseg\n\
		mov [si],cl\n\
		mov dx,ds\n\
		mov ax,si\n\
		pop si\n\
		pop ds\n\
		pop bp\
	");
}

#endif

#ifdef __x86_64__

#pragma pack(push,1)

struct smap_entry{
	u64 base;
	u64 len;
	u32 type;
	u32 acpi;
};

#pragma pack(pop)

#define SMAP_TABLE ((struct smap_entry*)0x500)
#define SMAP_COUNT (*((s16*)0x1000))

#define SMAP_TYPE_USABLE 1
#define SMAP_TYPE_RESERVED 2
#define SMAP_TYPE_ACPI_REC 3
#define SMAP_TYPE_ACPI_NVS 4
#define SMAP_TYPE_BAD 5

#pragma pack(push,1)

struct mat_block{
	void *addr;
	size_t len;
	u64 proc;
	u64 type;
};

struct mat{
	u64 count;
	u64 maxcount;
	size_t memsize;
	struct mat_block block[];
};

#pragma pack(pop)

#define MAT_TYPE_USED 1
#define MAT_TYPE_OTHER 2
#define MAT_TYPE_END 4

#define MEMORY_MAT (*((volatile struct mat**)0x7c0a))

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

void free(void* ptr);

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

s64 malloc_find(size_t size,size_t align)
{
	if(size==0)return -1;
	if(align==0)return -1;

	u64 i;
	for(i=0;i<MEMORY_MAT->count-1;i++)
	{
		void *start=MEMORY_MAT->block[i].addr+MEMORY_MAT->block[i].len;
		start=(((size_t)start-1)/align+1)*align;
		void *end=MEMORY_MAT->block[i+1].addr;
		if(start>=end)continue;
		if(end-start<size)continue;
		return i+1;
	}
}

void* malloc_align(size_t size,size_t align)
{
	if(MEMORY_MAT->count>=MEMORY_MAT->maxcount)
	{
		struct mat *prevmat=MEMORY_MAT;
		u64 newcount=prevmat->count*3/2;
		size_t newsize=sizeof(struct mat)
		                  +sizeof(struct mat_block)*newcount;
		s64 newblock=malloc_find(newsize,8);
		if(newblock==-1)
		{
			newcount=prevmat->count+2;
			newsize=sizeof(struct mat)+sizeof(struct mat_block)*newcount;
			newblock=malloc_find(newsize,8);
			if(newblock==-1)return NULL;
		}

		MEMORY_MAT=(((size_t)prevmat->block[newblock-1].addr
		                +prevmat->block[newblock-1].len-1)/8+1)*8;
		memcpy(MEMORY_MAT,prevmat,sizeof(struct mat)
		       +sizeof(struct mat_block)*(prevmat->count));
		MEMORY_MAT->maxcount=newcount;
		
		struct mat_block blockdata;
		blockdata.addr=MEMORY_MAT;
		blockdata.len=newsize;
		blockdata.proc=0;
		blockdata.type=MAT_TYPE_USED;
		if(matinsert(newblock,&blockdata)==-1)return NULL;
		
		free(prevmat);
	}

	s64 block=malloc_find(size,align);
	if(block==-1)return NULL;
	
	void *start=MEMORY_MAT->block[block-1].addr+MEMORY_MAT->block[block-1].len;
	start=(((size_t)start-1)/align+1)*align;

	struct mat_block data;
	data.addr=start;
	data.len=size;
	data.proc=0;
	data.type=MAT_TYPE_USED;
	
	if(matinsert(block,&data)==-1)return NULL;
	return start;
}

void* malloc(size_t size)
{
	return malloc_align(size,1);
}

void* calloc_align(size_t num,size_t size,size_t align)
{
	void *addr=malloc_align(size*num,align);
	if(!addr)return NULL;
	memset(addr,0,size);
	return addr;
}

void* calloc(size_t num,size_t size)
{
	return calloc_align(num,size,1);
}

void free(void* ptr)
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

u8 matbuild()
{
	u64 i,j;
	
	for(i=0;i<SMAP_COUNT-1;i++)
	{
		for(j=i+1;j<SMAP_COUNT;j++)
		{
			if((SMAP_TABLE+i)->base>(SMAP_TABLE+j)->base
			   ||((SMAP_TABLE+i)->base==(SMAP_TABLE+j)->base
			      &&(SMAP_TABLE+i)->len>(SMAP_TABLE+j)->len))
			{
				struct smap_entry smapent;
				memcpy(&smapent,SMAP_TABLE+i,sizeof(smapent));
				memcpy(SMAP_TABLE+i,SMAP_TABLE+j,sizeof(smapent));
				memcpy(SMAP_TABLE+j,&smapent,sizeof(smapent));
			}
		}
	}

	for(i=0;i<SMAP_COUNT-1;i++)
		if((SMAP_TABLE+i)->base+(SMAP_TABLE+i)->len>(SMAP_TABLE+i+1)->base)
			(SMAP_TABLE+i)->len=(SMAP_TABLE+i+1)->base-(SMAP_TABLE+i)->base;

	MEMORY_MAT=0x100000;

	MEMORY_MAT->memsize=(SMAP_TABLE+SMAP_COUNT-1)->base+(SMAP_TABLE+SMAP_COUNT-1)->len;
	MEMORY_MAT->count=2;
	MEMORY_MAT->maxcount=64;
	MEMORY_MAT->block[0].addr=0;
	MEMORY_MAT->block[0].len=0xec00;
	MEMORY_MAT->block[0].proc=0;
	MEMORY_MAT->block[0].type=MAT_TYPE_OTHER;
	MEMORY_MAT->block[1].addr=MEMORY_MAT->memsize;
	MEMORY_MAT->block[1].len=0;
	MEMORY_MAT->block[1].proc=0;
	MEMORY_MAT->block[1].type=MAT_TYPE_END;
	
	for(i=0;i<SMAP_COUNT-1;i++)
	{
		if((SMAP_TABLE+i)->len!=0&&(SMAP_TABLE+i)->type!=SMAP_TYPE_USABLE)
		{
			struct mat_block data;
			data.addr=(SMAP_TABLE+i)->base;
			data.len=(SMAP_TABLE+i)->len;
			data.type=MAT_TYPE_OTHER;
			if(matinsert(MEMORY_MAT->count-1,&data)==-1)return 0;
		}
		if((SMAP_TABLE+i)->base+(SMAP_TABLE+i)->len<(SMAP_TABLE+i+1)->base)
		{
			struct mat_block data;
			data.addr=(SMAP_TABLE+i)->base+(SMAP_TABLE+i)->len;
			data.len=(SMAP_TABLE+i+1)->base-(u64)data.addr;
			data.type=MAT_TYPE_OTHER;
			if(matinsert(MEMORY_MAT->count-1,&data)==-1)return 0;
		}
	}

	if((SMAP_TABLE+SMAP_COUNT-1)->len!=0&&(SMAP_TABLE+SMAP_COUNT-1)->type!=SMAP_TYPE_USABLE)
	{
		struct mat_block data;
		data.addr=(SMAP_TABLE+SMAP_COUNT-1)->base;
		data.len=(SMAP_TABLE+SMAP_COUNT-1)->len;
		data.type=MAT_TYPE_OTHER;
		if(matinsert(MEMORY_MAT->count-1,&data)==-1)return 0;
	}
	
	for(i=0;i<MEMORY_MAT->count-1;i++)
	{
		while(MEMORY_MAT->block[i].addr+MEMORY_MAT->block[i].len
		      >=MEMORY_MAT->block[i+1].addr
		      &&MEMORY_MAT->block[i].type==MEMORY_MAT->block[i+1].type)
		{
			MEMORY_MAT->block[i].len
				=MEMORY_MAT->block[i+1].addr
				+MEMORY_MAT->block[i+1].len
				-MEMORY_MAT->block[i].addr;
			if(!matremove(i+1))return 0;
		}
	}

	struct mat_block insdata;
	
	for(i=0;i<MEMORY_MAT->count;i++)
		if(MEMORY_MAT->block[i].addr>0x100000)break;
	if(i>=MEMORY_MAT->count)return 0;
	insdata.addr=0x100000;
	insdata.len=sizeof(struct mat)+sizeof(struct mat_block)*(MEMORY_MAT->maxcount);
	insdata.proc=0;
	insdata.type=MAT_TYPE_USED;
	if(matinsert(i,&insdata)==-1)return 0;

	for(i=0;i<MEMORY_MAT->count;i++)
		if(MEMORY_MAT->block[i].addr>0x80000)break;
	if(i>=MEMORY_MAT->count)return 0;
	insdata.addr=0x80000;
	insdata.len=0x1000*2+512*16*8;
	insdata.proc=0;
	insdata.type=MAT_TYPE_USED;
	if(matinsert(i,&insdata)==-1)return 0;

	return 1;
}

#endif

#endif
