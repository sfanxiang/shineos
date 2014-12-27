#include "cpu.h"q

#include "interrupt.h"

struct idt_ptr *idtr;
struct idt_desc *idt_table;

extern void* getinthandler_base();
extern size_t getinthandler_base_size();

u8 buildinterrupt(u16 count)
{
	idtr=kmalloc(sizeof(struct idt_ptr));
	if(!idtr)return 0;
	idt_table=kcalloc(count,sizeof(struct idt_desc));
	if(!idt_table)
	{
		kfree(idtr);
		return 0;
	}
	idtr->base=idt_table;
	idtr->limit=sizeof(struct idt_desc)*count-1;
	loadidt(idtr);

	return 1;
}

u8 registerinterrupt(u16 num,void(*handler)
                     (u16 num,u16 ss,u64 rsp,u32 eflags,u16 cs,u64 rip),u8 attr)
{
	if((idt_table+num)->attr!=0)return 0;
	
	void* handler_base=kmalloc(getinthandler_base_size());
	if(!handler_base)return 0;

	memcpy(handler_base,getinthandler_base(),getinthandler_base_size());
	*((u16*)(handler_base))=num;
	*((size_t*)(handler_base+2))=(size_t)handler;

	(idt_table+num)->sel=SEL_CODE;
	(idt_table+num)->zero0=0;
	(idt_table+num)->zero1=0;
	(idt_table+num)->attr=attr;
	(idt_table+num)->off_lo=(size_t)handler_base+10;
	(idt_table+num)->off_hi=((size_t)handler_base+10)>>16;

	return 1;
}

u8 unregisterinterrupt(u16 num)
{
	if((idt_table+num)->attr==0)return 0;

	kfree((idt_table+num)->off_hi<<16+(idt_table+num)->off_lo-10);
	memset(idt_table+num,0,sizeof(struct idt_desc));
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
