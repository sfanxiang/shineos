#ifndef STRING_H
#define STRING_H

#ifdef __x86_64__

#include "defines.h"

extern void spinlock_acquire(int *spinlock);
extern int spinlock_acquire_nowait(int *spinlock);
extern void spinlock_release(int *spinlock);

#endif

#endif
