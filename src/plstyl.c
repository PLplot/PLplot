/* $Id$
   $Log$
   Revision 1.1  1992/05/20 21:34:49  furnish
   Initial checkin of the whole PLPLOT project.

*/

/*	plstyl.c

	Sets up a line style.
*/

#include "plplot.h"

/*----------------------------------------------------------------------*\
* void plstyl()
*
* Set up a new line style of "nms" elements, with mark and space
* lengths given by arrays "mark" and "space".
\*----------------------------------------------------------------------*/

void 
c_plstyl(nms, mark, space)
PLINT nms, *mark, *space;
{
    short int i;
    PLINT level;

    glev(&level);
    if (level < 1)
	plexit("Please call plstar before calling plstyl.");

    if ((nms < 0) || (nms > 10)) 
	plexit("Broken lines cannot have <0 or >10 elements");

    for (i = 0; i < nms; i++) {
	if ((mark[i] < 0) || (space[i] < 0))
	    plexit("Mark and space lengths must be > 0 in plstyl.");
    }

    smark(mark, space, nms);
    scure(0, 1, 0, (nms > 0 ? mark[0] : 0));
}
