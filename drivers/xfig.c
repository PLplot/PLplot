/* $Id$
   $Log$
   Revision 1.11  1993/07/16 22:11:23  mjl
   Eliminated low-level coordinate scaling; now done by driver interface.

 * Revision 1.10  1993/07/01  21:59:45  mjl
 * Changed all plplot source files to include plplotP.h (private) rather than
 * plplot.h.  Rationalized namespace -- all externally-visible plplot functions
 * now start with "pl"; device driver functions start with "plD_".
 *
 * Revision 1.9  1993/03/15  21:39:22  mjl
 * Changed all _clear/_page driver functions to the names _eop/_bop, to be
 * more representative of what's actually going on.
 *
 * Revision 1.8  1993/03/03  19:42:09  mjl
 * Changed PLSHORT -> short everywhere; now all device coordinates are expected
 * to fit into a 16 bit address space (reasonable, and good for performance).
 *
 * Revision 1.7  1993/02/22  23:11:03  mjl
 * Eliminated the plP_adv() driver calls, as these were made obsolete by
 * recent changes to plmeta and plrender.  Also eliminated page clear commands
 * from plP_tidy() -- plend now calls plP_clr() and plP_tidy() explicitly.
 *
 * Revision 1.6  1993/01/23  05:41:54  mjl
 * Changes to support new color model, polylines, and event handler support
 * (interactive devices only).
 *
 * Revision 1.5  1992/11/07  07:48:49  mjl
 * Fixed orientation operation in several files and standardized certain startup
 * operations. Fixed bugs in various drivers.
 *
 * Revision 1.4  1992/10/22  17:04:58  mjl
 * Fixed warnings, errors generated when compling with HP C++.
 *
 * Revision 1.3  1992/09/30  18:24:59  furnish
 * Massive cleanup to irradicate garbage code.  Almost everything is now
 * prototyped correctly.  Builds on HPUX, SUNOS (gcc), AIX, and UNICOS.
 *
 * Revision 1.2  1992/09/29  04:44:50  furnish
 * Massive clean up effort to remove support for garbage compilers (K&R).
 *
 * Revision 1.1  1992/05/20  21:32:44  furnish
 * Initial checkin of the whole PLPLOT project.
 *
*/

/*	xfig.c

	PLPLOT xfig device driver.
*/
#ifdef XFIG

#define PL_NEED_MALLOC
#include "plplotP.h"
#include <stdio.h>
#include <stdlib.h>

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
static int curwid;
static int firstline = 1;

/* (dev) will get passed in eventually, so this looks weird right now */

static PLDev device;
static PLDev *dev = &device;

/* INDENT ON */
/*----------------------------------------------------------------------*\
* plD_init_xfig()
*
* Initialize device.
\*----------------------------------------------------------------------*/

void
plD_init_xfig(PLStream *pls)
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
	    tempptr = (short *) realloc((char *) buffptr, bufflen * sizeof(short));
	    if (tempptr == NULL) {
		free((char *) buffptr);
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
    free((char *) buffptr);
    fclose(pls->OutFile);
    pls->fileset = 0;
    pls->page = 0;
    pls->OutFile = NULL;
}

/*----------------------------------------------------------------------*\
* plD_color_xfig()
*
* Set pen color.
\*----------------------------------------------------------------------*/

void
plD_color_xfig(PLStream *pls)
{
}

/*----------------------------------------------------------------------*\
* plD_text_xfig()
*
* Switch to text mode.
\*----------------------------------------------------------------------*/

void
plD_text_xfig(PLStream *pls)
{
}

/*----------------------------------------------------------------------*\
* plD_graph_xfig()
*
* Switch to graphics mode.
\*----------------------------------------------------------------------*/

void
plD_graph_xfig(PLStream *pls)
{
}

/*----------------------------------------------------------------------*\
* plD_width_xfig()
*
* Set pen width.
\*----------------------------------------------------------------------*/

void
plD_width_xfig(PLStream *pls)
{
    flushbuffer(pls);
    firstline = 1;

    if (pls->width <= 1)
	curwid = 1;
    else if (pls->width >= 4)
	curwid = 3;
    else
	curwid = (int) pls->width;
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

#endif				/* XFIG */
