/* $Id$
   $Log$
   Revision 1.2  1992/09/29 04:44:51  furnish
   Massive clean up effort to remove support for garbage compilers (K&R).

 * Revision 1.1  1992/05/20  21:32:45  furnish
 * Initial checkin of the whole PLPLOT project.
 *
*/

/*	xterm.c

	PLPLOT xterm device driver.
*/
#ifdef XTERM

#include <stdio.h>
#include "plplot.h"
#include "dispatch.h"

/* top level declarations */

#define TEKX   1023
#define TEKY    779

/* Graphics control characters. */

#define BEL  7
#define FF   12
#define CAN  24
#define ESC  27
#define GS   29
#define US   31
#define ETX  3

/* (dev) will get passed in eventually, so this looks weird right now */

static PLDev device;
static PLDev *dev = &device;

/*----------------------------------------------------------------------*\
* xteinit()
*
* Initialize device.
\*----------------------------------------------------------------------*/

void 
xteinit (PLStream *pls)
{
    /* tell plplot that this is an interactive device (so pause after */
    /* drawing graph).  use if smod(0) if sending grphx to a file. */

    pls->termin = 1;

    /* set default pen color (this should be the desired pen number) */
    /* plplot will actually tell the device to use this pen by */
    /* making a call to plcolor. */

    pls->color = 1;
    pls->width = 1;
    pls->bytecnt = 0;
    pls->page = 0;

    dev->xold = UNDEFINED;
    dev->yold = UNDEFINED;
    dev->xmin = 0;
    dev->xmax = TEKX;
    dev->ymin = 0;
    dev->ymax = TEKY;

    /* set device resolution in dots/mm */

    setpxl((PLFLT) (4.771 * 16), (PLFLT) (4.653 * 16));

    /* set page size using setphy(xmin,xmax,ymin,ymax) */
    /* plplot assumes that the min coordinates are in the lower left */

    setphy(0, 16 * TEKX, 0, 16 * TEKY);

    printf("%c[?38h", ESC);	/* open graphics window */
    printf("%c", GS);		/* set to vector mode */
    printf("%c%c", ESC, FF);

    pls->graphx = GRAPHICS_MODE;
}

/*----------------------------------------------------------------------*\
* xteline()
*
* Draw a line in the current color from (x1,y1) to (x2,y2).
\*----------------------------------------------------------------------*/

void 
xteline (PLStream *pls, PLINT x1a, PLINT y1a, PLINT x2a, PLINT y2a)
{
    int x1, y1, x2, y2, hy, ly, hx, lx;

    x1a >>= 4;
    y1a >>= 4;
    x2a >>= 4;
    y2a >>= 4;

    x1 = x1a;
    y1 = y1a;
    x2 = x2a;
    y2 = y2a;

    if (pls->pscale)
	plSclPhy(pls, dev, &x1, &y1, &x2, &y2);

/* If continuation of previous line just send new point */

    if (x1 == dev->xold && y1 == dev->yold) {
	hy = y2 / 32 + 32;
	ly = y2 - (y2 / 32) * 32 + 96;
	hx = x2 / 32 + 32;
	lx = x2 - (x2 / 32) * 32 + 64;
	printf("%c%c%c%c", hy, ly, hx, lx);
    }
    else {
	printf("%c", GS);
	hy = y1 / 32 + 32;
	ly = y1 - (y1 / 32) * 32 + 96;
	hx = x1 / 32 + 32;
	lx = x1 - (x1 / 32) * 32 + 64;
	printf("%c%c%c%c", hy, ly, hx, lx);
	hy = y2 / 32 + 32;
	ly = y2 - (y2 / 32) * 32 + 96;
	hx = x2 / 32 + 32;
	lx = x2 - (x2 / 32) * 32 + 64;
	printf("%c%c%c%c", hy, ly, hx, lx);
    }
    dev->xold = x2;
    dev->yold = y2;
}

/*----------------------------------------------------------------------*\
* xteclear()
*
* Clear page.  User must hit a <CR> to continue.
\*----------------------------------------------------------------------*/

void 
xteclear (PLStream *pls)
{
    putchar(BEL);
    fflush(stdout);
    if (!pls->nopause)
	while (getchar() != '\n')
	    ;
    printf("%c%c", ESC, FF);
}

/*----------------------------------------------------------------------*\
* xtepage()
*
* Set up for the next page.
\*----------------------------------------------------------------------*/

void 
xtepage (PLStream *pls)
{
    dev->xold = UNDEFINED;
    dev->yold = UNDEFINED;
    pls->page++;
}

/*----------------------------------------------------------------------*\
* xteadv()
*
* Advance to the next page.
\*----------------------------------------------------------------------*/

void 
xteadv (PLStream *pls)
{
    xteclear(pls);
    xtepage(pls);
}

/*----------------------------------------------------------------------*\
* xtetidy()
*
* Close graphics file
\*----------------------------------------------------------------------*/

void 
xtetidy (PLStream *pls)
{
    putchar(BEL);
    fflush(stdout);
    if (!pls->nopause)
	while (getchar() != '\n')
	    ;

    if (pls->graphx == GRAPHICS_MODE) {
	pls->graphx = TEXT_MODE;
	printf("%c%c", US, CAN);
	printf("%c%c", ESC, ETX);
    }
    fflush(stdout);
    pls->fileset = 0;
    pls->page = 0;
    pls->OutFile = NULL;
}

/*----------------------------------------------------------------------*\
* xtecolor()
*
* Set pen color.
\*----------------------------------------------------------------------*/

void 
xtecolor (PLStream *pls)
{
}

/*----------------------------------------------------------------------*\
* xtetext()
*
* Switch to text mode.
\*----------------------------------------------------------------------*/

void 
xtetext (PLStream *pls)
{
    if (pls->graphx == GRAPHICS_MODE) {
	pls->graphx = TEXT_MODE;
	printf("%c%c", ESC, ETX);
    }
}

/*----------------------------------------------------------------------*\
* xtegraph()
*
* Switch to graphics mode.
\*----------------------------------------------------------------------*/

void 
xtegraph (PLStream *pls)
{
    if (pls->graphx == TEXT_MODE) {
	pls->graphx = GRAPHICS_MODE;
	printf("%c[?38h", ESC);
    }
}

/*----------------------------------------------------------------------*\
* xtewidth()
*
* Set pen width.
\*----------------------------------------------------------------------*/

void 
xtewidth (PLStream *pls)
{
}

/*----------------------------------------------------------------------*\
* xteesc()
*
* Escape function.
\*----------------------------------------------------------------------*/

void 
xteesc (PLStream *pls, PLINT op, char *ptr)
{
}
#endif	/* XTERM */
