/* $Id$
   $Log$
   Revision 1.2  1992/11/07 07:56:46  mjl
   (correction) Null device driver, for use when you want the graphics to
   disappear into the bit bucket.  Always appears as the last device.

 * Revision 1.1  1992/11/07  07:54:13  mjl
 *
*/

/*	null.c

	PLPLOT Null device driver.
*/
static int dummy;
#ifdef NULLDEV

#include <stdio.h>
#include <stdlib.h>

#include "plplot.h"
#include "dispatch.h"

static PLFLT lpage_x = 238.0;		/* Page length in x in virtual mm */
static PLFLT lpage_y = 178.0;		/* Page length in y in virtual mm */

static PLDev device;
static PLDev *dev = &device;

/*----------------------------------------------------------------------*\
* null_init()
*
* Initialize device (terminal).
\*----------------------------------------------------------------------*/

void 
null_init (PLStream *pls)
{
    printf("Sending output to Null device..\n");

    pls->termin = 0;		/* not an interactive terminal */
    pls->color = 1;
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
null_line (PLStream *pls, PLINT x1a, PLINT y1a, PLINT x2a, PLINT y2a)
{
}

/*----------------------------------------------------------------------*\
* null_clear()
*
* Clear page. 
\*----------------------------------------------------------------------*/

void 
null_clear (PLStream *pls)
{
}

/*----------------------------------------------------------------------*\
* null_page()
*
* Set up for the next page.  
* Advance to next family file if necessary (file output).
\*----------------------------------------------------------------------*/

void 
null_page (PLStream *pls)
{
}

/*----------------------------------------------------------------------*\
* null_adv()
*
* Advance to the next page.
\*----------------------------------------------------------------------*/

void 
null_adv (PLStream *pls)
{
}

/*----------------------------------------------------------------------*\
* null_tidy()
*
* Close graphics file or otherwise clean up.
\*----------------------------------------------------------------------*/

void 
null_tidy (PLStream *pls)
{
}

/*----------------------------------------------------------------------*\
* null_color()
*
* Set pen color.
\*----------------------------------------------------------------------*/

void 
null_color (PLStream *pls)
{
}

/*----------------------------------------------------------------------*\
* null_text()
*
* Switch to text mode.
\*----------------------------------------------------------------------*/

void 
null_text (PLStream *pls)
{
}

/*----------------------------------------------------------------------*\
* null_graph()
*
* Switch to graphics mode.
\*----------------------------------------------------------------------*/

void 
null_graph (PLStream *pls)
{
}

/*----------------------------------------------------------------------*\
* null_width()
*
* Set pen width.
\*----------------------------------------------------------------------*/

void 
null_width (PLStream *pls)
{
}

/*----------------------------------------------------------------------*\
* null_esc()
*
* Escape function.
\*----------------------------------------------------------------------*/

void 
null_esc (PLStream *pls, PLINT op, char *ptr)
{
}

#endif	/* NULLDEV */
