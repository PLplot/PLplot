/* $Id$
 * $Log$
 * Revision 1.16  1995/01/06 07:40:36  mjl
 * All drivers: pls->width now more sensibly handled.  If the driver supports
 * multiple widths, it first checks to see if it has been initialized
 * already (e.g. from the command line) before initializing it.  For drivers
 * that don't support multiple widths, pls->width is ignored.
 *
 * Revision 1.15  1994/07/19  22:30:30  mjl
 * All device drivers: enabling macro renamed to PLD_<driver>, where <driver>
 * is xwin, ps, etc.  See plDevs.h for more detail.
 *
 * Revision 1.14  1994/03/23  06:34:34  mjl
 * All drivers: cleaned up by eliminating extraneous includes (stdio.h and
 * stdlib.h now included automatically by plplotP.h), extraneous clears
 * of pls->fileset, pls->page, and pls->OutFile = NULL (now handled in
 * driver interface or driver initialization as appropriate).  Special
 * handling for malloc includes eliminated (no longer needed) and malloc
 * prototypes fixed as necessary.
 *
 * Revision 1.13  1993/12/06  22:37:31  mjl
 * Fixed zero line width bug (thanks to Paul Kirschner)
 *
 * Revision 1.12  1993/07/31  07:56:46  mjl
 * Several driver functions consolidated, for all drivers.  The width and color
 * commands are now part of a more general "state" command.  The text and
 * graph commands used for switching between modes is now handled by the
 * escape function (very few drivers require it).  The device-specific PLDev
 * structure is now malloc'ed for each driver that requires it, and freed when
 * the stream is terminated.
*/

/*	xfig.c

	PLPLOT xfig device driver.
*/
#include "plDevs.h"

#ifdef PLD_xfig

#include "plplotP.h"
#include "drivers.h"

/* Function prototypes */
/* INDENT OFF */

static void flushbuffer(PLStream *);

/* top level declarations */

#define FIGX	599
#define FIGY	599
#define DPI	80
#define BSIZE	25

static short *buffptr, bufflen;
static short count;
static int curwid = 1;
static int firstline = 1;

/* INDENT ON */
/*----------------------------------------------------------------------*\
* plD_init_xfig()
*
* Initialize device.
\*----------------------------------------------------------------------*/

void
plD_init_xfig(PLStream *pls)
{
    PLDev *dev;

    pls->termin = 0;		/* not an interactive terminal */
    pls->icol0 = 1;
    pls->color = 0;
    pls->bytecnt = 0;
    pls->page = 0;

    if (pls->width == 0)	/* Is 0 if uninitialized */
	pls->width = 1;

/* Initialize family file info */

    plFamInit(pls);

/* Prompt for a file name if not already set */

    plOpenFile(pls);

/* Allocate and initialize device-specific data */

    dev = plAllocDev(pls);

    dev->xold = UNDEFINED;
    dev->yold = UNDEFINED;
    dev->xmin = 0;
    dev->xmax = FIGX;
    dev->ymin = 0;
    dev->ymax = FIGY;

    plP_setpxl(3.1496, 3.1496);	/* 80 DPI */

    plP_setphy(0, FIGX, 0, FIGY);

    /* Write out header */

    fprintf(pls->OutFile, "#FIG 1.4X\n");
    fprintf(pls->OutFile, "%d 2\n", DPI);

    bufflen = 2 * BSIZE;
    buffptr = (short *) malloc(sizeof(short) * bufflen);
    if (buffptr == NULL)
	plexit("Out of memory!");
}

/*----------------------------------------------------------------------*\
* plD_line_xfig()
*
* Draw a line in the current color from (x1,y1) to (x2,y2).
\*----------------------------------------------------------------------*/

void
plD_line_xfig(PLStream *pls, short x1a, short y1a, short x2a, short y2a)
{
    PLDev *dev = (PLDev *) pls->dev;
    int x1 = x1a, y1 = y1a, x2 = x2a, y2 = y2a;
    short *tempptr;

    /* If starting point of this line is the same as the ending point of */
    /* the previous line then don't raise the pen. (This really speeds up */
    /* plotting and reduces the size of the file. */

    if (firstline) {
	count = 0;
	*(buffptr + count++) = x1;
	*(buffptr + count++) = y1;
	*(buffptr + count++) = x2;
	*(buffptr + count++) = y2;
	firstline = 0;
    }
    else if (x1 == dev->xold && y1 == dev->yold) {
	if (count + 2 >= bufflen) {
	    bufflen += 2 * BSIZE;
	    tempptr = (short *)
		realloc((void *) buffptr, bufflen * sizeof(short));
	    if (tempptr == NULL) {
		free((void *) buffptr);
		plexit("Out of memory!");
	    }
	    buffptr = tempptr;
	}
	*(buffptr + count++) = x2;
	*(buffptr + count++) = y2;
    }
    else {
	flushbuffer(pls);
	*(buffptr + count++) = x1;
	*(buffptr + count++) = y1;
	*(buffptr + count++) = x2;
	*(buffptr + count++) = y2;
    }
    dev->xold = x2;
    dev->yold = y2;
}

/*----------------------------------------------------------------------*\
* plD_polyline_xfig()
*
* Draw a polyline in the current color.
\*----------------------------------------------------------------------*/

void
plD_polyline_xfig(PLStream *pls, short *xa, short *ya, PLINT npts)
{
    PLINT i;

    for (i = 0; i < npts - 1; i++)
	plD_line_xfig(pls, xa[i], ya[i], xa[i + 1], ya[i + 1]);
}

/*----------------------------------------------------------------------*\
* plD_eop_xfig()
*
* End of page.
\*----------------------------------------------------------------------*/

void
plD_eop_xfig(PLStream *pls)
{
    if (!firstline)
	flushbuffer(pls);
}

/*----------------------------------------------------------------------*\
* plD_bop_xfig()
*
* Set up for the next page.
* Advance to next family file if necessary (file output).
\*----------------------------------------------------------------------*/

void
plD_bop_xfig(PLStream *pls)
{
    PLDev *dev = (PLDev *) pls->dev;

    dev->xold = UNDEFINED;
    dev->yold = UNDEFINED;
    firstline = 1;

    if (!pls->termin)
	plGetFam(pls);

    pls->page++;
}

/*----------------------------------------------------------------------*\
* plD_tidy_xfig()
*
* Close graphics file or otherwise clean up.
\*----------------------------------------------------------------------*/

void
plD_tidy_xfig(PLStream *pls)
{
    flushbuffer(pls);
    free((void *) buffptr);
    fclose(pls->OutFile);
}

/*----------------------------------------------------------------------*\
* plD_state_xfig()
*
* Handle change in PLStream state (color, pen width, fill attribute, etc).
\*----------------------------------------------------------------------*/

void 
plD_state_xfig(PLStream *pls, PLINT op)
{
    switch (op) {

    case PLSTATE_WIDTH:
	flushbuffer(pls);
	firstline = 1;

	if (pls->width <= 1)
	    curwid = 1;
	else if (pls->width >= 4)
	    curwid = 3;
	else
	    curwid = (int) pls->width;

	break;

    case PLSTATE_COLOR0:
	break;

    case PLSTATE_COLOR1:
	break;
    }
}

/*----------------------------------------------------------------------*\
* plD_esc_xfig()
*
* Escape function.
\*----------------------------------------------------------------------*/

void
plD_esc_xfig(PLStream *pls, PLINT op, void *ptr)
{
}

/*----------------------------------------------------------------------*\
* Utility functions.
\*----------------------------------------------------------------------*/

static void
flushbuffer(PLStream *pls)
{
    short i = 0;

    if (count == 0)
	return;

    fprintf(pls->OutFile, "2 1 0 %d 0 0 0 0 0.000 0 0\n", curwid);
    while (i < count) {
	fprintf(pls->OutFile, "%d %d ", *(buffptr + i),
		FIGY - *(buffptr + i + 1));
	i += 2;
    }
    fprintf(pls->OutFile, "9999 9999\n");
    count = 0;
}

#else
int 
pldummy_xfig()
{
    return 0;
}

#endif				/* PLD_xfig */
