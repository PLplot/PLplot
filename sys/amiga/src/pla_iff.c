/* $Id$
   $Log$
   Revision 1.2  1993/07/01 21:59:50  mjl
   Changed all plplot source files to include plplotP.h (private) rather than
   plplot.h.  Rationalized namespace -- all externally-visible plplot functions
   now start with "pl"; device driver functions start with "plD_".

 * Revision 1.1  1993/03/15  21:34:20  mjl
 * Reorganization and update of Amiga drivers.  Window driver now uses Amiga
 * OS 2.0 capabilities.
 *
*/

/*	pla_iff.c

	PLPLOT Amiga IFF device driver.
	Originally written by Tomas Rokicki (Radical Eye Software).
*/

#include "plplotP.h"
#include <stdio.h>
#include "drivers.h"
#include "plamiga.h"

/* top level declarations */

static PLINT xsubw, ysubw;
static PLINT vxwidth, vywidth;

/* (dev) will get passed in eventually, so this looks weird right now */

static PLDev device;
static PLDev (*dev) = &device;

/*----------------------------------------------------------------------*\
* plD_init_iff()
*
* Initialize device.
\*----------------------------------------------------------------------*/

void 
plD_init_iff(PLStream *pls)
{
    pls->termin = 0;		/* not an interactive terminal */
    pls->icol0 = 1;
    pls->width = 1;
    pls->bytecnt = 0;
    pls->page = 0;

    if (!pls->colorset)
        pls->color = 1;

    if (!pls->pageset) {
	pls->xdpi    = plGetFlt("Enter desired horizontal resolution (dpi): ");
	pls->ydpi    = plGetFlt("Enter desired vertical   resolution (dpi): ");
	pls->xlength = plGetInt("Enter desired horizontal size in pixels  : ");
	pls->ylength = plGetInt("Enter desired vertical   size in pixels  : ");
	pls->pageset = 1;
    }

    vxwidth = pls->xlength * 25;
    vywidth = pls->ylength * 25;

    dev->xold = UNDEFINED;
    dev->yold = UNDEFINED;
    dev->xmin = 0;
    dev->xmax = pls->xlength;
    dev->ymin = 0;
    dev->ymax = pls->ylength;

    if (!pls->orient) {
	plP_setpxl((PLFLT) (pls->xdpi * 25. / 25.4), (PLFLT) (pls->ydpi * 25 / 25.4));
	plP_setphy(0, vxwidth, 0, vywidth);
    }
    else {
	plP_setpxl((PLFLT) (pls->ydpi * 25 / 25.4), (PLFLT) (pls->xdpi * 25 / 25.4));
	plP_setphy(0, vywidth, 0, vxwidth);
    }

    xsubw = pls->xlength - 2;
    ysubw = pls->ylength - 2;

    /* Allocate bitmap and initialize for line drawing */
    if (mapinit(pls->xlength, pls->ylength)) {
	plexit("");
    }
}

/*----------------------------------------------------------------------*\
* plD_line_iff()
*
* Draw a line in the current color from (x1,y1) to (x2,y2).
\*----------------------------------------------------------------------*/

void 
plD_line_iff(PLStream *pls, short x1a, short y1a, short x2a, short y2a)
{
    int x1=x1a, y1=y1a, x2=x2a, y2=y2a;
    long xn1, yn1, xn2, yn2;

    if (!pls->orient) {
	xn1 = (x1 * xsubw) / vxwidth;
	yn1 = (y1 * ysubw) / vywidth;
	xn2 = (x2 * xsubw) / vxwidth;
	yn2 = (y2 * ysubw) / vywidth;
	switch (pls->width) {
	case 3:
	    mapline(xn1, ysubw - yn1, xn2, ysubw - yn2);
	case 2:
	    mapline(xn1 + 2, ysubw - yn1 + 2, xn2 + 2, ysubw - yn2 + 2);
	case 1:
	default:
	    mapline(xn1 + 1, ysubw - yn1 + 1, xn2 + 1, ysubw - yn2 + 1);
	}
    }
    else {
	xn1 = (x1 * ysubw) / vywidth;
	yn1 = (y1 * xsubw) / vxwidth;
	xn2 = (x2 * ysubw) / vywidth;
	yn2 = (y2 * xsubw) / vxwidth;
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
}

/*----------------------------------------------------------------------*\
* plD_polyline_iff()
*
* Draw a polyline in the current color.
\*----------------------------------------------------------------------*/

void 
plD_polyline_iff (PLStream *pls, short *xa, short *ya, PLINT npts)
{
    PLINT i;

    for (i=0; i<npts-1; i++) 
      plD_line_iff( pls, xa[i], ya[i], xa[i+1], ya[i+1] );
}

/*----------------------------------------------------------------------*\
* plD_eop_iff()
*
* End of page. 
* Here need to close file since only 1 page/file is allowed (for now).
\*----------------------------------------------------------------------*/

void 
plD_eop_iff(PLStream *pls)
{
    iffwritefile((PLINT) pls->xdpi, (PLINT) pls->ydpi, pls->OutFile);
    fclose(pls->OutFile);
}

/*----------------------------------------------------------------------*\
* plD_bop_iff()
*
* Set up for the next page.  
* Advance to next family file if necessary (file output).
* Here need to open a new file since only 1 page/file is allowed (for now).
\*----------------------------------------------------------------------*/

void 
plD_bop_iff(PLStream *pls)
{
    pls->page++;
    mapclear();
    plOpenFile(pls);
}

/*----------------------------------------------------------------------*\
* plD_tidy_iff()
*
* Close graphics file or otherwise clean up.
\*----------------------------------------------------------------------*/

void 
plD_tidy_iff(PLStream *pls)
{
    mapfree();
    pls->fileset = 0;
    pls->page = 0;
    pls->OutFile = NULL;
}

/*----------------------------------------------------------------------*\
* plD_color_iff()
*
* Set pen color.
\*----------------------------------------------------------------------*/

void 
plD_color_iff(PLStream *pls)
{
}

/*----------------------------------------------------------------------*\
* plD_text_iff()
*
* Switch to text mode.
\*----------------------------------------------------------------------*/

void 
plD_text_iff(PLStream *pls)
{
}

/*----------------------------------------------------------------------*\
* plD_graph_iff()
*
* Switch to graphics mode.
\*----------------------------------------------------------------------*/

void 
plD_graph_iff(PLStream *pls)
{
}

/*----------------------------------------------------------------------*\
* plD_width_iff()
*
* Set pen width.
\*----------------------------------------------------------------------*/

void 
plD_width_iff(PLStream *pls)
{
    if (pls->width < 1)
	pls->width = 1;
    else if (pls->width > 3)
	pls->width = 3;
}

/*----------------------------------------------------------------------*\
* plD_esc_iff()
*
* Escape function.
\*----------------------------------------------------------------------*/

void 
plD_esc_iff(PLStream *pls, PLINT op, char *ptr)
{
}
