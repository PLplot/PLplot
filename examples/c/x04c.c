/* $Id$
 * $Log$
 * Revision 1.6  1994/03/30 07:21:47  mjl
 * Changes to all C example programs: special handling for malloc re: header
 * files eliminated, include of stdio.h and stdlib.h eliminated (now done
 * by plplot.h), include of "plplot.h" changed to <plplot.h> to enable
 * simpler builds by the general user, some cleaning up also.
 *
 * Revision 1.5  1993/02/22  23:16:12  mjl
 * Changed over to new style of initialization using plinit(), and added
 * function to parse plplot command line flags.
*/

/* Illustration of logarithmic axes, and redefinition of window */

#include <plplot.h>
#include <math.h>

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

    plvpor((PLFLT) 0.15, (PLFLT) 0.85, (PLFLT) 0.1, (PLFLT) 0.9);
    plwind((PLFLT) 1.0, (PLFLT) 6.0, (PLFLT) -80.0, (PLFLT) 0.0);
    plcol(1);
    plbox("bclnst", (PLFLT) 0.0, 0, "bnstv", (PLFLT) 0.0, 0);
    plcol(2);
    plline(101, freql, ampl);
    plcol(1);
    plptex((PLFLT) 5.0, (PLFLT) -30.0, (PLFLT) 1.0, (PLFLT) -20.0, (PLFLT) 0.5,
	   "-20 dB/decade");
    plwind((PLFLT) 1.0, (PLFLT) 6.0, (PLFLT) -100.0, (PLFLT) 0.0);
    plbox("", (PLFLT) 0.0, 0, "cmstv", (PLFLT) 30.0, 3);
    plcol(3);
    plline(101, freql, phase);

    plcol(1);
    plmtex("b", (PLFLT) 3.2, (PLFLT) 0.5, (PLFLT) 0.5, "Frequency");
    plmtex("t", (PLFLT) 2.0, (PLFLT) 0.5, (PLFLT) 0.5, "Single Pole Low-Pass Filter");
    plcol(2);
    plmtex("l", (PLFLT) 5.0, (PLFLT) 0.5, (PLFLT) 0.5, "Amplitude (dB)");
    plcol(3);
    plmtex("r", (PLFLT) 5.0, (PLFLT) 0.5, (PLFLT) 0.5, "Phase shift (degrees)");

    plend();
    exit(0);
}
