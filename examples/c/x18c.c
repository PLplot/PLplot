/* $Id$
 * $Log$
 * Revision 1.8  1995/04/12 08:19:03  mjl
 * Changes to all C demos: now include "plcdemos.h" to get all startup
 * definitions and includes that are useful to share between them.
 *
 * Revision 1.7  1995/03/16  23:18:59  mjl
 * All example C programs: changed plParseInternalOpts() call to plParseOpts().
 *
 * Revision 1.6  1994/08/10  01:13:12  mjl
 * Put in conditional for PI define.
 *
 * Revision 1.5  1994/07/22  16:03:29  furnish
 * "Little Miss Muffet, sat on a tuffet, eating her curds and whey.
 * Along came a spider and sat down beside her, and frightened Miss
 * Muffet away."
 *
 * Revision 1.4  1994/07/21  10:12:10  mjl
 * Changed plpoin3 to use fast point draw (code=-1).
 *
 * Revision 1.3  1994/07/20  06:06:12  mjl
 * Minor changes to make it easier to play with the altitude/azimuth
 * settings.  Changed to the new API calls for the 3d functions.
 *
 * Revision 1.2  1994/07/19  22:14:05  furnish
 * Added new plots for showing hidden surface removal using pl3poly().
 *
 * Revision 1.1  1994/07/15  20:38:09  furnish
 * Example program to show off 3-d line and point plotting.
*/

/*	x18c.c

	3-d line and point plot demo.  Adapted from x08c.c.
*/

#include <plcdemos.h>

static int opt[] = { 1, 0, 1, 0 };
static PLFLT alt[] = {20.0, 35.0, 50.0, 65.0};
static PLFLT az[] = {30.0, 40.0, 50.0, 60.0};

void test_poly(int k);

/*--------------------------------------------------------------------------*\
 * main
 *
 * Does a series of 3-d plots for a given data set, with different
 * viewing options in each plot.
\*--------------------------------------------------------------------------*/

#define NPTS 1000

int
main(int argc, char *argv[])
{
    int i, j, k;
    PLFLT *x, *y, *z;
    PLFLT r;
    char title[80];

/* Parse and process command line arguments */

    (void) plParseOpts(&argc, argv, PL_PARSE_FULL);

/* Initialize plplot */

    plinit();

    for( k=0; k < 4; k++ )
	test_poly(k);

    x = (PLFLT *) malloc(NPTS * sizeof(PLFLT));
    y = (PLFLT *) malloc(NPTS * sizeof(PLFLT));
    z = (PLFLT *) malloc(NPTS * sizeof(PLFLT));

/* From the mind of a sick and twisted physicist... */

    for (i = 0; i < NPTS; i++) {
	z[i] = -1. + 2. * i / NPTS;

/* Pick one ... */

/*	r    = 1. - ( (float) i / (float) NPTS ); */
	r    = z[i];

	x[i] = r * cos( 2. * PI * 6. * i / NPTS );
	y[i] = r * sin( 2. * PI * 6. * i / NPTS );
    }

    for (k = 0; k < 4; k++) {
	pladv(0);
	plvpor(0.0, 1.0, 0.0, 0.9);
	plwind(-1.0, 1.0, -0.9, 1.1);
	plcol(1);
	plw3d(1.0, 1.0, 1.0, -1.0, 1.0, -1.0, 1.0, -1.0, 1.0, alt[k], az[k]);
	plbox3("bnstu", "x axis", 0.0, 0,
	       "bnstu", "y axis", 0.0, 0,
	       "bcdmnstuv", "z axis", 0.0, 0);

	plcol(2);

	if (opt[k])
	    plline3( NPTS, x, y, z );
	else
	    plpoin3( NPTS, x, y, z, -1 );

	plcol(3);
	sprintf(title, "#frPLplot Example 18 - Alt=%.0f, Az=%.0f",
		alt[k], az[k]);
	plmtex("t", 1.0, 0.5, 0.5, title);
    }

    plend();
    exit(0);
}

void test_poly(int k)
{
    float *x, *y, *z;
    float theta, phi;
    int i, j;
    float pi, two_pi;
    int draw[][4] = { { 1, 1, 1, 1 },
		      { 1, 0, 1, 0 },
		      { 0, 1, 0, 1 },
		      { 1, 1, 0, 0 } };

    pi = 3.1415927, two_pi = 2. * pi;

    x = (PLFLT *) malloc(5 * sizeof(PLFLT));
    y = (PLFLT *) malloc(5 * sizeof(PLFLT));
    z = (PLFLT *) malloc(5 * sizeof(PLFLT));

    pladv(0);
    plvpor(0.0, 1.0, 0.0, 0.9);
    plwind(-1.0, 1.0, -0.9, 1.1);
    plcol(1);
    plw3d(1.0, 1.0, 1.0, -1.0, 1.0, -1.0, 1.0, -1.0, 1.0, alt[k], az[k]);
    plbox3("bnstu", "x axis", 0.0, 0,
	   "bnstu", "y axis", 0.0, 0,
	   "bcdmnstuv", "z axis", 0.0, 0);

    plcol(2);

#define THETA(a) (two_pi * (a) /20.)
#define PHI(a)    (pi * (a) / 20.1)

/*
  x = r sin(phi) cos(theta)
  y = r sin(phi) sin(theta)
  z = r cos(phi)
  r = 1 :=)
  */

    for( i=0; i < 20; i++ ) {
	for( j=0; j < 20; j++ ) {
	    x[0] = sin( PHI(j) ) * cos( THETA(i) );
	    y[0] = sin( PHI(j) ) * sin( THETA(i) );
	    z[0] = cos( PHI(j) );
	    
	    x[1] = sin( PHI(j+1) ) * cos( THETA(i) );
	    y[1] = sin( PHI(j+1) ) * sin( THETA(i) );
	    z[1] = cos( PHI(j+1) );
	    
	    x[2] = sin( PHI(j+1) ) * cos( THETA(i+1) );
	    y[2] = sin( PHI(j+1) ) * sin( THETA(i+1) );
	    z[2] = cos( PHI(j+1) );
	    
	    x[3] = sin( PHI(j) ) * cos( THETA(i+1) );
	    y[3] = sin( PHI(j) ) * sin( THETA(i+1) );
	    z[3] = cos( PHI(j) );
	    
	    x[4] = sin( PHI(j) ) * cos( THETA(i) );
	    y[4] = sin( PHI(j) ) * sin( THETA(i) );
	    z[4] = cos( PHI(j) );

	    plpoly3( -5, x, y, z, draw[k] );
	}
    }

    plcol(3);
    plmtex("t", 1.0, 0.5, 0.5, "unit radius sphere" );
}
