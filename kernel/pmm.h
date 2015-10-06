#ifndef PMM_H
#define PMM_H

#ifdef __x86_64__

#include "defines.h"
#include "early_memory.h"
#include "vmm.h"

#define PMM_STACK ((size_t**)0x7000)

extern void* get_phy_kernel_start();
extern void* get_phy_kernel_end();
extern size_t* pmm_init();
extern void* pmm_alloc();
extern void pmm_free(void *p);

#endif

#endif
