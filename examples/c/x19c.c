/* $Id$

	Illustrates backdrop plotting of world, US maps.
	Contributed by Wesley Ebisuzaki.
*/

#include "plplot/plcdemos.h"

/*--------------------------------------------------------------------------*\
 * mapform
 *
 * Defines our coordinate transformation.
 * x[], y[] are the coordinates to be plotted.
\*--------------------------------------------------------------------------*/

void 
mapform(PLINT n, PLFLT *x, PLFLT *y) 
{
    int i;
    double xp, yp, radius;
    for (i = 0; i < n; i++) {
	radius = 90.0 - y[i];
	xp = radius * cos(x[i] * PI / 180.0);
	yp = radius * sin(x[i] * PI / 180.0);
	x[i] = xp;
	y[i] = yp;
    }	
}

/*--------------------------------------------------------------------------*\
 * main
 *
 * Shows two views of the world map.
\*--------------------------------------------------------------------------*/

void 
main(int argc, char **argv) 
{
    PLFLT minx, maxx, miny, maxy;
    int c;

/* Parse and process command line arguments */

    (void) plParseOpts(&argc, argv, PL_PARSE_FULL);

/* Longitude (x) and latitude (y) */

    miny = -70;
    maxy = 80;

    plinit();

/* Cartesian plots */
/* Most of world */

    minx = 190;
    maxx = 190+360;

    plcol0(1);
    plenv(minx, maxx, miny, maxy, 1, -1);
    plmap(NULL, "usaglobe", minx, maxx, miny, maxy);

/* The Americas */

    minx = 190;
    maxx = 340;

    plcol0(1);
    plenv(minx, maxx, miny, maxy, 1, -1);
    plmap(NULL, "usaglobe", minx, maxx, miny, maxy);

/* Polar, Northern hemisphere */

    minx = 0;
    maxx = 360;

    plenv(-75., 75., -75., 75., 1, -1);
    plmap(mapform,"globe", minx, maxx, miny, maxy);

    pllsty(2);
    plmeridians(mapform,10.0, 10.0, 0.0, 360.0, -10.0, 80.0);
    plend();
}
