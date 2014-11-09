//this only supports SATA
#ifndef AHCI_H
#define AHCI_H

#ifdef __AS386_64__

#include "defines.h"
#include "pci.h"

u8 findahci(struct pci_device* dev)
{
	u16 bus;	//u8 can't reach 256
	for(bus=0;bus<PCI_BUS_MAX;bus++)
		for(dev->slot=0;dev->slot<PCI_SLOT_MAX;dev->slot++)
		{
			dev->bus=bus;
			dev->func=0;
			if(checkpcidevice(dev)&&to8bit(readpciconfig(
				dev,offsetof(struct pci_config_std,class)))
				==PCI_MASS_STORAGE
				&&to8bit(readpciconfig(dev,offsetof(
				struct pci_config_std,subclass)))
				==PCI_SUB_SATA)
				return 1;
		}
	return 0;
}

#endif

#endif
