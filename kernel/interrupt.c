#include "cpu.h"q

#include "interrupt.h"

struct idt_ptr *idtr;

extern void* getinthandler_base();
extern size_t getinthandler_base_size();

u8 buildinterrupt(u16 count)
{
	idtr=kmalloc(sizeof(struct idt_ptr));
	if(!idtr)return 0;
	idtr->base=kcalloc(count,sizeof(struct idt_desc));
	if(!(idtr->base))
	{
		kfree(idtr);
		idtr=NULL;
		return 0;
	}
	idtr->limit=sizeof(struct idt_desc)*count-1;
	loadidt(idtr);

	return 1;
}

u8 registerinterrupt(u16 num,void(*handler)
                     (u16 num,u16 ss,u64 rsp,u32 eflags,u16 cs,u64 rip),u8 attr)
{
	if(!idtr||!(idtr->base))return 0;
	if((idtr->limit+1)/sizeof(struct idt_desc)<=num)return 0;
	if((idtr->base+num)->attr!=0)return 0;
	
	void* handler_base=kmalloc(getinthandler_base_size());
	if(!handler_base)return 0;

	memcpy(handler_base,getinthandler_base(),getinthandler_base_size());
	*((u16*)(handler_base))=num;
	*((size_t*)(handler_base+2))=(size_t)handler;

	(idtr->base+num)->sel=SEL_CODE;
	(idtr->base+num)->zero0=0;
	(idtr->base+num)->zero1=0;
	(idtr->base+num)->attr=attr;
	(idtr->base+num)->off_lo=(size_t)handler_base+10;
	(idtr->base+num)->off_hi=((size_t)handler_base+10)>>16;

	return 1;
}

u8 unregisterinterrupt(u16 num)
{
	if(!idtr||!(idtr->base))return 0;
	if((idtr->limit+1)/sizeof(struct idt_desc)<=num)return 0;
	if((idtr->base+num)->attr==0)return 0;

	kfree((idtr->base+num)->off_hi<<16+(idtr->base+num)->off_lo-10);
	memset(idtr->base+num,0,sizeof(struct idt_desc));
	return 1;
}

void exceptionhandler(u16 num,u16 ss,u64 rsp,u32 eflags,u16 cs,u64 rip)
{
	haltcpu();
}

u8 initinterrupt()
{
	if(!buildinterrupt(0x100))return 0;
	if(!registerinterrupt(0,exceptionhandler,INTERRUPT_ATTR))return 0;

	return 1;
}
