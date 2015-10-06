#include "vmm.h"

struct vminfo vi[255];

void vm_unmap(int n,size_t vp){
	vp=GET_PAGE(vp);
	if(vp<VM_KERNEL)return 0;

	size_t i1=vp/0x8000000000;
	vp-=i1*0x8000000000;
	size_t i2=vp/0x40000000;
	vp-=i2*0x40000000;
	size_t i3=vp/0x200000;
	vp-=i3*0x200000;
	size_t i4=vp/0x1000;

	int i;
	if(vi[n].pagetable!=NULL){
		u64 *p1=(u64*)GET_PAGE(vi[n].pagetable[i1]);
		if(p1!=NULL){
			u64 *p2=(u64*)GET_PAGE(p1[i2]);
			if(p2!=NULL){
				u64 *p3=(u64*)GET_PAGE(p2[i3]);
				if(p3!=NULL){
					p3[i4]=NULL;
					for(i=0;i<4096/8;i++){
						if(GET_PAGE(p3[i])!=NULL){
							invlpaging();
							return;
						}
					}
					p2[i3]=NULL;
					pmm_free(p3);
					for(i=0;i<4096/8;i++){
						if(GET_PAGE(p2[i])!=NULL){
							invlpaging();
							return;
						}
					}
					p1[i2]=NULL;
					invlpaging();
					pmm_free(p2);
				}
			}
		}
	}
}

int vm_map(int n,size_t vp,size_t pp){
	if(!vm_rawmap(n,vp,pp))return 0;
	invlpaging();
	return 1;
}

int vm_rawmap(int n,size_t vp,size_t pp){
	vp=GET_PAGE(vp);
	if(vp<VM_KERNEL)return 0;
	pp=GET_PAGE(pp);

	size_t i1=vp/0x8000000000;
	vp-=i1*0x8000000000;
	size_t i2=vp/0x40000000;
	vp-=i2*0x40000000;
	size_t i3=vp/0x200000;
	vp-=i3*0x200000;
	size_t i4=vp/0x1000;
	
	if(vi[n].pagetable==NULL){
		vi[n].pagetable=pmm_alloc();
		if(vi[n].pagetable==NULL)return 0;
		memset(vi[n].pagetable,0,4096);
	}

	u64 *p=(u64*)GET_PAGE(vi[n].pagetable[i1]),*q;
	if(p==NULL){
		p=vi[n].pagetable[i1]=pmm_alloc();
		if(p==NULL)return 0;
		vi[n].pagetable[i1]|=3;
		memset(p,0,4096);
	}

	if((u64*)p[i2]==NULL){
		q=p[i2]=pmm_alloc();
		if(q==NULL)return 0;
		p[i2]|=3;
		p=q;
		memset(p,0,4096);
	}else{
		p=GET_PAGE(p[i2]);
	}

	if((u64*)p[i3]==NULL){
		q=p[i3]=pmm_alloc();
		if(q==NULL)return 0;
		p[i3]|=3;
		p=q;
		memset(p,0,4096);
	}else{
		p=GET_PAGE(p[i3]);
	}

	p[i4]=pp|3;
	return 1;
}

size_t vm_getphypage(int n,size_t vp){
	vp=GET_PAGE(vp);
	if(vp<VM_KERNEL)return vp;
	
	size_t i1=vp/0x8000000000;
	vp-=i1*0x8000000000;
	size_t i2=vp/0x40000000;
	vp-=i2*0x40000000;
	size_t i3=vp/0x200000;
	vp-=i3*0x200000;
	size_t i4=vp/0x1000;

	u64 *p=vi[n].pagetable;
	if(p==NULL){
		return 0;
	}
	p=(u64*)GET_PAGE(p[i1]);
	if(p==NULL){
		return 0;
	}
	p=(u64*)GET_PAGE(p[i2]);
	if(p==NULL){
		return 0;
	}
	p=(u64*)GET_PAGE(p[i3]);
	if(p==NULL){
		return 0;
	}
	p=(u64*)GET_PAGE(p[i4]);

	return (size_t)p;
}

int vmm_init(int n){
	if(!vmm_init_raw(n))return 0;
	setpaging(vi[n].pagetable);
	return 1;
}

int vmm_init_raw(int n){
	size_t p,i;
	for((p=(size_t)get_phy_kernel_start()),(i=0);
		p<(size_t)get_phy_kernel_end();
		(p+=4096),(i+=4096)){
		if(!vm_rawmap(n,VM_KERNEL+i,p)){
			return 0;
		}
	}

	vi[n].pagetable[0]=GET_PAGE(*((u64*)0x50000))|3;

	return 1;
}

struct vminfo *getvminfo(int n){
	return &vi[n];
}
