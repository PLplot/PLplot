/* $Id$
   $Log$
   Revision 1.4  1994/03/23 08:56:06  mjl
   Header file rearrangement, also removed some redundant variable clears.

 * Revision 1.3  1993/07/31  07:56:54  mjl
 * Several driver functions consolidated, for all drivers.  The width and color
 * commands are now part of a more general "state" command.  The text and
 * graph commands used for switching between modes is now handled by the
 * escape function (very few drivers require it).  The device-specific PLDev
 * structure is now malloc'ed for each driver that requires it, and freed when
 * the stream is terminated.
 *
 * Revision 1.2  1993/07/01  21:59:50  mjl
 * Changed all plplot source files to include plplotP.h (private) rather than
 * plplot.h.  Rationalized namespace -- all externally-visible plplot functions
 * now start with "pl"; device driver functions start with "plD_".
*/

/*	pla_iff.c

	PLPLOT Amiga IFF device driver.
	Originally written by Tomas Rokicki (Radical Eye Software).
*/

#include "plplotP.h"
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
    PLDev *dev;

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

/* Allocate and initialize device-specific data */

    dev = plAllocDev(pls);

    dev->xold = UNDEFINED;
    dev->yold = UNDEFINED;
    dev->xmin = 0;
    dev->xmax = pls->xlength;
    dev->ymin = 0;
    dev->ymax = pls->ylength;
    dev->pxlx = pls->xdpi * 25. / 25.4;
    dev->pxly = pls->ydpi * 25. / 25.4;

    plP_setpxl(dev->pxlx, dev->pxly);
    plP_setphy(0, vxwidth, 0, vywidth);

    xsubw = pls->xlength - 2;
    ysubw = pls->ylength - 2;

/* Allocate bitmap and initialize for line drawing */

    if (mapinit(pls->xlength, pls->ylength)) 
	plexit("");
}

/*----------------------------------------------------------------------*\
* plD_line_iff()
*
* Draw a line in the current color from (x1,y1) to (x2,y2).
\*----------------------------------------------------------------------*/

void 
plD_line_iff(PLStream *pls, short x1a, short y1a, short x2a, short y2a)
{
    PLDev *dev = (PLDev *) pls->dev;
    int x1=x1a, y1=y1a, x2=x2a, y2=y2a;
    long xn1, yn1, xn2, yn2;

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
}

/*----------------------------------------------------------------------*\
* plD_state_iff()
*
* Handle change in PLStream state (color, pen width, fill attribute, etc).
\*----------------------------------------------------------------------*/

void 
plD_state_iff(PLStream *pls, PLINT op)
{
    switch (op) {

    case PLSTATE_WIDTH:
	if (pls->width < 1)
	    pls->width = 1;
	else if (pls->width > 3)
	    pls->width = 3;

	break;

    case PLSTATE_COLOR0:
	break;

    case PLSTATE_COLOR1:
	break;
    }
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
