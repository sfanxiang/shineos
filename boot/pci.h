#ifndef PCI_H
#define PCI_H

#ifdef __AS386_64__

#include "defines.h"
#include "io.h"

struct pci_config_std{
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

enum pci_class{
	PCI_NONE=0,
	PCI_MASS_STORAGE=1,
	//todo
};

enum pci_subclass_mass_storage{
	PCI_SUB_SATA=6,
	//todo
};

struct pci_device{
	u8 bus,slot,func;
};

u32 readpciconfig(struct pci_device *spec,u8 reg)
{
	out32(0xcf8,(((u32)(spec->bus)<<16)|((u32)(spec->slot)<<11)|
		((u32)(spec->func)<<8)|(((u32)bitsrange(reg,2,7))<<2)|
		((u32)0x80000000)));
	return (in32(0xcfc)>>(((u16)reg&3)<<3));
}

u8 checkpcidevice(struct pci_device *spec)
{
	return ((to16bit(readpciconfig(spec,offsetof(struct pci_config_std,vendor_id)))==0xffff)?0:1);
}

#endif

#endif
