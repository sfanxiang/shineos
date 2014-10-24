asm("\
	jmp continue\n\
_thispart:\n\
	db 0\n\
continue:\n\
	mov ax,#0x7c0\n\
	mov ds,ax\n\
	mov ss,ax\n\
	xor sp,sp\n\
	jmpf #_main,0x7c0\
");

#include "memory.h"

u32 segment,segbase32;
u8 gdt_desc[(sizeof(SEG_DESC_LO(0,0))+sizeof(SEG_DESC_HI(0,0,0)))*4];
#define gdt_desc_empty gdt_desc
#define gdt_desc_code (gdt_desc+sizeof(SEG_DESC_LO(0,0))+sizeof(SEG_DESC_HI(0,0,0)))
#define gdt_desc_data (gdt_desc_code+sizeof(SEG_DESC_LO(0,0))+sizeof(SEG_DESC_HI(0,0,0)))
#define gdt_desc_video (gdt_desc_data+sizeof(SEG_DESC_LO(0,0))+sizeof(SEG_DESC_HI(0,0,0)))
u8 gdt_ptr[sizeof(GDT_PTR_LO(0,0))+sizeof(GDT_PTR_HI(0))];

void* getendaddr()
{
	asm("mov ax,#(_cend+128)");
}

void main()
{
	segment=(u32)getsegaddr()<<4;
	segbase32=segment+(u32)getendaddr();
	
	*(u32*)gdt_desc_empty=SEG_DESC_LO(0,0);
	*(u32*)(gdt_desc_empty+sizeof(SEG_DESC_LO(0,0)))=SEG_DESC_HI(0,0,0);
	*(u32*)gdt_desc_code=SEG_DESC_LO(segbase32,0xf);
	*(u32*)(gdt_desc_code+sizeof(SEG_DESC_LO(0,0)))=SEG_DESC_HI(segbase32,0xf,SEG_DESC_ATTR_FLAT_C);
	*(u32*)gdt_desc_data=SEG_DESC_LO(segbase32,0xf);
	*(u32*)(gdt_desc_data+sizeof(SEG_DESC_LO(0,0)))=SEG_DESC_HI(segbase32,0xf,SEG_DESC_ATTR_FLAT_D);
	*(u32*)gdt_desc_video=SEG_DESC_LO(0xb8000,0xf);
	*(u32*)(gdt_desc_video+sizeof(SEG_DESC_LO(0,0)))=SEG_DESC_HI(0xb8000,0xf,SEG_DESC_ATTR_FLAT_D);
	
	*(u32*)gdt_ptr=GDT_PTR_LO(segment+(u32)gdt_desc,sizeof(gdt_desc));
	*(u16*)(gdt_ptr+sizeof(GDT_PTR_LO(0,0)))=GDT_PTR_HI(segment+(u32)gdt_desc);
	
	lgdt(gdt_ptr);
	enable_a20();
	
	asm("\
		mov cl,[_thispart]\n\
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
