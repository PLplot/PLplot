/* $Id$
 * $Log$
 * Revision 1.3  1993/12/06 22:38:16  mjl
 * Added #include <stdio.h> to pick up definition of NULL under SunOS.
 *
 * Revision 1.2  1993/08/31  17:57:11  mjl
 * Actually works now.  Makes three plots, shading a single or multiple
 * regions.
 *
 * Revision 1.1  1993/08/18  19:06:44  mjl
 * This file for testing plshade capability.  Just sketched out at this
 * point.
*/

/*	x15c.c

	plshade test functions.

	Maurice LeBrun
	IFS, University of Texas at Austin
	31 Aug 1993
*/

#define PL_NEED_MALLOC
#include "plplot.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/* Utility macros */

#ifndef PI
#define PI	3.1415926535897932384
#endif
#ifndef MAX
#define MAX(a,b)    (((a) > (b)) ? (a) : (b))
#endif
#ifndef MIN
#define MIN(a,b)    (((a) < (b)) ? (a) : (b))
#endif

/* Function prototypes */

void plot1	(void);
void plot2	(void);
void plot3	(void);
void f2mnmx	(PLFLT *, PLINT, PLINT, PLFLT *, PLFLT *);

/* Data to plot */

#define XPTS    35
#define YPTS    46

PLFLT z[XPTS][YPTS], zmin, zmax;

/*----------------------------------------------------------------------*\
* main
*
* Does a variety of shade plots.
\*----------------------------------------------------------------------*/

int
main(int argc, char *argv[])
{
    int i, j;
    PLFLT xx, yy;

/* Parse and process command line arguments */

    (void) plParseInternalOpts(&argc, argv, PL_PARSE_FULL);

/* Initialize plplot */

    plinit();

/* Set up data array */

    for (i = 0; i < XPTS; i++) {
	xx = (double) (i - (XPTS / 2)) / (double) (XPTS / 2);
	for (j = 0; j < YPTS; j++) {
	    yy = (double) (j - (YPTS / 2)) / (double) (YPTS / 2) - 1.0;

	    z[i][j] = xx*xx - yy*yy + (xx - yy)/(xx*xx+yy*yy + 0.1);
	}
    }
    f2mnmx(&z[0][0], XPTS, YPTS, &zmin, &zmax);

    plot1();
    plot2();
    plot3();

    plend();
    exit(0);
}

/*----------------------------------------------------------------------*\
* plot1
*
* Illustrates a single shaded region.
\*----------------------------------------------------------------------*/

void 
plot1(void)
{
    PLFLT shade_min, shade_max;
    PLINT sh_color, sh_width;
    PLINT min_color = 0, min_width = 0, max_color = 0, max_width = 0;

    plcol(1);
    plenv(-1.0, 1.0, -1.0, 1.0, 0, 0);

    shade_min = zmin + (zmax-zmin)*0.4;
    shade_max = zmin + (zmax-zmin)*0.6;
    sh_color = 7;
    sh_width = 2;
    min_color = 9;
    max_color = 2;
    min_width = 2;
    max_width = 2;

/* Plot using identity transform */

    plpsty(8);
    plshade(&z[0][0], XPTS, YPTS, NULL, -1., 1., -1., 1., 
	    NULL, shade_min, shade_max, 
	    sh_color, sh_width, min_color, min_width,
	    max_color, max_width, plfill, 1);

    plcol(2);
    pllab("distance", "altitude", "Bogon flux");
}

/*----------------------------------------------------------------------*\
* plot2
*
* Illustrates multiple adjacent shaded regions.
\*----------------------------------------------------------------------*/

void 
plot2(void)
{
    PLFLT shade_min, shade_max;
    PLINT sh_color, sh_width;
    PLINT min_color = 0, min_width = 0, max_color = 0, max_width = 0;
    int i;

    plcol(1);
    plenv(-1.0, 1.0, -1.0, 1.0, 0, 0);

/* Plot using identity transform */
    
    for (i = 0; i < 10; i++) {
	shade_min = zmin + (zmax - zmin) * i / 10.0;
	shade_max = zmin + (zmax - zmin) * (i +1) / 10.0;
	sh_color = i+6;
	sh_width = 2;
	plpsty(8);

	plshade(&z[0][0], XPTS, YPTS, NULL, -1., 1., -1., 1., 
		NULL, shade_min, shade_max, 
		sh_color, sh_width, min_color, min_width,
		max_color, max_width, plfill, 1);
    }
    plcol(2);
    pllab("distance", "altitude", "Bogon flux");
}

/*----------------------------------------------------------------------*\
* plot3
*
* Same as plot2, but uses different fill patterns for each region.
\*----------------------------------------------------------------------*/

void 
plot3(void)
{
    PLFLT shade_min, shade_max;
    PLINT sh_color, sh_width;
    PLINT min_color = 0, min_width = 0, max_color = 0, max_width = 0;
    int i;

    plcol(1);
    plenv(-1.0, 1.0, -1.0, 1.0, 0, 0);

/* Plot using identity transform */
    
    for (i = 0; i < 10; i++) {
	shade_min = zmin + (zmax - zmin) * i / 10.0;
	shade_max = zmin + (zmax - zmin) * (i +1) / 10.0;
	sh_color = i+6;
	sh_width = 2;
	plpsty((i + 2) % 8 + 1);

	plshade(&z[0][0], XPTS, YPTS, NULL, -1., 1., -1., 1., 
		NULL, shade_min, shade_max, 
		sh_color, sh_width, min_color, min_width,
		max_color, max_width, plfill, 1);
    }
    plcol(2);
    pllab("distance", "altitude", "Bogon flux");
}

/*----------------------------------------------------------------------*\
* f2mnmx
*
* Returns min & max of input 2d array.
\*----------------------------------------------------------------------*/

#define F(a,b) (f[a*ny+b])

void
f2mnmx(PLFLT *f, PLINT nx, PLINT ny, PLFLT *fmin, PLFLT *fmax)
{
    int i, j;

    *fmax = F(0,0);
    *fmin = *fmax;

    for (i = 0; i < nx; i++) {
	for (j = 0; j < ny; j++) {
            *fmax = MAX(*fmax, F(i, j));
            *fmin = MIN(*fmin, F(i, j));
	}
    }
}
