/* $Id$
 * $Log$
 * Revision 1.6  1994/03/30 07:21:56  mjl
 * Changes to all C example programs: special handling for malloc re: header
 * files eliminated, include of stdio.h and stdlib.h eliminated (now done
 * by plplot.h), include of "plplot.h" changed to <plplot.h> to enable
 * simpler builds by the general user, some cleaning up also.
 *
 * Revision 1.5  1993/02/22  23:16:20  mjl
 * Changed over to new style of initialization using plinit(), and added
 * function to parse plplot command line flags.
 *
 * Revision 1.4  1993/01/23  06:10:33  mjl
 * Instituted exit codes for all example codes.  Also deleted color functions
 * no longer supported (plancol).  Enhanced x09c to exploit new contour
 * capabilities.
*/

/* Demonstration program for PLPLOT: Bar chart example. */

#include <plplot.h>

void
plfbox(PLFLT x0, PLFLT y0)
{
    PLFLT x[4], y[4];

    x[0] = x0;
    y[0] = 0.;
    x[1] = x0;
    y[1] = y0;
    x[2] = x0 + 1.;
    y[2] = y0;
    x[3] = x0 + 1.;
    y[3] = 0.;
    plfill(4, x, y);
    plcol(1);
    pllsty(1);
    plline(4, x, y);
}

int
main(int argc, char *argv[])
{
    int i;
    char string[20];
    PLFLT y0[10];

/* Parse and process command line arguments */

    (void) plParseInternalOpts(&argc, argv, PL_PARSE_FULL);

/* Initialize plplot */

    plinit();

    pladv(0);
    plvsta();
    plwind((PLFLT) 1980., (PLFLT) 1990., (PLFLT) 0., (PLFLT) 35.);
    plbox("bc", (PLFLT) 1., 0, "bcnv", (PLFLT) 10., 0);
    plcol(2);
    pllab("Year", "Widget Sales (millions)", "#frPLPLOT Example 12");

    y0[0] = 5;
    y0[1] = 15;
    y0[2] = 12;
    y0[3] = 24;
    y0[4] = 28;
    y0[5] = 30;
    y0[6] = 20;
    y0[7] = 8;
    y0[8] = 12;
    y0[9] = 3;

    for (i = 0; i < 10; i++) {
/*	plcol(i % 4 + 1);*/
	plcol(i + 1);
/*	plpsty((i + 3) % 8 + 1);*/
/*	plpsty(-(i + 3));*/
	plpsty(0);
/*	pllsty(i % 8 + 1);*/
	plfbox((1980. + i), y0[i]);
	sprintf(string, "%.0f", y0[i]);
	plptex((1980. + i + .5), (y0[i] + 1.), 1., 0.,
	       .5, string);
	sprintf(string, "%d", 1980 + i);
	plmtex("b", 1., ((i + 1) * .1 - .05), .5, string);
    }

    /* Don't forget to call PLEND to finish off! */
    plend();
    exit(0);
}
