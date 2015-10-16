#ifndef STDARG_H
#define STDARG_H

#ifdef __x86_64__

/* normal definition of va_list doesn't work 
*  because gcc doesn't put arguments into the stack
*  with optimizations like -O2
*  so we have to use __builtin_va_list */

typedef __builtin_va_list va_list;

#define _INTSIZEOF(n) ( (sizeof(n) + sizeof(int) - 1) & ~(sizeof(int) - 1) )

#define va_start(ap, param) __builtin_va_start(ap, param)
#define va_end(ap)          __builtin_va_end(ap)
#define va_arg(ap, type)    __builtin_va_arg(ap, type)
#define va_copy(dest, src)  __builtin_va_copy(dest, src)

#endif

#endif
