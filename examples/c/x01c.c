/* Demonstration program for PLPLOT: */
/* $Id$
   $Log$
   Revision 1.3  1992/09/30 18:25:13  furnish
   Massive cleanup to irradicate garbage code.  Almost everything is now
   prototyped correctly.  Builds on HPUX, SUNOS (gcc), AIX, and UNICOS.

 * Revision 1.2  1992/09/29  04:45:09  furnish
 * Massive clean up effort to remove support for garbage compilers (K&R).
 *
 * Revision 1.1  1992/05/20  21:32:49  furnish
 * Initial checkin of the whole PLPLOT project.
 *
*/

/* Plots several simple functions */
/* Note the compiler should automatically convert all non-pointer arguments
   to satisfy the prototype, but some have problems with constants. */

#include "plplot.h"
#include <stdio.h>
#include <math.h>

static PLFLT xs[6] = {1.0, 2.0, 3.0, 4.0, 5.0, 6.0};
static PLFLT ys[6] = {1.0, 4.0, 9.0, 16.0, 25.0, 36.0};
static PLFLT x[101], y[101];
static PLFLT xscale, yscale, xoff, yoff, xs1[6], ys1[6];
static PLINT space0 = 0, mark0 = 0, space1 = 1500, mark1 = 1500;

void plot1();
void plot2();
void plot3();

int 
main (void)
{
    PLINT digmax;

    /* Ask user to specify the output device */
    plstar(2, 2);

    /* Set up the data */
    /* Original case */

    xscale = 6.;
    yscale = 1.;
    xoff = 0.;
    yoff = 0.;

    /* Do a plot */

    plot1();

    /* Set up the data */

    xscale = 1.;
    yscale = 0.0014;
    yoff = 0.0185;

    /* Do a plot */

    digmax = 5;
    plsyax(digmax, 0);
    plot1();

    plot2();

    plot3();

    /* Don't forget to call PLEND to finish off! */

    plend();
}

 /* =============================================================== */

void 
plot1 (void)
{
    int i;
    PLFLT xmin, xmax, ymin, ymax;

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

    /* Redefine the colors, just for fun */

    plancol(0, "blue");
    plancol(1, "yellow");
    plancol(2, "green");
    plancol(3, "red");

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
