/* $Id$
   $Log$
   Revision 1.1  1992/05/20 21:32:44  furnish
   Initial checkin of the whole PLPLOT project.

*/

/*	xfig.c

	PLPLOT xfig device driver.
*/
#ifdef XFIG

#include <stdio.h>
#include "plplot.h"
#include "dispatch.h"

/* Function prototypes */

static void flushbuffer	PLARGS((PLStream *));

/* top level declarations */

#define FIGX        599
#define FIGY        599
#define DPI          80
#define BSIZE  25

static short *buffptr, bufflen;
static short count;
static int curwid;
static int firstline = 1;

/* (dev) will get passed in eventually, so this looks weird right now */

static PLDev device;
static PLDev *dev = &device;

/*----------------------------------------------------------------------*\
* xfiginit()
*
* Initialize device.
\*----------------------------------------------------------------------*/

void
xfiginit(pls)
PLStream *pls;
{
    pls->termin = 0;		/* not an interactive terminal */
    pls->color = 1;
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
* xfigline()
*
* Draw a line in the current color from (x1,y1) to (x2,y2).
\*----------------------------------------------------------------------*/

void
xfigline(pls, x1a, y1a, x2a, y2a)
PLStream *pls;
PLINT x1a, y1a, x2a, y2a;
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
* xfigclear()
*
* Clear page.
\*----------------------------------------------------------------------*/

void
xfigclear(pls)
PLStream *pls;
{
    if (!firstline)
	flushbuffer(pls);
}

/*----------------------------------------------------------------------*\
* xfigpage()
*
* Set up for the next page.
* Advance to next family file if necessary (file output).
\*----------------------------------------------------------------------*/

void
xfigpage(pls)
PLStream *pls;
{
    dev->xold = UNDEFINED;
    dev->yold = UNDEFINED;
    firstline = 1;

    if (!pls->termin)
	plGetFam(pls);

    pls->page++;
}

/*----------------------------------------------------------------------*\
* xfigadv()
*
* Advance to the next page.
\*----------------------------------------------------------------------*/

void
xfigadv(pls)
PLStream *pls;
{
    xfigclear(pls);
    xfigpage(pls);
}

/*----------------------------------------------------------------------*\
* xfigtidy()
*
* Close graphics file or otherwise clean up.
\*----------------------------------------------------------------------*/

void
xfigtidy(pls)
PLStream *pls;
{
    flushbuffer(pls);
    free((char *) buffptr);
    fclose(pls->OutFile);
    pls->fileset = 0;
    pls->page = 0;
    pls->OutFile = NULL;
}

/*----------------------------------------------------------------------*\
* xfigcolor()
*
* Set pen color.
\*----------------------------------------------------------------------*/

void
xfigcolor(pls)
PLStream *pls;
{
}

/*----------------------------------------------------------------------*\
* xfigtext()
*
* Switch to text mode.
\*----------------------------------------------------------------------*/

void
xfigtext(pls)
PLStream *pls;
{
}

/*----------------------------------------------------------------------*\
* xfiggraph()
*
* Switch to graphics mode.
\*----------------------------------------------------------------------*/

void
xfiggraph(pls)
PLStream *pls;
{
}

/*----------------------------------------------------------------------*\
* xfigwidth()
*
* Set pen width.
\*----------------------------------------------------------------------*/

void
xfigwidth(pls)
PLStream *pls;
{
    flushbuffer(pls);
    firstline = 1;

    if (pls->width <= 1)
	curwid = 1;
    else if (pls->width >= 4)
	curwid = 3;
    else
	curwid = pls->width;
}

/*----------------------------------------------------------------------*\
* xfigesc()
*
* Escape function.
\*----------------------------------------------------------------------*/

void
xfigesc(pls, op, ptr)
PLStream *pls;
PLINT op;
char *ptr;
{
}

/*----------------------------------------------------------------------*\
* Utility functions.
\*----------------------------------------------------------------------*/

static void 
flushbuffer(pls)
PLStream *pls;
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
#endif	/* XFIG */
