/* 

	Polar plot demo.
*/

import plplot;
import std.math;
import std.string;

/*--------------------------------------------------------------------------*\
 * main
 *
 * Generates polar plot, with 1-1 scaling.
\*--------------------------------------------------------------------------*/
int main( char[][] args )
{
  PLFLT dtr, theta, dx, dy, r, offset;
  char[] text;
  PLFLT[361] x0, y0;
  PLFLT[361] x, y;

  dtr = PI/180.0;
  for( size_t i=0; i<=360; i++ ) {
    x0[i] = cos(dtr*i);
    y0[i] = sin(dtr*i);
  }

  /* Parse and process command line arguments */
  char*[] c_args = new char*[args.length];
  foreach( size_t i, char[] arg; args ) {
    c_args[i] = toStringz(arg);
  }
  int argc = c_args.length;
  plparseopts( &argc, cast(char**)c_args, PL_PARSE_FULL );

  /* Initialize plplot */
  plinit();

  /* Set up viewport and window, but do not draw box */
  plenv(-1.3, 1.3, -1.3, 1.3, 1, -2);
  for( size_t i=1; i<=10; i++ ) {
    for( size_t j=0; j<=360; j++ ) {
      x[j] = 0.1*i*x0[j];
	    y[j] = 0.1*i*y0[j];
    }

    /* Draw circles for polar grid */
  	plline( 361, cast(PLFLT*)x, cast(PLFLT*)y );
  }

  plcol0( 2 );
  for( size_t i=0; i<=11; i++ ) {
    theta = 30.0*i;
    dx = cos(dtr*theta);
    dy = sin(dtr*theta);

    /* Draw radial spokes for polar grid */
    pljoin( 0.0, 0.0, dx, dy );
    text = format( "%.0f", round(theta) );

    /* Write labels for angle */

    if (theta < 9.99) {
       	offset = 0.45;
    } else if (theta < 99.9) {
	offset = 0.30;
    } else {
	offset = 0.15;
    }

    /* Slightly off zero to avoid floating point logic flips at 90 and 270 deg. */
    if( dx>=-0.00001 )
	    plptex( dx, dy, dx, dy, -offset, toStringz(text) );
    else
	    plptex(dx, dy, -dx, -dy, 1.+offset, toStringz(text) );
  }

  /* Draw the graph */
  for( size_t i=0; i<=360; i++ ) {
    r = sin(dtr*(5*i));
    x[i] = x0[i]*r;
    y[i] = y0[i]*r;
  }
  plcol0( 3 );
  plline( 361, cast(PLFLT*)x, cast(PLFLT*)y );

  plcol0( 4 );
  plmtex( "t", 2.0, 0.5, 0.5, "#frPLplot Example 3 - r(#gh)=sin 5#gh" );

  /* Close the plot at end */
  plend();
  return 0;
}
