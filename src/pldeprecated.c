/* $Id$

    Copyright (C) 2005  Alan W. Irwin

    This file is part of PLplot.

    PLplot is free software; you can redistribute it and/or modify
    it under the terms of the GNU Library General Public License as published
    by the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    PLplot is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Library General Public License for more details.

    You should have received a copy of the GNU Library General Public License
    along with PLplot; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA

* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

    This file contains deprecated routines to provide backwards compatibility
    for a while.  For each routine the new routine you should be using instead
    is explicitly commented.
*/

#include "plplotP.h"

/*--------------------------------------------------------------------------*\
 * Use plparseopts instead.
\*--------------------------------------------------------------------------*/
int
plParseOpts(int *p_argc, char **argv, PLINT mode)
{
   return c_plparseopts(p_argc, argv, mode);
}


/*--------------------------------------------------------------------------*\
 * Use plhlsrgb instead.
\*--------------------------------------------------------------------------*/
void
plHLS_RGB(PLFLT h, PLFLT l, PLFLT s, PLFLT *p_r, PLFLT *p_g, PLFLT *p_b)
{
   c_plhlsrgb(h, l, s, p_r, p_g, p_b);
}

/*--------------------------------------------------------------------------*\
 * Use plrgbhls instead.
\*--------------------------------------------------------------------------*/
void
plRGB_HLS(PLFLT r, PLFLT g, PLFLT b, PLFLT *p_h, PLFLT *p_l, PLFLT *p_s)
{
   c_plrgbhls(r, g, b, p_h, p_l, p_s);
}
