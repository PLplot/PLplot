/* $Id$
 * $Log$
 * Revision 1.4  1995/06/01 21:40:15  mjl
 * All C demo files: changed file inclusion to use quotes instead of angle
 * brackets so that dependencies are retained during development.
 *
 * Revision 1.3  1995/04/12  08:19:03  mjl
 * Changes to all C demos: now include "plcdemos.h" to get all startup
 * definitions and includes that are useful to share between them.
 *
 * Revision 1.2  1995/03/16  23:19:00  mjl
 * All example C programs: changed plParseInternalOpts() call to plParseOpts().
 *
 * Revision 1.1  1994/07/29  20:14:45  mjl
 * Demo for generating world map backgrounds.
 * Contributed by Wesley Ebisuzaki.
*/

/*	x19c.c

	Illustrates backdrop plotting of world, US maps.
	Contributed by Wesley Ebisuzaki.
*/

#include "plcdemos.h"

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

    plcol(1);
    plenv(minx, maxx, miny, maxy, 1, -1);
    plmap(NULL, "usaglobe", minx, maxx, miny, maxy);

/* The Americas */

    minx = 190;
    maxx = 340;

    plcol(1);
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
