/* $Id$
   $Log$
   Revision 1.10  1993/07/16 22:11:14  mjl
   Eliminated low-level coordinate scaling; now done by driver interface.

 * Revision 1.9  1993/07/01  21:59:32  mjl
 * Changed all plplot source files to include plplotP.h (private) rather than
 * plplot.h.  Rationalized namespace -- all externally-visible plplot functions
 * now start with "pl"; device driver functions start with "plD_".
 *
 * Revision 1.8  1993/03/15  21:39:04  mjl
 * Changed all _clear/_page driver functions to the names _eop/_bop, to be
 * more representative of what's actually going on.
 *
 * Revision 1.7  1993/03/03  19:41:54  mjl
 * Changed PLSHORT -> short everywhere; now all device coordinates are expected
 * to fit into a 16 bit address space (reasonable, and good for performance).
 *
 * Revision 1.6  1993/02/27  04:46:31  mjl
 * Fixed errors in ordering of header file inclusion.  "plplotP.h" should
 * always be included first.
 *
 * Revision 1.5  1993/02/22  23:10:50  mjl
 * Eliminated the plP_adv() driver calls, as these were made obsolete by
 * recent changes to plmeta and plrender.  Also eliminated page clear commands
 * from plP_tidy() -- plend now calls plP_clr() and plP_tidy() explicitly.
 *
 * Revision 1.4  1993/01/23  05:41:39  mjl
 * Changes to support new color model, polylines, and event handler support
 * (interactive devices only).
 *
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

#include "plplotP.h"
#include <stdio.h>
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
* plD_init_vga()
*
* Initialize device.
\*----------------------------------------------------------------------*/

void
plD_init_vga(PLStream *pls)
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

    plP_setpxl(2.5, 2.5);		/* My best guess.  Seems to work okay. */

    plP_setphy((PLINT) 0, (PLINT) VGAX, (PLINT) 0, (PLINT) VGAY);

    plD_graph_vga(pls);
}

/*----------------------------------------------------------------------*\
* plD_line_vga()
*
* Draw a line in the current color from (x1,y1) to (x2,y2).
\*----------------------------------------------------------------------*/

void
plD_line_vga(PLStream *pls, short x1a, short y1a, short x2a, short y2a)
{
    int x1 = x1a, y1 = y1a, x2 = x2a, y2 = y2a;

    y1 = VGAY - y1;
    y2 = VGAY - y2;

    _moveto(x1, y1);
    _lineto(x2, y2);

    page_state = DIRTY;
}

/*----------------------------------------------------------------------*\
* plD_polyline_vga()
*
* Draw a polyline in the current color.
\*----------------------------------------------------------------------*/

void
plD_polyline_vga(PLStream *pls, short *xa, short *ya, PLINT npts)
{
    PLINT i;

    for (i = 0; i < npts - 1; i++)
	plD_line_vga(pls, xa[i], ya[i], xa[i + 1], ya[i + 1]);
}

/*----------------------------------------------------------------------*\
* plD_eop_vga()
*
* End of page.
\*----------------------------------------------------------------------*/

void
plD_eop_vga(PLStream *pls)
{
    if (page_state == DIRTY)
	pause();
    _eopscreen(_GCLEARSCREEN);
    page_state = CLEAN;
}

/*----------------------------------------------------------------------*\
* plD_bop_vga()
*
* Set up for the next page.
* Advance to next family file if necessary (file output).
\*----------------------------------------------------------------------*/

void
plD_bop_vga(PLStream *pls)
{
    pls->page++;
    plD_eop_vga(pls);
}

/*----------------------------------------------------------------------*\
* plD_tidy_vga()
*
* Close graphics file or otherwise clean up.
\*----------------------------------------------------------------------*/

void
plD_tidy_vga(PLStream *pls)
{
    plD_text_vga(pls);
    pls->page = 0;
    pls->OutFile = NULL;
}

/*----------------------------------------------------------------------*\
* plD_color_vga()
*
* Set pen color.
\*----------------------------------------------------------------------*/

void
plD_color_vga(PLStream *pls)
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
* plD_text_vga()
*
* Switch to text mode.
\*----------------------------------------------------------------------*/

void
plD_text_vga(PLStream *pls)
{
    if (pls->graphx == GRAPHICS_MODE) {
	if (page_state == DIRTY)
	    pause();
	_setvideomode(_DEFAULTMODE);
	pls->graphx = TEXT_MODE;
    }
}

/*----------------------------------------------------------------------*\
* plD_graph_vga()
*
* Switch to graphics mode.
\*----------------------------------------------------------------------*/

void
plD_graph_vga(PLStream *pls)
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
* plD_width_vga()
*
* Set pen width.
\*----------------------------------------------------------------------*/

void
plD_width_vga(PLStream *pls)
{
}

/*----------------------------------------------------------------------*\
* plD_esc_vga()
*
* Escape function.
\*----------------------------------------------------------------------*/

void
plD_esc_vga(PLStream *pls, PLINT op, void *ptr)
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
