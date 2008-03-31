/* 

	3-d plot demo.

  Copyright (C) 2008  Werner Smekal
  Copyright (C) 2004  Alan W. Irwin
  Copyright (C) 2004  Rafael Laboissiere

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
*/

import plplot;
import std.string;
import std.math;
import std.c.stdlib;

int sombrero;

PLOptionTable[] options = [
  {
    "sombrero",			/* Turns on use of Sombrero function */
    null,
    null,
    &sombrero,
    PL_OPT_BOOL,
    "-sombrero",
    "Use the \"sombrero\" function." },
  {
    null,			/* option */
    null,			/* handler */
    null,			/* client data */
    null,			/* address of variable to set */
    0,				/* mode flag */
    null,			/* short syntax */
    null }			/* long syntax */
];

/*--------------------------------------------------------------------------*\
 * cmap1_init1
 *
 * Initializes color map 1 in HLS space.
 * Basic grayscale variation from half-dark (which makes more interesting
 * looking plot compared to dark) to light.
 * An interesting variation on this:
 *	s[1] = 1.0
\*--------------------------------------------------------------------------*/
void cmap1_init( bool gray )
{
  PLFLT[2] i, h, l, s;

  i[] = [ 0.0, 1.0 ];		/* boundaries */

  if( gray ) {
    h[] = [ 0.0, 0.0 ];		/* hue -- low: red (arbitrary if s=0), high: red (arbitrary if s=0) */
    l[] = [ 0.5, 1.0];		/* lightness -- low: half-dark, high: light */
    s[] = [ 0.0, 0.0 ];		/* minimum saturation */
  } else {
    h[] = [ 240.0, 0.0 ]; /* blue -> green -> yellow -> red */
    l[] = [ 0.6, 0.6 ];
    s[] = [ 0.8, 0.8 ];
  }

  plscmap1n( 256 );
  c_plscmap1l( 0, 2, cast(PLFLT*)i, cast(PLFLT*)h, cast(PLFLT*)l,
               cast(PLFLT*)s, null );
}


/*--------------------------------------------------------------------------*\
 * main
 *
 * Does a series of 3-d plots for a given data set, with different
 * viewing options in each plot.
\*--------------------------------------------------------------------------*/
int main( char[][] args )
{
  const nlevel=10;
  const XPTS=35;		/* Data points in x */
  const YPTS=46;		/* Data points in y */

  PLFLT* x, y;
  PLFLT** z;
  PLFLT xx, yy, r;
  PLFLT zmin, zmax, step;
  PLFLT[nlevel] clevel;
  bool rosen=true;

  PLFLT[] alt = [ 60.0, 20.0 ];
  PLFLT[] az = [ 30.0, 60.0 ];

  char[] title[] = [ "#frPLplot Example 8 - Alt=60, Az=30",
                   "#frPLplot Example 8 - Alt=20, Az=60" ];

  /* Parse and process command line arguments */
  plMergeOpts( cast(PLOptionTable*)options, "x08c options",  null );
  char*[] c_args = new char*[args.length];
  foreach( size_t i, char[] arg; args ) {
    c_args[i] = toStringz(arg);
  }
  int argc = c_args.length;
  plparseopts( &argc, cast(char**)c_args, PL_PARSE_FULL );
  if( sombrero )
    rosen=false;

  /* Initialize plplot */
  plinit();

  /* Allocate data structures */
  x = cast(PLFLT*)std.c.stdlib.calloc( XPTS, PLFLT.sizeof );
  y = cast(PLFLT*)std.c.stdlib.calloc( YPTS, PLFLT.sizeof );

  plAlloc2dGrid( &z, XPTS, YPTS );

  for( int i=0; i<XPTS; i++)  {
    x[i] = (cast(PLFLT)(i-(XPTS/2))/cast(PLFLT)(XPTS/2));
    if( rosen )
      x[i] *=  1.5;
  }

  for( int i=0; i<YPTS; i++ ) {
    y[i] = cast(PLFLT)(i-(YPTS/2))/cast(PLFLT)(YPTS/2);
    if( rosen )
      y[i] += 0.5;
  }

  for( size_t i=0; i<XPTS; i++ ) {
    xx = x[i];
    for( size_t j=0; j<YPTS; j++ ) {
      yy = y[j];
      if( rosen ) {
	      z[i][j] = pow(1.-xx, 2.)+100.*pow(yy-pow(xx, 2.), 2.);
	      /* The log argument may be zero for just the right grid.  */
	      if( z[i][j]>0. )
	        z[i][j] = log(z[i][j]);
	      else
	        z[i][j] = -5.; /* -MAXFLOAT would mess-up up the scale */
      } else {
	      r = sqrt(xx*xx+yy*yy);
	      z[i][j] = exp(-r*r)*cos(2.0*PI*r);
      }
    }
  }

  plMinMax2dGrid( z, XPTS, YPTS, &zmax, &zmin );
  step = (zmax-zmin)/(nlevel+1);
  for( size_t i=0; i<nlevel; i++ )
    clevel[i] = zmin+step+step*i;

  pllightsource( 1.,1.,1. );

  for( size_t k=0; k<2; k++ ) {
    for( size_t ifshade=0; ifshade<4; ifshade++ ) {
      pladv( 0 );
      plvpor( 0.0, 1.0, 0.0, 0.9 );
      plwind( -1.0, 1.0, -0.9, 1.1 );
      plcol0( 3 );
      plmtex( "t", 1.0, 0.5, 0.5, toStringz(title[k]) );
      plcol0( 1 );
      if( rosen )
        plw3d( 1.0, 1.0, 1.0, -1.5, 1.5, -0.5, 1.5, zmin, zmax, alt[k], az[k] );
      else
        plw3d( 1.0, 1.0, 1.0, -1.0, 1.0, -1.0, 1.0, zmin, zmax, alt[k], az[k] );

      plbox3( "bnstu", "x axis", 0.0, 0,
              "bnstu", "y axis", 0.0, 0,
              "bcdmnstuv", "z axis", 0.0, 0 );
      plcol0( 2 );

      switch( ifshade ) {
      case 0:
        /* diffuse light surface plot */
        cmap1_init( 1 );
        plsurf3d( x, y, z, XPTS, YPTS, 0, null, 0 );
        break;
      case 1:
        /* magnitude colored plot */
        cmap1_init( 0 );
        plsurf3d( x, y, z, XPTS, YPTS, MAG_COLOR, null, 0 );
        break;
      case 2:
        /*  magnitude colored plot with faceted squares */
        cmap1_init( 0 );
        plsurf3d( x, y, z, XPTS, YPTS, MAG_COLOR | FACETED, null, 0 );
        break;
      default:
        /* magnitude colored plot with contours */
        cmap1_init( 0 );
        plsurf3d( x, y, z, XPTS, YPTS, MAG_COLOR | SURF_CONT | BASE_CONT,
                  cast(PLFLT*)clevel, nlevel );
        break;
      }
    }
  }

  /* Clean up */
  std.c.stdlib.free( x );
  std.c.stdlib.free( y );
  plFree2dGrid( z, XPTS, YPTS );

  plend();
  return 0;
}
