/* Test of drawing a histogram */
/* $Id$
   $Log$
   Revision 1.2  1992/09/29 04:45:13  furnish
   Massive clean up effort to remove support for garbage compilers (K&R).

 * Revision 1.1  1992/05/20  21:32:53  furnish
 * Initial checkin of the whole PLPLOT project.
 *
*/

/* Note the compiler should automatically convert all non-pointer arguments
   to satisfy the prototype, but some have problems with constants. */

#include "plplot.h"
#include <math.h>

int 
main (void)
{
    int i;
    static FLOAT data[2048];

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
}
