/*	x18c.c

	3-d line and point plot demo.  Adapted from x08c.c.

  $Id$
  $Log$
  Revision 1.1  1994/07/15 20:38:09  furnish
  Example program to show off 3-d line and point plotting.

*/

#include <plplot.h>

static int opt[] =
{ 1, 0, 1, 0 };

static PLFLT alt[] =
{60.0, 20.0, 60.0, 60.0};

static PLFLT az[] =
{30.0, 60.0, 120.0, 160.0};

static char *title[4] =
{
    "#frPLplot Example 18 - Alt=60, Az=30, Opt=1",
    "#frPLplot Example 18 - Alt=20, Az=60, Opt=2",
    "#frPLplot Example 18 - Alt=60, Az=120, Opt=3",
    "#frPLplot Example 18 - Alt=60, Az=160, Opt=3"
};

/*----------------------------------------------------------------------*\
 * main
 *
 * Does a series of 3-d plots for a given data set, with different
 * viewing options in each plot.
\*----------------------------------------------------------------------*/

#define PTS 200
#define PI 3.1415927

int
main(int argc, char *argv[])
{
    int i, j, k;
    PLFLT *x, *y, *z;
    PLFLT r;

/* Parse and process command line arguments */

    (void) plParseInternalOpts(&argc, argv, PL_PARSE_FULL);

/* Initialize plplot */

    plinit();

    x = (PLFLT *) malloc(PTS * sizeof(PLFLT));
    y = (PLFLT *) malloc(PTS * sizeof(PLFLT));
    z = (PLFLT *) malloc(PTS * sizeof(PLFLT));

/* From the mind of a sick and twisted physicist... */

    for (i = 0; i < PTS; i++) {
	z[i] = -1. + 2. * i / PTS;

/* Pick one ... */

/*	r    = 1. - ( (float) i / (float) PTS ); */
	r    = z[i];

	x[i] = r * cos( 2. * PI * 6. * i / PTS );
	y[i] = r * sin( 2. * PI * 6. * i / PTS );
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
	    pl3line( x, y, z, PTS );
	else
	    pl3poin( x, y, z, PTS );

	plcol(3);
	plmtex("t", 1.0, 0.5, 0.5, title[k]);
    }

    plend();
    exit(0);
}
