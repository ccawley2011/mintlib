/* Low-level functions for atomic operations.  m680x0 version, x >= 2.
   Copyright (C) 1997 Free Software Foundation, Inc.
   Contributed by Andreas Schwab <schwab@issan.informatik.uni-dortmund.de>.

   This file is part of the GNU C Library.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public License as
   published by the Free Software Foundation; either version 2 of the
   License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public
   License along with the GNU C Library; if not, see
   <https://www.gnu.org/licenses/>.  */

/* Modified for MiNTLib by Frank Naumann <fnaumann@freemint.de>.  */

#ifndef _M68020_ATOMICITY_H
#define _M68020_ATOMICITY_H	1

#include <inttypes.h>


static inline int
__attribute__ ((unused))
exchange_and_add (volatile uint32_t *mem, int val)
{
  register int result = *mem;
  register int temp;
  __asm__ __volatile__ ("1: move%.l %0,%1\n\t"
			"add%.l	%2,%1\n\t"
			"cas%.l	%0,%1,%3\n\t"
			"jbne	1b"
			: "=d" (result), "=&d" (temp)
			: "d" (val), "m" (*mem), "0" (result) : "memory");
  return result;
}

static inline void
__attribute__ ((unused))
atomic_add (volatile uint32_t *mem, int val)
{
  /* XXX Use cas here as well?  */
  __asm__ __volatile__ ("add%.l	%0,%1"
			: : "ir" (val), "m" (*mem) : "memory");
}

static inline int
__attribute__ ((unused))
compare_and_swap (volatile long int *p, long int oldval, long int newval)
{
  char ret;
  long int readval;

  __asm__ __volatile__ ("cas%.l	%2,%3,%1\n\t"
			"seq	%0"
                        : "=dm" (ret), "=m" (*p), "=d" (readval)
                        : "d" (newval), "m" (*p), "2" (oldval));
  return ret;
}

#endif /* atomicity.h */
