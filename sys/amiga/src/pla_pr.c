/* $Id$
   $Log$
   Revision 1.2  1993/07/01 21:59:53  mjl
   Changed all plplot source files to include plplotP.h (private) rather than
   plplot.h.  Rationalized namespace -- all externally-visible plplot functions
   now start with "pl"; device driver functions start with "plD_".

 * Revision 1.1  1993/03/15  21:34:24  mjl
 * Reorganization and update of Amiga drivers.  Window driver now uses Amiga
 * OS 2.0 capabilities.
 *
*/

/*	pla_pr.c

	PLPLOT Amiga (preferences) printer device driver.
	Most of the code is in plsupport.c where it is shared with the
	menu selection printer dump.
*/

#include "plplotP.h"
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
* plD_init_amipr()
*
* Initialize device.
\*----------------------------------------------------------------------*/

void
plD_init_amipr(PLStream *pls)
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
	plP_setpxl((PLFLT) (ydpi / 25.4), (PLFLT) (xdpi / 25.4));
	plP_setphy(0, bmapymax, 0, bmapxmax);
    }
    else {
	plP_setpxl((PLFLT) (xdpi / 25.4), (PLFLT) (ydpi / 25.4));
	plP_setphy(0, bmapxmax, 0, bmapymax);
    }

/* Allocate bitmap and initial for line drawing */

    if (mapinit(bmapx, bmapy)) {
	closeprinter();
	plexit("");
    }
}

/*----------------------------------------------------------------------*\
* plD_line_amipr()
*
* Draw a line in the current color from (x1,y1) to (x2,y2).
\*----------------------------------------------------------------------*/

void 
plD_line_amipr(PLStream *pls, short x1a, short y1a, short x2a, short y2a)
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
* plD_polyline_amipr()
*
* Draw a polyline in the current color.
\*----------------------------------------------------------------------*/

void 
plD_polyline_amipr (PLStream *pls, short *xa, short *ya, PLINT npts)
{
    PLINT i;

    for (i=0; i<npts-1; i++) 
      plD_line_amipr( pls, xa[i], ya[i], xa[i+1], ya[i+1] );
}

/*----------------------------------------------------------------------*\
* plD_eop_amipr()
*
* End of page. 
\*----------------------------------------------------------------------*/

void 
plD_eop_amipr(PLStream *pls)
{
    dmpport(0L, bmapx, bmapy);
    /* Eject the page. */
    ejectpage();
}

/*----------------------------------------------------------------------*\
* plD_bop_amipr()
*
* Set up for the next page.  
* Advance to next family file if necessary (file output).
\*----------------------------------------------------------------------*/

void 
plD_bop_amipr(PLStream *pls)
{
    mapclear();
    pls->page++;
}

/*----------------------------------------------------------------------*\
* plD_tidy_amipr()
*
* Close graphics file or otherwise clean up.
\*----------------------------------------------------------------------*/

void 
plD_tidy_amipr(PLStream *pls)
{
    mapfree();
    closeprinter();
    pls->page = 0;
    pls->OutFile = NULL;
}

/*----------------------------------------------------------------------*\
* plD_color_amipr()
*
* Set pen color.
\*----------------------------------------------------------------------*/

void 
plD_color_amipr(PLStream *pls)
{
}

/*----------------------------------------------------------------------*\
* plD_text_amipr()
*
* Switch to text mode.
\*----------------------------------------------------------------------*/

void 
plD_text_amipr(PLStream *pls)
{
}

/*----------------------------------------------------------------------*\
* plD_graph_amipr()
*
* Switch to graphics mode.
\*----------------------------------------------------------------------*/

void 
plD_graph_amipr(PLStream *pls)
{
}

/*----------------------------------------------------------------------*\
* plD_width_amipr()
*
* Set pen width.
\*----------------------------------------------------------------------*/

void 
plD_width_amipr(PLStream *pls)
{
    if (pls->width < 1)
	pls->width = 1;
    else if (pls->width > 3)
	pls->width = 3;
}

/*----------------------------------------------------------------------*\
* plD_esc_amipr()
*
* Escape function.
\*----------------------------------------------------------------------*/

void 
plD_esc_amipr(PLStream *pls, PLINT op, char *ptr)
{
}
