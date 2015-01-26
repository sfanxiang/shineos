#ifndef ATA_H
#define ATA_H

#ifdef __x86_64__

#include "io.h"

#define ATA_BUS0_PORT 0x1f0
#define ATA_BUS1_PORT 0x170

#define ATA_DRIVE_MASTER 0xa0
#define ATA_DRIVE_SLAVE 0xb0

#define ATA_REG_DATA 0
#define ATA_REG_FEATURES 1
#define ATA_REG_SECCNT 2
#define ATA_REG_LBALO 3
#define ATA_REG_LBAMID 4
#define ATA_REG_LBAHI 5
#define ATA_REG_DRIVE 6
#define ATA_REG_CMD 7
#define ATA_REG_STATUS 7
#define ATA_REG_CON 0x206
#define ATA_REG_ALTSTATUS 0x206

#define ATA_STATUS_ERR 1
#define ATA_STATUS_DRQ (1<<3)
#define ATA_STATUS_SRV (1<<4)
#define ATA_STATUS_DF (1<<5)
#define ATA_STATUS_RDY (1<<6)
#define ATA_STATUS_BSY (1<<7)

#define ATA_CON_NIEN (1<<1)
#define ATA_CON_SRST (1<<2)
#define ATA_CON_HOB (1<<7)

#define ATA_CMD_IDENTIFY 0xec

u8 ata_isfloatingbus(u16 port)
{
	return (inb(port+ATA_REG_STATUS)==0xff);
}

void ata_delay(u16 port)
{
	inb(port+ATA_REG_ALTSTATUS);
	inb(port+ATA_REG_ALTSTATUS);
	inb(port+ATA_REG_ALTSTATUS);
	inb(port+ATA_REG_ALTSTATUS);
}

struct atadrive{
	u16 port;
	u8 drive;
};

u8 ata_identify(const struct atadrive *drv)
{
	outb(drv->port+ATA_REG_DRIVE,drv->drive);
	ata_delay(drv->port);
	outb(drv->port+ATA_REG_SECCNT,0);
	outb(drv->port+ATA_REG_LBALO,0);
	outb(drv->port+ATA_REG_LBAMID,0);
	outb(drv->port+ATA_REG_LBAHI,0);
	outb(drv->port+ATA_REG_CMD,ATA_CMD_IDENTIFY);
	
	u8 status=inb(drv->port+ATA_REG_STATUS);
	
	if(status==0)return 0;
	if(inb(drv->port+ATA_REG_LBAMID)!=0
		||inb(drv->port+ATA_REG_LBAHI)!=0)
		return 0;

	while(status&ATA_STATUS_BSY)
	{
		if(inb(drv->port+ATA_REG_LBAMID)!=0
			||inb(drv->port+ATA_REG_LBAHI)!=0)
			return 0;
		status=inb(drv->port+ATA_REG_STATUS);
	}

	if(inb(drv->port+ATA_REG_LBAMID)!=0
		||inb(drv->port+ATA_REG_LBAHI)!=0)
		return 0;
	
	do{
		status=inb(drv->port+ATA_REG_ALTSTATUS);
	}while(!(status&ATA_STATUS_DRQ)&&!(status&ATA_STATUS_ERR));

	if(status&ATA_STATUS_ERR)return 0;

	u16 i;
	for(i=0;i<256;i++)
		inw(drv->port+ATA_REG_DATA);

	return 1;
}

struct atadrive __ata_drives[8];
u8 __ata_drivescnt;

s8 atainit()
{
	__ata_drivescnt=0;

	if(!ata_isfloatingbus(ATA_BUS0_PORT))
	{
		__ata_drives[__ata_drivescnt].port=ATA_BUS0_PORT;
		__ata_drives[__ata_drivescnt].drive=ATA_DRIVE_MASTER;
		if(ata_identify(&__ata_drives[__ata_drivescnt]))
			__ata_drivescnt++;
		__ata_drives[__ata_drivescnt].port=ATA_BUS0_PORT;
		__ata_drives[__ata_drivescnt].drive=ATA_DRIVE_SLAVE;
		if(ata_identify(&__ata_drives[__ata_drivescnt]))
			__ata_drivescnt++;
	}
	if(!ata_isfloatingbus(ATA_BUS1_PORT))
	{
		__ata_drives[__ata_drivescnt].port=ATA_BUS1_PORT;
		__ata_drives[__ata_drivescnt].drive=ATA_DRIVE_MASTER;
		if(ata_identify(&__ata_drives[__ata_drivescnt]))
			__ata_drivescnt++;
		__ata_drives[__ata_drivescnt].port=ATA_BUS1_PORT;
		__ata_drives[__ata_drivescnt].drive=ATA_DRIVE_SLAVE;
		if(ata_identify(&__ata_drives[__ata_drivescnt]))
			__ata_drivescnt++;
	}
	
	return __ata_drivescnt;
}

#endif

#endif
