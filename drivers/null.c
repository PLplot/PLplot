/* $Id$
   $Log$
   Revision 1.9  1993/07/16 22:11:22  mjl
   Eliminated low-level coordinate scaling; now done by driver interface.

 * Revision 1.8  1993/07/01  21:59:40  mjl
 * Changed all plplot source files to include plplotP.h (private) rather than
 * plplot.h.  Rationalized namespace -- all externally-visible plplot functions
 * now start with "pl"; device driver functions start with "plD_".
 *
 * Revision 1.7  1993/03/15  21:39:13  mjl
 * Changed all _clear/_page driver functions to the names _eop/_bop, to be
 * more representative of what's actually going on.
 *
 * Revision 1.6  1993/03/03  19:42:02  mjl
 * Changed PLSHORT -> short everywhere; now all device coordinates are expected
 * to fit into a 16 bit address space (reasonable, and good for performance).
 *
 * Revision 1.5  1993/02/27  04:46:38  mjl
 * Fixed errors in ordering of header file inclusion.  "plplotP.h" should
 * always be included first.
 *
 * Revision 1.4  1993/02/22  23:10:58  mjl
 * Eliminated the plP_adv() driver calls, as these were made obsolete by
 * recent changes to plmeta and plrender.  Also eliminated page clear commands
 * from plP_tidy() -- plend now calls plP_clr() and plP_tidy() explicitly.
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

#include "plplotP.h"
#include <stdio.h>
#include "drivers.h"

static PLFLT lpage_x = 238.0;	/* Page length in x in virtual mm */
static PLFLT lpage_y = 178.0;	/* Page length in y in virtual mm */

static PLDev device;
static PLDev *dev = &device;

/*----------------------------------------------------------------------*\
* plD_init_null()
*
* Initialize device (terminal).
\*----------------------------------------------------------------------*/

void
plD_init_null(PLStream *pls)
{
    int xmin = 0;
    int xmax = PIXELS_X - 1;
    int ymin = 0;
    int ymax = PIXELS_Y - 1;

    float pxlx = (double) PIXELS_X / (double) LPAGE_X;
    float pxly = (double) PIXELS_Y / (double) LPAGE_Y;

    printf("Sending output to Null device..\n");

    pls->termin = 0;		/* not an interactive terminal */
    pls->icol0 = 1;
    pls->color = 0;
    pls->width = 1;
    pls->bytecnt = 0;
    pls->page = 0;

/* Set up device parameters */

    plP_setpxl(pxlx, pxly);
    plP_setphy(xmin, xmax, ymin, ymax);
}

/*----------------------------------------------------------------------*\
* plD_line_null()
*
* Draw a line in the current color from (x1,y1) to (x2,y2).
\*----------------------------------------------------------------------*/

void
plD_line_null(PLStream *pls, short x1a, short y1a, short x2a, short y2a)
{
}

/*----------------------------------------------------------------------*\
* plD_polyline_null()
*
* Draw a polyline in the current color.
\*----------------------------------------------------------------------*/

void
plD_polyline_null(PLStream *pls, short *xa, short *ya, PLINT npts)
{
}

/*----------------------------------------------------------------------*\
* plD_eop_null()
*
* End of page.
\*----------------------------------------------------------------------*/

void
plD_eop_null(PLStream *pls)
{
}

/*----------------------------------------------------------------------*\
* plD_bop_null()
*
* Set up for the next page.
* Advance to next family file if necessary (file output).
\*----------------------------------------------------------------------*/

void
plD_bop_null(PLStream *pls)
{
}

/*----------------------------------------------------------------------*\
* plD_tidy_null()
*
* Close graphics file or otherwise clean up.
\*----------------------------------------------------------------------*/

void
plD_tidy_null(PLStream *pls)
{
}

/*----------------------------------------------------------------------*\
* plD_color_null()
*
* Set pen color.
\*----------------------------------------------------------------------*/

void
plD_color_null(PLStream *pls)
{
}

/*----------------------------------------------------------------------*\
* plD_text_null()
*
* Switch to text mode.
\*----------------------------------------------------------------------*/

void
plD_text_null(PLStream *pls)
{
}

/*----------------------------------------------------------------------*\
* plD_graph_null()
*
* Switch to graphics mode.
\*----------------------------------------------------------------------*/

void
plD_graph_null(PLStream *pls)
{
}

/*----------------------------------------------------------------------*\
* plD_width_null()
*
* Set pen width.
\*----------------------------------------------------------------------*/

void
plD_width_null(PLStream *pls)
{
}

/*----------------------------------------------------------------------*\
* plD_esc_null()
*
* Escape function.
\*----------------------------------------------------------------------*/

void
plD_esc_null(PLStream *pls, PLINT op, void *ptr)
{
}

#else
int 
pldummy_null()
{
    return 0;
}

#endif				/* NULLDEV */
