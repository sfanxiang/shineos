#ifndef PCI_H
#define PCI_H

#ifdef __AS386_64__

#include "defines.h"
#include "io.h"

struct pci_std{
	u16 vendor_id;
	u16 device_id;
	u16 command;
	u16 status;
	u8 revision_id;
	u8 prog_if;
	u8 subclass;
	u8 class;
	u8 cache_size;
	u8 latency;
	u8 header_type;
	u8 bist;
	u32 bar[5];
	u32 cardbus_ptr;
	u16 subsys_id;
	u16 subsys_vendor_id;
	u32 rom_base_addr;
	u8 caps_ptr;
	u8 reserved[7];
	u8 int_line;
	u8 int_pin;
	u8 min_grant;
	u8 max_latency;
}__attribute__ ((aligned(1)));

u32 readpciconfig(u8 bus,u8 slot,u8 func,u8 reg)
{
	out32(0xcf8,(((u32)bus<<16)|((u32)slot<<11)|
		((u32)func<<8)|((u32)reg&0xfc)|
		((u32)0x80000000)));
	return (in32(0xcfc)>>(((u16)reg&3)<<3));
}

#endif

#endif
