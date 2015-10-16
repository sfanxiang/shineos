#include "apic.h"

u64 apicread(u32 reg)
{
	return readmsr(0x800+reg);
}

void apicwrite(u32 reg,u64 val)
{
	writemsr(0x800+reg,val);
}

void sendeoi()
{
	apicwrite(APIC_REG_EOI,0);
}

void int_spurious(u16 num,u16 ss,u64 rsp,u64 rflags,u16 cs,u64 rip,u64 errorcode)
{
	sendeoi();
}

void int_timer(u16 num,u16 ss,u64 rsp,u64 rflags,u16 cs,u64 rip,u64 errorcode)
{
	sendeoi();
}

u8 initapic()
{
	struct apic_base base;
	*((u64*)(&base))=readmsr(APIC_BASE_MSR);
	base.enable_x2apic=base.enable_global=1;
	writemsr(APIC_BASE_MSR,*((u64*)(&base)));

	if(!registerinterrupt(0x7f,int_spurious,0))
		return 0;

	struct apic_spurious spurious;
	memset(&spurious,0,sizeof(spurious));
	spurious.enable=1;
	spurious.vector=0x7f;
	apicwrite(APIC_REG_SPURIOUS,*((u32*)(&spurious)));

	if(!registerinterrupt(0x81,int_timer,0))
		return 0;

	struct apic_lvt lvt;
	memset(&lvt,0,sizeof(lvt));
	lvt.vector=0x81;
	lvt.delmode=APIC_DELIVERYMODE_FIXED;
	lvt.trigger=APIC_TRIGGER_EDGE;
	lvt.mask=0;
	lvt.timer=APIC_TIMER_PERIODIC;
	apicwrite(APIC_REG_LVT_TMR,*((u32*)(&lvt)));

	apicwrite(APIC_REG_TMRDIV,0x3);
	apicwrite(APIC_REG_TMRINITCNT,0x80);

	enable_int();
	return 1;
}
