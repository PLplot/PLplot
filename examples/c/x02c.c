/* Demonstrates multiple windows */
/* $Id$
   $Log$
   Revision 1.4  1993/01/23 06:10:22  mjl
   Instituted exit codes for all example codes.  Also deleted color functions
   no longer supported (plancol).  Enhanced x09c to exploit new contour
   capabilities.

 * Revision 1.3  1992/09/30  18:25:14  furnish
 * Massive cleanup to irradicate garbage code.  Almost everything is now
 * prototyped correctly.  Builds on HPUX, SUNOS (gcc), AIX, and UNICOS.
 *
 * Revision 1.2  1992/09/29  04:45:10  furnish
 * Massive clean up effort to remove support for garbage compilers (K&R).
 *
 * Revision 1.1  1992/05/20  21:32:50  furnish
 * Initial checkin of the whole PLPLOT project.
 *
*/

/* Note the compiler should automatically convert all non-pointer arguments
   to satisfy the prototype, but some have problems with constants. */

#include "plplot.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int
main(void)
{
    int i, j;
    PLFLT vmin, vmax;
    char text[3];

    /* Divide screen into 16 regions */
    plstar(4, 4);
    plschr((PLFLT) 0.0, (PLFLT) 3.5);
    plfont(4);

    for (i = 0; i <= 15; i++) {
	plcol(i);
	sprintf(text, "%d", i);
	pladv(0);
	vmin = 0.1;
	vmax = 0.9;
	for (j = 0; j <= 2; j++) {
	    plwid(j + 1);
	    plvpor(vmin, vmax, vmin, vmax);
	    plwind((PLFLT) 0.0, (PLFLT) 1.0, (PLFLT) 0.0, (PLFLT) 1.0);
	    plbox("bc", (PLFLT) 0.0, 0, "bc", (PLFLT) 0.0, 0);
	    vmin = vmin + 0.1;
	    vmax = vmax - 0.1;
	}
	plwid(1);
	plptex((PLFLT) 0.5, (PLFLT) 0.5, (PLFLT) 1.0, (PLFLT) 0.0, (PLFLT) 0.5, text);
    }

    plend();
    exit(0);
}
