/*
  Alpha color values demonstration.

  Copyright (C) 2008 Hazen Babcock


  This file is part of PLplot.
  
  PLplot is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Library Public License as published
  by the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.
  
  PLplot is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU Library General Public License for more details.
  
  You should have received a copy of the GNU Library General Public License
  along with PLplot; if not, write to the Free Software
  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
  
  This example will only really be interesting when used with devices that 
  support or alpha (or transparency) values, such as the cairo device family.
 */


#include "plcdemos.h"
static PLINT red[]   = {  0, 255,   0,   0};
static PLINT green[] = {  0,   0, 255,   0};
static PLINT blue[]  = {  0,   0,   0, 255};
static PLFLT alpha[] = {1.0, 1.0, 1.0, 1.0};

static PLFLT px[] = {0.1, 0.5, 0.5, 0.1};
static PLFLT py[] = {0.1, 0.1, 0.5, 0.5};

int
main(int argc, const char *argv[])
{
  int i, j, icol, r, g, b;
  double a;

  plparseopts (&argc, argv, PL_PARSE_FULL);

  plinit ();

  pladv (0);
  plvpor (0.0, 1.0, 0.0, 1.0);
  plwind (0.0, 1.0, 0.0, 1.0);
  plcol0 (0);
  plbox ("", 1.0, 0, "", 1.0, 0);

  plscmap0n (4);
  plscmap0a (red, green, blue, alpha, 4);

  for (i = 0; i < 9; i++) {
    icol = i%3 + 1;
    plgcol0a (icol, &r, &g, &b, &a);
    plscol0a (icol, r, g, b, 1.0 - (double)i/9.0);
    plcol0 (icol);
    plfill (4, px, py);
    for (j = 0; j < 4; j++){
      px[j] += 0.5/9.0;
      py[j] += 0.5/9.0;
    }
  }

  plend();
  exit(0);
}

