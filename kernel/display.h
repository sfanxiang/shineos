#ifndef DISPLAY_H
#define DISPLAY_H

#ifdef __x86_64__

#include "defines.h"
#include "string.h"

#define SCREEN_XMAX 80
#define SCREEN_YMAX 25

extern u16 getcursorpos();
extern void setcursorpos(u16 pos);
extern void writechar(char chr,u16 pos);
extern void scrollscreen(u8 num);
extern void putchar(char chr);
extern void puts(char* str);

#endif

#endif
