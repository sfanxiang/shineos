#ifndef MISC_H
#define MISC_H

#ifdef __x86_64

#include "defines.h"
#include "mp.h"

extern u8 checksum(u8* start,u8* end);
extern void message(char *msg);
extern void error(char *msg,u8 halt);

#endif

#endif
