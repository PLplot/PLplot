/* $Id$
 * $Log$
 * Revision 1.4  1994/03/30 07:22:00  mjl
 * Changes to all C example programs: special handling for malloc re: header
 * files eliminated, include of stdio.h and stdlib.h eliminated (now done
 * by plplot.h), include of "plplot.h" changed to <plplot.h> to enable
 * simpler builds by the general user, some cleaning up also.
 *
 * Revision 1.3  1993/12/06  22:38:16  mjl
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

#include <plplot.h>
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
static void	cmap1_init1(void);
static void	cmap1_init2(void);

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

/* Set up color map 0 */
/*
    plscmap0n(3);
    */
/* Set up color map 1 */

    cmap1_init2();

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

/*    plot1();*/
    plot2();
/*    plot3();*/

    plend();
    exit(0);
}

/*----------------------------------------------------------------------*\
* cmap1_init1
*
* Initializes color map 1 in HLS space.
\*----------------------------------------------------------------------*/

static void
cmap1_init1(void)
{
    PLFLT i[4], h[4], l[4], s[4];

    i[0] = 0;		/* left boundary */
    i[1] = 0.45;	/* just before center */
    i[2] = 0.55;	/* just after center */
    i[3] = 1;		/* right boundary */

    h[0] = 260;		/* hue -- low: blue-violet */
    h[1] = 260;		/* only change as we go over vertex */
    h[2] = 20;		/* hue -- high: red */
    h[3] = 20;		/* keep fixed */

#if 1
    l[0] = 0.5;		/* lightness -- low */
    l[1] = 0.0;		/* lightness -- center */
    l[2] = 0.0;		/* lightness -- center */
    l[3] = 0.5;		/* lightness -- high */
#else
    plscolbg(255,255,255);
    l[0] = 0.5;		/* lightness -- low */
    l[1] = 1.0;		/* lightness -- center */
    l[2] = 1.0;		/* lightness -- center */
    l[3] = 0.5;		/* lightness -- high */
#endif
    s[0] = 1;		/* maximum saturation */
    s[1] = 1;		/* maximum saturation */
    s[2] = 1;		/* maximum saturation */
    s[3] = 1;		/* maximum saturation */

    c_plscmap1l(0, 4, i, h, l, s);
}

/*----------------------------------------------------------------------*\
* cmap1_init2
*
* Initializes color map 1 in HLS space.
\*----------------------------------------------------------------------*/

static void
cmap1_init2(void)
{
    PLFLT i[4], h[4], l[4], s[4];

    i[0] = 0;		/* left boundary */
    i[1] = 0.45;	/* just before center */
    i[2] = 0.55;	/* just after center */
    i[3] = 1;		/* right boundary */

    h[0] = 260;		/* hue -- low: blue-violet */
    h[1] = 260;		/* only change as we go over vertex */
    h[2] = 20;		/* hue -- high: red */
    h[3] = 20;		/* keep fixed */

#if 1
    l[0] = 0.6;		/* lightness -- low */
    l[1] = 0.0;		/* lightness -- center */
    l[2] = 0.0;		/* lightness -- center */
    l[3] = 0.6;		/* lightness -- high */
#else
    plscolbg(255,255,255);
    l[0] = 0.5;		/* lightness -- low */
    l[1] = 1.0;		/* lightness -- center */
    l[2] = 1.0;		/* lightness -- center */
    l[3] = 0.5;		/* lightness -- high */
#endif
    s[0] = 1;		/* saturation -- low */
    s[1] = 0.5;		/* saturation -- center */
    s[2] = 0.5;		/* saturation -- center */
    s[3] = 1;		/* saturation -- high */

    c_plscmap1l(0, 4, i, h, l, s);
}

/*----------------------------------------------------------------------*\
* plot1
*
* Illustrates a single shaded region.
\*----------------------------------------------------------------------*/

void 
plot1(void)
{
    PLFLT shade_min, shade_max, sh_color;
    PLINT sh_cmap = 0, sh_width;
    PLINT min_color = 0, min_width = 0, max_color = 0, max_width = 0;

    pladv(0);
    plvpor(0.1, 0.9, 0.1, 0.9);
    plwind(-1.0, 1.0, -1.0, 1.0);

/* Plot using identity transform */

    shade_min = zmin + (zmax-zmin)*0.4;
    shade_max = zmin + (zmax-zmin)*0.6;
    sh_color = 7;
    sh_width = 2;
    min_color = 9;
    max_color = 2;
    min_width = 2;
    max_width = 2;

    plpsty(8);
    plshade1(&z[0][0], XPTS, YPTS, NULL, -1., 1., -1., 1., 
	     shade_min, shade_max, 
	     sh_cmap, sh_color, sh_width,
	     min_color, min_width, max_color, max_width,
	     plfill, 1, NULL, NULL);

    plcol(1);
    plbox("bcnst", 0.0, 0, "bcnstv", 0.0, 0);
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
    PLFLT shade_min, shade_max, sh_color;
    PLINT sh_cmap = 1, sh_width;
    PLINT min_color = 1, min_width = 0, max_color = 0, max_width = 0;
    int i, nmax = 14;

    pladv(0);
    plvpor(0.1, 0.9, 0.1, 0.9);
    plwind(-1.0, 1.0, -1.0, 1.0);

/* Plot using identity transform */
    
    for (i = 0; i < nmax; i++) {
	shade_min = zmin + (zmax - zmin) * i / (float) nmax;
	shade_max = zmin + (zmax - zmin) * (i +1) / (float) nmax;
	sh_color = i / (float) (nmax-1);
	sh_width = 2;
/*	plpsty(8);*/
	plpsty(0);

	plshade1(&z[0][0], XPTS, YPTS, NULL, -1., 1., -1., 1., 
		 shade_min, shade_max, 
		 sh_cmap, sh_color, sh_width,
		 min_color, min_width, max_color, max_width,
		 plfill, 1, NULL, NULL);
    }

    plcol(1);
    plbox("bcnst", 0.0, 0, "bcnstv", 0.0, 0);
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
    PLFLT shade_min, shade_max, sh_color;
    PLINT sh_cmap = 0, sh_width;
    PLINT min_color = 0, min_width = 0, max_color = 0, max_width = 0;
    int i;

    pladv(0);
    plvpor(0.1, 0.9, 0.1, 0.9);
    plwind(-1.0, 1.0, -1.0, 1.0);

/* Plot using identity transform */
    
    for (i = 0; i < 10; i++) {
	shade_min = zmin + (zmax - zmin) * i / 10.0;
	shade_max = zmin + (zmax - zmin) * (i +1) / 10.0;
	sh_color = i+6;
	sh_width = 2;
	plpsty((i + 2) % 8 + 1);

	plshade1(&z[0][0], XPTS, YPTS, NULL, -1., 1., -1., 1., 
		 shade_min, shade_max, 
		 sh_cmap, sh_color, sh_width,
		 min_color, min_width, max_color, max_width,
		 plfill, 1, NULL, NULL);
    }

    plcol(1);
    plbox("bcnst", 0.0, 0, "bcnstv", 0.0, 0);
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
