/* Demonstrates multiple windows */
/* $Id$
   $Log$
   Revision 1.1  1992/05/20 21:32:50  furnish
   Initial checkin of the whole PLPLOT project.

*/

/* Note the compiler should automatically convert all non-pointer arguments
   to satisfy the prototype, but some have problems with constants. */

#include "plplot.h"
#include <stdio.h>
#include <math.h>

main()
{
    int i, j;
    FLOAT vmin, vmax;
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
}
