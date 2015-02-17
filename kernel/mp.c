#include "mp.h"

u32 processor_cnt;

u32 initmp()
{
	struct madt *madt=(struct madt*)acpifindsdt(ACPI_SIGN_MADT);
	if(!madt)return 0;

	void *madtend=(void*)madt+madt->header.len;
	struct madt_entry_header *madtent=(void*)madt+sizeof(struct madt);
	processor_cnt=0;

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
			processor_cnt++;
			if(processor_cnt>1)
			{
				writecmos(0xf,0xa);
				*((u32*)0x467)=0x00007c00;
				*((u64*)0x1000)=0x00007c00ea;
				
				*((u8*)0x7c0b)=0;	//start flag

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
				for(i=0;i<25000;i++)inb(0x70);
				
				if(!*((u8*)0x7c0b))
				{
					icr.vector=1;
					icr.delmode=APIC_DELIVERYMODE_STARTUP;
					icr.trigger=APIC_TRIGGER_EDGE;
					icr.destshort=APIC_DESTSHORTHAND_NONE;
					icr.dest=apic_id;
					apicwrite(APIC_REG_ICR,*(u64*)&icr);
					for(i=0;i<500;i++)inb(0x70);
					
					if(!*((u8*)0x7c0b))
					{
						apicwrite(APIC_REG_ICR,*(u64*)&icr);
						for(i=0;i<1000;i++)inb(0x70);
						if(!*((u8*)0x7c0b))processor_cnt--;
					}
				}
			}
		}
		madtent=(void*)madtent+madtent->len;
	}

	return processor_cnt;	
}

u32 getprocessorcount()
{
	return processor_cnt;
}
