#include "paging.h"

u64* buildpaging()
{
	size_t memsize=getmat()->memsize;
	u64 *p0,*p1,*p2,*p3;
	u64 c0,c1,c2,c3;
	u64 e0,e1,e2,e3;
	
	c0=(memsize+0x8000000000-1)/0x8000000000;
	e0=(c0+511)/512*512;
	p0=kmalloc_align(e0*8,4096);
	if(!p0)return NULL;
	
	c1=(memsize+0x40000000-1)/0x40000000;
	e1=(c1+511)/512*512;
	p1=kmalloc_align(e1*8,4096);
	if(!p1)
	{
		kfree(p0);
		return NULL;
	}
	
	c2=(memsize+0x200000-1)/0x200000;
	e2=(c2+511)/512*512;
	p2=kmalloc_align(e2*8,4096);
	if(!p2)
	{
		kfree(p0);
		kfree(p1);
		return NULL;
	}
	
	c3=(memsize+0x1000-1)/0x1000;
	e3=(c3+511)/512*512;
	p3=kmalloc_align(e3*8,4096);
	if(!p3)
	{
		kfree(p0);
		kfree(p1);
		kfree(p2);
		return NULL;
	}
	
	size_t i;
	for(i=0;i<c0;i++)
		*(p0+i)=(size_t)(p1+512*i)|3;
	for(i=c0;i<e0;i++)
		*(p0+i)=0;
	for(i=0;i<c1;i++)
		*(p1+i)=(size_t)(p2+512*i)|3;
	for(i=c1;i<e1;i++)
		*(p1+i)=0;
	for(i=0;i<c2;i++)
		*(p2+i)=(size_t)(p3+512*i)|3;
	for(i=c2;i<e2;i++)
		*(p2+i)=0;
	for(i=0;i<c3;i++)
		*(p3+i)=((size_t)4096*i)|3;
	for(i=c3;i<e3;i++)
		*(p3+i)=0;
	
	return p0;
}

u64 *pagetable;

u8 initpaging()
{
	pagetable=buildpaging();
	if(!pagetable)return 0;
	setpaging(pagetable);
	return 1;
}

u64* getpageentry(void *addr)
{
	return (((u64*)((*(((u64*)((*(((u64*)((*(pagetable
		+(((size_t)addr>>39)&0x1ff)))&(~0xfffL)))
		+(((size_t)addr>>30)&0x1ff)))&(~0xfffL)))
		+(((size_t)addr>>21)&0x1ff)))&(~0xfffL)))
		+(((size_t)addr>>12)&0x1ff));
}
