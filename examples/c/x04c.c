/* Illustration of logarithmic axes, and redefinition of window */
/* $Id$
   $Log$
   Revision 1.4  1993/01/23 06:10:24  mjl
   Instituted exit codes for all example codes.  Also deleted color functions
   no longer supported (plancol).  Enhanced x09c to exploit new contour
   capabilities.

 * Revision 1.3  1992/09/30  18:25:16  furnish
 * Massive cleanup to irradicate garbage code.  Almost everything is now
 * prototyped correctly.  Builds on HPUX, SUNOS (gcc), AIX, and UNICOS.
 *
 * Revision 1.2  1992/09/29  04:45:12  furnish
 * Massive clean up effort to remove support for garbage compilers (K&R).
 *
 * Revision 1.1  1992/05/20  21:32:52  furnish
 * Initial checkin of the whole PLPLOT project.
 *
*/

/* Note the compiler should automatically convert all non-pointer arguments
   to satisfy the prototype, but some have problems with constants. */

#include "plplot.h"
#include <stdlib.h>
#include <math.h>

int
main(void)
{
    int i;
    static PLFLT freql[101], ampl[101], phase[101];
    PLFLT f0, freq;

    plstar(1, 1);
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
