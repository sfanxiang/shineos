#ifndef MP_H
#define MP_H

#ifdef __x86_64__

#include "acpi.h"
#include "apic.h"
#include "cmos.h"
#include "io.h"
#include "memory.h"
#include "misc.h"

#define MP_APSTART_ADDR 0x8000
#define MP_PROCESSOR_INFO ((processor_info*)(VM_KERNEL))

#define MP_AP_PAGETABLE (*(volatile u64**)(MP_APSTART_ADDR+2))
#define MP_AP_APMAIN (*(volatile u64**)(MP_APSTART_ADDR+0xa))
#define MP_AP_PROCESSOR (*(volatile u32*)(MP_APSTART_ADDR+0x12))
#define MP_AP_START (*(volatile u8*)(MP_APSTART_ADDR+0x16))
#define MP_AP_READY (*(volatile u8*)(MP_APSTART_ADDR+0x17))
#define MP_AP_STACK (*(volatile void**)(MP_APSTART_ADDR+0x18))

typedef struct{
	int n;
}processor_info;

extern void* getapstartptr();
extern size_t getapstartlen();

extern u32 initmp();
extern u32 getprocessorcount();
extern void apmain();

#endif

#endif
