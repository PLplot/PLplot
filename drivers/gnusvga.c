/* $Id$
   $Log$
   Revision 1.4  1993/01/23 05:41:40  mjl
   Changes to support new color model, polylines, and event handler support
   (interactive devices only).

 * Revision 1.3  1992/11/07  07:48:38  mjl
 * Fixed orientation operation in several files and standardized certain startup
 * operations. Fixed bugs in various drivers.
 *
 * Revision 1.2  1992/09/29  04:44:40  furnish
 * Massive clean up effort to remove support for garbage compilers (K&R).
 *
 * Revision 1.1  1992/05/20  21:32:33  furnish
 * Initial checkin of the whole PLPLOT project.
 *
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

#include <stdio.h>
#include "plplot.h"
#include "drivers.h"

#include <graphics.h>

/* Prototypes:	Since GNU CC, we can rest in the safety of ANSI prototyping. */

static void pause(void);
static void init_palette(void);

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

/* (dev) will get passed in eventually, so this looks weird right now */

static PLDev device;
static PLDev *dev = &device;

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
* svgainit()
*
* Initialize device.
\*----------------------------------------------------------------------*/

void
svgainit(PLStream *pls)
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

    svgagraph(pls);		/* Can't get current device info unless in
				   graphics mode. */

    vgax = GrSizeX() - 1;	/* should I use -1 or not??? */
    vgay = GrSizeY() - 1;

    dev->xold = UNDEFINED;
    dev->yold = UNDEFINED;
    dev->xmin = 0;
    dev->xmax = vgax;
    dev->ymin = 0;
    dev->ymax = vgay;

    setpxl(2.5, 2.5);		/* My best guess.  Seems to work okay. */

    setphy(0, vgax, 0, vgay);
}

/*----------------------------------------------------------------------*\
* svgaline()
*
* Draw a line in the current color from (x1,y1) to (x2,y2).
\*----------------------------------------------------------------------*/

void
svgaline(PLStream *pls, PLSHORT x1a, PLSHORT y1a, PLSHORT x2a, PLSHORT y2a)
{
    int x1 = x1a, y1 = y1a, x2 = x2a, y2 = y2a;

    if (pls->pscale)
	plSclPhy(pls, dev, &x1, &y1, &x2, &y2);

    y1 = vgay - y1;
    y2 = vgay - y2;

    GrLine(x1, y1, x2, y2, col);

    page_state = DIRTY;
}

/*----------------------------------------------------------------------*\
* svgapolyline()
*
* Draw a polyline in the current color.
\*----------------------------------------------------------------------*/

void
svgapolyline(PLStream *pls, PLSHORT *xa, PLSHORT *ya, PLINT npts)
{
    PLINT i;

    for (i = 0; i < npts - 1; i++)
	svgaline(pls, xa[i], ya[i], xa[i + 1], ya[i + 1]);
}

/*----------------------------------------------------------------------*\
* svgaclear()
*
* Clear page.
\*----------------------------------------------------------------------*/

void
svgaclear(PLStream *pls)
{
    if (page_state == DIRTY)
	pause();

    GrSetMode(GR_default_graphics);
    init_palette();

    page_state = CLEAN;
}

/*----------------------------------------------------------------------*\
* svgapage()
*
* Set up for the next page.
* Advance to next family file if necessary (file output).
\*----------------------------------------------------------------------*/

void
svgapage(PLStream *pls)
{
    pls->page++;
    svgaclear(pls);
}

/*----------------------------------------------------------------------*\
* svgaadv()
*
* Advance to the next page.
\*----------------------------------------------------------------------*/

void
svgaadv(PLStream *pls)
{
    svgaclear(pls);
    svgapage(pls);
}

/*----------------------------------------------------------------------*\
* svgatidy()
*
* Close graphics file or otherwise clean up.
\*----------------------------------------------------------------------*/

void
svgatidy(PLStream *pls)
{
    svgatext(pls);
    pls->page = 0;
    pls->OutFile = NULL;
}

/*----------------------------------------------------------------------*\
* svgacolor()
*
* Set pen color.
\*----------------------------------------------------------------------*/

void
svgacolor(PLStream *pls)
{
    col = pls->icol0;
}

/*----------------------------------------------------------------------*\
* svgatext()
*
* Switch to text mode.
\*----------------------------------------------------------------------*/

void
svgatext(PLStream *pls)
{
    if (pls->graphx == GRAPHICS_MODE) {
	if (page_state == DIRTY)
	    pause();
	GrSetMode(GR_default_text);
	pls->graphx = TEXT_MODE;
    }
}

/*----------------------------------------------------------------------*\
* svgagraph()
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

void
svgagraph(PLStream *pls)
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
* svgawidth()
*
* Set pen width.
\*----------------------------------------------------------------------*/

void
svgawidth(PLStream *pls)
{
}

/*----------------------------------------------------------------------*\
* svgaesc()
*
* Escape function.
\*----------------------------------------------------------------------*/

void
svgaesc(PLStream *pls, PLINT op, char *ptr)
{
    switch (op) {
	case PL_SET_RGB:{
	    pleRGB *cols = (pleRGB *) ptr;
	    int r = 255 * cols->red;
	    int g = 255 * cols->green;
	    int b = 255 * cols->blue;
	    if (totcol < 255) {
		GrSetColor(++totcol, r, g, b);
		col = totcol;
	    }
	}
	break;
    }
}

/*----------------------------------------------------------------------*\
* pause()
*
* Wait for a keystroke.
\*----------------------------------------------------------------------*/

static void
pause(void)
{
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
