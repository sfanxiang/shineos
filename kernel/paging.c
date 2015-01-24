#include "paging.h"

u64* buildpaging()
{
	size_t memsize=getmat()->memsize;
	u64 *p0,*p1,*p2,*p3;
	u64 c0,c1,c2,c3;
	
	c0=(memsize-1)/0x8000000000+1;
	p0=kmalloc_align(c0*8,4096);
	if(!p0)return NULL;
	
	c1=(memsize-1)/0x40000000+1;
	p1=kmalloc_align(c1*8,4096);
	if(!p1)
	{
		kfree(p0);
		return NULL;
	}
	
	c2=(memsize-1)/0x200000+1;
	p2=kmalloc_align(c2*8,4096);
	if(!p2)
	{
		kfree(p0);
		kfree(p1);
		return NULL;
	}
	
	c3=(memsize-1)/0x1000+1;
	p3=kmalloc_align(c3*8,4096);
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
	for(i=0;i<c1;i++)
		*(p1+i)=(size_t)(p2+512*i)|3;
	for(i=0;i<c2;i++)
		*(p2+i)=(size_t)(p3+512*i)|3;
	for(i=0;i<c3;i++)
		*(p3+i)=((size_t)4096*i)|3;
	
	return p0;
}

u64 *pagetable;

u8 initpaging()
{
	pagetable=buildpaging();
	if(!pagetable)return 0;
	setpaging(pagetable);
	kfree(0x80000);
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
