/* $Id$
 * $Log$
 * Revision 1.6  1994/03/30 07:21:48  mjl
 * Changes to all C example programs: special handling for malloc re: header
 * files eliminated, include of stdio.h and stdlib.h eliminated (now done
 * by plplot.h), include of "plplot.h" changed to <plplot.h> to enable
 * simpler builds by the general user, some cleaning up also.
 *
 * Revision 1.5  1993/02/22  23:16:13  mjl
 * Changed over to new style of initialization using plinit(), and added
 * function to parse plplot command line flags.
*/

/* Test of drawing a histogram */

#include <plplot.h>
#include <math.h>

int
main(int argc, char *argv[])
{
    int i;
    static PLFLT data[2048];

/* Parse and process command line arguments */

    (void) plParseInternalOpts(&argc, argv, PL_PARSE_FULL);

/* Initialize plplot */

    plinit();

/* Fill up data points */

    for (i = 0; i < 2048; i++)
	data[i] = sin(0.01 * (i + 1));

    plcol(1);
    plhist(2048, data, (PLFLT) -1.1, (PLFLT) 1.1, 44, 0);
    plcol(2);
    pllab("#frValue", "#frFrequency",
	  "#frPLPLOT Example 5 - Probability function of Oscillator");

    plend();
    exit(0);
}
