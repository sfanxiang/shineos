#include "cpu.h"
#include "display.h"
#include "interrupt.h"

struct idt_ptr *idtr[255];

extern void* getinthandler_base();
extern size_t getinthandler_base_size();

u8 buildinterrupt(u16 count)
{
	int processor=MP_PROCESSOR_INFO->n;

	idtr[processor]=malloc(sizeof(struct idt_ptr));
	if(!idtr[processor])return 0;
	idtr[processor]->base=calloc(count,sizeof(struct idt_desc));
	if(!(idtr[processor]->base))
	{
		free(idtr[processor]);
		idtr[processor]=NULL;
		return 0;
	}
	idtr[processor]->limit=sizeof(struct idt_desc)*count-1;
	loadidt(idtr[processor]);

	return 1;
}

u8 registerinterrupt(u16 num,void(*handler)
                     (u16 num,u16 ss,u64 rsp,u64 rflags,
                      u16 cs,u64 rip,u64 errorcode),u8 haserrcode)
{
	int processor=MP_PROCESSOR_INFO->n;

	if(!idtr[processor]||!(idtr[processor]->base))return 0;
	if((idtr[processor]->limit+1)/sizeof(struct idt_desc)<=num)return 0;
	if(isinterruptregistered(num,processor))return 0;
	
	void* handler_base=malloc(getinthandler_base_size());
	if(!handler_base)return 0;

	memcpy(handler_base,getinthandler_base(),getinthandler_base_size());
	*((size_t*)(handler_base))=(size_t)handler;
	*((u16*)(handler_base+8))=num;
	*((u8*)(handler_base+8+2))=haserrcode;

	(idtr[processor]->base+num)->sel=SEL_CODE;
	(idtr[processor]->base+num)->zero0=0;
	(idtr[processor]->base+num)->zero1=0;
	(idtr[processor]->base+num)->attr=INTERRUPT_ATTR;
	(idtr[processor]->base+num)->off_lo=(size_t)handler_base+8+2+1;
	(idtr[processor]->base+num)->off_hi=((size_t)handler_base+8+2+1)>>16;

	return 1;
}

u8 unregisterinterrupt(u16 num)
{
	int processor=MP_PROCESSOR_INFO->n;

	if(!idtr[processor]||!(idtr[processor]->base))return 0;
	if((idtr[processor]->limit+1)/sizeof(struct idt_desc)<=num)return 0;
	if(!isinterruptregistered(num,processor))return 0;

	free(((size_t)((idtr[processor]->base+num)->off_hi))<<16+(idtr[processor]->base+num)->off_lo-8-2-1);
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
	if(num==3)
		message("Debug:");
	else
		message("Exception!");
	int n=MP_PROCESSOR_INFO->n;
	printf("processor=%d,vector=%u,ss=0x%lx,rsp=0x%lx,rflags=0x%lx,cs=0x%x,rip=0x%lx,error=0x%lx\n",
	       n,(unsigned int)num,(unsigned int)ss,rsp,rflags,(unsigned int)cs,rip,errorcode);

	if(num!=3){
		puts("Stopped.\n");
		//todo: broadcast exception
		disable_int();
		haltcpu();
	}
	
	return;
}

u8 initinterrupt()
{
	if(!buildinterrupt(0xff))return 0;
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
