/* Interfaces to system-dependent kernel and library entries.
   Copyright (C) 1985, 1986, 1987, 1988, 1993 Free Software Foundation, Inc.

This file is part of GNU Emacs.

GNU Emacs is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 1, or (at your option)
any later version.

GNU Emacs is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with GNU Emacs; see the file COPYING.  If not, write to
the Free Software Foundation, 675 Mass Ave, Cambridge, MA 02139, USA.  */

#include <stdio.h>
#include <descrip.h>
#include <string.h>

char *
VMSgetenv (name)
     char *name;
{
  register char *val;
  static char buf[256];
  static struct dsc$descriptor_s equiv
    = {sizeof (buf), DSC$K_DTYPE_T, DSC$K_CLASS_S, buf};
  static struct dsc$descriptor_s d_name
    = {0, DSC$K_DTYPE_T, DSC$K_CLASS_S, 0};
  short eqlen;
  static char *DECWDPY = "DECW$DISPLAY";
  char *str;

  val = (char *) getenv (name);
  if (val)
    return val;

  d_name.dsc$w_length = strlen (name);
  d_name.dsc$a_pointer = name;
  if (LIB$SYS_TRNLOG (&d_name, &eqlen, &equiv) != 1){
    if(strcmp(name,"DISPLAY") != 0)
      return NULL;
    d_name.dsc$w_length = strlen (DECWDPY);
    d_name.dsc$a_pointer = DECWDPY;
    if (LIB$SYS_TRNLOG (&d_name, &eqlen, &equiv) != 1)
      return NULL;
  }
  str = (char *) malloc (eqlen + 1);
  strncpy (str, buf, eqlen);
  str[eqlen] = '\0';
  /* This is a storage leak, but a pain to fix.  With luck,
     no one will ever notice.  */
  return str;
}
