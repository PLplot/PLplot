/* $Id$

	Filling and clipping polygons.
*/

import plplot;
import std.string;


/*--------------------------------------------------------------------------*\
 * main
 *
 * Test program for filling polygons and proper clipping
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
  plssub(3, 3);
  plinit();

  static PLFLT[2][9] xextreme = [ [-120.0, 120.0], 
                                  [-120.0, 120.0],
                                  [-120.0, 120.0],
                                  [ -80.0,  80.0],
                                  [-220.0,-120.0], 
                                  [ -20.0,  20.0],
                                  [ -20.0,  20.0],
                                  [ -80.0,  80.0],
                                  [  20.0, 120.0] ];

  static PLFLT[2][9] yextreme = [ [-120.0, 120.0],
                                  [  20.0, 120.0],
                                  [ -20.0, 120.0],
                                  [ -20.0, 120.0],
                                  [-120.0, 120.0],
                                  [-120.0, 120.0],
                                  [ -20.0,  20.0],
                                  [ -80.0,  80.0],
                                  [-120.0, 120.0] ];

  PLFLT[] x0, y0;
  x0.length = 4;
  y0.length = 4;
  for(int j=0; j<4; j++) {
    switch(j) {
    case 0:
      /* Polygon 1: a diamond */
      x0[] = [   0.0, -100.0,   0.0, 100.0 ];
      y0[] = [-100.0,    0.0, 100.0,   0.0 ]; 
      break;
    case 1:
      /* Polygon 1: a diamond - reverse direction */
      x0[] = [ 100.0,   0.0, -100.0,    0.0 ];
      y0[] = [   0.0, 100.0,    0.0, -100.0 ];
      break;
    case 2:
      /* Polygon 2: a square with punctures */
      x0.length = 10;
      y0.length = 10;
      x0[] = [ -100.0, -100.0, 80.0, -100.0, -100.0, -80.0,  0.0,  80.0, 100.0,  100.0 ];
      y0[] = [ -100.0,  -80.0,  0.0,   80.0,  100.0, 100.0, 80.0, 100.0, 100.0, -100.0 ];
      break;
    case 3:
      /* Polygon 2: a square with punctures - reversed direction */
      x0[] = [  100.0, 100.0,  80.0,  0.0, -80.0, -100.0, -100.0, 80.0, -100.0, -100.0 ];
      y0[] = [ -100.0, 100.0, 100.0, 80.0, 100.0,  100.0,   80.0,  0.0,  -80.0, -100.0 ];
      break;
    }

    for(int i=0; i<9 ; i++) {
      pladv(0);
      plvsta();
      plwind(xextreme[i][0], xextreme[i][1], yextreme[i][0], yextreme[i][1]);

      plcol0(2);
      plbox("bc", 1.0, 0, "bcnv", 10.0, 0);
      plcol0(1);
      plpsty(0);
      plfill(x0.length, cast(PLFLT*)x0, cast(PLFLT*)y0);
      plcol0(2);
      pllsty(1);
      plline(y0.length, cast(PLFLT*)x0, cast(PLFLT*)y0);
    }
  }

  /* Don't forget to call plend() to finish off! */
  plend();
  return 0;
}
