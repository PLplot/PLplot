/* $Id$
   $Log$
   Revision 1.8  1993/03/03 19:41:59  mjl
   Changed PLSHORT -> short everywhere; now all device coordinates are expected
   to fit into a 16 bit address space (reasonable, and good for performance).

 * Revision 1.7  1993/03/03  16:17:08  mjl
 * Fixed orientation-swapping code.
 *
 * Revision 1.6  1993/02/22  23:10:55  mjl
 * Eliminated the gradv() driver calls, as these were made obsolete by
 * recent changes to plmeta and plrender.  Also eliminated page clear commands
 * from grtidy() -- plend now calls grclr() and grtidy() explicitly.
 *
 * Revision 1.5  1993/01/23  05:41:45  mjl
 * Changes to support new color model, polylines, and event handler support
 * (interactive devices only).
 *
 * Revision 1.4  1992/11/07  07:48:43  mjl
 * Fixed orientation operation in several files and standardized certain startup
 * operations. Fixed bugs in various drivers.
 *
 * Revision 1.3  1992/09/30  18:24:54  furnish
 * Massive cleanup to irradicate garbage code.  Almost everything is now
 * prototyped correctly.  Builds on HPUX, SUNOS (gcc), AIX, and UNICOS.
 *
 * Revision 1.2  1992/09/29  04:44:44  furnish
 * Massive clean up effort to remove support for garbage compilers (K&R).
 *
 * Revision 1.1  1992/05/20  21:32:37  furnish
 * Initial checkin of the whole PLPLOT project.
 *
*/

/*	ljii.c

	PLPLOT Laser Jet II device driver.
	Note only the 150 dpi mode is supported.  The others (75,100,300)
	should work by just changing the value of DPI and changing the
	values passed to setphy().
*/
#ifdef LJII

#define PL_NEED_MALLOC
#include "plplot.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "drivers.h"

/* Function prototypes */

static void setpoint(PLINT, PLINT);

/* top level declarations */

#define JETX     1409
#define JETY     1103

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
static PLINT orient;

/* (dev) will get passed in eventually, so this looks weird right now */

static PLDev device;
static PLDev *dev = &device;

/*----------------------------------------------------------------------*\
* jet_init()
*
* Initialize device.
\*----------------------------------------------------------------------*/

void
jet_init(PLStream *pls)
{
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

/* Set up device parameters */

    dev->xold = UNDEFINED;
    dev->yold = UNDEFINED;
    dev->xmin = 0;
    dev->ymin = 0;

    setpxl((PLFLT) 5.905, (PLFLT) 5.905);

/* Because portrait mode addressing is used by the LJII, we need to
   rotate by 90 degrees to get the right mapping. */

    dev->xmin = 0;
    dev->ymin = 0;

    orient = pls->orient + 1;
    if (orient%2 == 1) {
	dev->xmax = JETY;
	dev->ymax = JETX;
    }
    else {
	dev->xmax = JETX;
	dev->ymax = JETY;
    }

    dev->xlen = dev->xmax - dev->xmin;
    dev->ylen = dev->ymax - dev->ymin;

    setphy(dev->xmin, dev->xmax, dev->ymin, dev->ymax);

/* Allocate storage for bit map matrix */

#ifdef MSDOS
    if ((bitmap = (char _HUGE *) halloc((long) NBYTES, sizeof(char))) == NULL)
	plexit("Out of memory in call to calloc");
#else
    if ((bitmap = (char *) calloc(NBYTES, sizeof(char))) == NULL)
	plexit("Out of memory in call to calloc");
#endif

/* Reset Printer */

    fprintf(pls->OutFile, "%cE", ESC);
}

/*----------------------------------------------------------------------*\
* jet_line()
*
* Draw a line in the current color from (x1,y1) to (x2,y2).
\*----------------------------------------------------------------------*/

void
jet_line(PLStream *pls, short x1a, short y1a, short x2a, short y2a)
{
    int i;
    int x1 = x1a, y1 = y1a, x2 = x2a, y2 = y2a;
    PLINT x1b, y1b, x2b, y2b;
    float length, fx, fy, dx, dy;

/* Because portrait mode addressing is used here, we need to complement
   the orientation flag to get the right mapping. */

    orient = pls->orient + 1;
    plRotPhy(orient, dev, &x1, &y1, &x2, &y2);

    if (pls->pscale)
	plSclPhy(pls, dev, &x1, &y1, &x2, &y2);

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
* jet_polyline()
*
* Draw a polyline in the current color.
\*----------------------------------------------------------------------*/

void
jet_polyline(PLStream *pls, short *xa, short *ya, PLINT npts)
{
    PLINT i;

    for (i = 0; i < npts - 1; i++)
	jet_line(pls, xa[i], ya[i], xa[i + 1], ya[i + 1]);
}

/*----------------------------------------------------------------------*\
* jet_clear()
*
* Clear page (prints it here).
\*----------------------------------------------------------------------*/

void
jet_clear(PLStream *pls)
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
	fprintf(pls->OutFile, "%c*b%dW", ESC, BPROW);
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
* jet_page()
*
* Set up for the next page.
* Advance to next family file if necessary (file output).
\*----------------------------------------------------------------------*/

void
jet_page(PLStream *pls)
{
    if (!pls->termin)
	plGetFam(pls);

    pls->page++;
}

/*----------------------------------------------------------------------*\
* jet_tidy()
*
* Close graphics file or otherwise clean up.
\*----------------------------------------------------------------------*/

void
jet_tidy(PLStream *pls)
{
/* Reset Printer */

    fprintf(pls->OutFile, "%cE", ESC);
    fclose(pls->OutFile);
    free((char *) bitmap);
    pls->fileset = 0;
    pls->page = 0;
    pls->OutFile = NULL;
}

/*----------------------------------------------------------------------*\
* jet_color()
*
* Set pen color.
\*----------------------------------------------------------------------*/

void
jet_color(PLStream *pls)
{
}

/*----------------------------------------------------------------------*\
* jet_text()
*
* Switch to text mode.
\*----------------------------------------------------------------------*/

void
jet_text(PLStream *pls)
{
}

/*----------------------------------------------------------------------*\
* jet_graph()
*
* Switch to graphics mode.
\*----------------------------------------------------------------------*/

void
jet_graph(PLStream *pls)
{
}

/*----------------------------------------------------------------------*\
* jet_width()
*
* Set pen width.
\*----------------------------------------------------------------------*/

void
jet_width(PLStream *pls)
{
}

/*----------------------------------------------------------------------*\
* jet_esc()
*
* Escape function.
\*----------------------------------------------------------------------*/

void
jet_esc(PLStream *pls, PLINT op, char *ptr)
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
