/* $Id$
   $Log$
   Revision 1.7  1993/03/15 21:39:13  mjl
   Changed all _clear/_page driver functions to the names _eop/_bop, to be
   more representative of what's actually going on.

 * Revision 1.6  1993/03/03  19:42:02  mjl
 * Changed PLSHORT -> short everywhere; now all device coordinates are expected
 * to fit into a 16 bit address space (reasonable, and good for performance).
 *
 * Revision 1.5  1993/02/27  04:46:38  mjl
 * Fixed errors in ordering of header file inclusion.  "plplot.h" should
 * always be included first.
 *
 * Revision 1.4  1993/02/22  23:10:58  mjl
 * Eliminated the gradv() driver calls, as these were made obsolete by
 * recent changes to plmeta and plrender.  Also eliminated page clear commands
 * from grtidy() -- plend now calls grclr() and grtidy() explicitly.
 *
 * Revision 1.3  1993/01/23  05:41:47  mjl
 * Changes to support new color model, polylines, and event handler support
 * (interactive devices only).
 *
 * Revision 1.2  1992/11/07  07:56:46  mjl
 * (correction) Null device driver, for use when you want the graphics to
 * disappear into the bit bucket.  Always appears as the last device.
 *
 * Revision 1.1  1992/11/07  07:54:13  mjl
 *
*/

/*	null.c

	PLPLOT Null device driver.
*/
#ifdef NULLDEV

#include "plplot.h"
#include <stdio.h>
#include "drivers.h"

static PLFLT lpage_x = 238.0;	/* Page length in x in virtual mm */
static PLFLT lpage_y = 178.0;	/* Page length in y in virtual mm */

static PLDev device;
static PLDev *dev = &device;

/*----------------------------------------------------------------------*\
* null_init()
*
* Initialize device (terminal).
\*----------------------------------------------------------------------*/

void
null_init(PLStream *pls)
{
    printf("Sending output to Null device..\n");

    pls->termin = 0;		/* not an interactive terminal */
    pls->icol0 = 1;
    pls->color = 0;
    pls->width = 1;
    pls->bytecnt = 0;
    pls->page = 0;

/* Set up device parameters */

    dev->xold = UNDEFINED;
    dev->yold = UNDEFINED;
    dev->xmin = 0;
    dev->xmax = 32767;
    dev->ymin = 0;
    dev->ymax = 32767;

    dev->pxlx = (dev->xmax - dev->xmin) / lpage_x;
    dev->pxly = (dev->ymax - dev->ymin) / lpage_y;

    setpxl(dev->pxlx, dev->pxly);
    setphy(dev->xmin, dev->xmax, dev->ymin, dev->ymax);
}

/*----------------------------------------------------------------------*\
* null_line()
*
* Draw a line in the current color from (x1,y1) to (x2,y2).
\*----------------------------------------------------------------------*/

void
null_line(PLStream *pls, short x1a, short y1a, short x2a, short y2a)
{
}

/*----------------------------------------------------------------------*\
* null_polyline()
*
* Draw a polyline in the current color.
\*----------------------------------------------------------------------*/

void
null_polyline(PLStream *pls, short *xa, short *ya, PLINT npts)
{
}

/*----------------------------------------------------------------------*\
* null_eop()
*
* End of page.
\*----------------------------------------------------------------------*/

void
null_eop(PLStream *pls)
{
}

/*----------------------------------------------------------------------*\
* null_bop()
*
* Set up for the next page.
* Advance to next family file if necessary (file output).
\*----------------------------------------------------------------------*/

void
null_bop(PLStream *pls)
{
}

/*----------------------------------------------------------------------*\
* null_tidy()
*
* Close graphics file or otherwise clean up.
\*----------------------------------------------------------------------*/

void
null_tidy(PLStream *pls)
{
}

/*----------------------------------------------------------------------*\
* null_color()
*
* Set pen color.
\*----------------------------------------------------------------------*/

void
null_color(PLStream *pls)
{
}

/*----------------------------------------------------------------------*\
* null_text()
*
* Switch to text mode.
\*----------------------------------------------------------------------*/

void
null_text(PLStream *pls)
{
}

/*----------------------------------------------------------------------*\
* null_graph()
*
* Switch to graphics mode.
\*----------------------------------------------------------------------*/

void
null_graph(PLStream *pls)
{
}

/*----------------------------------------------------------------------*\
* null_width()
*
* Set pen width.
\*----------------------------------------------------------------------*/

void
null_width(PLStream *pls)
{
}

/*----------------------------------------------------------------------*\
* null_esc()
*
* Escape function.
\*----------------------------------------------------------------------*/

void
null_esc(PLStream *pls, PLINT op, char *ptr)
{
}

#else
int 
pldummy_null()
{
    return 0;
}

#endif				/* NULLDEV */
