/* Demonstration program for PLPLOT: */
/* $Id$
   $Log$
   Revision 1.2  1992/09/29 04:45:22  furnish
   Massive clean up effort to remove support for garbage compilers (K&R).

 * Revision 1.1  1992/05/20  21:33:03  furnish
 * Initial checkin of the whole PLPLOT project.
 *
*/


/* Plots several simple functions */
/* Based on several example programs */
/* This version sends the output of the first 4 plots (one page) to two
   independent streams.
   The X driver is chosen since that makes for a rather nice display,
   although the offset flags do not yet work correctly. */
/* Note the compiler should automatically convert all non-pointer arguments
   to satisfy the prototype, but some have problems with constants. */

#include "plplot.h"
#include <stdio.h>
#include <math.h>

static FLOAT xs[6] = {1.0, 2.0, 3.0, 4.0, 5.0, 6.0};
static FLOAT ys[6] = {1.0, 4.0, 9.0, 16.0, 25.0, 36.0};
static FLOAT x[101], y[101];
static FLOAT xscale, yscale, xoff, yoff, xs1[6], ys1[6];
static INT space0 = 0, mark0 = 0, space1 = 1500, mark1 = 1500;

void plot1();
void plot2();
void plot3();
void plot4();
void plot5();

int 
main (void)
{
    int i, digmax;
    int xleng0=0, yleng0=0, xoff0=0, yoff0=0;
    int xleng1=0, yleng1=0, xoff1=0, yoff1=0;

    printf("Demo for illustrating multiple output streams via X.\n");
    printf("Running with plplot_1 window as slave.\n");
    printf("\n");

/* Query user for parameters. */

    printf("Enter parameters for window 0\n");
    xleng0 = plGetInt("Enter x length: ");
    yleng0 = plGetInt("Enter y length: ");

/* Skip these since they don't work, don't ask me why !*/
/*
    xoff0  = plGetInt("Enter x offset: ");
    yoff0  = plGetInt("Enter y offset: ");
*/
    printf("\nPlease move first window out of way before proceeding.\n\n");

/* Select X driver */

    plspage(0., 0., xleng0, yleng0, xoff0, yoff0);
    plstart("xwin", 2, 2);

/* Start next stream */

    plsstrm(1);

    printf("Enter parameters for window 1\n");
    xleng1 = plGetInt("Enter x length: ");
    yleng1 = plGetInt("Enter y length: ");

/* Skip these since they don't work, don't ask me why !*/
/*
    xoff1  = plGetInt("Enter x offset: ");
    yoff1  = plGetInt("Enter y offset: ");
*/

/* Select X driver */
/* Turn off pause to make this a slave (must follow master) */

    plspage(0., 0., xleng1, yleng1, xoff1, yoff1);
    plspause(0);
    plstart("xwin", 1, 1);
    plsstrm(0);

/* Set up the data & plot */
/* Original case */

    xscale = 6.;
    yscale = 1.;
    xoff = 0.;
    yoff = 0.;
    plot1();

/* Set up the data & plot */

    xscale = 1.;
    yscale = 1.e+6;
    plot1();

/* Set up the data & plot */

    xscale = 1.;
    yscale = 1.e-6;
    digmax = 2;
    plsyax(digmax, 0);
    plot1();

/* Set up the data & plot */

    xscale = 1.;
    yscale = 0.0014;
    yoff = 0.0185;
    digmax = 5;
    plsyax(digmax, 0);
    plot1();

/* To slave */

    plsstrm(1);
    plot4();

/* Back to master */

    plsstrm(0);
    plot2();
    plot3();

/* To slave */

    plsstrm(1);
    plot5();

/* Don't forget to call PLEND to finish off! */

    plend();
}

 /* =============================================================== */

void 
plot1 (void)
{
    int i;
    FLOAT xmin, xmax, ymin, ymax;

    for (i = 0; i < 60; i++) {
	x[i] = xoff + xscale * (i + 1) / 60.0;
	y[i] = yoff + yscale * pow(x[i], 2.);
    }

    xmin = x[0];
    xmax = x[59];
    ymin = y[0];
    ymax = y[59];

    for (i = 0; i < 6; i++) {
	xs1[i] = x[i * 10 + 3];
	ys1[i] = y[i * 10 + 3];
    }

/* Set up the viewport and window using PLENV. The range in X is */
/* 0.0 to 6.0, and the range in Y is 0.0 to 30.0. The axes are */
/* scaled separately (just = 0), and we just draw a labelled */
/* box (axis = 0). */

    plrgb((PLFLT) 0.0, (PLFLT) 0.0, (PLFLT) 1.0);
    plenv(xmin, xmax, ymin, ymax, 0, 0);
    plrgb((PLFLT) 1.0, (PLFLT) 0.0, (PLFLT) 0.0);
    pllab("(x)", "(y)", "#frPLPLOT Example 1 - y=x#u2");

/* Plot the data points */

    plrgb((PLFLT) 0.0, (PLFLT) 1.0, (PLFLT) 0.0);
    plpoin(6, xs1, ys1, 9);

/* Draw the line through the data */

    plrgb((PLFLT) 0.5, (PLFLT) 0.5, (PLFLT) 0.5);
    plline(60, x, y);
}


 /* =============================================================== */

void 
plot2 (void)
{
    int i;

/* Set up the viewport and window using PLENV. The range in X is -2.0 to
       10.0, and the range in Y is -0.4 to 2.0. The axes are scaled separately
       (just = 0), and we draw a box with axes (axis = 1). */

    plcol(1);
    plenv((PLFLT) -2.0, (PLFLT) 10.0, (PLFLT) -0.4, (PLFLT) 1.2, 0, 1);
    plcol(2);
    pllab("(x)", "sin(x)/x", "#frPLPLOT Example 1 - Sinc Function");

/* Fill up the arrays */

    for (i = 0; i < 100; i++) {
	x[i] = (i - 19.0) / 6.0;
	y[i] = 1.0;
	if (x[i] != 0.0)
	    y[i] = sin(x[i]) / x[i];
    }

/* Draw the line */

    plcol(3);
    plline(100, x, y);

}

 /* =============================================================== */

void 
plot3 (void)
{
    int i;

/* For the final graph we wish to override the default tick intervals, and
       so do not use PLENV */

    pladv(0);

/* Use standard viewport, and define X range from 0 to 360 degrees, Y range
       from -1.2 to 1.2. */

    plvsta();
    plwind((PLFLT) 0.0, (PLFLT) 360.0, (PLFLT) -1.2, (PLFLT) 1.2);

    /* Draw a box with ticks spaced 60 degrees apart in X, and 0.2 in Y. */

    plcol(1);
    plbox("bcnst", (PLFLT) 60.0, 2, "bcnstv", (PLFLT) 0.2, 2);

    /* Superimpose a dashed line grid, with 1.5 mm marks and spaces. plstyl
       expects a pointer!! */

    plstyl(1, &mark1, &space1);
    plcol(2);
    plbox("g", (PLFLT) 30.0, 0, "g", (PLFLT) 0.2, 0);
    plstyl(0, &mark0, &space0);

    plcol(3);
    pllab("Angle (degrees)", "sine", "#frPLPLOT Example 1 - Sine function");

    for (i = 0; i < 101; i++) {
	x[i] = 3.6 * i;
	y[i] = sin(x[i] * 3.141592654 / 180.0);
    }

    plcol(4);
    plline(101, x, y);
}

 /* =============================================================== */

void 
plot4 (void)
{
    int i, j;
    FLOAT dtr, theta, dx, dy, r;
    char text[3];
    FLOAT x0[361], y0[361];
    FLOAT x[361], y[361];

    dtr = 3.141592654 / 180.0;
    for (i = 0; i <= 360; i++) {
	x0[i] = cos(dtr * i);
	y0[i] = sin(dtr * i);
    }

/* Set up viewport and window, but do not draw box */

    plenv((PLFLT) -1.3, (PLFLT) 1.3, (PLFLT) -1.3, (PLFLT) 1.3, 1, -2);
    for (i = 1; i <= 10; i++) {
	for (j = 0; j <= 360; j++) {
	    x[j] = 0.1 * i * x0[j];
	    y[j] = 0.1 * i * y0[j];
	}

/* Draw circles for polar grid */

	plline(361, x, y);
    }

    plcol(2);
    for (i = 0; i <= 11; i++) {
	theta = 30.0 * i;
	dx = cos(dtr * theta);
	dy = sin(dtr * theta);

/* Draw radial spokes for polar grid */

	pljoin((PLFLT) 0.0, (PLFLT) 0.0, dx, dy);
	sprintf(text, "%d", ROUND(theta));

/* Write labels for angle */

	if (dx >= 0)
	    plptex(dx, dy, dx, dy, (PLFLT) -0.15, text);
	else
	    plptex(dx, dy, -dx, -dy, (PLFLT) 1.15, text);
    }

/* Draw the graph */

    for (i = 0; i <= 360; i++) {
	r = sin(dtr * (5 * i));
	x[i] = x0[i] * r;
	y[i] = y0[i] * r;
    }
    plcol(3);
    plline(361, x, y);

    plcol(4);
    plmtex("t", (PLFLT) 2.0, (PLFLT) 0.5, (PLFLT) 0.5, 
	   "#frPLPLOT Example 3 - r(#gh)=sin 5#gh");
}

 /* =============================================================== */

/* Demonstration of contour plotting */

#define XPTS      35
#define YPTS      46
#define XSPA      2./(XPTS-1)
#define YSPA      2./(YPTS-1)

FLOAT tr[6] = {XSPA, 0.0, -1.0, 0.0, YSPA, -1.0};

#ifdef PLSTDC
void 
xform(FLOAT x, FLOAT y, FLOAT * tx, FLOAT * ty)
#else
void 
xform(x, y, tx, ty)
FLOAT x, y, *tx, *ty;
#endif
{
    *tx = tr[0] * x + tr[1] * y + tr[2];
    *ty = tr[3] * x + tr[4] * y + tr[5];
}

static FLOAT clevel[11] = {-1., -.8, -.6, -.4, -.2, 0, .2, .4, .6, .8, 1.};

void 
plot5 (void)
{
    int i, j;
    FLOAT xx, yy;
    FLOAT **z, **w;
    static INT mark = 1500, space = 1500;
    char *malloc();

    z = (FLOAT **) malloc(XPTS * sizeof(FLOAT *));
    w = (FLOAT **) malloc(XPTS * sizeof(FLOAT *));
    for (i = 0; i < XPTS; i++) {
	z[i] = (FLOAT *) malloc(YPTS * sizeof(FLOAT));
	w[i] = (FLOAT *) malloc(YPTS * sizeof(FLOAT));
	xx = (double) (i - (XPTS / 2)) / (double) (XPTS / 2);
	for (j = 0; j < YPTS; j++) {
	    yy = (double) (j - (YPTS / 2)) / (double) (YPTS / 2) - 1.0;
	    z[i][j] = xx * xx - yy * yy;
	    w[i][j] = 2 * xx * yy;
	}
    }
    plenv((PLFLT) -1.0, (PLFLT) 1.0, (PLFLT) -1.0, (PLFLT) 1.0, 0, 0);
    plcol(2);
    plcont(z, XPTS, YPTS, 1, XPTS, 1, YPTS, clevel, 11, xform);
    plstyl(1, &mark, &space);
    plcol(3);
    plcont(w, XPTS, YPTS, 1, XPTS, 1, YPTS, clevel, 11, xform);
    plcol(1);
    pllab("X Coordinate", "Y Coordinate", "Contour Plots of Saddle Points");
}
