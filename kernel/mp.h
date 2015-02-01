#ifndef MP_H
#define MP_H

#ifdef __x86_64__

#include "acpi.h"
#include "apic.h"
#include "cmos.h"
#include "io.h"
#include "misc.h"

extern u32 initmp();

#endif

#endif
