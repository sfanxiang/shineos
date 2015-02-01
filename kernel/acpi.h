#ifndef ACPI_H
#define ACPI_H

#ifdef __x86_64__

#include "misc.h"

#define ACPI_SIGN_MADT 0x43495041	//"APIC"
#define ACPI_SIGN_RSDP 0x2052545020445352	//"RSD PTR "
#define ACPI_SIGN_RSDT 0x54445352	//"RSDT"
#define ACPI_SIGN_XSDT 0x54445358	//"XSDT"

struct acpi_sdt_header{
	u32 sign;
	u32 len;
	u8 rev;
	u8 checksum;
	char oem_id[6];
	char oemtable_id[8];
	u32 oem_rev;
	u32 creator_id;
	u32 creator_rev;
}__attribute__((packed));

struct rsdt{
	struct acpi_sdt_header header;
	u32 sdtptr[];
}__attribute__((packed));

struct xsdt{
	struct acpi_sdt_header header;
	struct acpi_sdt_header *sdtptr[];
}__attribute__((packed));

struct rsdp_desc{
	u64 sign;
	u8 checksum;
	char oem_id[6];
	u8 rev;
	size_t rsdtaddr:32;
	/*
	u32 len;
	struct xsdt *xsdtaddr;
	u8 extchecksum;
	u8 reserved[3];
	*/
}__attribute__((packed));

struct madt{
	struct acpi_sdt_header header;
	size_t local_ic_addr:32;
	u32 flags;
}__attribute__((packed));

#define MADT_TYPE_XAPIC 0
#define MADT_TYPE_X2APIC 9

struct madt_entry_header{
	u8 type;
	u8 len;
}__attribute__((packed));

struct madt_entry_xapic{
	struct madt_entry_header header;
	u8 acpi_id;
	u8 apic_id;
	u32 en:1;
	u32 reserved:31;
}__attribute__((packed));

struct madt_entry_x2apic{
	struct madt_entry_header header;
	u16 reserved0;
	u32 apic_id;
	u32 en:1;
	u32 reserved1:31;
	u32 acpi_uid;
};

extern u8 initacpi();
extern struct acpi_sdt_header* acpifindsdt(u32 sign);

#endif

#endif
