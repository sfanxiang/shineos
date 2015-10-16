#ifndef VMM_H
#define VMM_H

#ifdef __x86_64__

#include "cpu.h"
#include "display.h"
#include "paging.h"
#include "string.h"
#include "pmm.h"

#define GET_PAGE(x) ((((size_t)(x))/4096)*4096)

#define VM_KERNEL ((((*PMM_STACK)[1]*4096LL-1LL+4096LL*512LL+0x8000000000LL)/0x8000000000LL)*0x8000000000LL)
#define VM_KERNEL_CODE (VM_KERNEL+4096*512)
#define VM_USER (VM_KERNEL*2)

struct vminfo{
	size_t kernel_size;
	u64 *pagetable;
};

extern int vm_map(int n,size_t vp,size_t pp);
extern int vm_rawmap(int n,size_t vp,size_t pp);
extern size_t vm_getphypage(int n,size_t vp);
extern int vmm_init(int n);
extern int vmm_init_raw(int n);
extern struct vminfo *getvminfo(int n);
extern int vm_rawmap_2m(int n,size_t vp,size_t pp);
extern int vmm_initphy();

#endif

#endif
