/* $Id:

	Histogram demo.
*/

import plplot;
import std.math;

/*--------------------------------------------------------------------------*\
 * main
 *
 * Draws a histogram from sample data.
\*--------------------------------------------------------------------------*/
int main( char[][] args )
{
  const int NPTS=2047;
  
  int i;
  PLFLT data[NPTS];
  PLFLT delta;

  /* Parse and process command line arguments */
  char*[] c_args = new char*[args.length];
  foreach( int i, char[] arg; args ) {
     c_args[i] = toStringz(arg);
  }
  int argc = c_args.length;
  plparseopts( &argc, argv, PL_PARSE_FULL );

  /* Initialize plplot */
  plinit();

  /* Fill up data points */

  delta = 2.0 * PI / cast(double)NPTS;
  for( i=0; i<NPTS; i++ )
    data[i] = sin(i*delta);

  plcol0( 1 );
  plhist( NPTS, &data[0], -1.1, 1.1, 44, 0 );
  plcol0( 2 );
  pllab( "#frValue", "#frFrequency",
         "#frPLplot Example 5 - Probability function of Oscillator" );

  plend();
  
  return 0;
}
