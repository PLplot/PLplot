/* $Id$
 * $Log$
 * Revision 1.4  1994/05/25 09:36:54  mjl
 * All VGA driver function names now end with "_vga", a nice simplification.
 * Since all are compiler-dependent and mutually exclusive, this should pose
 * no problem.  Also HP pen plotter driver were consolidated.  Both
 * contributions by Mark Olesen (olesen@weber.me.queensu.ca).
 *
 * Revision 1.3  1994/04/08  11:35:58  mjl
 * Put nopause support back into the drivers where it is better off.
 * I don't know WHAT I was thinking.
 *
 * Revision 1.2  1994/03/23  06:34:27  mjl
 * All drivers: cleaned up by eliminating extraneous includes (stdio.h and
 * stdlib.h now included automatically by plplotP.h), extraneous clears
 * of pls->fileset, pls->page, and pls->OutFile = NULL (now handled in
 * driver interface or driver initialization as appropriate).  Special
 * handling for malloc includes eliminated (no longer needed) and malloc
 * prototypes fixed as necessary.
 *
 * Revision 1.1  1993/08/03  03:21:54  mjl
 * Added contributions from Sergio Fanchiotti for use under Linux.
 *
*/

/*
	linuxvga.c
	S. Fanchiotti (Using gnusvga.c by Geoffrey Furnish)
	4 May 1993
	
	This file constitutes the driver for an VGA display under Linux
	using the GNU CC compiler and vgalib 1.2 library by T. Fradsen

	Things to note: NEEDS vgalib to compile!!!!!

*/
#ifdef LINUXVGA			/* Only compile for Linux + Vgalib 1.2 */

#include "plplotP.h"
#include "drivers.h"
#include <vga.h>

/* Function prototypes */
/* INDENT OFF */

static void lxvga_text	(PLStream *pls);
static void lxvga_graph	(PLStream *pls);
static void pause	(PLStream *pls);

/* INDENT ON */

static PLINT vgax = 639;
static PLINT vgay = 479;

/* A flag to tell us whether we are in text or graphics mode */

#define TEXT_MODE 0
#define GRAPHICS_MODE 1

/* gmf; should probably query this on start up... Maybe later. */
/* sf; Will set them dynamically! */

static int mode = TEXT_MODE;
static int col = 1;
static int totcol = 16;

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

/* What kind of VGA mode one wants is set up here.
   It can be easyly made interactive! */
    mode = G640x480x16;		/* See <vga.h> for a list */
    if (vga_hasmode(mode))
	vga_setmode(mode);
    else {
	printf("Error: Video mode not supported by graphics card\n");
	exit(-1);
    }

    /* If all is fine we get the dimensions and # of colors */
    vgax = vga_getxdim() - 1;
    vgay = vga_getydim() - 1;

    totcol = vga_getcolors();

    setpxl(2.5, 2.5);		/* My best guess.  Seems to work okay. */

    setphy(0, vgax, 0, vgay);
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

    y1 = vgay - y1;
    y2 = vgay - y2;

    vga_drawline(x1, y1, x2, y2);

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

    /* vga_setmode(mode); */
    vga_clear();		/* just clean it */

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
    lxvga_text(pls);
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

      case PLSTATE_COLOR0:
	if (pls->color) {

	    /* Maybe it would be wiser to use a set of 16 relevant colors only
	       and just fix it to black if col is exceeded 16.        */

	    col = (pls->icol0) % totcol;	/* Color modulo # of colors
						   avail */
	    vga_setcolor(col);
	}
	break;

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
	lxvga_text(pls);
	break;

      case PLESC_GRAPH:
	lxvga_graph(pls);
	break;
    }
}

/*----------------------------------------------------------------------*\
* lxvga_text()
*
* Switch to text mode.
\*----------------------------------------------------------------------*/

static void
lxvga_text(PLStream *pls)
{
    if (pls->graphx == GRAPHICS_MODE) {
	if (page_state == DIRTY)
	    pause(pls);
	vga_setmode(TEXT);
	pls->graphx = TEXT_MODE;
    }
}

/*----------------------------------------------------------------------*\
* lxvga_graph()
*
* Switch to graphics mode.
\*----------------------------------------------------------------------*/

static void
lxvga_graph(PLStream *pls)
{
    if (pls->graphx == TEXT_MODE) {
	vga_setmode(mode);	/* mode should be set right or ... */
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

    vga_getch();
}

#else
int
pldummy_vga()
{
    return 0;
}

#endif				/* LINUXVGA */
