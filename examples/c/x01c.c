/* $Id$
 * $Log$
 * Revision 1.8  1994/03/30 07:21:44  mjl
 * Changes to all C example programs: special handling for malloc re: header
 * files eliminated, include of stdio.h and stdlib.h eliminated (now done
 * by plplot.h), include of "plplot.h" changed to <plplot.h> to enable
 * simpler builds by the general user, some cleaning up also.
 *
 * Revision 1.7  1993/07/02  07:04:40  mjl
 * Eliminated plrgb calls (to be deprecated in 5.0).
 *
 * Revision 1.6  1993/03/02  18:58:46  mjl
 * Inserted a retrieve & display of the plplot library version as a
 * demonstration.
*/

/* Demonstration program for PLPLOT: */
/* Plots several simple functions */

#include <plplot.h>
#include <math.h>

static PLFLT xs[6] =
{1.0, 2.0, 3.0, 4.0, 5.0, 6.0};
static PLFLT ys[6] =
{1.0, 4.0, 9.0, 16.0, 25.0, 36.0};
static PLFLT x[101], y[101];
static PLFLT xscale, yscale, xoff, yoff, xs1[6], ys1[6];
static PLINT space0 = 0, mark0 = 0, space1 = 1500, mark1 = 1500;

void plot1(void);
void plot2(void);
void plot3(void);

int
main(int argc, char *argv[])
{
    PLINT digmax;
    char ver[80];

/* plplot initialization */
/* Divide page into 2x2 plots unless user overrides */

    plssub(2, 2);

/* Parse and process command line arguments */

    (void) plParseInternalOpts(&argc, argv, PL_PARSE_FULL);

/* Get version number, just for kicks */

    plgver(ver);
    printf("Plplot library version: %s\n", ver);

/* Initialize plplot */

    plinit();

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
    exit(0);
}

 /* =============================================================== */

void
plot1(void)
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

/* Set up the viewport and window using PLENV. The range in X is 
 * 0.0 to 6.0, and the range in Y is 0.0 to 30.0. The axes are 
 * scaled separately (just = 0), and we just draw a labelled 
 * box (axis = 0). 
 */
    plcol(1);
    plenv(xmin, xmax, ymin, ymax, 0, 0);
    plcol(2);
    pllab("(x)", "(y)", "#frPLPLOT Example 1 - y=x#u2");

/* Plot the data points */

    plcol(4);
    plpoin(6, xs1, ys1, 9);

/* Draw the line through the data */

    plcol(3);
    plline(60, x, y);
}

 /* =============================================================== */

void
plot2(void)
{
    int i;

/* Set up the viewport and window using PLENV. The range in X is -2.0 to
 * 10.0, and the range in Y is -0.4 to 2.0. The axes are scaled separately
 * (just = 0), and we draw a box with axes (axis = 1). 
 */
    plcol(1);
    plenv(-2.0, 10.0, -0.4, 1.2, 0, 1);
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
plot3(void)
{
    int i;

/* For the final graph we wish to override the default tick intervals, and
 * so do not use PLENV 
 */
    pladv(0);

/* Use standard viewport, and define X range from 0 to 360 degrees, Y range
 * from -1.2 to 1.2. 
 */
    plvsta();
    plwind(0.0, 360.0, -1.2, 1.2);

/* Draw a box with ticks spaced 60 degrees apart in X, and 0.2 in Y. */

    plcol(1);
    plbox("bcnst", 60.0, 2, "bcnstv", 0.2, 2);

/* Superimpose a dashed line grid, with 1.5 mm marks and spaces. 
 * plstyl expects a pointer!! 
 */
    plstyl(1, &mark1, &space1);
    plcol(2);
    plbox("g", 30.0, 0, "g", 0.2, 0);
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
