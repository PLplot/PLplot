/* $Id$
   $Log$
   Revision 1.4  1993/01/23 05:41:39  mjl
   Changes to support new color model, polylines, and event handler support
   (interactive devices only).

 * Revision 1.3  1992/11/07  07:48:37  mjl
 * Fixed orientation operation in several files and standardized certain startup
 * operations. Fixed bugs in various drivers.
 *
 * Revision 1.2  1992/09/29  04:44:39  furnish
 * Massive clean up effort to remove support for garbage compilers (K&R).
 *
 * Revision 1.1  1992/05/20  21:32:32  furnish
 * Initial checkin of the whole PLPLOT project.
 *
*/

/*
	dosvga.c
	Geoffrey Furnish
	5-1-91
	
	This file constitutes the driver for a VGA display under DOS or
	OS/2 DOS mode.  It is based on the xterm driver.

	UPDATE: 5-3-91
	It became apparent during my initial day of development with this
	software that the structure of the Xterm driver was too pathetic
	for description in polite company.  At this point I am performing a
	massive overhaul of the entire basic structure.  Hoepfully the result
	will be usable.

	NOTE:
	This file is intended to be compiled with Microsoft C or QuickC.
	I use graphics functions from their library, and I use some MS
	language extensions (like //).

	The Geoffrey Furnish Standard Disclaimer:
	"I hate any C compiler that isn't ANSI compliant, and I refuse to waste
	my time trying to support such garbage.  If you can't compile with an
	ANSI compiler, then don't expect this to work.  No appologies,
	now or ever."
*/
#ifdef MSDOS			/* Only compile for MSDOS */

#include <stdio.h>
#include "plplot.h"
#include "drivers.h"

#include <graph.h>
#include <conio.h>

/* Prototypes:  This should only be compiled by DOS or OS/2 compilers, so
	I think we can safely forget about non-ANSI compilers raising their
	ugly heads here... */

static void pause(void);

#define VGAX	639
#define VGAY	479

/* A flag to tell us whether we are in text or graphics mode */

#define TEXT_MODE 0
#define GRAPHICS_MODE 1

/* gmf; should probably query this on start up... Maybe later. */

static int mode = TEXT_MODE;

#define CLEAN 0
#define DIRTY 1

static page_state;

/* (dev) will get passed in eventually, so this looks weird right now */

static PLDev device;
static PLDev *dev = &device;

/*----------------------------------------------------------------------*\
* vgainit()
*
* Initialize device.
\*----------------------------------------------------------------------*/

void
vgainit(PLStream *pls)
{
    pls->termin = 1;		/* is an interactive terminal */
    pls->icol0 = 1;
    pls->width = 1;
    pls->bytecnt = 0;
    pls->page = 0;
    pls->graphx = TEXT_MODE;

    if (!pls->colorset)
	pls->color = 1;

/* Set up device parameters */

    dev->xold = UNDEFINED;
    dev->yold = UNDEFINED;
    dev->xmin = 0;
    dev->xmax = VGAX;
    dev->ymin = 0;
    dev->ymax = VGAY;

    setpxl(2.5, 2.5);		/* My best guess.  Seems to work okay. */

    setphy((PLINT) 0, (PLINT) VGAX, (PLINT) 0, (PLINT) VGAY);

    vgagraph(pls);
}

/*----------------------------------------------------------------------*\
* vgaline()
*
* Draw a line in the current color from (x1,y1) to (x2,y2).
\*----------------------------------------------------------------------*/

void
vgaline(PLStream *pls, PLSHORT x1a, PLSHORT y1a, PLSHORT x2a, PLSHORT y2a)
{
    int x1 = x1a, y1 = y1a, x2 = x2a, y2 = y2a;

    if (pls->pscale)
	plSclPhy(pls, dev, &x1, &y1, &x2, &y2);

    y1 = VGAY - y1;
    y2 = VGAY - y2;

    _moveto(x1, y1);
    _lineto(x2, y2);

    page_state = DIRTY;
}

/*----------------------------------------------------------------------*\
* vgapolyline()
*
* Draw a polyline in the current color.
\*----------------------------------------------------------------------*/

void
vgapolyline(PLStream *pls, PLSHORT *xa, PLSHORT *ya, PLINT npts)
{
    PLINT i;

    for (i = 0; i < npts - 1; i++)
	vgaline(pls, xa[i], ya[i], xa[i + 1], ya[i + 1]);
}

/*----------------------------------------------------------------------*\
* vgaclear()
*
* Clear page.
\*----------------------------------------------------------------------*/

void
vgaclear(PLStream *pls)
{
    if (page_state == DIRTY)
	pause();
    _clearscreen(_GCLEARSCREEN);
    page_state = CLEAN;
}

/*----------------------------------------------------------------------*\
* vgapage()
*
* Set up for the next page.
* Advance to next family file if necessary (file output).
\*----------------------------------------------------------------------*/

void
vgapage(PLStream *pls)
{
    pls->page++;
    vgaclear(pls);
}

/*----------------------------------------------------------------------*\
* vgaadv()
*
* Advance to the next page.
\*----------------------------------------------------------------------*/

void
vgaadv(PLStream *pls)
{
    vgaclear(pls);
    vgapage(pls);
}

/*----------------------------------------------------------------------*\
* vgatidy()
*
* Close graphics file or otherwise clean up.
\*----------------------------------------------------------------------*/

void
vgatidy(PLStream *pls)
{
    vgatext(pls);
    pls->page = 0;
    pls->OutFile = NULL;
}

/*----------------------------------------------------------------------*\
* vgacolor()
*
* Set pen color.
\*----------------------------------------------------------------------*/

void
vgacolor(PLStream *pls)
{
    static long cmap[16] = {
	_WHITE, _RED, _LIGHTYELLOW, _GREEN,
	_CYAN, _WHITE, _WHITE, _GRAY,
	_WHITE, _BLUE, _GREEN, _CYAN,
	_RED, _MAGENTA, _LIGHTYELLOW, _WHITE
    };

    if (pls->icol0 < 0 || pls->icol0 > 15)
	pls->icol0 = 15;

    _remappalette((short) pls->icol0, cmap[pls->icol0]);
    _setcolor((short) pls->icol0);
}

/*----------------------------------------------------------------------*\
* vgatext()
*
* Switch to text mode.
\*----------------------------------------------------------------------*/

void
vgatext(PLStream *pls)
{
    if (pls->graphx == GRAPHICS_MODE) {
	if (page_state == DIRTY)
	    pause();
	_setvideomode(_DEFAULTMODE);
	pls->graphx = TEXT_MODE;
    }
}

/*----------------------------------------------------------------------*\
* vgagraph()
*
* Switch to graphics mode.
\*----------------------------------------------------------------------*/

void
vgagraph(PLStream *pls)
{
    if (pls->graphx == TEXT_MODE) {
	if (!_setvideomode(_VRES16COLOR)) {
	    printf("Unable to set graphics mode.");
	    exit(0);
	}
	pls->graphx = GRAPHICS_MODE;
	page_state = CLEAN;
    }
}

/*----------------------------------------------------------------------*\
* vgawidth()
*
* Set pen width.
\*----------------------------------------------------------------------*/

void
vgawidth(PLStream *pls)
{
}

/*----------------------------------------------------------------------*\
* vgaesc()
*
* Escape function.
\*----------------------------------------------------------------------*/

void
vgaesc(PLStream *pls, PLINT op, char *ptr)
{
}

/*----------------------------------------------------------------------*\
* pause()
*
* Wait for a keystroke.
\*----------------------------------------------------------------------*/

static void
pause(void)
{
    _settextposition(0, 0);
    _outtext("pause->");
    while (!kbhit());
    getch();
}

#else
int 
pldummy_dosvga()
{
    return 0;
}

#endif				/* MSDOS */
