/* $Id$
 * $Log$
 * Revision 1.21  2000/12/18 21:01:49  airwin
 * Change to new style plplot/*.h header file locations.
 *
 * Revision 1.20  1995/03/11 20:27:09  mjl
 * All drivers: eliminated unnecessary variable initializations, other cleaning
 * up.
 *
 * Revision 1.19  1995/01/06  07:40:31  mjl
 * All drivers: pls->width now more sensibly handled.  If the driver supports
 * multiple widths, it first checks to see if it has been initialized
 * already (e.g. from the command line) before initializing it.  For drivers
 * that don't support multiple widths, pls->width is ignored.
*/

/*	ljii.c

	PLplot Laser Jet II device driver.

	Note only the 150 dpi mode is supported.  The others (75,100,300)
	should work by just changing the value of DPI and changing the
	values passed to plP_setphy().  
*/
#include "plplot/plDevs.h"

#ifdef PLD_ljii

#include "plplot/plplotP.h"
#include "plplot/drivers.h"
#include <math.h>
#include <string.h>

#ifdef __GO32__			/* dos386/djgpp */
#ifdef MSDOS
#undef MSDOS
#endif
#endif

/* Function prototypes */

static void setpoint(PLINT, PLINT);

/* top level declarations */

#define JETX    1103
#define JETY    1409

#define OF	pls->OutFile
#define DPI     150		/* Resolution Dots per Inch */
#define CURX    51
#define CURY    61
#define XDOTS	1104L		/* # dots across */
#define YDOTS	1410L		/* # dots down	 */
#define BPROW	XDOTS/8L	/* # bytes across */
#define NBYTES	BPROW*YDOTS	/* total number of bytes */

/* Graphics control characters. */

#define ESC      0x1b
#define FF       0x0c

static char mask[8] =
{'\200', '\100', '\040', '\020', '\010', '\004', '\002', '\001'};

#ifndef MSDOS
#define _HUGE
#else
#define _HUGE _huge
#endif

static char _HUGE *bitmap;	/* points to memory area NBYTES in size */

/*--------------------------------------------------------------------------*\
 * plD_init_ljii()
 *
 * Initialize device.
\*--------------------------------------------------------------------------*/

void
plD_init_ljii(PLStream *pls)
{
    PLDev *dev;

/* Initialize family file info */

    plFamInit(pls);

/* Prompt for a file name if not already set */

    plOpenFile(pls);

/* Allocate and initialize device-specific data */

    dev = plAllocDev(pls);

    dev->xold = UNDEFINED;
    dev->yold = UNDEFINED;
    dev->xmin = 0;
    dev->ymin = 0;

    plP_setpxl((PLFLT) 5.905, (PLFLT) 5.905);

/* Rotate by 90 degrees since portrait mode addressing is used */

    dev->xmin = 0;
    dev->ymin = 0;
    dev->xmax = JETY;
    dev->ymax = JETX;
    dev->xlen = dev->xmax - dev->xmin;
    dev->ylen = dev->ymax - dev->ymin;

    plP_setphy(dev->xmin, dev->xmax, dev->ymin, dev->ymax);

/* Allocate storage for bit map matrix */

#ifdef MSDOS
    if ((bitmap = (char _HUGE *) halloc((long) NBYTES, sizeof(char))) == NULL)
	plexit("Out of memory in call to calloc");
#else
    if ((bitmap = (void *) calloc(NBYTES, sizeof(char))) == NULL)
	plexit("Out of memory in call to calloc");
#endif

/* Reset Printer */

    fprintf(OF, "%cE", ESC);
}

/*--------------------------------------------------------------------------*\
 * plD_line_ljii()
 *
 * Draw a line in the current color from (x1,y1) to (x2,y2).
\*--------------------------------------------------------------------------*/

void
plD_line_ljii(PLStream *pls, short x1a, short y1a, short x2a, short y2a)
{
    PLDev *dev = (PLDev *) pls->dev;
    int i;
    int x1 = x1a, y1 = y1a, x2 = x2a, y2 = y2a;
    PLINT x1b, y1b, x2b, y2b;
    float length, fx, fy, dx, dy;

/* Take mirror image, since PCL expects (0,0) to be at top left */

    y1 = dev->ymax - (y1 - dev->ymin);
    y2 = dev->ymax - (y2 - dev->ymin);

/* Rotate by 90 degrees */

    plRotPhy(1, dev->xmin, dev->ymin, dev->xmax, dev->ymax, &x1, &y1);
    plRotPhy(1, dev->xmin, dev->ymin, dev->xmax, dev->ymax, &x2, &y2);

    x1b = x1, x2b = x2, y1b = y1, y2b = y2;
    length = (float) sqrt((double)
		     ((x2b - x1b) * (x2b - x1b) + (y2b - y1b) * (y2b - y1b)));

    if (length == 0.)
	length = 1.;
    dx = (x2 - x1) / length;
    dy = (y2 - y1) / length;

    fx = x1;
    fy = y1;
    setpoint((PLINT) x1, (PLINT) y1);
    setpoint((PLINT) x2, (PLINT) y2);

    for (i = 1; i <= (int) length; i++)
	setpoint((PLINT) (fx += dx), (PLINT) (fy += dy));
}

/*--------------------------------------------------------------------------*\
 * plD_polyline_ljii()
 *
 * Draw a polyline in the current color.
\*--------------------------------------------------------------------------*/

void
plD_polyline_ljii(PLStream *pls, short *xa, short *ya, PLINT npts)
{
    PLINT i;

    for (i = 0; i < npts - 1; i++)
	plD_line_ljii(pls, xa[i], ya[i], xa[i + 1], ya[i + 1]);
}

/*--------------------------------------------------------------------------*\
 * plD_eop_ljii()
 *
 * End of page.(prints it here).
\*--------------------------------------------------------------------------*/

void
plD_eop_ljii(PLStream *pls)
{
    PLINT i, j;

/* First move cursor to origin */

    fprintf(OF, "%c*p%dX", ESC, CURX);
    fprintf(OF, "%c*p%dY", ESC, CURY);

/* Then put Laser Printer in 150 dpi mode */

    fprintf(OF, "%c*t%dR", ESC, DPI);
    fprintf(OF, "%c*r1A", ESC);

/* Write out raster data */

    for (j = 0; j < YDOTS; j++) {
	fprintf(OF, "%c*b%ldW", ESC, BPROW);
	for (i = 0; i < BPROW; i++)
	    putc(*(bitmap + i + j * BPROW), OF);
    }
    pls->bytecnt += NBYTES;

/* End raster graphics and send Form Feed */

    fprintf(OF, "%c*rB", ESC);
    fprintf(OF, "%c", FF);

/* Finally, clear out bitmap storage area */

    memset(bitmap, '\0', NBYTES);
}

/*--------------------------------------------------------------------------*\
 * plD_bop_ljii()
 *
 * Set up for the next page.
 * Advance to next family file if necessary (file output).
\*--------------------------------------------------------------------------*/

void
plD_bop_ljii(PLStream *pls)
{
    if (!pls->termin)
	plGetFam(pls);

    pls->page++;
}

/*--------------------------------------------------------------------------*\
 * plD_tidy_ljii()
 *
 * Close graphics file or otherwise clean up.
\*--------------------------------------------------------------------------*/

void
plD_tidy_ljii(PLStream *pls)
{
/* Reset Printer */

    fprintf(OF, "%cE", ESC);
    fclose(OF);
    free((void *) bitmap);
}

/*--------------------------------------------------------------------------*\
 * plD_state_ljii()
 *
 * Handle change in PLStream state (color, pen width, fill attribute, etc).
\*--------------------------------------------------------------------------*/

void 
plD_state_ljii(PLStream *pls, PLINT op)
{
}

/*--------------------------------------------------------------------------*\
 * plD_esc_ljii()
 *
 * Escape function.
\*--------------------------------------------------------------------------*/

void
plD_esc_ljii(PLStream *pls, PLINT op, void *ptr)
{
}

/*--------------------------------------------------------------------------*\
 * setpoint()
 *
 * Sets a bit in the bitmap.
\*--------------------------------------------------------------------------*/

static void
setpoint(PLINT x, PLINT y)
{
    PLINT index;
    index = x / 8 + y * BPROW;
    *(bitmap + index) = *(bitmap + index) | mask[x % 8];
}

#else
int 
pldummy_ljii()
{
    return 0;
}

#endif				/* PLD_ljii */
