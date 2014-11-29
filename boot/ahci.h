//this only supports SATA
#ifndef AHCI_H
#define AHCI_H

#ifdef __AS386_64__

#include "defines.h"
#include "pci.h"

#pragma pack(push)
#pragma pack(1)

struct hba_port{
	void* cmd_list_base;
	void* fis_base;
	u32 int_status;
	u32 int_enable;
	u32 cmd;
	u32 reserved0;
	u32 task_file;
	u32 sign;
	u32 sstatus;
	u32 sctrl;
	u32 serr;
	u32 sactive;
	u32 cmd_issue;
	u32 snotify;
	u32 fis_switch;
	u32 reserved1[11];
	u32 vendor[4];
};

struct hba_mem{
	u32 host_cap;
	u32 glb_host_ctrl;
	u32 int_status;
	u32 port;
	u32 ver;
	u32 ccc_ctrl;
	u32 ccc_ports;
	u32 em_loc;
	u32 em_ctrl;
	u32 host_cap_ex;
	u32 handoff_ctrl;
	u8 reserved[0xa0-0x2c];
	u8 vendor[0x100-0xa0];
	struct hba_port ports[];
};

#pragma pack(pop)

enum ahci_dev{
	AHCI_DEV_NULL=0,
	AHCI_DEV_SATA=0x101,
	AHCI_DEV_SATAPI=0xeb140101,
	AHCI_DEV_SEMB=0xc33c0101,
	AHCI_DEV_PM=0x96690101,
};

#define HBA_PORT_DET_PRESENT 3
#define HBA_PORT_IPM_ACTIVE 1

volatile struct hba_mem* getabar()
{
	struct pci_device dev;
	u16 bus;	//u8 can't reach PCI_BUS_MAX
	for(bus=0;bus<PCI_BUS_MAX;bus++)
		for(dev.slot=0;dev.slot<PCI_SLOT_MAX;dev.slot++)
		{
			dev.bus=bus;
			dev.func=0;
			if(checkpcidevice(&dev)&&((u8)readpciconfig(
				&dev,offsetof(struct pci_config_std,class)))
				==PCI_MASS_STORAGE
				&&((u8)readpciconfig(&dev,offsetof(
				struct pci_config_std,subclass)))
				==PCI_SUB_SATA)
				return (struct hba_mem*)readpciconfig(&dev,
					offsetof(struct pci_config_std,bar[5]));
		}
	return NULL;
}

u32 getahcidevtype(volatile struct hba_port *port)
{
	if((((port->sstatus)&0xf)!=HBA_PORT_DET_PRESENT)
		||((((port->sstatus)>>8)&0xf)!=HBA_PORT_IPM_ACTIVE))
		return AHCI_DEV_NULL;
	
	return port->sign;
}

u8 findahciport(volatile struct hba_mem *abar,u8 begin,u32 type)
{
	u32 port=abar->port;
	port>>=begin;
	u8 i;
	for(i=begin;i<32;i++)
	{
		if((port&1)&&(getahcidevtype(&(abar->ports[i]))==type))
			return i;
		port>>=1;
	}
	return 0xff;
}

#endif

#endif
