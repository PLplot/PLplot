/* $Id$
   $Log$
   Revision 1.1  1992/05/20 21:34:22  furnish
   Initial checkin of the whole PLPLOT project.

*/

/*	plend.c

	End a plotting session.
*/

#include "plplot.h"

/* Ends a plotting session for the current stream only */

void 
c_plend1()
{
    PLINT level;

    glev(&level);
    if (level == 0)
	return;
    grtidy();
    slev(0);
}

/* Ends a plotting session for all open streams */

void 
c_plend()
{
    PLINT i;

    for (i = 0; i < PL_NSTREAMS; i++) {
	plsstrm(i);
	c_plend1();
    }
    plsstrm(0);
    plfontrel();
}
