/* $Id$
   $Log$
   Revision 1.1  1992/05/20 21:34:45  furnish
   Initial checkin of the whole PLPLOT project.

*/

/*	plptex.c

	Writes a text string at given world coordinates.
*/

#include "plplot.h"
#include <math.h>

/*----------------------------------------------------------------------*\
* void plptex()
*
* Prints out "text" at world cooordinate (x,y). The text may be
* at any angle "angle" relative to the horizontal. The parameter
* "just" adjusts the horizontal justification of the string:
*	just = 0.0 => left hand edge of string is at (x,y)
*	just = 1.0 => right hand edge of string is at (x,y)
*	just = 0.5 => centre of string is at (x,y) etc.
\*----------------------------------------------------------------------*/

#ifdef PLSTDC
void
c_plptex (PLFLT x, PLFLT y, PLFLT dx, PLFLT dy, PLFLT just, char *text)
#else
void 
c_plptex(x, y, dx, dy, just, text)
PLFLT x, y, dx, dy, just;
char *text;
#endif
{
    PLINT refx, refy;
    PLFLT shift, cc, ss;
    PLFLT xform[4], diag;
    PLFLT xscl, xoff, yscl, yoff;
    PLINT level;

    glev(&level);
    if (level < 3)
	plexit("Please set up window before calling plptex.");

    gwm(&xscl, &xoff, &yscl, &yoff);

    if (dx == 0.0 && dy == 0.0) {
	dx = 1.0;
	dy = 0.0;
    }
    cc = xscl * dx;
    ss = yscl * dy;
    diag = sqrt(cc * cc + ss * ss);
    cc = cc / diag;
    ss = ss / diag;

    gmp(&xscl, &xoff, &yscl, &yoff);
    shift = 0.0;

    xform[0] = cc;
    xform[1] = -ss;
    xform[2] = ss;
    xform[3] = cc;

    if (just != 0.0)
	shift = plstrl(text) * just;
    refx = wcpcx(x) - shift * cc * xscl;
    refy = wcpcy(y) - shift * ss * yscl;
    plstr(0, xform, refx, refy, text);
}
