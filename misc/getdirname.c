/* Copyright (C) 1992, 1997, 1998, 2000 Free Software Foundation, Inc.
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

#include <unistd.h>
#include <include/sys/stat.h>
#include <stdlib.h>
#include <string.h>

/* Return a malloc'd string containing the current directory name.
   If the environment variable `PWD' is set, and its value is correct,
   that value is used.  */

char *
get_current_dir_name (void)
{
  char *pwd;
  struct stat dotstat, pwdstat;

  pwd = getenv ("PWD");
  if (pwd != NULL
      && stat (".", &dotstat) == 0
      && stat (pwd, &pwdstat) == 0
      && pwdstat.st_dev == dotstat.st_dev
      && pwdstat.st_ino == dotstat.st_ino)
    /* The PWD value is correct.  Use it.  */
    return __strdup (pwd);

  return __getcwd ((char *) NULL, 0);
}
