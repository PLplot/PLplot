/* $Id$

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


#include "plc++demos.h"

#ifdef USE_NAMESPACE
using namespace std;
#endif

class x30 {
public:
  x30(int, const char **);

private:
  plstream *pls;

  static PLINT red[];
  static PLINT green[];
  static PLINT blue[];
  static PLFLT alpha[];

  static PLFLT px[];
  static PLFLT py[];

  static PLFLT pos[];
  static PLFLT rcoord[];
  static PLFLT gcoord[];
  static PLFLT bcoord[];
  static PLFLT acoord[];
  static PLBOOL rev[];
};

PLINT x30::red[]   = {  0, 255,   0,   0};
PLINT x30::green[] = {  0,   0, 255,   0};
PLINT x30::blue[]  = {  0,   0,   0, 255};
PLFLT x30::alpha[] = {1.0, 1.0, 1.0, 1.0};

PLFLT x30::px[] = {0.1, 0.5, 0.5, 0.1};
PLFLT x30::py[] = {0.1, 0.1, 0.5, 0.5};

PLFLT x30::pos[] = {0.0, 1.0};
PLFLT x30::rcoord[] = {1.0, 1.0};
PLFLT x30::gcoord[] = {0.0, 0.0};
PLFLT x30::bcoord[] = {0.0, 0.0};
PLFLT x30::acoord[] = {0.0, 1.0};

x30::x30(int argc, const char *argv[])
{
  int i, j;
  PLINT icol, r, g, b;
  PLFLT a;
  PLFLT clevel[101];
  PLFLT **z;

  pls = new plstream;

  pls->parseopts (&argc, argv, PL_PARSE_FULL);

  pls->init ();
  pls->scmap0n (4);
  pls->scmap0a (red, green, blue, alpha, 4);

  /* 
     Page 1:

     This is a series of red, green and blue rectangles overlaid
     on each other with gradually increasing transparency.
  */

  /* Set up the window */
  pls->adv (0);
  pls->vpor (0.0, 1.0, 0.0, 1.0);
  pls->wind (0.0, 1.0, 0.0, 1.0);
  pls->col0 (0);
  pls->box ("", 1.0, 0, "", 1.0, 0);

  /* Draw the boxes */
  for (i = 0; i < 9; i++) {
    icol = i%3 + 1;

    /* Get a color, change its transparency and */
    /* set it as the current color. */
    pls->gcol0a (icol, r, g, b, a);
    pls->scol0a (icol, r, g, b, 1.0 - (double)i/9.0);
    pls->col0 (icol);

    /* Draw the rectangle */
    pls->fill (4, px, py);

    /* Shift the rectangles coordinates */
    for (j = 0; j < 4; j++){
      px[j] += 0.5/9.0;
      py[j] += 0.5/9.0;
    }
  }

  /* 
     Page 2:

     This is a bunch of boxes colored red, green or blue with a single 
     large (red) box of linearly varying transparency overlaid. The
     overlaid box is completely transparent at the bottom and completely
     opaque at the top.
  */

  /* Set up the window */
  pls->adv(0);
  pls->vpor(0.1, 0.9, 0.1, 0.9);
  pls->wind(0.0, 1.0, 0.0, 1.0);

  /* Draw the boxes. There are 25 of them drawn on a 5 x 5 grid. */
  for(i = 0; i < 5; i++){
    /* Set box X position */
    px[0] = 0.05 + 0.2 * i;
    px[1] = px[0] + 0.1;
    px[2] = px[1];
    px[3] = px[0];

    /* We don't want the boxes to be transparent, so since we changed   */
    /* the colors transparencies in the first example we have to change */
    /* the transparencies back to completely opaque. */
    icol = i%3 + 1;
    pls->gcol0a (icol, r, g, b, a);
    pls->scol0a (icol, r, g, b, 1.0);
    pls->col0 (icol);
    for(j = 0; j < 5; j++){
      /* Set box y position and draw the box. */
      py[0] = 0.05 + 0.2 * j;
      py[1] = py[0];
      py[2] = py[0] + 0.1;
      py[3] = py[2];
      pls->fill(4, px, py);
    }
  }

  /* The overlaid box is drawn using plshades with a color map that is */
  /* the same color but has a linearly varying transparency. */

  /* Create the color map with 128 colors and use plscmap1la to initialize */
  /* the color values with a linear varying transparency (or alpha) */
  pls->scmap1n(128);
  pls->scmap1la(true, 2, pos, rcoord, gcoord, bcoord, acoord);

  /* Create a 2 x 2 array that contains the z values (0.0 to 1.0) that will */
  /* used for the shade plot. plshades will use linear interpolation to */
  /* calculate the z values of all the intermediate points in this array. */
  pls->Alloc2dGrid(&z, 2, 2);
  z[0][0] = 0.0;
  z[1][0] = 0.0;
  z[0][1] = 1.0;
  z[1][1] = 1.0;

  /* Set the color levels array. These levels are also between 0.0 and 1.0 */
  for(i=0;i<101;i++){
    clevel[i] = 0.01 * (PLFLT)i;
  }

  /* Draw the shade plot with zmin = 0.0, zmax = 1.0 and x and y coordinate ranges */
  /* such that it fills the entire plotting area. */
  pls->shades(z, 2, 2, NULL, 0.0, 1.0, 0.0, 1.0, clevel, 101, 0, -1, 2, plfill, true, NULL, NULL);

  pls->Free2dGrid(z, 2, 2);

  delete pls;
}

int main( int argc, const char **argv )
{
  x30 *x = new x30( argc, argv );

  delete x;
}
