/* $Id$
   $Log$
   Revision 1.10  1993/07/31 07:56:37  mjl
   Several driver functions consolidated, for all drivers.  The width and color
   commands are now part of a more general "state" command.  The text and
   graph commands used for switching between modes is now handled by the
   escape function (very few drivers require it).  The device-specific PLDev
   structure is now malloc'ed for each driver that requires it, and freed when
   the stream is terminated.

 * Revision 1.9  1993/07/16  22:11:22  mjl
 * Eliminated low-level coordinate scaling; now done by driver interface.
 *
 * Revision 1.8  1993/07/01  21:59:40  mjl
 * Changed all plplot source files to include plplotP.h (private) rather than
 * plplot.h.  Rationalized namespace -- all externally-visible plplot functions
 * now start with "pl"; device driver functions start with "plD_".
*/

/*	null.c

	PLPLOT Null device driver.
*/
#ifdef NULLDEV

#include "plplotP.h"
#include <stdio.h>
#include "drivers.h"

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
* plD_state_null()
*
* Handle change in PLStream state (color, pen width, fill attribute, etc).
\*----------------------------------------------------------------------*/

void 
plD_state_null(PLStream *pls, PLINT op)
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
