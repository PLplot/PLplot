/* $Id$
 * $Log$
 * Revision 1.17  1994/04/30 16:14:43  mjl
 * Fixed format field (%ld instead of %d) or introduced casts where
 * appropriate to eliminate warnings given by gcc -Wall.
 *
 * Revision 1.16  1994/04/08  11:34:38  mjl
 * Fix for DOS machines running DJGPP.
 *
 * Revision 1.15  1994/03/23  06:34:28  mjl
 * All drivers: cleaned up by eliminating extraneous includes (stdio.h and
 * stdlib.h now included automatically by plplotP.h), extraneous clears
 * of pls->fileset, pls->page, and pls->OutFile = NULL (now handled in
 * driver interface or driver initialization as appropriate).  Special
 * handling for malloc includes eliminated (no longer needed) and malloc
 * prototypes fixed as necessary.
 *
 * Revision 1.14  1993/08/09  22:12:32  mjl
 * Changed call syntax to plRotPhy to allow easier usage.
 *
 * Revision 1.13  1993/07/31  07:56:34  mjl
 * Several driver functions consolidated, for all drivers.  The width and color
 * commands are now part of a more general "state" command.  The text and
 * graph commands used for switching between modes is now handled by the
 * escape function (very few drivers require it).  The device-specific PLDev
 * structure is now malloc'ed for each driver that requires it, and freed when
 * the stream is terminated.
*/

/*	ljii.c

	PLPLOT Laser Jet II device driver.
	Note only the 150 dpi mode is supported.  The others (75,100,300)
	should work by just changing the value of DPI and changing the
	values passed to plP_setphy().
*/
#ifdef LJII

#include "plplotP.h"
#include "drivers.h"
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

#define JETX     1103
#define JETY     1409

#define DPI      150		/* Resolution Dots per Inch */
#define CURX     51
#define CURY     61
#define XDOTS	 1104L		/* # dots across */
#define YDOTS	 1410L		/* # dots down	 */
#define BPROW	 XDOTS/8L	/* # bytes across */
#define NBYTES	 BPROW*YDOTS	/* total number of bytes */

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

/*----------------------------------------------------------------------*\
* plD_init_ljii()
*
* Initialize device.
\*----------------------------------------------------------------------*/

void
plD_init_ljii(PLStream *pls)
{
    PLDev *dev;

    pls->termin = 0;		/* not an interactive terminal */
    pls->icol0 = 1;
    pls->color = 0;
    pls->width = 1;
    pls->bytecnt = 0;
    pls->page = 0;

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

    fprintf(pls->OutFile, "%cE", ESC);
}

/*----------------------------------------------------------------------*\
* plD_line_ljii()
*
* Draw a line in the current color from (x1,y1) to (x2,y2).
\*----------------------------------------------------------------------*/

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

/*----------------------------------------------------------------------*\
* plD_polyline_ljii()
*
* Draw a polyline in the current color.
\*----------------------------------------------------------------------*/

void
plD_polyline_ljii(PLStream *pls, short *xa, short *ya, PLINT npts)
{
    PLINT i;

    for (i = 0; i < npts - 1; i++)
	plD_line_ljii(pls, xa[i], ya[i], xa[i + 1], ya[i + 1]);
}

/*----------------------------------------------------------------------*\
* plD_eop_ljii()
*
* End of page.(prints it here).
\*----------------------------------------------------------------------*/

void
plD_eop_ljii(PLStream *pls)
{
    PLINT i, j;

    /* First move cursor to origin */

    fprintf(pls->OutFile, "%c*p%dX", ESC, CURX);
    fprintf(pls->OutFile, "%c*p%dY", ESC, CURY);

    /* Then put Laser Printer in 150 dpi mode */

    fprintf(pls->OutFile, "%c*t%dR", ESC, DPI);
    fprintf(pls->OutFile, "%c*r1A", ESC);

    /* Write out raster data */

    for (j = 0; j < YDOTS; j++) {
	fprintf(pls->OutFile, "%c*b%ldW", ESC, BPROW);
	for (i = 0; i < BPROW; i++)
	    putc(*(bitmap + i + j * BPROW), pls->OutFile);
    }
    pls->bytecnt += NBYTES;

    /* End raster graphics and send Form Feed */

    fprintf(pls->OutFile, "%c*rB", ESC);
    fprintf(pls->OutFile, "%c", FF);

    /* Finally, clear out bitmap storage area */

    memset(bitmap, '\0', NBYTES);
}

/*----------------------------------------------------------------------*\
* plD_bop_ljii()
*
* Set up for the next page.
* Advance to next family file if necessary (file output).
\*----------------------------------------------------------------------*/

void
plD_bop_ljii(PLStream *pls)
{
    if (!pls->termin)
	plGetFam(pls);

    pls->page++;
}

/*----------------------------------------------------------------------*\
* plD_tidy_ljii()
*
* Close graphics file or otherwise clean up.
\*----------------------------------------------------------------------*/

void
plD_tidy_ljii(PLStream *pls)
{
/* Reset Printer */

    fprintf(pls->OutFile, "%cE", ESC);
    fclose(pls->OutFile);
    free((void *) bitmap);
}

/*----------------------------------------------------------------------*\
* plD_state_ljii()
*
* Handle change in PLStream state (color, pen width, fill attribute, etc).
\*----------------------------------------------------------------------*/

void 
plD_state_ljii(PLStream *pls, PLINT op)
{
}

/*----------------------------------------------------------------------*\
* plD_esc_ljii()
*
* Escape function.
\*----------------------------------------------------------------------*/

void
plD_esc_ljii(PLStream *pls, PLINT op, void *ptr)
{
}

/*----------------------------------------------------------------------*\
* setpoint()
*
* Sets a bit in the bitmap.
\*----------------------------------------------------------------------*/

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

#endif				/* LJII */
