/* $Id$
   $Log$
   Revision 1.2  1992/09/29 04:46:28  furnish
   Massive clean up effort to remove support for garbage compilers (K&R).

 * Revision 1.1  1992/05/20  21:35:00  furnish
 * Initial checkin of the whole PLPLOT project.
 *
*/

/*	plwid.c

	Sets line width.
*/

#include "plplot.h"

void 
c_plwid( PLINT width )
{
    PLINT level;

    glev(&level);
    if (level < 1)
	plexit("Please call plstar before calling plwid.");

    swid(width);
    grwid();
}
