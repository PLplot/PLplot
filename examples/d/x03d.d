/* $Id$

	Polar plot demo.
*/

import std.math;
import std.string;

import plplot;

/*--------------------------------------------------------------------------*\
 * main
 *
 * Generates polar plot, with 1-1 scaling.
\*--------------------------------------------------------------------------*/
int main( char[][] args )
{
  string text;

  PLFLT dtr = PI/180.0;
  PLFLT[361] x0, y0;
  for(size_t i=0; i<x0.length; i++) {
    x0[i] = cos(dtr*i);
    y0[i] = sin(dtr*i);
  }

  /* Parse and process command line arguments */
  plparseopts(args, PL_PARSE_FULL);

  /* Initialize plplot */
  plinit();

  /* Set up viewport and window, but do not draw box */
  plenv(-1.3, 1.3, -1.3, 1.3, 1, -2);

  PLFLT[361] x, y;
  for(size_t i=1; i<=10; i++) {
    for(size_t j=0; j<x.length; j++) {
      x[j] = 0.1*i*x0[j];
	    y[j] = 0.1*i*y0[j];
    }

    /* Draw circles for polar grid */
  	plline(x, y);
  }

  plcol0(2);
  for(size_t i=0; i<=11; i++) {
    PLFLT theta = 30.0*i;
    PLFLT dx = cos(dtr*theta);
    PLFLT dy = sin(dtr*theta);

    /* Draw radial spokes for polar grid */
    pljoin(0.0, 0.0, dx, dy);
    text = format("%.0f", round(theta));

    /* Write labels for angle */
    PLFLT offset;
    if(theta<9.99)
      offset = 0.45;
    else if(theta<99.9)
	    offset = 0.30;
    else
	    offset = 0.15;

    /* Slightly off zero to avoid floating point logic flips at 90 and 270 deg. */
    if(dx>=-0.00001)
	    plptex(dx, dy, dx, dy, -offset, text);
    else
	    plptex(dx, dy, -dx, -dy, 1.+offset, text);
  }

  /* Draw the graph */
  PLFLT r;
  for(size_t i=0; i<x.length; i++) {
    r = sin(dtr*(5*i));
    x[i] = x0[i]*r;
    y[i] = y0[i]*r;
  }
  plcol0(3);
  plline(x, y);

  plcol0(4);
  plmtex("t", 2.0, 0.5, 0.5, "#frPLplot Example 3 - r(#gh)=sin 5#gh");

  /* Close the plot at end */
  plend();
  return 0;
}
