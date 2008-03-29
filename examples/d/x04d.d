/* $Id: 

	Log plot demo.
*/

import plplot;
import std.math;
import std.string;

/*--------------------------------------------------------------------------*\
 * main
 *
 * Illustration of logarithmic axes, and redefinition of window.
\*--------------------------------------------------------------------------*/
int main( char[][] args )
{
  /* Parse and process command line arguments */
  char*[] c_args = new char*[args.length];
  foreach( size_t i, char[] arg; args ) {
    c_args[i] = toStringz(arg);
  }
  int argc = c_args.length;
  plparseopts( &argc, cast(char**)c_args, PL_PARSE_FULL );

  /* Initialize plplot */
  plinit();
  plfont( 2 );

  /* Make log plots using two different styles. */
  plot1( 0 );
  plot1( 1 );

  plend();
  return 0;
}


/*--------------------------------------------------------------------------*\
 * plot1
 *
 * Log-linear plot.
\*--------------------------------------------------------------------------*/
void plot1( int type )
{
  PLFLT[101] freql, ampl, phase;
  PLFLT f0, freq;

  pladv( 0 );

  /* Set up data for log plot */
  f0 = 1.0;
  for( size_t i=0;  i<=100; i++ ) {
    freql[i] = -2.0+i/20.0;
    freq = pow( 10.0, freql[i] );
    ampl[i] = 20.0*log10(1.0/sqrt(1.0+pow((freq/f0), 2.0)));
    phase[i] = -(180.0/PI)*atan(freq/f0);
  }

  plvpor( 0.15, 0.85, 0.1, 0.9 );
  plwind( -2.0, 3.0, -80.0, 0.0 );

  /* Try different axis and labelling styles. */
  plcol0( 1 );
  switch( type ) {
  case 0:
    plbox( "bclnst", 0.0, 0, "bnstv", 0.0, 0 );
    break;
  case 1:
    plbox( "bcfghlnst", 0.0, 0, "bcghnstv", 0.0, 0 );
    break;
  }

  /* Plot ampl vs freq */
  plcol0( 2 );
  plline( 101, cast(PLFLT*)freql, cast(PLFLT*)ampl );
  plcol0( 1 );
  plptex( 1.6, -30.0, 1.0, -20.0, 0.5, "-20 dB/decade" );

  /* Put labels on */
  plcol0( 1 );
  plmtex( "b", 3.2, 0.5, 0.5, "Frequency" );
  plmtex( "t", 2.0, 0.5, 0.5, "Single Pole Low-Pass Filter" );
  plcol0( 2 );
  plmtex( "l", 5.0, 0.5, 0.5, "Amplitude (dB)" );

  /* For the gridless case, put phase vs freq on same plot */
  if( type==0 ) {
    plcol0( 1 );
    plwind( -2.0, 3.0, -100.0, 0.0 );
    plbox( "", 0.0, 0, "cmstv", 30.0, 3 );
    plcol0( 3 );
    plline( 101, cast(PLFLT*)freql, cast(PLFLT*)phase );
    plcol0( 3 );
    plmtex( "r", 5.0, 0.5, 0.5, "Phase shift (degrees)" );
    }
}
