//this only supports SATA
#ifndef AHCI_H
#define AHCI_H

#ifdef __AS386_64__

#include "defines.h"
#include "pci.h"

enum fis_type{
	FIS_TYPE_REG_H2D=0x27,
	FIS_TYPE_REG_D2H=0x34,
	FIS_TYPE_DMA_ACT=0x39,
	FIS_TYPE_DMA_SETUP=0x41,
	FIS_TYPE_DATA=0x46,
	FIS_TYPE_BIST=0x58,
	FIS_TYPE_PIO_SETUP=0x5f,
	FIS_TYPE_DEV_BITS=0xa1,
};

#pragma pack(push)
#pragma pack(1)

struct fis_reg_h2d{
	u8 type;

	u8 pmport:4;
	u8 reserved0:3;
	u8 cmd:1;

	u8 command;
	u8 featurel;

	u32 lba0:24;
	u8 device;

	u32 lba1:24;
	u8 featureh;

	u16 count;
	u8 icc;
	u8 control;

	u8 reserved1[4];
};

struct fis_reg_d2h{
	u8 type;
	
	u8 pmport:4;
	u8 reserved0:2;
	u8 i:1;
	u8 reserved1:1;
	
	u8 status;
	u8 error;
	
	u32 lba0:24;
	u8 device;
	
	u32 lba1:24;
	u8 reserved2;
	
	u16 count;
	u8 reserved3[6];
};

struct fis_pio_setup{
	u8 type;
	
	u8 pmport:4;
	u8 reserved0:1;
	u8 data:1;
	u8 i:1;
	u8 reserved1:1;
	
	u8 status;
	u8 error;
	
	u32 lba0:24;
	u8 device;

	u32 lba1:24;
	u8 reserved2;
	
	u16 count;
	u8 reserved3;
	u8 e_status;
	
	u16 trans_count;
	u8 reserved4[2];
};

struct fis_dma_setup{
	u8 type;
	
	u8 pmport:4;
	u8 reserved0:1;
	u8 data:1;
	u8 i:1;
	u8 auto_act:1;
	
	u8 reserved1[2];
	
	u64 dma_buffer_id;
	u32 reserved2;
	u32 dma_buffer_offset;
	u32 trans_count;
	u32 reserved3;
};

struct fis_dev_bits{
	u8 type;
	
	u8 receive:5;
	u8 reserved0:1;
	u8 i:1;
	u8 reserved1:1;
	
	u8 statusl:3;
	u8 reserved2:1;
	u8 statush:3;
	u8 reserved3:1;
	
	u8 error;
	u32 s_active;
};

struct hba_fis{
	struct fis_dma_setup dma_setup;
	u8 pad0[4];
	struct fis_pio_setup pio_setup;
	u8 pad1[12];
	struct fis_reg_d2h reg;
	u8 pad2[4];
	struct fis_dev_bits dev_bits;
	u8 u[64];
	u8 reserved[0x100-0xa0];
};

struct hba_prdt_entry{
	void* data;
	u32 reserved0;
	
	u32 data_byte_cnt:22;
	u32 reserved1:9;
	u32 i:1;
};

struct hba_cmd_table{
	u8 cmd_fis[64];
	u8 atapi_cmd[16];
	u8 reserved[48];
	struct hba_prdt_entry prdt_entry[];
};

struct hba_cmd_header{
	u8 cmd_fis_len:5;
	u8 atapi:1;
	u8 write:1;
	u8 prefetch:1;
	
	u8 reset:1;
	u8 bist:1;
	u8 clear:1;
	u8 reserved0:1;
	u8 pmp:4;
	
	u16 prdt_len;
	volatile u32 prdb_cnt;
	
	struct hba_cmd_table* cmd_table;
	
	u32 reserved1[4];
};

struct hba_port{
	struct hba_cmd_header* cmd_list;
	struct hba_fis* fis;
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

#define ATA_CMD_READ_DMA_EX 0x25
#define ATA_CMD_WRITE_DMA_EX 0x35

#define ATA_DEV_BUSY 0x80
#define ATA_DEV_DRQ 0x08

#define HBA_PORT_DET_PRESENT 3
#define HBA_PORT_IPM_ACTIVE 1

#define HBA_PORT_CMD_ST 1
#define HBA_PORT_CMD_FRE (1<<4)
#define HBA_PORT_CMD_FR (1<<14)
#define HBA_PORT_CMD_CR (1<<15)

#define HBA_PORT_IS_TFES (1<<30)

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

s8 findahciport(volatile struct hba_mem *abar,u8 begin,u32 type)
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
	return -1;
}

s8 findahcicmdslot(volatile struct hba_port *port)
{
	u32 slots=port->sactive|port->cmd_issue;
	u8 i;
	for(i=0;i<32;i++)
	{
		if(!(slots&1))
			return i;
		slots>>=1;
	}
	return -1;
}

u8 readahcidrive(volatile struct hba_port *port,u64 start,u32 count,void *buf)
{
	port->int_status=0xffffffffU;
	s8 slot=findahcicmdslot(port);
	if(slot==-1)
		return 0;

	volatile struct hba_cmd_header *cmdheader=port->cmd_list;
	cmdheader+=slot;
	cmdheader->cmd_fis_len=sizeof(struct fis_reg_h2d)/sizeof(u32);
	cmdheader->write=0;
	cmdheader->prdt_len=((count-1)>>4)+1;

	volatile struct hba_cmd_table *cmdtable=cmdheader->cmd_table;
	memset(cmdtable,0,sizeof(*cmdtable)+sizeof(struct hba_prdt_entry)*(cmdheader->prdt_len));

	u16 i;
	for(i=0;i<cmdheader->prdt_len-1;i++)
	{
		cmdtable->prdt_entry[i].data=buf;
		cmdtable->prdt_entry[i].data_byte_cnt=8192;
		cmdtable->prdt_entry[i].i=1;
		buf+=8192;
		count-=16;
	}
	cmdtable->prdt_entry[i].data=buf;
	cmdtable->prdt_entry[i].data_byte_cnt=count<<9;
	cmdtable->prdt_entry[i].i=1;

	volatile struct fis_reg_h2d *cmdfis=(volatile struct fis_reg_h2d*)&(cmdtable->cmd_fis);
	cmdfis->type=FIS_TYPE_REG_H2D;
	cmdfis->cmd=1;
	cmdfis->command=ATA_CMD_READ_DMA_EX;

	cmdfis->lba0=start&0xffffff;
	cmdfis->device=1<<6;	//LBA mode
	cmdfis->lba1=start>>24;

	cmdfis->count=count;

	u32 spin=0;
	while(spin<2147483647&&(port->task_file&(ATA_DEV_BUSY|ATA_DEV_DRQ)))
		spin++;

	if(spin==2147483647)
		return 0;

	port->cmd_issue=1<<slot;

	while(1)
	{
		if((port->cmd_issue&(1<<slot))==0)
			break;
		if(port->int_status&HBA_PORT_IS_TFES)
			return 0;
	}

	if(port->int_status&HBA_PORT_IS_TFES)
		return 0;

	return 1;
}

/*
void start_ahci_cmd(volatile struct hba_port *port)
{
	while(port->cmd&HBA_PORT_CMD_CR);
	port->cmd|=HBA_PORT_CMD_FRE;
	port->cmd|=HBA_PORT_CMD_ST;
}

void stop_ahci_cmd(volatile struct hba_port *port)
{
	port->cmd&=~HBA_PORT_CMD_ST;
	while((port->cmd&HBA_PORT_CMD_FR)||(port->cmd&HBA_PORT_CMD_CR);
	port->cmd&=~HBA_PORT_CMD_FRE;
}
*/

#endif

#endif
