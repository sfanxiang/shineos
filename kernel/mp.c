#include "mp.h"

u32 initmp()
{
	struct madt *madt=(struct madt*)acpifindsdt(ACPI_SIGN_MADT);
	if(!madt)return 0;

	void *madtend=(void*)madt+madt->header.len;
	struct madt_entry_header *madtent=(void*)madt+sizeof(struct madt);
	u32 count=0;

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
			count++;
			if(count>1)
			{
				writecmos(0xf,0xa);
				*((u32*)0x467)=0x00007c00;
				*((u64*)0x1000)=0x00007c00ea;

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

				u8 i;
				for(i=0;i<100;i++)
					inb(0x70);

				icr.vector=1;
				icr.delmode=APIC_DELIVERYMODE_STARTUP;
				icr.trigger=APIC_TRIGGER_EDGE;
				icr.destshort=APIC_DESTSHORTHAND_NONE;
				icr.dest=apic_id;
				apicwrite(APIC_REG_ICR,*(u64*)&icr);

				inb(0x70);inb(0x70);
				apicwrite(APIC_REG_ICR,*(u64*)&icr);
				inb(0x70);inb(0x70);
			}
		}
		madtent=(void*)madtent+madtent->len;
	}

	return count;	
}
