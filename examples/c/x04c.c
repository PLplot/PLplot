/* $Id$
 * $Log$
 * Revision 1.7  1994/06/30 17:57:14  mjl
 * All C example programs: made another pass to eliminate warnings when using
 * gcc -Wall.  Lots of cleaning up: got rid of includes of math.h or string.h
 * (now included by plplot.h), eliminated redundant casts, put in more
 * uniform comments, and other minor changes.
 *
 * Revision 1.6  1994/03/30  07:21:47  mjl
 * Changes to all C example programs: special handling for malloc re: header
 * files eliminated, include of stdio.h and stdlib.h eliminated (now done
 * by plplot.h), include of "plplot.h" changed to <plplot.h> to enable
 * simpler builds by the general user, some cleaning up also.
*/

/*	x04c.c

	Log plot demo.
*/

#include <plplot.h>

/*----------------------------------------------------------------------*\
 * main
 *
 * Illustration of logarithmic axes, and redefinition of window.
\*----------------------------------------------------------------------*/

int
main(int argc, char *argv[])
{
    int i;
    static PLFLT freql[101], ampl[101], phase[101];
    PLFLT f0, freq;

/* Parse and process command line arguments */

    (void) plParseInternalOpts(&argc, argv, PL_PARSE_FULL);

/* Initialize plplot */

    plinit();

    pladv(0);
    plfont(2);

    f0 = 1000.0;
    for (i = 0; i <= 100; i++) {
	freql[i] = 1.0 + i / 20.0;
	freq = pow(10.0, freql[i]);
	ampl[i] = 20.0 * log10(1.0 / sqrt(1.0 + pow((freq / f0), 2.)));
	phase[i] = -(180.0 / 3.141592654) * atan(freq / f0);
    }

    plvpor(0.15, 0.85, 0.1, 0.9);
    plwind(1.0, 6.0, -80.0, 0.0);
    plcol(1);
    plbox("bclnst", 0.0, 0, "bnstv", 0.0, 0);
    plcol(2);
    plline(101, freql, ampl);
    plcol(1);
    plptex(5.0, -30.0, 1.0, -20.0, 0.5, "-20 dB/decade");

    plwind(1.0, 6.0, -100.0, 0.0);
    plbox("", 0.0, 0, "cmstv", 30.0, 3);
    plcol(3);
    plline(101, freql, phase);

    plcol(1);
    plmtex("b", 3.2, 0.5, 0.5, "Frequency");
    plmtex("t", 2.0, 0.5, 0.5, "Single Pole Low-Pass Filter");
    plcol(2);
    plmtex("l", 5.0, 0.5, 0.5, "Amplitude (dB)");
    plcol(3);
    plmtex("r", 5.0, 0.5, 0.5, "Phase shift (degrees)");

    plend();
    exit(0);
}
