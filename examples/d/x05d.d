/* $Id:

	Histogram demo.
*/

import plplot;
import std.math;
import std.string;

/*--------------------------------------------------------------------------*\
 * main
 *
 * Draws a histogram from sample data.
\*--------------------------------------------------------------------------*/
int main( char[][] args )
{
  const int NPTS=2047;
  
  PLFLT[NPTS] data;
  PLFLT delta;

  /* Parse and process command line arguments */
  char*[] c_args = new char*[args.length];
  foreach( size_t i, char[] arg; args ) {
    c_args[i] = toStringz(arg);
  }
  int argc = c_args.length;
  plparseopts( &argc, cast(char**)c_args, PL_PARSE_FULL );

  /* Initialize plplot */
  plinit();

  /* Fill up data points */

  delta = 2.0 * PI / cast(double)NPTS;
  for( size_t i=0; i<NPTS; i++ )
    data[i] = sin(i*delta);

  plcol0( 1 );
  plhist( NPTS, &data[0], -1.1, 1.1, 44, 0 );
  plcol0( 2 );
  pllab( "#frValue", "#frFrequency",
         "#frPLplot Example 5 - Probability function of Oscillator" );

  plend();
  
  return 0;
}
