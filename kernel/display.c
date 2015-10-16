#include "display.h"

void scrollscreen(u8 num)
{
	memcpy((void*)0xb8000,(void*)(0xb8000+num*SCREEN_XMAX*2),(SCREEN_XMAX*SCREEN_YMAX-num*SCREEN_XMAX)*2);
	u16 *p;
	for(p=(u16*)(0xb8000+(SCREEN_XMAX*SCREEN_YMAX-num*SCREEN_XMAX)*2);p<(u16*)(0xb8000+(SCREEN_XMAX*SCREEN_YMAX)*2);p++)
		*p=0x720;
}
