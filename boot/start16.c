#include "memory.h"

u32 segment,segbase32;
u8 gdt_desc[(sizeof(SEG_DESC_LO(0,0))+sizeof(SEG_DESC_HI(0,0,0)))*3];
#define gdt_desc_code (gdt_desc+sizeof(SEG_DESC_LO(0,0))+sizeof(SEG_DESC_HI(0,0,0)))
#define gdt_desc_video (gdt_desc_code+sizeof(SEG_DESC_LO(0,0))+sizeof(SEG_DESC_HI(0,0,0)))
u8 gdt_ptr[sizeof(GDT_PTR_LO(0,0))+sizeof(GDT_PTR_HI(0))];

void* getendaddr()
{
	asm("mov ax,#(_cend+128)");
}

void main16()
{
	segment=(u32)getsegaddr()<<4;
	segbase32=segment+(u32)getendaddr();
	
	*(u32*)gdt_desc_code=SEG_DESC_LO(segbase32,8192);
	*(u32*)(gdt_desc_code+sizeof(SEG_DESC_LO(0,0)))=SEG_DESC_HI(segbase32,8192,SEG_DESC_ATTR_FLAT_C);
	*(u32*)gdt_desc_video=SEG_DESC_LO(0xb800,0xffff);
	*(u32*)(gdt_desc_video+sizeof(SEG_DESC_LO(0,0)))=SEG_DESC_HI(0xb800,0xffff,SEG_DESC_ATTR_FLAT_D);
	
	*(u32*)gdt_ptr=GDT_PTR_LO(segment+(u32)gdt_desc,sizeof(gdt_desc));
	*(u16*)(gdt_ptr+sizeof(GDT_PTR_LO(0,0)))=GDT_PTR_HI(segment+(u32)gdt_desc);
	
	lgdt(gdt_ptr);
	enable_a20();
	
	asm("\
		cli\n\
		mov eax,cr0\n\
		or eax,#1\n\
		mov cr0,eax\n\
		db 0x66\n\
		USE32\n\
		jmpf 0,#8\n\
		USE16\
	");
}
