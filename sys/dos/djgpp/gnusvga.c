/* $Id$
 * $Log$
 * Revision 1.1  1994/05/25 09:19:22  mjl
 * The new locations for DJGPP (DOS 386) files.
 *
 * Revision 1.14  1994/04/08  11:35:57  mjl
 * Put nopause support back into the drivers where it is better off.
 * I don't know WHAT I was thinking.
 *
 * Revision 1.13  1994/03/23  06:34:23  mjl
 * All drivers: cleaned up by eliminating extraneous includes (stdio.h and
 * stdlib.h now included automatically by plplotP.h), extraneous clears
 * of pls->fileset, pls->page, and pls->OutFile = NULL (now handled in
 * driver interface or driver initialization as appropriate).  Special
 * handling for malloc includes eliminated (no longer needed) and malloc
 * prototypes fixed as necessary.
 *
 * Revision 1.12  1993/12/08  06:12:22  mjl
 * Miscellaneous bug fixes contributed by Paul Kirschner.
 *
 * Revision 1.11  1993/07/31  07:56:30  mjl
 * Several driver functions consolidated, for all drivers.  The width and color
 * commands are now part of a more general "state" command.  The text and
 * graph commands used for switching between modes is now handled by the
 * escape function (very few drivers require it).  The device-specific PLDev
 * structure is now malloc'ed for each driver that requires it, and freed when
 * the stream is terminated.
 *
 * Revision 1.10  1993/07/16  22:11:15  mjl
 * Eliminated low-level coordinate scaling; now done by driver interface.
 *
 * Revision 1.9  1993/07/01  21:59:33  mjl
 * Changed all plplot source files to include plplotP.h (private) rather than
 * plplot.h.  Rationalized namespace -- all externally-visible plplot functions
 * now start with "pl"; device driver functions start with "plD_".
*/

/*
	gnusvga.c
	Geoffrey Furnish
	20 March 1992
	
	This file constitutes the driver for an SVGA display under DOS
	using the GNU CC compiler and grpahics library by DJ Delorie.

	There are some things to note:
	1)  DJ Delorie's DOS port of GNU CC 386 uses a VCPI DOS extender.
		This means, among other things, that it won't run under
		any flavor of OS/2.  A DPMI port was rumored to be
		underway, so it is possible that by the time you read
		this, this restriction will have been lifted.
	2)  The currently available video drivers only support Super VGA
		type display cards, and only in resolutions with 256
		colors.  Thus, if you only have regular VGA, you're gonna
		get 320 x 200 resolution.  On the other hand, if you do
		have a card with say, 800 x 600 x 256 colors, then you're
		gonna get a picture much better than what could be delivered
		by the standard DOS version of PLPLOT using Micrsoft
		graphics.
	3)  This driver supports the PLPLOT RGB escape function.  So, you
		can draw lines of any color you like, which is again
		superior to the standard DOS version based on MS graphics.
*/
#ifdef GNUSVGA			/* Only compile for DOS 386 with GNU CC
				   compiler */
#include "plplotP.h"
#include "drivers.h"
#include <graphics.h>

/* Prototypes:	Since GNU CC, we can rest in the safety of ANSI prototyping. */

static void	pause		(PLStream *);
static void	init_palette	(void);
static void	svga_text	(PLStream *);
static void	svga_graph	(PLStream *);

static PLINT vgax = 639;
static PLINT vgay = 479;

/* A flag to tell us whether we are in text or graphics mode */

#define TEXT_MODE 0
#define GRAPHICS_MODE 1

/* gmf; should probably query this on start up... Maybe later. */

static int mode = TEXT_MODE;
static int col = 1;
static int totcol = 16;

#define CLEAN 0
#define DIRTY 1

static page_state;

typedef struct {
    int r;
    int g;
    int b;
} RGB;

static RGB colors[] = {
    {0, 0, 0},			/* coral */
    {255, 0, 0},		/* red */
    {255, 255, 0},		/* yellow */
    {0, 255, 0},		/* green */
    {127, 255, 212},		/* acquamarine */
    {255, 192, 203},		/* pink */
    {245, 222, 179},		/* wheat */
    {190, 190, 190},		/* grey */
    {165, 42, 42},		/* brown */
    {0, 0, 255},		/* blue */
    {138, 43, 226},		/* Blue Violet */
    {0, 255, 255},		/* cyan */
    {64, 224, 208},		/* turquoise */
    {255, 0, 255},		/* magenta */
    {250, 128, 114},		/* salmon */
    {255, 255, 255},		/* white */
    {0, 0, 0},			/* black */
};


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

    svga_graph(pls);		/* Can't get current device info unless in
				   graphics mode. */

    vgax = GrSizeX() - 1;	/* should I use -1 or not??? */
    vgay = GrSizeY() - 1;

    plP_setpxl(2.5, 2.5);	/* My best guess.  Seems to work okay. */

    plP_setphy(0, vgax, 0, vgay);
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

    GrLine(x1, y1, x2, y2, col);

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

    init_palette();

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
    GrClearScreen(0);
}

/*----------------------------------------------------------------------*\
* plD_tidy_vga()
*
* Close graphics file or otherwise clean up.
\*----------------------------------------------------------------------*/

void
plD_tidy_vga(PLStream *pls)
{
    svga_text(pls);
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
	col = pls->icol0;
	if (col == PL_RGB_COLOR) {
	    int r = pls->curcolor.r;
	    int g = pls->curcolor.g;
	    int b = pls->curcolor.b;
	    if (totcol < 255) {
		GrSetColor(++totcol, r, g, b);
		col = totcol;
	    }
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
	svga_text(pls);
	break;

      case PLESC_GRAPH:
	svga_graph(pls);
	break;
    }
}

/*----------------------------------------------------------------------*\
* svga_text()
*
* Switch to text mode.
\*----------------------------------------------------------------------*/

static void
svga_text(PLStream *pls)
{
    if (pls->graphx == GRAPHICS_MODE) {
	if (page_state == DIRTY)
	    pause(pls);
	GrSetMode(GR_default_text);
	GrResetColors();
	pls->graphx = TEXT_MODE;
	page_state = CLEAN;
    }
}

/*----------------------------------------------------------------------*\
* svga_graph()
*
* Switch to graphics mode.
*
* NOTE:  Moving to a new page causes the RGB map to be reset so that
* there will continue to be room.  This could conceivably cause a problem
* if an RGB escape was used to start drawing in a new color, and then
* it was expected that this would persist accross a page break.  If
* this happens, it will be necessary to rethink the logic of how this
* is handled.  Could wrap the index, for example.  This would require
* saving the RGB info used so far, which is not currently done.
\*----------------------------------------------------------------------*/

static void
svga_graph(PLStream *pls)
{
    if (pls->graphx == TEXT_MODE) {
	GrSetMode(GR_default_graphics);	/* Destroys the palette */
	init_palette();		/* Fix the palette */
	totcol = 16;		/* Reset RGB map so we don't run out of
				   indicies */
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

    GrTextXY(0, 0, "Pause->", 15, 0);
    getkey();
}

/*----------------------------------------------------------------------*\
* init_palette()
*
* Set up the nice RGB default color map.
\*----------------------------------------------------------------------*/

static void 
init_palette(void)
{
    int n;

    for (n = 0; n < sizeof(colors) / sizeof(RGB); n++)
	GrSetColor(n, colors[n].r, colors[n].g, colors[n].b);
}

#else
int 
pldummy_gnusvga()
{
    return 0;
}

#endif				/* GNUSVGA */
