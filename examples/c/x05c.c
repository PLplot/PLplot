/* Test of drawing a histogram */
/* $Id$
   $Log$
   Revision 1.4  1993/01/23 06:10:25  mjl
   Instituted exit codes for all example codes.  Also deleted color functions
   no longer supported (plancol).  Enhanced x09c to exploit new contour
   capabilities.

 * Revision 1.3  1992/09/30  18:25:17  furnish
 * Massive cleanup to irradicate garbage code.  Almost everything is now
 * prototyped correctly.  Builds on HPUX, SUNOS (gcc), AIX, and UNICOS.
 *
 * Revision 1.2  1992/09/29  04:45:13  furnish
 * Massive clean up effort to remove support for garbage compilers (K&R).
 *
 * Revision 1.1  1992/05/20  21:32:53  furnish
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
    static PLFLT data[2048];

    /* Fill up data points */

    for (i = 0; i < 2048; i++)
	data[i] = sin(0.01 * (i + 1));

    plstar(1, 1);
    plcol(1);
    plhist(2048, data, (PLFLT) -1.1, (PLFLT) 1.1, 44, 0);
    plcol(2);
    pllab("#frValue", "#frFrequency",
	  "#frPLPLOT Example 5 - Probability function of Oscillator");

    plend();
    exit(0);
}
