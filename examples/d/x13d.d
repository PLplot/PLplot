/* 

	Pie chart demo.
*/

import plplot;
import std.string;
import std.math;

char[] text[] = [ "Maurice",
                  "Geoffrey",
                  "Alan",
                  "Rafael",
                  "Vince" ];

/*--------------------------------------------------------------------------*\
 * main
 *
 * Does a simple pie chart.
\*--------------------------------------------------------------------------*/
int main( char[][] args )
{
  int dthet, theta0, theta1, theta;
  PLFLT just, dx, dy;
  PLFLT[500] x, y;
  PLFLT[5] per = [ 10.0, 32.0, 12.0, 30.0, 16.0 ];

  /* Parse and process command line arguments */
  plparseopts(args, PL_PARSE_FULL);

  /* Initialize plplot */
  plinit();

  plenv( 0., 10., 0., 10., 1, -2 );
  plcol0( 2 );
  /* n.b. all theta quantities scaled by 2*M_PI/500 to be integers to avoid
   * floating point logic problems. */
  theta0 = 0;
  dthet = 1;
  for( size_t i=0; i<=4; i++ ) {
    size_t j = 0;
    x[j] = 5.;
    y[j++] = 5.;
    /* n.b. the theta quantities multiplied by 2*M_PI/500 afterward so
     * in fact per is interpreted as a percentage. */
	  theta1 = cast(int)(theta0+5.0*per[i]);
	  if( i==4 )
	    theta1 = 500;
	  for( theta=theta0; theta<=theta1; theta+=dthet ) {
	    x[j] = 5+3*cos((2.*PI/500.)*theta);
	    y[j++] = 5+3*sin((2.*PI/500.)*theta);
	  }
	  plcol0( i+1 );
	  plpsty( (i+3)%8+1 );
	  plfill( j, cast(PLFLT*)x, cast(PLFLT*)y );
	  plcol0( 1 );
	  plline(x, y);
	  just = (2.*PI/500.)*(theta0 + theta1)/2.;
	  dx = .25*cos(just);
	  dy = .25*sin(just);
	  if( (theta0+theta1)<250 || (theta0+theta1)>750 ) 
	    just = 0.;
	  else 
	    just = 1.;

	  plptex((x[j/2]+dx), (y[j/2]+dy), 1.0, 0.0, just, text[i]);
	  theta0 = theta-dthet;
  }
  plfont( 2 );
  plschr( 0., 1.3 );
  plptex(5.0, 9.0, 1.0, 0.0, 0.5, "Percentage of Sales");

  /* Don't forget to call PLEND to finish off! */
  plend();
  return 0;
}
