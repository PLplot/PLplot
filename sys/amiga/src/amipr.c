/* $Id$
   $Log$
   Revision 1.1  1992/05/20 21:35:21  furnish
   Initial checkin of the whole PLPLOT project.

*/

/*	amipr.c

	PLPLOT Amiga (preferences) printer device driver.
	Most of the code is in plsupport.c where it is shared with the
	menu selection printer dump.
*/
#include <stdio.h>
#include "plplot.h"
#include "dispatch.h"

/* top level declarations */

static long bmapx, bmapy, bmapxmax, bmapymax, xdpi, ydpi;
static long dwidth, dheight;

/* (dev) will get passed in eventually, so this looks weird right now */

static PLDev device;
static PLDev (*dev) = &device;

/*----------------------------------------------------------------------*\
* amiprinit()
*
* Initialize device.
\*----------------------------------------------------------------------*/

void
amiprinit(pls)
PLStream *pls;
{
    int mode, openprinter(), mapinit(), queryprint();
    void closeprinter();

    pls->termin = 0;		/* not an interactive terminal */
    pls->color = 1;
    pls->width = 1;
    pls->bytecnt = 0;
    pls->page = 0;

    if (openprinter())
	plexit("");

    mode = queryprint(&bmapx, &bmapy, &bmapxmax, &bmapymax, &xdpi, &ydpi);

/* If mode == 1 we want to adjust the bitmap size so that the aspect
   ratio is maintained. 
*/
    if (mode) {
	if ((float) bmapxmax * bmapy > (float) bmapymax * bmapx)
	    bmapy = (int) (((float) bmapx * bmapymax) / bmapxmax + .5);
	else
	    bmapx = (int) (((float) bmapy * bmapxmax) / bmapymax + .5);
    }

/* Leave a little space for pen width. */

    dwidth = bmapx - 2;
    dheight = bmapy - 2;

/* Set up device parameters */

    dev->xold = UNDEFINED;
    dev->yold = UNDEFINED;
    dev->xmin = 0;
    dev->xmax = bmapxmax;
    dev->ymin = 0;
    dev->ymax = bmapymax;

    if (!pls->orient) {
	setpxl((PLFLT) (ydpi / 25.4), (PLFLT) (xdpi / 25.4));
	setphy(0, bmapymax, 0, bmapxmax);
    }
    else {
	setpxl((PLFLT) (xdpi / 25.4), (PLFLT) (ydpi / 25.4));
	setphy(0, bmapxmax, 0, bmapymax);
    }

/* Allocate bitmap and initial for line drawing */

    if (mapinit(bmapx, bmapy)) {
	closeprinter();
	plexit("");
    }
}

/*----------------------------------------------------------------------*\
* amiprline()
*
* Draw a line in the current color from (x1,y1) to (x2,y2).
\*----------------------------------------------------------------------*/

void 
amiprline(pls, x1a, y1a, x2a, y2a)
PLStream *pls;
PLINT x1a, y1a, x2a, y2a;
{
    int x1=x1a, y1=y1a, x2=x2a, y2=y2a;
    long xn1, yn1, xn2, yn2;
    void mapline();

    if (!pls->orient) {
	xn1 = (x1 * dheight) / bmapymax;
	yn1 = (y1 * dwidth) / bmapxmax;
	xn2 = (x2 * dheight) / bmapymax;
	yn2 = (y2 * dwidth) / bmapxmax;
	switch (pls->width) {
	case 3:
	    mapline(yn1, xn1, yn2, xn2);
	case 2:
	    mapline(yn1 + 2, xn1 + 2, yn2 + 2, xn2 + 2);
	case 1:
	default:
	    mapline(yn1 + 1, xn1 + 1, yn2 + 1, xn2 + 1);
	}
    }
    else {
	xn1 = (x1 * dwidth) / bmapxmax;
	yn1 = (y1 * dheight) / bmapymax;
	xn2 = (x2 * dwidth) / bmapxmax;
	yn2 = (y2 * dheight) / bmapymax;
	switch (pls->width) {
	case 3:
	    mapline(xn1, dheight - yn1, xn2, dheight - yn2);
	case 2:
	    mapline(xn1 + 2, dheight - yn1 + 2, xn2 + 2, dheight - yn2 + 2);
	case 1:
	default:
	    mapline(xn1 + 1, dheight - yn1 + 1, xn2 + 1, dheight - yn2 + 1);
	}
    }
}

/*----------------------------------------------------------------------*\
* amiprclear()
*
* Clear page. 
\*----------------------------------------------------------------------*/

void 
amiprclear(pls)
PLStream *pls;
{
    void ejectpage(), dmpport();

    dmpport(0L, bmapx, bmapy);
    /* Eject the page. */
    ejectpage();
}

/*----------------------------------------------------------------------*\
* amiprpage()
*
* Set up for the next page.  
* Advance to next family file if necessary (file output).
\*----------------------------------------------------------------------*/

void 
amiprpage(pls)
PLStream *pls;
{
    void mapclear();

    mapclear();
    pls->page++;
}

/*----------------------------------------------------------------------*\
* amipradv()
*
* Advance to the next page.
\*----------------------------------------------------------------------*/

void 
amipradv(pls)
PLStream *pls;
{
    amiprclear(pls);
    amiprpage(pls);
}

/*----------------------------------------------------------------------*\
* amiprtidy()
*
* Close graphics file or otherwise clean up.
\*----------------------------------------------------------------------*/

void 
amiprtidy(pls)
PLStream *pls;
{
    void dmpport(), mapfree(), closeprinter();

    dmpport(0L, bmapx, bmapy);
    mapfree();
    closeprinter();
    pls->page = 0;
    pls->OutFile = NULL;
}

/*----------------------------------------------------------------------*\
* amiprcolor()
*
* Set pen color.
\*----------------------------------------------------------------------*/

void 
amiprcolor(pls)
PLStream *pls;
{
}

/*----------------------------------------------------------------------*\
* amiprtext()
*
* Switch to text mode.
\*----------------------------------------------------------------------*/

void 
amiprtext(pls)
PLStream *pls;
{
}

/*----------------------------------------------------------------------*\
* amiprgraph()
*
* Switch to graphics mode.
\*----------------------------------------------------------------------*/

void 
amiprgraph(pls)
PLStream *pls;
{
}

/*----------------------------------------------------------------------*\
* amiprwidth()
*
* Set pen width.
\*----------------------------------------------------------------------*/

void 
amiprwidth(pls)
PLStream *pls;
{
    if (pls->width < 1)
	pls->width = 1;
    else if (pls->width > 3)
	pls->width = 3;
}

/*----------------------------------------------------------------------*\
* amipresc()
*
* Escape function.
\*----------------------------------------------------------------------*/

void 
amipresc(pls, op, ptr)
PLStream *pls;
PLINT op;
char *ptr;
{
}
