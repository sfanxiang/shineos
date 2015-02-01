#include "acpi.h"

struct rsdt* rsdt;

struct rsdt* findrsdt()
{
	struct rsdp_desc *rsdp=NULL;
	void* search=((u32)(*((u16*)0x40e)))<<4;
	void* end=search+1024-sizeof(struct rsdp_desc)+1;
	
	for(;search<end;search++)
	{
		struct rsdp_desc *tmp=search;
		if(tmp->sign==ACPI_SIGN_RSDP&&checksum(tmp,tmp+1))
		{
			rsdp=tmp;
			break;
		}
	}

	if(!rsdp)
	{
		end=0x100000-sizeof(struct rsdp_desc)+1;
		for(search=0xe0000;search<end;search++)
		{
			struct rsdp_desc *tmp=search;
			if(tmp->sign==ACPI_SIGN_RSDP&&checksum(tmp,tmp+1))
			{
				rsdp=tmp;
				break;
			}
		}
	}

	if(!rsdp)return NULL;

	struct rsdt *rsdttmp=rsdp->rsdtaddr;
	if((!rsdttmp)
	   ||(rsdttmp->header.sign!=ACPI_SIGN_RSDT)
	   ||(!checksum(rsdttmp,(void*)rsdttmp+rsdttmp->header.len)))
		return NULL;

	rsdt=rsdttmp;
	return rsdt;
}

u8 initacpi()
{
	return (findrsdt()!=NULL);
}

struct acpi_sdt_header* acpifindsdt(u32 sign)
{
	u64 rsdtent=(rsdt->header.len-sizeof(rsdt->header))/sizeof(rsdt->sdtptr[0]);
	u64 i;
	for(i=0;i<rsdtent;i++)
	{
		struct acpi_sdt_header *header=rsdt->sdtptr[i];
		if(header->sign==sign
		   &&checksum(header,(void*)header+header->len))
			return header;
	}
	return NULL;
}
