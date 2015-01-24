#include "apic.h"
#include "display.h"	//temporary

#ifndef ENABLE_X2APIC
void *apicbase;

void* getapicbase()
{
	return apicbase;
}

void setapicbase(void *base)
{
	apicbase=base;
}
#endif

u64 apicread(u32 reg)
{
#ifdef ENABLE_X2APIC
	return readmsr(0x800+reg);
#else
	return *((volatile u64*)(apicbase+reg*0x10));
#endif
}

void apicwrite(u32 reg,u64 val)
{
#ifdef ENABLE_X2APIC
	writemsr(0x800+reg,val);
#else
	*((volatile u64*)(apicbase+reg*0x10))=val;
#endif
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
	puts("int");
	sendeoi();
}

u8 initapic()
{
	struct apic_lvt lvt;

#ifndef ENABLE_X2APIC
	apicbase=kcalloc_align(1,4096,4096);	//todo: apicbase must be less then 4G
	if(!apicbase)return 0;
	u64 *basepage=getpageentry(apicbase);
	*basepage=((*basepage)&(~0xfff))|0b11011;
	invlpaging();

	memcpy(apicbase,0xfee00000,4096);

	apicwrite(APIC_REG_DFR,0xffffffff);
	apicwrite(APIC_REG_LDR,
		  (apicread(APIC_REG_LDR)&0xffffff)|1);
	memset(&lvt,0,sizeof(lvt));
	lvt.mask=1;
	apicwrite(APIC_REG_LVT_TMR,*((u32*)(&lvt)));
	apicwrite(APIC_REG_LVT_PERF,*((u32*)(&lvt)));
	apicwrite(APIC_REG_LVT_LINT0,*((u32*)(&lvt)));
	apicwrite(APIC_REG_LVT_LINT1,*((u32*)(&lvt)));
	apicwrite(APIC_REG_TASKPRIOR,0);
#endif

	struct apic_base base;
	*((u64*)(&base))=readmsr(APIC_BASE_MSR);
#ifdef ENABLE_X2APIC
	base.enable_x2apic=base.enable_global=1;
#else
	base.enable_global=1;base.enable_x2apic=0;
	base.base=(size_t)apicbase>>12;
#endif
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

	enable_int();

	memset(&lvt,0,sizeof(lvt));
	lvt.vector=0x81;
	lvt.dmode=APIC_LVT_DMODE_FIXED;
	lvt.trigger=APIC_LVT_TRIGGER_EDGE;
	lvt.mask=0;
	lvt.timer=APIC_LVT_TIMER_PERIODIC;
	apicwrite(APIC_REG_LVT_TMR,*((u32*)(&lvt)));

	apicwrite(APIC_REG_TMRDIV,0x10);
	apicwrite(APIC_REG_TMRINITCNT,0x10);

	return 1;
}
