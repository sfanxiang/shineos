#ifndef STDIO_H
#define STDIO_H

#ifdef __x86_64__

#include "defines.h"
#include "display.h"
#include "memory.h" 
#include "stdarg.h"
#include "string.h"

extern void putchar_unlocked(char chr);
extern void putchar(char chr);
extern void puts(char* str);
extern int vsnprintf(char *str, size_t size, const char *format, va_list ap);
extern int snprintf(char *str, size_t size, const char *format, ...);
extern int vprintf(const char *format, va_list ap);
extern int printf(const char *format, ...);

#endif

#endif
