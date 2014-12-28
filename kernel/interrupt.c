#include "cpu.h"
#include "display.h"
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
                     (u16 num,u16 ss,u64 rsp,u64 rflags,
                      u16 cs,u64 rip,u64 errorcode),u8 haveerrcode)
{
	if(!idtr||!(idtr->base))return 0;
	if((idtr->limit+1)/sizeof(struct idt_desc)<=num)return 0;
	if(isinterruptregistered(num))return 0;
	
	void* handler_base=kmalloc(getinthandler_base_size());
	if(!handler_base)return 0;

	memcpy(handler_base,getinthandler_base(),getinthandler_base_size());
	*((size_t*)(handler_base))=(size_t)handler;
	*((u16*)(handler_base+8))=num;
	*((u8*)(handler_base+8+2))=haveerrcode;

	(idtr->base+num)->sel=SEL_CODE;
	(idtr->base+num)->zero0=0;
	(idtr->base+num)->zero1=0;
	(idtr->base+num)->attr=INTERRUPT_ATTR;
	(idtr->base+num)->off_lo=(size_t)handler_base+8+2+1;
	(idtr->base+num)->off_hi=((size_t)handler_base+8+2+1)>>16;

	return 1;
}

u8 unregisterinterrupt(u16 num)
{
	if(!idtr||!(idtr->base))return 0;
	if((idtr->limit+1)/sizeof(struct idt_desc)<=num)return 0;
	if(!isinterruptregistered(num))return 0;

	kfree(((size_t)((idtr->base+num)->off_hi))<<16+(idtr->base+num)->off_lo-8-2-1);
	memset(idtr->base+num,0,sizeof(struct idt_desc));
	return 1;
}

u8 isinterruptregistered(u16 num)
{
	if(!idtr||!(idtr->base))return 0;
	if((idtr->limit+1)/sizeof(struct idt_desc)<=num)return 0;
	return ((idtr->base+num)->attr!=0);
}

void exceptionhandler(u16 num,u16 ss,u64 rsp,u64 rflags,u16 cs,u64 rip,u64 errorcode)
{
	char buf[20];
	puts("\nException!\n");
	puts("vector=");
	puts(itoa(num,buf,10));
	puts(",ss=0x");
	puts(itoa(ss,buf,16));
	puts(",rsp=0x");
	puts(itoa(rsp,buf,16));
	puts(",rflags=0x");
	puts(itoa(rflags,buf,16));
	puts(",cs=0x");
	puts(itoa(cs,buf,16));
	puts(",rip=0x");
	puts(itoa(rip,buf,16));
	puts(",error=0x");
	puts(itoa(errorcode,buf,16));
	puts("\nStopped.\n");
	haltcpu();
}

u8 initinterrupt()
{
	if(!buildinterrupt(0x100))return 0;
	u16 i;
	for(i=0;i<=7;i++)
	{
		if(!registerinterrupt(i,exceptionhandler,0))
			return 0;
	}
	if(!registerinterrupt(8,exceptionhandler,1))return 0;
	if(!registerinterrupt(9,exceptionhandler,0))return 0;
	for(i=10;i<=14;i++)
	{
		if(!registerinterrupt(i,exceptionhandler,1))
			return 0;
	}
	if(!registerinterrupt(16,exceptionhandler,0))return 0;
	if(!registerinterrupt(17,exceptionhandler,1))return 0;
	if(!registerinterrupt(18,exceptionhandler,0))return 0;
	if(!registerinterrupt(19,exceptionhandler,0))return 0;

	return 1;
}
