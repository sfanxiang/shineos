#include "mp.h"

u32 processor_cnt;
u32 mp_apic_id[255];

u32 initmp()
{
	struct madt *madt=(struct madt*)acpifindsdt(ACPI_SIGN_MADT);
	if(!madt)return 0;

	void *madtend=(void*)madt+madt->header.len;
	struct madt_entry_header *madtent=(void*)madt+sizeof(struct madt);
	processor_cnt=0;

	memcpy(MP_APSTART_ADDR,getapstartptr(),getapstartlen());
	MP_AP_APMAIN=&apmain;
	if(!vmm_init_raw(1)){
		processor_cnt=1;
		return 1;
	}
	MP_AP_PAGETABLE=getvminfo(1)->pagetable;

	while((void*)madtent<madtend)
	{
		u32 apic_id=0xffffffff;
		if(madtent->type==MADT_TYPE_XAPIC)
		{
			struct madt_entry_xapic *xapic=(struct madt_entry_xapic*)madtent;
			if(xapic->en)apic_id=xapic->apic_id;
		}
		else if(madtent->type==MADT_TYPE_X2APIC)
		{
			struct madt_entry_x2apic *x2apic=(struct madt_entry_x2apic*)madtent;
			if(x2apic->en)apic_id=x2apic->apic_id;
		}

		if(apic_id!=0xffffffff)
		{
			mp_apic_id[processor_cnt]=apic_id;
			processor_cnt++;
			if(processor_cnt>1)
			{
				writecmos(0xf,0xa);
				*((u32*)0x467)=MP_APSTART_ADDR;
				*((u64*)0x1000)=MP_APSTART_ADDR*0x100+0xea;
				
				MP_AP_START=0;
				MP_AP_PROCESSOR=processor_cnt-1;
				MP_AP_STACK=pmm_alloc();
				MP_AP_STACK+=4096;

				struct apic_icr icr;
				memset(&icr,0,sizeof(icr));

				icr.delmode=APIC_DELIVERYMODE_INIT;
				icr.trigger=APIC_TRIGGER_LEVEL;
				icr.destshort=APIC_DESTSHORTHAND_NONE;
				icr.level=APIC_LEVEL_ASSERT;
				icr.dest=apic_id;
				apicwrite(APIC_REG_ICR,*(u64*)&icr);

				icr.level=APIC_LEVEL_DEASSERT;
				apicwrite(APIC_REG_ICR,*(u64*)&icr);

				u16 i;
				for(i=0;i<25000;i++)
				{
					if(MP_AP_START)break;
					inb(0x70);
				}
				
				if(!MP_AP_START)
				{
					icr.vector=1;
					icr.delmode=APIC_DELIVERYMODE_STARTUP;
					icr.trigger=APIC_TRIGGER_EDGE;
					icr.destshort=APIC_DESTSHORTHAND_NONE;
					icr.dest=apic_id;
					apicwrite(APIC_REG_ICR,*(u64*)&icr);
					for(i=0;i<500;i++)
					{
						if(MP_AP_START)break;
						inb(0x70);
					}
					
					if(!MP_AP_START)
					{
						apicwrite(APIC_REG_ICR,*(u64*)&icr);
						for(i=0;i<1000;i++)
						{
							if(MP_AP_START)break;
							inb(0x70);
						}
					}
				}

				if(MP_AP_START)
				{
					while(!MP_AP_READY);
					if(!vmm_init_raw(processor_cnt)){
						writecmos(0xf,0);
						return processor_cnt;
					}
					MP_AP_PAGETABLE=getvminfo(processor_cnt)->pagetable;
				}
				else
				{
					processor_cnt--;
					pmm_free(MP_AP_STACK);
				}
			}
		}
		madtent=(void*)madtent+madtent->len;
	}

	writecmos(0xf,0);
	return processor_cnt;	
}

u32 getprocessorcount()
{
	return processor_cnt;
}

void apmain()
{
	u32 processor=MP_AP_PROCESSOR;

	//todo: error messages
	initmemory(processor);
	void *stack=malloc(8192*1024);
	if(stack!=NULL){
		setstack(MP_AP_STACK,stack+8192*1024,4096);
	}
	initinterrupt();
	initapic();

	MP_AP_READY=1;
	haltcpu();
}
