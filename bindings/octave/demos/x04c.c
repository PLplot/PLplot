/* $Id$
 * $Log$
 * Revision 1.1  2001/03/02 23:47:59  jcard
 * Initial import of Octave bindings
 *
 * Revision 1.1.1.1  2000/04/10 17:23:07  qual
 * import  module
 *
 * Revision 1.11  1995/06/01  21:40:06  mjl
 * All C demo files: changed file inclusion to use quotes instead of angle
 * brackets so that dependencies are retained during development.
 *
 * Revision 1.10  1995/04/12  08:18:53  mjl
 * Changes to all C demos: now include "plcdemos.h" to get all startup
 * definitions and includes that are useful to share between them.
 *
 * Revision 1.9  1995/03/16  23:18:50  mjl
 * All example C programs: changed plParseInternalOpts() call to plParseOpts().
 *
 * Revision 1.8  1995/01/06  07:59:17  mjl
 * Added a second plot which illustrates the new options ('f' -- to force
 * fixed point labels even in a log plot, and 'h' -- to print a grid at the
 * subtick intervals) to plbox.
 *
 * Revision 1.7  1994/06/30  17:57:14  mjl
 * All C example programs: made another pass to eliminate warnings when using
 * gcc -Wall.  Lots of cleaning up: got rid of includes of math.h or string.h
 * (now included by plplot.h), eliminated redundant casts, put in more
 * uniform comments, and other minor changes.
*/

/*	x04c.c

	Log plot demo.
*/

#include "plcdemos.h"

void plot1(int type);

/*--------------------------------------------------------------------------*\
 * main
 *
 * Illustration of logarithmic axes, and redefinition of window.
\*--------------------------------------------------------------------------*/

int
main(int argc, char *argv[])
{
/* Parse and process command line arguments */

    (void) plParseOpts(&argc, argv, PL_PARSE_FULL);

/* Initialize plplot */

    plinit();
    plfont(2);

/* Make log plots using two different styles. */

    plot1(0);
    plot1(1);

    plend();
    exit(0);
}

/*--------------------------------------------------------------------------*\
 * plot1
 *
 * Log-linear plot.
\*--------------------------------------------------------------------------*/

void
plot1(int type)
{
    int i;
    static PLFLT freql[101], ampl[101], phase[101];
    PLFLT f0, freq;

    pladv(0);

/* Set up data for log plot */

    f0 = 1.0;
    for (i = 0; i <= 100; i++) {
	freql[i] = -2.0 + i / 20.0;
	freq = pow(10.0, freql[i]);
	ampl[i] = 20.0 * log10(1.0 / sqrt(1.0 + pow((freq / f0), 2.)));
	phase[i] = -(180.0 / 3.141592654) * atan(freq / f0);
    }

    plvpor(0.15, 0.85, 0.1, 0.9);
    plwind(-2.0, 3.0, -80.0, 0.0);

/* Try different axis and labelling styles. */

    plcol(1);
    switch (type) {
    case 0:
	plbox("bclnst", 0.0, 0, "bnstv", 0.0, 0);
	break;
    case 1:
	plbox("bcfghlnst", 0.0, 0, "bcghnstv", 0.0, 0);
	break;
    }

/* Plot ampl vs freq */

    plcol(2);
    plline(101, freql, ampl);
    plcol(1);
    plptex(5.0, -30.0, 1.0, -20.0, 0.5, "-20 dB/decade");

/* Put labels on */

    plcol(1);
    plmtex("b", 3.2, 0.5, 0.5, "Frequency");
    plmtex("t", 2.0, 0.5, 0.5, "Single Pole Low-Pass Filter");
    plcol(2);
    plmtex("l", 5.0, 0.5, 0.5, "Amplitude (dB)");

/* For the gridless case, put phase vs freq on same plot */

    if (type == 0) {
	plcol(1);
	plwind(-2.0, 3.0, -100.0, 0.0);
	plbox("", 0.0, 0, "cmstv", 30.0, 3);
	plcol(3);
	plline(101, freql, phase);
	plcol(3);
	plmtex("r", 5.0, 0.5, 0.5, "Phase shift (degrees)");
    }
}
