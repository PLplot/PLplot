/* $Id$
   $Log$
   Revision 1.1  1992/05/20 21:34:43  furnish
   Initial checkin of the whole PLPLOT project.

*/

/*	plpat.c

	Set line width.
*/

#include "plplot.h"

void 
c_plpat(nlin, inc, del)
PLINT nlin, *inc, *del;
{
    PLINT i, level;

    glev(&level);
    if (level < 1)
	plexit("Please call plstar before calling plpat.");
    if (nlin < 1 || nlin > 2)
	plexit("Only 1 or 2 line styles allowed in plpat.");

    for (i = 0; i < nlin; i++) {
	if (del[i] < 0)
	    plexit("Line spacing must be greater than 0 in plpat.");
    }
    spat(inc, del, nlin);
}
