/* $Id$
   $Log$
   Revision 1.13  1994/04/08 11:35:56  mjl
   Put nopause support back into the drivers where it is better off.
   I don't know WHAT I was thinking.

 * Revision 1.12  1994/03/23  06:34:21  mjl
 * All drivers: cleaned up by eliminating extraneous includes (stdio.h and
 * stdlib.h now included automatically by plplotP.h), extraneous clears
 * of pls->fileset, pls->page, and pls->OutFile = NULL (now handled in
 * driver interface or driver initialization as appropriate).  Special
 * handling for malloc includes eliminated (no longer needed) and malloc
 * prototypes fixed as necessary.
 *
 * Revision 1.11  1993/07/31  07:56:28  mjl
 * Several driver functions consolidated, for all drivers.  The width and color
 * commands are now part of a more general "state" command.  The text and
 * graph commands used for switching between modes is now handled by the
 * escape function (very few drivers require it).  The device-specific PLDev
 * structure is now malloc'ed for each driver that requires it, and freed when
 * the stream is terminated.
 *
 * Revision 1.10  1993/07/16  22:11:14  mjl
 * Eliminated low-level coordinate scaling; now done by driver interface.
 *
 * Revision 1.9  1993/07/01  21:59:32  mjl
 * Changed all plplot source files to include plplotP.h (private) rather than
 * plplot.h.  Rationalized namespace -- all externally-visible plplot functions
 * now start with "pl"; device driver functions start with "plD_".
*/

/*
	dosvga.c
	Geoffrey Furnish
	5-1-91
	
	This file constitutes the driver for a VGA display under DOS or
	OS/2 DOS mode.  It is based on the xterm driver.

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
#include "drivers.h"
#include <graph.h>
#include <conio.h>

/* Prototypes:  This should only be compiled by DOS or OS/2 compilers, so
	I think we can safely forget about non-ANSI compilers raising their
	ugly heads here... */

static void	pause		(PLStream *);
static void	vga_text	(PLStream *);
static void	vga_graph	(PLStream *);

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

    plP_setpxl(2.5, 2.5);	/* My best guess.  Seems to work okay. */

    plP_setphy((PLINT) 0, (PLINT) VGAX, (PLINT) 0, (PLINT) VGAY);

    vga_graph(pls);
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
	pause(pls);
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
    vga_text(pls);
}

/*----------------------------------------------------------------------*\
* plD_state_vga()
*
* Handle change in PLStream state (color, pen width, fill attribute, etc).
\*----------------------------------------------------------------------*/

void 
plD_state_vga(PLStream *pls, PLINT op)
{
    switch (op) {

    case PLSTATE_WIDTH:
	break;

    case PLSTATE_COLOR0:{
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
	break;
    }

    case PLSTATE_COLOR1:
	break;
    }
}

/*----------------------------------------------------------------------*\
* plD_esc_vga()
*
* Escape function.
\*----------------------------------------------------------------------*/

void
plD_esc_vga(PLStream *pls, PLINT op, void *ptr)
{
    switch (op) {

      case PLESC_TEXT:
	vga_text(pls);
	break;

      case PLESC_GRAPH:
	vga_graph(pls);
	break;
    }
}

/*----------------------------------------------------------------------*\
* vga_text()
*
* Switch to text mode.
\*----------------------------------------------------------------------*/

void
vga_text(PLStream *pls)
{
    if (pls->graphx == GRAPHICS_MODE) {
	if (page_state == DIRTY)
	    pause(pls);
	_setvideomode(_DEFAULTMODE);
	pls->graphx = TEXT_MODE;
    }
}

/*----------------------------------------------------------------------*\
* vga_graph()
*
* Switch to graphics mode.
\*----------------------------------------------------------------------*/

void
vga_graph(PLStream *pls)
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
* pause()
*
* Wait for a keystroke.
\*----------------------------------------------------------------------*/

static void
pause(PLStream *pls)
{
    if (pls->nopause)
	return;

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
