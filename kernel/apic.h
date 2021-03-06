#ifndef APIC_H
#define APIC_H

#ifdef __x86_64__

#include "cpu.h"
#include "interrupt.h"
#include "memory.h"
#include "paging.h"

#define APIC_BASE_MSR 0x1b

enum apic_regs{
	APIC_REG_ID=0x2,
	APIC_REG_VER=0x3,
	APIC_REG_TASKPRIOR=0x8,
	APIC_REG_EOI=0xb,
	APIC_REG_LDR=0xd,
	APIC_REG_SPURIOUS=0xf,
	APIC_REG_ESR=0x28,
	APIC_REG_ICR=0x30,
	APIC_REG_LVT_TMR=0x32,
	APIC_REG_LVT_PERF=0x34,
	APIC_REG_LVT_LINT0=0x35,
	APIC_REG_LVT_LINT1=0x36,
	APIC_REG_LVT_ERR=0x37,
	APIC_REG_TMRINITCNT=0x38,
	APIC_REG_TMRCURCNT=0x39,
	APIC_REG_TMRDIV=0x3e,
};

struct apic_base{
	u8 reserved0;
	u8 bsp:1;
	u8 reserved1:1;
	u8 enable_x2apic:1;
	u8 enable_global:1;
	u32 base:24;
	u32 reserved2:28;
}__attribute__((packed));

struct apic_spurious{
	u8 vector;
	u8 enable:1;
	u8 reserved0:3;
	u8 eoi_broadcast:1;
	u32 reserved1:19;
}__attribute__((packed));

struct apic_icr{
	u8 vector;
	u8 delmode:3;
	u8 destmode:1;
	u8 reserved0:2;
	u8 level:1;
	u8 trigger:1;
	u8 reserved1:2;
	u8 destshort:2;
	u16 reserved2:12;
	u32 dest;
}__attribute__((packed));

struct apic_lvt{
	u8 vector;
	u8 delmode:3;
	u8 reserved0:1;
	u8 delstatus:1;
	u8 pinpolar:1;
	u8 remoteirr:1;
	u8 trigger:1;
	u8 mask:1;
	u8 timer:2;
	u16 reserved1:13;
}__attribute__((packed));

#define APIC_DELIVERYMODE_FIXED 0
#define APIC_DELIVERYMODE_INIT 0b101
#define APIC_DELIVERYMODE_STARTUP 0b110
#define APIC_DESTINATIONMODE_PHYSICAL 0
#define APIC_DESTINATIONMODE_LOGICAL 1
#define APIC_LEVEL_DEASSERT 0
#define APIC_LEVEL_ASSERT 1
#define APIC_TRIGGER_EDGE 0
#define APIC_TRIGGER_LEVEL 1
#define APIC_DESTSHORTHAND_NONE 0
#define APIC_DESTSHORTHAND_SELF 1
#define APIC_DESTSHORTHAND_ALLINCSELF 2
#define APIC_DESTSHORTHAND_ALLEXCSELF 3
#define APIC_TIMER_ONESHOT 0
#define APIC_TIMER_PERIODIC 1

extern u64 apicread(u32 reg);
extern void apicwrite(u32 reg,u64 val);
extern void sendeoi();
extern u8 initapic();

#endif

#endif
