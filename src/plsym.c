/* $Id$
   $Log$
   Revision 1.4  1993/01/23 06:01:36  mjl
   Now holds all routines dealing with PLPLOT symbols.

 * Revision 1.3  1992/09/29  04:46:21  furnish
 * Massive clean up effort to remove support for garbage compilers (K&R).
 *
 * Revision 1.2  1992/06/02  23:27:35  furnish
 * short -> PLINT so can plot more than 32767 points.
 *
 * Revision 1.1  1992/05/20  21:34:51  furnish
 * Initial checkin of the whole PLPLOT project.
 *
*/

/*	plsym.c

	Point and symbol plotting routines.
*/

#include "plplot.h"

extern short int *fntlkup;
extern short int numberfonts, numberchars;

/*----------------------------------------------------------------------*\
* void plsym()
*
* Plots single precision array y against x for n points using
* Hershey symbol "code".
\*----------------------------------------------------------------------*/

void
c_plsym(PLINT n, PLFLT *x, PLFLT *y, PLINT code)
{
    PLINT i;
    PLINT level;

    glev(&level);
    if (level < 3)
	plexit("plsym: Please set up window first.");

    if (code < 0)
	plexit("plsym: Invalid code.");

    for (i = 0; i < n; i++)
	plhrsh(code, wcpcx(x[i]), wcpcy(y[i]));
}

/*----------------------------------------------------------------------*\
* void plpoin()
*
* Plots single precision array y against x for n points using
* ASCII code "code".
\*----------------------------------------------------------------------*/

void
c_plpoin(PLINT n, PLFLT *x, PLFLT *y, PLINT code)
{
    PLINT i;
    PLINT level, sym, font, col;

    glev(&level);
    if (level < 3)
	plexit("plpoin: Please set up window first.");

    if (code < 0 || code > 127)
	plexit("plpoin: Invalid code.");

    gatt(&font, &col);
    sym = *(fntlkup + (font - 1) * numberchars + code);

    for (i = 0; i < n; i++)
	plhrsh(sym, wcpcx(x[i]), wcpcy(y[i]));
}

/*----------------------------------------------------------------------*\
* void plhrsh()
*
* Writes the Hershey symbol "ch" centred at the physical
* coordinate (x,y).
\*----------------------------------------------------------------------*/

void
plhrsh(PLINT ch, PLINT x, PLINT y)
{
    PLINT cx, cy, k, penup;
    SCHAR *xygrid;
    PLFLT symdef, symht, scale, xscale, yscale, xpmm, ypmm;

    gsym(&symdef, &symht);
    gpixmm(&xpmm, &ypmm);
    penup = 1;
    scale = 0.05 * symht;

    if (!plcvec(ch, &xygrid)) {
	movphy(x, y);
	return;
    }

    /* Compute how many physical pixels correspond to a character pixel */

    xscale = scale * xpmm;
    yscale = scale * ypmm;

    k = 4;
    for (;;) {
	cx = xygrid[k++];
	cy = xygrid[k++];
	if (cx == 64 && cy == 64) {
	    movphy(x, y);
	    return;
	}
	else if (cx == 64 && cy == 0)
	    penup = 1;
	else {
	    if (penup != 0) {
		movphy(ROUND(x + xscale * cx), ROUND(y + yscale * cy));
		penup = 0;
	    }
	    else
		draphy(ROUND(x + xscale * cx), ROUND(y + yscale * cy));
	}
    }
}
