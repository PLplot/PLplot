/* $Id$
 * $Log$
 * Revision 1.12  1994/07/19 22:30:25  mjl
 * All device drivers: enabling macro renamed to PLD_<driver>, where <driver>
 * is xwin, ps, etc.  See plDevs.h for more detail.
 *
 * Revision 1.11  1994/03/23  06:34:32  mjl
 * All drivers: cleaned up by eliminating extraneous includes (stdio.h and
 * stdlib.h now included automatically by plplotP.h), extraneous clears
 * of pls->fileset, pls->page, and pls->OutFile = NULL (now handled in
 * driver interface or driver initialization as appropriate).  Special
 * handling for malloc includes eliminated (no longer needed) and malloc
 * prototypes fixed as necessary.
*/

/*	null.c

	PLPLOT Null device driver.
*/
#include "plDevs.h"

#ifdef PLD_null

#include "plplotP.h"
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

#endif				/* PLD_nulldev */
