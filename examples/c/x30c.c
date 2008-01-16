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

static PLFLT pos[] = {0.0, 1.0};
static PLFLT rcoord[] = {1.0, 1.0};
static PLFLT gcoord[] = {0.0, 0.0};
static PLFLT bcoord[] = {0.0, 0.0};
static PLFLT acoord[] = {0.0, 1.0};
static PLBOOL rev[] = {0, 0};

int
main(int argc, const char *argv[])
{
  int i, j, icol, r, g, b;
  PLFLT a;
  PLFLT clevel[101];
  PLFLT **z;

  plparseopts (&argc, argv, PL_PARSE_FULL);

  plinit ();
  plscmap0n (4);
  plscmap0a (red, green, blue, alpha, 4);

  /* page 1 */
  pladv (0);
  plvpor (0.0, 1.0, 0.0, 1.0);
  plwind (0.0, 1.0, 0.0, 1.0);
  plcol0 (0);
  plbox ("", 1.0, 0, "", 1.0, 0);

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

  /* page 2 */
  plAlloc2dGrid(&z, 2, 2);
  z[0][0] = 0.0;
  z[1][0] = 0.0;
  z[0][1] = 1.0;
  z[1][1] = 1.0;

  pladv(0);
  plvpor(0.1, 0.9, 0.1, 0.9);
  plwind(0.0, 1.0, 0.0, 1.0);

  for(i = 0; i < 5; i++){
    px[0] = 0.05 + 0.2 * i;
    px[1] = px[0] + 0.1;
    px[2] = px[1];
    px[3] = px[0];
    icol = i%3 + 1;
    plgcol0a (icol, &r, &g, &b, &a);
    plscol0a (icol, r, g, b, 1.0);
    plcol0 (icol);
    for(j = 0; j < 5; j++){
      py[0] = 0.05 + 0.2 * j;
      py[1] = py[0];
      py[2] = py[0] + 0.1;
      py[3] = py[2];
      plfill(4, px, py);
    }
  }

  plscmap1n(128);
  plscmap1la(1, 2, pos, rcoord, gcoord, bcoord, acoord, rev);

  for(i=0;i<101;i++){
    clevel[i] = 0.01 * (PLFLT)i;
  }
  plshades(z, 2, 2, NULL, 0.0, 1.0, 0.0, 1.0, clevel, 101, 10, -1, 2, plfill, 1, NULL, NULL);

  plend();
  exit(0);
}

