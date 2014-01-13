/* TI COFF information for Texas Instruments TMS320C6X.
   This file customizes the settings in coff/ti.h. 
   
   Copyright 2002, 2003, 2010, 2013 Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 3 of the License, or
   (at your option) any later version.
   
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street - Fifth Floor, Boston,
   MA 02110-1301, USA.  */

#ifndef COFF_TIC6X_H
#define COFF_TIC6X_H

#include "bfd.h"

#define TIC6X_TARGET_ID 	0x0099
/* Octets per byte, as a power of two.  */
#define TI_TARGET_ID 		TIC6X_TARGET_ID
#define OCTETS_PER_BYTE_POWER 	0 
/* Add to howto to get absolute/sect-relative version.  */
#define HOWTO_BANK 		6		
#define TICOFF_TARGET_ARCH 	bfd_arch_tic6x
/* We use COFF2.  */
#define TICOFF_DEFAULT_MAGIC 	TICOFF2MAGIC 

#define COFF_LONG_SECTION_NAMES

static inline
unsigned long TICOFF_TARGET_MACHINE_GET(const unsigned short in_f_flags ATTRIBUTE_UNUSED)
{
    return TICOFF_TARGET_ARCH;
}
#if 0
#define TICOFF_TARGET_MACHINE_GET(FLAGS) (TICOFF_TARGET_ARCH)

#define TICOFF_TARGET_MACHINE_SET(FLAGSP, MACHINE)	\
  do							\
    {							\
      if ((MACHINE) == bfd_mach_tic6x)			\
        *(FLAGSP) |= F_VERS;				\
    }							\
  while (0)
#endif

#include "coff/ti.h"

#endif /* COFF_TIC6X_H */

