/* Test FD* macros.
   Copyright (C) 1997 Free Software Foundation, Inc.
   This file is part of the GNU C Library.
   Contributed by Robert Bihlmeyer <robbe@orcus.priv.at>.

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

#include <stdio.h>
#include <sys/types.h>

int
main (void)
{
  int retval = 0;
  int i;
  fd_set set;

  FD_ZERO (&set);
  for (i=0; i < FD_SETSIZE; ++i)
    {
      printf ("%d =>  check:", i);
      if (FD_ISSET (i, &set) == 0)
	fputs ("ok", stdout);
      else
	{
	  fputs ("nok", stdout);
	  retval = 1;
	}
      fputs (", set", stdout);
      FD_SET (i, &set);
      fputs (", check:", stdout);
      if (FD_ISSET (i, &set))
	fputs ("ok", stdout);
      else
	{
	  fputs ("nok", stdout);
	  retval = 1;
	}
      fputs (", clear", stdout);
      FD_CLR (i, &set);
      fputs (", check:", stdout);
      if (FD_ISSET (i, &set) == 0)
	puts ("ok");
      else
	{
	  puts ("nok");
	  retval = 1;
	}
    }

  return retval;
}
