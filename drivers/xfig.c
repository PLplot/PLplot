/* $Id$
   $Log$
   Revision 1.6  1993/01/23 05:41:54  mjl
   Changes to support new color model, polylines, and event handler support
   (interactive devices only).

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
#include "plplot.h"
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
* xfig_init()
*
* Initialize device.
\*----------------------------------------------------------------------*/

void
xfig_init(PLStream *pls)
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

    setpxl(3.1496, 3.1496);	/* 80 DPI */

    setphy(0, FIGX, 0, FIGY);

    /* Write out header */

    fprintf(pls->OutFile, "#FIG 1.4X\n");
    fprintf(pls->OutFile, "%d 2\n", DPI);

    bufflen = 2 * BSIZE;
    buffptr = (short *) malloc(sizeof(short) * bufflen);
    if (buffptr == NULL)
	plexit("Out of memory!");
}

/*----------------------------------------------------------------------*\
* xfig_line()
*
* Draw a line in the current color from (x1,y1) to (x2,y2).
\*----------------------------------------------------------------------*/

void
xfig_line(PLStream *pls, PLSHORT x1a, PLSHORT y1a, PLSHORT x2a, PLSHORT y2a)
{
    int x1 = x1a, y1 = y1a, x2 = x2a, y2 = y2a;
    short *tempptr;

    if (pls->pscale)
	plSclPhy(pls, dev, &x1, &y1, &x2, &y2);

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
* xfig_polyline()
*
* Draw a polyline in the current color.
\*----------------------------------------------------------------------*/

void
xfig_polyline(PLStream *pls, PLSHORT *xa, PLSHORT *ya, PLINT npts)
{
    PLINT i;

    for (i = 0; i < npts - 1; i++)
	xfig_line(pls, xa[i], ya[i], xa[i + 1], ya[i + 1]);
}

/*----------------------------------------------------------------------*\
* xfig_clear()
*
* Clear page.
\*----------------------------------------------------------------------*/

void
xfig_clear(PLStream *pls)
{
    if (!firstline)
	flushbuffer(pls);
}

/*----------------------------------------------------------------------*\
* xfig_page()
*
* Set up for the next page.
* Advance to next family file if necessary (file output).
\*----------------------------------------------------------------------*/

void
xfig_page(PLStream *pls)
{
    dev->xold = UNDEFINED;
    dev->yold = UNDEFINED;
    firstline = 1;

    if (!pls->termin)
	plGetFam(pls);

    pls->page++;
}

/*----------------------------------------------------------------------*\
* xfig_adv()
*
* Advance to the next page.
\*----------------------------------------------------------------------*/

void
xfig_adv(PLStream *pls)
{
    xfig_clear(pls);
    xfig_page(pls);
}

/*----------------------------------------------------------------------*\
* xfig_tidy()
*
* Close graphics file or otherwise clean up.
\*----------------------------------------------------------------------*/

void
xfig_tidy(PLStream *pls)
{
    flushbuffer(pls);
    free((char *) buffptr);
    fclose(pls->OutFile);
    pls->fileset = 0;
    pls->page = 0;
    pls->OutFile = NULL;
}

/*----------------------------------------------------------------------*\
* xfig_color()
*
* Set pen color.
\*----------------------------------------------------------------------*/

void
xfig_color(PLStream *pls)
{
}

/*----------------------------------------------------------------------*\
* xfig_text()
*
* Switch to text mode.
\*----------------------------------------------------------------------*/

void
xfig_text(PLStream *pls)
{
}

/*----------------------------------------------------------------------*\
* xfig_graph()
*
* Switch to graphics mode.
\*----------------------------------------------------------------------*/

void
xfig_graph(PLStream *pls)
{
}

/*----------------------------------------------------------------------*\
* xfig_width()
*
* Set pen width.
\*----------------------------------------------------------------------*/

void
xfig_width(PLStream *pls)
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
* xfig_esc()
*
* Escape function.
\*----------------------------------------------------------------------*/

void
xfig_esc(PLStream *pls, PLINT op, char *ptr)
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
