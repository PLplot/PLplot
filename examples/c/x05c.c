/* $Id$
 * $Log$
 * Revision 1.8  1995/03/16 23:18:51  mjl
 * All example C programs: changed plParseInternalOpts() call to plParseOpts().
 *
 * Revision 1.7  1994/06/30  17:57:15  mjl
 * All C example programs: made another pass to eliminate warnings when using
 * gcc -Wall.  Lots of cleaning up: got rid of includes of math.h or string.h
 * (now included by plplot.h), eliminated redundant casts, put in more
 * uniform comments, and other minor changes.
 *
 * Revision 1.6  1994/03/30  07:21:48  mjl
 * Changes to all C example programs: special handling for malloc re: header
 * files eliminated, include of stdio.h and stdlib.h eliminated (now done
 * by plplot.h), include of "plplot.h" changed to <plplot.h> to enable
 * simpler builds by the general user, some cleaning up also.
*/

/*	x05c.c

	Histogram demo.
*/

#include <plplot.h>

#define NPTS 2047

/*--------------------------------------------------------------------------*\
 * main
 *
 * Draws a histogram from sample data.
\*--------------------------------------------------------------------------*/

int
main(int argc, char *argv[])
{
    int i;
    PLFLT data[NPTS], delta;

/* Parse and process command line arguments */

    (void) plParseOpts(&argc, argv, PL_PARSE_FULL);

/* Initialize plplot */

    plinit();

/* Fill up data points */

    delta = 2.0 * 3.141592654 / (double) NPTS;
    for (i = 0; i < NPTS; i++)
	data[i] = sin(i * delta);

    plcol(1);
    plhist(NPTS, data, -1.1, 1.1, 44, 0);
    plcol(2);
    pllab("#frValue", "#frFrequency",
	  "#frPLplot Example 5 - Probability function of Oscillator");

    plend();
    exit(0);
}
