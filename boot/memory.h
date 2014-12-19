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
	u64 task;
	u64 type;
};

struct mat{
	u64 count;
	u64 maxcount;
	size_t memsize;
	struct mat_block block[];
};

#pragma pack(pop)

#define MAT_TYPE_FREE 1
#define MAT_TYPE_USED 2
#define MAT_TYPE_OTHER 4
#define MAT_TYPE_END 8

s64 matfindblock(struct mat *mat,u64 startblock,void *addr)
{
	if(startblock>=mat->count)return -1;
	if((size_t)addr>=mat->memsize)return -1;
	if(mat->block[startblock].addr>addr)return -1;
	
	while(startblock<mat->count)
	{
		if(mat->block[startblock].addr>addr)return startblock-1;
		startblock++;
	}
	return -1;
}

s64 matsplitblock(struct mat *mat,u64 block,void *addr)
{
	if(block>=mat->count)return -1;
	if((size_t)addr>=mat->memsize)return -1;
	if(mat->block[block].addr>addr)return -1;
	if(mat->block[block+1].addr<=addr)return -1;
	
	if(addr==mat->block[block].addr)return block;

	//todo: check

	u64 i;
	for(i=mat->count-1;i>block;i--)
		memcpy(&(mat->block[i+1]),&(mat->block[i]),sizeof(mat->block[i]));

	mat->block[block+1].addr=addr;
	mat->block[block+1].task=mat->block[block].task;
	mat->block[block+1].type=mat->block[block].type;
	mat->count++;

	return block+1;
}

s64 matmergeblock(struct mat *mat,u64 startblock,u64 endblock,u64 task,u64 type)
{
	if(startblock>=mat->count||endblock>=mat->count)return -1;
	if(startblock>endblock)return -1;

	u64 i;
	for(i=startblock+1;i<mat->count;i++)
		memcpy(&(mat->block[i]),&(mat->block[i+endblock-startblock]),sizeof(mat->block[i]));

	mat->block[startblock].task=task;
	mat->block[startblock].type=type;
	mat->count--;

	return startblock;
}

s64 matpickblock(struct mat *mat,u64 block,void *start,void *end,u64 task,u64 type)  //end doesn't belong to the picked block
{
	if(block>=mat->count)return -1;
	if(start<mat->block[block].addr||end>mat->block[block+1].addr)return -1;
	if(start>=end)return -1;

	if(mat->block[block].task==task&&mat->block[block].type==type)
		return block;
	
	s64 ret;
	if(start==mat->block[block].addr)
	{
		if(block!=0
		   &&mat->block[block-1].task==mat->block[block].task
		   &&mat->block[block-1].type==mat->block[block].type)
			block=ret=matmergeblock(mat,block-1,block,mat->block[block].task,mat->block[block].type);
		else
			ret=block;
	}
	else
		block=ret=matsplitblock(mat,block,start);

	if(end==mat->block[block+1].addr)
	{
		if(mat->block[block+1].task==mat->block[block].task
		   &&mat->block[block+1].type==mat->block[block].type)
			matmergeblock(mat,block,block+1,mat->block[block].task,mat->block[block].type);
	}
	else
		matsplitblock(mat,block,end);

	mat->block[block].task=task;
	mat->block[block].type=type;

	return ret;
}

static struct mat *__memory_mat;

u8 matbuild()
{
	u64 i,j;
	
	for(i=0;i<SMAP_COUNT-1;i++)
	{
		for(j=i+1;j<SMAP_COUNT;j++)
		{
			if((SMAP_TABLE+i)->base>(SMAP_TABLE+j)->base)
			{
				struct smap_entry smapent;
				memcpy(&smapent,SMAP_TABLE+i,sizeof(smapent));
				memcpy(SMAP_TABLE+i,SMAP_TABLE+j,sizeof(smapent));
				memcpy(SMAP_TABLE+j,&smapent,sizeof(smapent));
			}
		}
	}

	for(i=0;i<SMAP_COUNT-1;i++)
	{
		if((SMAP_TABLE+i)->base+(SMAP_TABLE+i)->len>(SMAP_TABLE+i+1)->base)
			(SMAP_TABLE+i)->len=(SMAP_TABLE+i+1)->base-(SMAP_TABLE+i)->base;
	}

	__memory_mat=0x100000;

	__memory_mat->memsize=(SMAP_TABLE+SMAP_COUNT-1)->base+(SMAP_TABLE+SMAP_COUNT-1)->len;
	__memory_mat->count=2;
	__memory_mat->maxcount=64;  //todo: need to set later
	__memory_mat->block[0].addr=0;
	__memory_mat->block[0].task=0;
	__memory_mat->block[0].type=MAT_TYPE_OTHER;
	__memory_mat->block[1].addr=__memory_mat->memsize;
	__memory_mat->block[1].task=0;
	__memory_mat->block[1].type=MAT_TYPE_END;

	for(i=0;i<SMAP_COUNT;i++)
	{
		if((SMAP_TABLE+i)->len>0)
		{
			u64 startblock=matfindblock(__memory_mat,0,(SMAP_TABLE+i)->base);
			matpickblock(__memory_mat,startblock,(SMAP_TABLE+i)->base,
			             (SMAP_TABLE+i)->base+(SMAP_TABLE+i)->len,0,
			             ((SMAP_TABLE+i)->type==SMAP_TYPE_USABLE?
			              MAT_TYPE_FREE:MAT_TYPE_OTHER));
		}
	}

	//todo
	matpickblock(__memory_mat,matfindblock(__memory_mat,0,__memory_mat),
	             __memory_mat,__memory_mat
	             +sizeof(struct mat_block)*(__memory_mat->maxcount)
	             +sizeof(*__memory_mat),0,MAT_TYPE_USED);

	return 1;
}

#endif

#endif
