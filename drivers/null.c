/* $Id$
 * $Log$
 * Revision 1.14  1995/10/22 17:40:33  mjl
 * Removed message "Sending output to null device.." being sent to stderr -- a
 * null device should act accordingly.
 *
 * Revision 1.13  1995/03/11  20:27:12  mjl
 * All drivers: eliminated unnecessary variable initializations, other cleaning
 * up.
 *
 * Revision 1.12  1994/07/19  22:30:25  mjl
 * All device drivers: enabling macro renamed to PLD_<driver>, where <driver>
 * is xwin, ps, etc.  See plDevs.h for more detail.
*/

/*	null.c

	PLplot Null device driver.
*/
#include "plDevs.h"

#ifdef PLD_null

#include "plplotP.h"
#include "drivers.h"

/*--------------------------------------------------------------------------*\
 * plD_init_null()
 *
 * Initialize device (terminal).
\*--------------------------------------------------------------------------*/

void
plD_init_null(PLStream *pls)
{
    int xmin = 0;
    int xmax = PIXELS_X - 1;
    int ymin = 0;
    int ymax = PIXELS_Y - 1;

    float pxlx = (double) PIXELS_X / (double) LPAGE_X;
    float pxly = (double) PIXELS_Y / (double) LPAGE_Y;

/* Set up device parameters */

    plP_setpxl(pxlx, pxly);
    plP_setphy(xmin, xmax, ymin, ymax);
}

/*--------------------------------------------------------------------------*\
 * The remaining driver functions are all null.
\*--------------------------------------------------------------------------*/

void
plD_line_null(PLStream *pls, short x1a, short y1a, short x2a, short y2a)
{
}

void
plD_polyline_null(PLStream *pls, short *xa, short *ya, PLINT npts)
{
}

void
plD_eop_null(PLStream *pls)
{
}

void
plD_bop_null(PLStream *pls)
{
}

void
plD_tidy_null(PLStream *pls)
{
}

void 
plD_state_null(PLStream *pls, PLINT op)
{
}

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
