/* $Id$
 * $Log$
 * Revision 1.19  2000/07/19 21:12:20  furnish
 * Jumbo patch by Joao Cardoso.  Adds XOR, a polygon-fill light-shading
 * surface plotter, contour labelling, and demo updates to show off these
 * new features.
 *
 * Revision 1.18  1995/10/14 17:15:13  mjl
 * Changed the "Plplot library version" message to go to stderr so that program
 * can be used as the beginning of a pipeline.
 *
 * Revision 1.17  1995/06/01  21:40:51  mjl
 * All C demo files: changed file inclusion to use quotes instead of angle
 * brackets so that dependencies are retained during development.  Fixed bogus
 * options table specification.
 *
 * Revision 1.16  1995/05/06  21:27:34  mjl
 * Updated options table syntax -- PL_OPT_ENABLED no longer used.  Fixed
 * error exit.
 *
 * Revision 1.15  1995/04/12  08:18:51  mjl
 * Changes to all C demos: now include "plcdemos.h" to get all startup
 * definitions and includes that are useful to share between them.
 *
 * Revision 1.14  1995/03/16  23:21:50  mjl
 * All example C programs: changed plParseInternalOpts() call to plParseOpts().
 * Changed locate code to new syntax (using a PLGraphicsIn).  Added a command
 * line option "-locate" to turn on the locate code (off by default).  This
 * also illustrates how easy it is for PLplot to parse user command line flags.
 *
 * Revision 1.13  1995/01/06  07:58:20  mjl
 * Switched to the new syntax for the window structure variables and
 * shortened the info printed out by the get cursor loop.
 *
 * Revision 1.12  1994/11/02  21:14:33  mjl
 * Name changes to locator coordinate variables.
 *
 * Revision 1.11  1994/08/05  22:24:05  mjl
 * Made new test code a bit more robust and unobtrusive.
 *
 * Revision 1.10  1994/07/29  20:13:02  mjl
 * Added cursor-reporting test code at the end of the page.
 * Contributed by Paul Casteels.
 *
 * Revision 1.9  1994/06/30  17:57:10  mjl
 * All C example programs: made another pass to eliminate warnings when using
 * gcc -Wall.  Lots of cleaning up: got rid of includes of math.h or string.h
 * (now included by plplot.h), eliminated redundant casts, put in more
 * uniform comments, and other minor changes.
*/

/*	x01c.c

	Simple line plot and multiple windows demo.
*/

#include "plcdemos.h"
#include "plevent.h"

/* Variables and data arrays used by plot generators */

static PLFLT x[101], y[101];
static PLFLT xscale, yscale, xoff, yoff, xs[6], ys[6];
static PLGraphicsIn gin;

static int locate_mode;

/* Options data structure definition. */

static PLOptionTable options[] = {
{
    "locate",			/* Turns on test of API locate function */
    NULL,
    NULL,
    &locate_mode,
    PL_OPT_BOOL,
    "-locate",
    "Turns on test of API locate function" },
{
    NULL,			/* option */
    NULL,			/* handler */
    NULL,			/* client data */
    NULL,			/* address of variable to set */
    0,				/* mode flag */
    NULL,			/* short syntax */
    NULL }			/* long syntax */
};

char *notes[] = {"Make sure you get it right!", NULL};

/* Function prototypes */

void plot1(void);
void plot11(void);
void plot2(void);
void plot3(void);

/*--------------------------------------------------------------------------*\
 * main
 *
 * Generates several simple line plots.  Demonstrates:
 *   - subwindow capability
 *   - setting up the window, drawing plot, and labelling
 *   - changing the color
 *   - automatic axis rescaling to exponential notation
 *   - placing the axes in the middle of the box
 *   - gridded coordinate axes
\*--------------------------------------------------------------------------*/

int
main(int argc, char *argv[])
{
    PLINT digmax;
    char ver[80];

/* plplot initialization */
/* Divide page into 2x2 plots unless user overrides */

    plssub(2, 2);

/* Parse and process command line arguments */

    plMergeOpts(options, "x01c options", notes);
    plParseOpts(&argc, argv, PL_PARSE_FULL);

/* Get version number, just for kicks */

    plgver(ver);
    fprintf(stderr, "Plplot library version: %s\n", ver);

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

    plot11();

    plot2();

    plot3();

/* Let's get some user input */

    if (locate_mode) {
	for (;;) {
	    if (! plGetCursor(&gin)) break;
	    if (gin.keysym == PLK_Escape) break;

	    pltext();
	    if (gin.keysym < 0xFF && isprint(gin.keysym)) 
		printf("wx = %f,  wy = %f, dx = %f,  dy = %f,  c = '%c'\n",
		       gin.wX, gin.wY, gin.dX, gin.dY, gin.keysym);
	    else
		printf("wx = %f,  wy = %f, dx = %f,  dy = %f,  c = 0x%02x\n",
		       gin.wX, gin.wY, gin.dX, gin.dY, gin.keysym);

	    plgra();
	}
    }

/* Don't forget to call plend() to finish off! */

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
	xs[i] = x[i * 10 + 3];
	ys[i] = y[i * 10 + 3];
    }

/* Set up the viewport and window using PLENV. The range in X is 
 * 0.0 to 6.0, and the range in Y is 0.0 to 30.0. The axes are 
 * scaled separately (just = 0), and we just draw a labelled 
 * box (axis = 0). 
 */
    plcol(1);
    plenv(xmin, xmax, ymin, ymax, 0, 0);
    plcol(2);
    pllab("(x)", "(y)", "#frPLplot Example 1 - y=x#u2");

/* Plot the data points */

    plcol(4);
    plpoin(6, xs, ys, 9);

/* Draw the line through the data */

    plcol(3);
    plline(60, x, y);
}
 
void
plot11(void)
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
	xs[i] = x[i * 10 + 3];
	ys[i] = y[i * 10 + 3];
    }

/* Set up the viewport and window using PLENV. The range in X is 
 * 0.0 to 6.0, and the range in Y is 0.0 to 30.0. The axes are 
 * scaled separately (just = 0), and we just draw a labelled 
 * box (axis = 0). 
 */
    plcol(1);
    plenv(xmin, xmax, ymin, ymax, 0, 0);
    plcol(2);
    pllab("(x)", "(y)", "#frPLplot Example 1 - y=x#u2");

/* Plot the data points */

    plcol(4);
    plpoin(6, xs, ys, 9);

/* Draw the line through the data */

    plcol(3);
    plline(60, x, y);

    /* xor mode enable erasing a line/point/text by reploting it again */
    /* it does not work in double buffering mode, however */
    plxormod(1);  /* enter xor mode */
    for (i=0; i<60; i++) {
      plpoin(1, x+i, y+i,9);/* draw a point */
      usleep(50000); /* wait a little */
      plflush();/* this is here to force an update of the tk driver */
      plpoin(1, x+i, y+i,9);/* erase point */
    }
    plxormod(0); /* leave xor mode */
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
    pllab("(x)", "sin(x)/x", "#frPLplot Example 1 - Sinc Function");

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
    PLINT space0 = 0, mark0 = 0, space1 = 1500, mark1 = 1500;
    int i;

/* For the final graph we wish to override the default tick intervals, and
 * so do not use plenv().
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
 * plstyl expects a pointer!
*/
    plstyl(1, &mark1, &space1);
    plcol(2);
    plbox("g", 30.0, 0, "g", 0.2, 0);
    plstyl(0, &mark0, &space0);

    plcol(3);
    pllab("Angle (degrees)", "sine", "#frPLplot Example 1 - Sine function");

    for (i = 0; i < 101; i++) {
	x[i] = 3.6 * i;
	y[i] = sin(x[i] * 3.141592654 / 180.0);
    }

    plcol(4);
    plline(101, x, y);
}
