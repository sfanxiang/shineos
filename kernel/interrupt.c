#include "cpu.h"
#include "display.h"
#include "interrupt.h"

struct idt_ptr *idtr[255];

extern void* getinthandler_base();
extern size_t getinthandler_base_size();

u8 buildinterrupt(u16 count,u32 processor)
{
	idtr[processor]=kmalloc(sizeof(struct idt_ptr));
	if(!idtr[processor])return 0;
	idtr[processor]->base=kcalloc(count,sizeof(struct idt_desc));
	if(!(idtr[processor]->base))
	{
		kfree(idtr[processor]);
		idtr[processor]=NULL;
		return 0;
	}
	idtr[processor]->limit=sizeof(struct idt_desc)*count-1;
	loadidt(idtr[processor]);

	return 1;
}

u8 registerinterrupt(u16 num,u32 processor,void(*handler)
                     (u16 num,u16 ss,u64 rsp,u64 rflags,
                      u16 cs,u64 rip,u64 errorcode),u8 haveerrcode)
{
	if(!idtr[processor]||!(idtr[processor]->base))return 0;
	if((idtr[processor]->limit+1)/sizeof(struct idt_desc)<=num)return 0;
	if(isinterruptregistered(num,processor))return 0;
	
	void* handler_base=kmalloc(getinthandler_base_size());
	if(!handler_base)return 0;

	memcpy(handler_base,getinthandler_base(),getinthandler_base_size());
	*((size_t*)(handler_base))=(size_t)handler;
	*((u16*)(handler_base+8))=num;
	*((u8*)(handler_base+8+2))=haveerrcode;

	(idtr[processor]->base+num)->sel=SEL_CODE;
	(idtr[processor]->base+num)->zero0=0;
	(idtr[processor]->base+num)->zero1=0;
	(idtr[processor]->base+num)->attr=INTERRUPT_ATTR;
	(idtr[processor]->base+num)->off_lo=(size_t)handler_base+8+2+1;
	(idtr[processor]->base+num)->off_hi=((size_t)handler_base+8+2+1)>>16;

	return 1;
}

u8 unregisterinterrupt(u16 num,u32 processor)
{
	if(!idtr[processor]||!(idtr[processor]->base))return 0;
	if((idtr[processor]->limit+1)/sizeof(struct idt_desc)<=num)return 0;
	if(!isinterruptregistered(num,processor))return 0;

	kfree(((size_t)((idtr[processor]->base+num)->off_hi))<<16+(idtr[processor]->base+num)->off_lo-8-2-1);
	memset(idtr[processor]->base+num,0,sizeof(struct idt_desc));
	return 1;
}

u8 isinterruptregistered(u16 num,u32 processor)
{
	if(!idtr[processor]||!(idtr[processor]->base))return 0;
	if((idtr[processor]->limit+1)/sizeof(struct idt_desc)<=num)return 0;
	return ((idtr[processor]->base+num)->attr!=0);
}

void exceptionhandler(u16 num,u16 ss,u64 rsp,u64 rflags,u16 cs,u64 rip,u64 errorcode)
{
	char buf[20];
	message("Exception!");
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

	//todo: broadcast exception
	disable_int();
	haltcpu();
}

u8 initinterrupt(u32 processor)
{
	if(!buildinterrupt(0xff,processor))return 0;
	u16 i;
	for(i=0;i<=7;i++)
	{
		if(!registerinterrupt(i,processor,exceptionhandler,0))
			return 0;
	}
	if(!registerinterrupt(8,processor,exceptionhandler,1))return 0;
	if(!registerinterrupt(9,processor,exceptionhandler,0))return 0;
	for(i=10;i<=14;i++)
	{
		if(!registerinterrupt(i,processor,exceptionhandler,1))
			return 0;
	}
	if(!registerinterrupt(16,processor,exceptionhandler,0))return 0;
	if(!registerinterrupt(17,processor,exceptionhandler,1))return 0;
	if(!registerinterrupt(18,processor,exceptionhandler,0))return 0;
	if(!registerinterrupt(19,processor,exceptionhandler,0))return 0;

	return 1;
}
