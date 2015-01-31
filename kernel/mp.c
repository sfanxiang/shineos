#include "mp.h"

void initmp()
{
	writecmos(0xf,0xa);
	*((u32*)0x467)=0x00007c00;
	*((u64*)0x1000)=0x00007c00ea;

	struct apic_icr icr;
	memset(icr,0,sizeof(icr));
	
	icr.delmode=APIC_DELIVERYMODE_INIT;
	icr.destmode=APIC_DESTINATIONMODE_LOGICAL;
	icr.trigger=APIC_TRIGGER_LEVEL;
	icr.destshort=APIC_DESTSHORTHAND_ALLEXCSELF;
	icr.level=APIC_LEVEL_ASSERT;
	apicwrite(APIC_REG_ICR,*(u64*)&icr);
	icr.level=APIC_LEVEL_DEASSERT;
	apicwrite(APIC_REG_ICR,*(u64*)&icr);
	
	u8 i;
	for(i=0;i<100;i++)
		inb(0x70);
	
	icr.vector=1;
	icr.delmode=APIC_DELIVERYMODE_STARTUP;
	icr.destmode=APIC_DESTINATIONMODE_LOGICAL;
	icr.trigger=APIC_TRIGGER_EDGE;
	icr.destshort=APIC_DESTSHORTHAND_ALLEXCSELF;
	apicwrite(APIC_REG_ICR,*(u64*)&icr);

	inb(0x70);inb(0x70);
	apicwrite(APIC_REG_ICR,*(u64*)&icr);
}
