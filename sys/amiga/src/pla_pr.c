/* $Id$
   $Log$
   Revision 1.1  1993/03/15 21:34:24  mjl
   Reorganization and update of Amiga drivers.  Window driver now uses Amiga
   OS 2.0 capabilities.

*/

/*	pla_pr.c

	PLPLOT Amiga (preferences) printer device driver.
	Most of the code is in plsupport.c where it is shared with the
	menu selection printer dump.
*/

#include "plplot.h"
#include <stdio.h>
#include "drivers.h"
#include "plamiga.h"

/* top level declarations */

static long bmapx, bmapy, bmapxmax, bmapymax, xdpi, ydpi;
static long dwidth, dheight;

/* (dev) will get passed in eventually, so this looks weird right now */

static PLDev device;
static PLDev (*dev) = &device;

/*----------------------------------------------------------------------*\
* amipr_init()
*
* Initialize device.
\*----------------------------------------------------------------------*/

void
amipr_init(PLStream *pls)
{
    int mode;

    pls->termin = 0;		/* not an interactive terminal */
    pls->icol0 = 1;
    pls->width = 1;
    pls->bytecnt = 0;
    pls->page = 0;

    if (!pls->colorset)
        pls->color = 0;		/* no color by default: user can override */

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
* amipr_line()
*
* Draw a line in the current color from (x1,y1) to (x2,y2).
\*----------------------------------------------------------------------*/

void 
amipr_line(PLStream *pls, short x1a, short y1a, short x2a, short y2a)
{
    int x1=x1a, y1=y1a, x2=x2a, y2=y2a;
    long xn1, yn1, xn2, yn2;

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
* amipr_polyline()
*
* Draw a polyline in the current color.
\*----------------------------------------------------------------------*/

void 
amipr_polyline (PLStream *pls, short *xa, short *ya, PLINT npts)
{
    PLINT i;

    for (i=0; i<npts-1; i++) 
      amipr_line( pls, xa[i], ya[i], xa[i+1], ya[i+1] );
}

/*----------------------------------------------------------------------*\
* amipr_eop()
*
* End of page. 
\*----------------------------------------------------------------------*/

void 
amipr_eop(PLStream *pls)
{
    dmpport(0L, bmapx, bmapy);
    /* Eject the page. */
    ejectpage();
}

/*----------------------------------------------------------------------*\
* amipr_bop()
*
* Set up for the next page.  
* Advance to next family file if necessary (file output).
\*----------------------------------------------------------------------*/

void 
amipr_bop(PLStream *pls)
{
    mapclear();
    pls->page++;
}

/*----------------------------------------------------------------------*\
* amipr_tidy()
*
* Close graphics file or otherwise clean up.
\*----------------------------------------------------------------------*/

void 
amipr_tidy(PLStream *pls)
{
    mapfree();
    closeprinter();
    pls->page = 0;
    pls->OutFile = NULL;
}

/*----------------------------------------------------------------------*\
* amipr_color()
*
* Set pen color.
\*----------------------------------------------------------------------*/

void 
amipr_color(PLStream *pls)
{
}

/*----------------------------------------------------------------------*\
* amipr_text()
*
* Switch to text mode.
\*----------------------------------------------------------------------*/

void 
amipr_text(PLStream *pls)
{
}

/*----------------------------------------------------------------------*\
* amipr_graph()
*
* Switch to graphics mode.
\*----------------------------------------------------------------------*/

void 
amipr_graph(PLStream *pls)
{
}

/*----------------------------------------------------------------------*\
* amipr_width()
*
* Set pen width.
\*----------------------------------------------------------------------*/

void 
amipr_width(PLStream *pls)
{
    if (pls->width < 1)
	pls->width = 1;
    else if (pls->width > 3)
	pls->width = 3;
}

/*----------------------------------------------------------------------*\
* amipr_esc()
*
* Escape function.
\*----------------------------------------------------------------------*/

void 
amipr_esc(PLStream *pls, PLINT op, char *ptr)
{
}
