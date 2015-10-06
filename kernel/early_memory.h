#ifndef EARLY_MEMORY_H
#define EARLY_MEMORY_H

#ifdef __x86_64__

#include "defines.h"
#include "mp.h"

#define SEL_CODE 24
#define SEL_DATA 32
#define SEL_CODE_USER 40
#define SEL_DATA_USER 48

#pragma pack(push,1)

struct smap_entry{
	u64 base;
	u64 len;
	u32 type;
	u32 acpi;
};

#pragma pack(pop)

#define SMAP_TABLE ((struct smap_entry*)0x500)
#define SMAP_COUNT (*((s16*)0x1000))

#define SMAP_TYPE_USABLE 1
#define SMAP_TYPE_RESERVED 2
#define SMAP_TYPE_ACPI_REC 3
#define SMAP_TYPE_ACPI_NVS 4
#define SMAP_TYPE_BAD 5

#define EARLY_MEMORY_MAT (*((volatile struct mat**)0x7c0a))

#pragma pack(push,1)

struct mat_block{
	void *addr;
	size_t len;
	u64 proc;
	u64 type;
};

struct mat{
	u64 count;
	u64 maxcount;
	size_t memsize;
	struct mat_block block[];
};

#pragma pack(pop)

#define MAT_TYPE_USED 1
#define MAT_TYPE_OTHER 2
#define MAT_TYPE_END 4

extern void* early_malloc_align(size_t size,size_t align);
extern void* early_malloc(size_t size);
extern void* early_calloc_align(size_t num,size_t size,size_t align);
extern void* early_calloc(size_t num,size_t size);
extern void early_free(void* ptr);

#define MEMORY_MAT EARLY_MEMORY_MAT

#include "paging.h"

#endif

#endif
