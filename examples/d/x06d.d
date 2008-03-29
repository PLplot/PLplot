/* 

	Font demo.
*/

import plplot;
import std.string;

/*--------------------------------------------------------------------------*\
 * main
 *
 * Displays the entire "plpoin" symbol (font) set.
\*--------------------------------------------------------------------------*/
int main( char[][] args )
{
  char[] text;
  PLFLT x, y;

  /* Parse and process command line arguments */
  char*[] c_args = new char*[args.length];
  foreach( size_t i, char[] arg; args ) {
    c_args[i] = toStringz(arg);
  }
  int argc = c_args.length;
  plparseopts( &argc, cast(char**)c_args, PL_PARSE_FULL );

  /* Initialize plplot */
  plinit();
  pladv( 0 );

  /* Set up viewport and window */
  plcol0( 2 );
  plvpor( 0.1, 1.0, 0.1, 0.9 );
  plwind( 0.0, 1.0, 0.0, 1.3 );

  /* Draw the grid using plbox */
  plbox( "bcg", 0.1, 0, "bcg", 0.1, 0 );

  /* Write the digits below the frame */
  plcol0( 15 );
  for( size_t i=0; i<=9; i++ ) {
    text = format( "%d", i );
    plmtex("b", 1.5, (0.1*i+0.05), 0.5, toStringz(text) );
  }

  size_t k = 0;
  for( size_t i=0; i<=12; i++ ) {
    /* Write the digits to the left of the frame */
    text = format( "%d", 10*i );
    plmtex( "lv", 1.0, (1.0-(2*i+1)/26.0), 1.0, toStringz(text) );
    for( size_t j=0; j<=9; j++ ) {
	    x = 0.1*j+0.05;
	    y = 1.25-0.1*i;

      /* Display the symbols (plpoin expects that x and y are arrays so */
      /* pass pointers) */
	    if( k<128 )
        plpoin( 1, &x, &y, k );
      k = k+1;
    }
  }

  plmtex( "t", 1.5, 0.5, 0.5, "PLplot Example 6 - plpoin symbols" );
  plend();
  return 0;
}
