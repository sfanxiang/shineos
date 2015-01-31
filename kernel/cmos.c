#include "cmos.h"

u8 readcmos(u8 reg)
{
	disable_int();
	int org=inb(0x70);
	outb(0x70,(org&0b10000000)|(reg&0b01111111));
	u8 ret=inb(0x71);
	enable_int();
	return ret;
}

void writecmos(u8 reg,u8 val)
{
	disable_int();
	int org=inb(0x70);
	outb(0x70,(org&0b10000000)|(reg&0b01111111));
	outb(0x71,val);
	enable_int();
}
