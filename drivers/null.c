/* $Id$

	PLplot Null device driver.
*/
#include "plplot/plDevs.h"

#ifdef PLD_null

#include "plplot/plplotP.h"
#include "plplot/drivers.h"

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

    PLFLT pxlx = (double) PIXELS_X / (double) LPAGE_X;
    PLFLT pxly = (double) PIXELS_Y / (double) LPAGE_Y;

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
