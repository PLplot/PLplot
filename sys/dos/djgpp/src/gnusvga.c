/* $Id$
 * $Log$
 * Revision 1.5  2001/01/22 20:17:14  airwin
 * Change these files to windows-style line termination to be consistent
 * with all other files in the sys/dos/djgpp tree.  This change also makes
 * it easier to accept patches from dos/djgpp developers.
 *
 * Revision 1.4  2000/12/18 21:01:50  airwin
 * Change to new style plplot/*.h header file locations.
 *
 * Revision 1.3  1996/02/24 05:33:17  shouman
 * Updated driver to use PLGraphicsIn struct instead of obsolete PLKey.
 * Added translation of more keys to PLK_ keysyms (eg pageup, pagedown, F keys).
 * Added mouse button event handler.  Added PLESC_EH support to allow polling
 * for button, keyboard events.
 *
 * Eliminated default color stuff, since color maps are now reachable through
 * PLStreams.  Cmap0 colors no longer revert to default with each new page.
 *
 * Revision 1.2  1994/08/10  20:23:47  mjl
 * Latest changes for DJGPP port by Paul Kirschner.
 *
 * Revision 1.1  1994/08/10  01:08:29  mjl
 * New directory structure, config files, and other modifications for PLplot
 * 4.99h/djgpp DOS port, submitted by Paul Kirschner.  New driver supports
 * color fill.
 *
 * Revision 1.1  1994/05/25  09:19:22  mjl
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
#include "plplot/plDevs.h"

#ifdef PLD_gnusvga		/* Only compile for DOS 386 with GNU CC
				   compiler */
#include "plplot/plplotP.h"
#include "plplot/drivers.h"
#include "plplot/plevent.h"
#include <grx.h>
#include <mousex.h>
#include <keys.h>

/* Prototypes:	Since GNU CC, we can rest in the safety of ANSI prototyping. */

static void	plpause		(PLStream *);
static void	svga_text	(PLStream *);
static void	svga_graph	(PLStream *);

static void	fill_polygon	(PLStream *pls);
static void	setcmap		(PLStream *pls);
static void	WaitForPage	(PLStream *pls);
static void	EventHandler	(PLStream *pls, MouseEvent *event);
static void     TranslateEvent  (MouseEvent *event, PLGraphicsIn *gin);

static PLINT vgax = 639;
static PLINT vgay = 479;
static MouseEvent   mevent;
static PLGraphicsIn gin;		        /* Graphics input structure */

/* A flag to tell us whether we are in text or graphics mode */

#define TEXT_MODE 0
#define GRAPHICS_MODE 1

/* gmf; should probably query this on start up... Maybe later. */

static int mode = TEXT_MODE;
static int col = 1;
static int totcol = 16;
static exit_eventloop = 0;

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

    pls->dev_fill0 = 1;

    if (!pls->colorset)
	pls->color = 1;

/* Set up device parameters */
    svga_graph(pls);		/* Can't get current device info unless in
				   graphics mode. */

    vgax = GrSizeX() - 1;	/* should I use -1 or not??? */
    vgay = GrSizeY() - 1;

    plP_setpxl(2.5, 2.5);	/* My best guess.  Seems to work okay. */

    plP_setphy(0, vgax, 0, vgay);
    
    MouseEventMode(1);		/* Use interrupt driven mouse handler */
    MouseInit();
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
    if (page_state == DIRTY) {
	if ( ! pls->nopause) 
	    WaitForPage(pls);
    }

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
    MouseUnInit();		/* Restore interrupt vectors */
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
	    if (totcol < GrNumColors()) {
		GrSetColor(++totcol, r, g, b);
		col = totcol;
	    }
	}
	break;

    case PLSTATE_COLOR1:
        { int icol1, ncol1, r, g, b;
        if ((ncol1 = MIN(GrNumColors() - pls->ncol0, pls->ncol1)) < 1)
            break;

        icol1 = pls->ncol0 + (pls->icol1 * (ncol1-1)) / (pls->ncol1-1);

        r = pls->curcolor.r;
        g = pls->curcolor.g;
        b = pls->curcolor.b;
        GrSetColor(icol1, r, g, b);
        col = icol1;
	}
	break;

    case PLSTATE_CMAP0:
    case PLSTATE_CMAP1:
	if (pls->color)
	    setcmap(pls);
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

      case PLESC_FILL:
	fill_polygon(pls);
	break;

      case PLESC_EH:
	for (;;) {
  	    MouseGetEvent(M_BUTTON_DOWN | M_KEYPRESS | M_POLL, &mevent);
	    if (M_EVENT & mevent.flags)
	        EventHandler(pls, &mevent);
	    else
	        break;
	}
	break;
    }
}

/*----------------------------------------------------------------------*\
 * fill_polygon()
 *
 * Fill polygon described in points pls->dev_x[] and pls->dev_y[].
\*----------------------------------------------------------------------*/

static void
fill_polygon(PLStream *pls)
{
    int i;
    int points[PL_MAXPOLY][2];

    if (pls->dev_npts < 1)
	return;

/* Select the fill pattern */

/* Specify boundary */

    for (i = 0; i < pls->dev_npts; i++) {
	points[i][0] = pls->dev_x[i];
        points[i][1] = vgay - pls->dev_y[i];
    }
    GrFilledPolygon(pls->dev_npts, points, col);

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
	    plpause(pls);
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
 * NOTE: Moving to a new page causes the RGB map to be reset so that
 * there will continue to be room.  This could conceivably cause a
 * problem if an RGB escape was used to start drawing in a new color, and
 * then it was expected that this would persist accross a page break.  If
 * this happens, it will be necessary to rethink the logic of how this is
 * handled.  Could wrap the index, for example.  This would require
 * saving the RGB info used so far, which is not currently done.
\*----------------------------------------------------------------------*/

static void
svga_graph(PLStream *pls)
{
    if (pls->graphx == TEXT_MODE) {
				/* Destroys the palette */
	GrSetMode(GR_default_graphics);
/*	GrSetMode(GR_320_200_graphics); */
	setcmap(pls);
	totcol = 16;		/* Reset RGB map so we don't run out of
				   indices */
	pls->graphx = GRAPHICS_MODE;
	page_state = CLEAN;
    }
}

/*----------------------------------------------------------------------*\
 * plpause()
 *
 * Wait for a keystroke.
\*----------------------------------------------------------------------*/

static void
plpause(PLStream *pls)
{
    if (pls->nopause) 
	return;

    GrTextXY(0, 0, "Pause->", 15, 0);
    getkey();
}

/*----------------------------------------------------------------------*\
 * setcmap()
 *
 * Sets up color palette.
\*----------------------------------------------------------------------*/

static void
setcmap(PLStream *pls)
{
    int i, ncol1;
    PLColor cmap1col;

/* Initialize cmap 0 colors */

    for (i = 0; i < pls->ncol0; i++) 
	GrSetColor(i, pls->cmap0[i].r, pls->cmap0[i].g, pls->cmap0[i].b);

/* Initialize any remaining slots for cmap1 */

    ncol1 = GrNumFreeColors();
    for (i = 0; i < ncol1; i++) {
	plcol_interp(pls, &cmap1col, i, ncol1);
	GrSetColor(i + pls->ncol0, cmap1col.r, cmap1col.g, cmap1col.b);
    }
}

/*----------------------------------------------------------------------*\
 * WaitForPage()
 *
 * This routine waits for the user to advance the plot, while handling
 * all other events.
\*----------------------------------------------------------------------*/

static void
WaitForPage(PLStream *pls)
{

/*    putchar(BEL); */
/*    fflush(stdout); */

    while ( ! exit_eventloop) {
        MouseGetEvent(M_BUTTON_DOWN | M_KEYPRESS, &mevent);
  	    EventHandler(pls, &mevent); 
    }
    exit_eventloop = FALSE;
}

/*----------------------------------------------------------------------*\
 * EventHandler()
 *
 * Event handler routine.
 * Reacts to keyboard or mouse input.
\*----------------------------------------------------------------------*/

static void
EventHandler(PLStream *pls, MouseEvent *event)
{
    TranslateEvent(event, &gin);

#ifdef DEBUG
    {
      char buf[200];
      sprintf(buf, "flags 0x%x, buttons 0x%x, key 0x%x                ",
	      event->flags, event->buttons, event->key);
      GrTextXY(0, 0, buf, 15, 0);
      sprintf(buf, "Input char 0x%x, Keycode 0x%x, string: %s               ",
	      event->key, gin.keysym, gin.string);
      GrTextXY(0, 50, buf, 15, 0);
      sprintf(buf, "button %d, pX %d, pY %d, state 0x%x                     ", 
	      gin.button, gin.pX, gin.pY, gin.state);
      GrTextXY(0, 100, buf, 15, 0);
    }
#endif

/* Call user event handler */
/* Since this is called first, the user can disable all plplot internal
   event handling by setting key.code to 0 and key.string to '\0' */

    if (gin.button) {
        if (pls->ButtonEH != NULL)
	    (*pls->ButtonEH) (&gin, pls->ButtonEH_data, &exit_eventloop);
    } 
    else {
        if (pls->KeyEH != NULL)
            (*pls->KeyEH) (&gin, pls->KeyEH_data, &exit_eventloop);
    }

/* Handle internal events */

/* Advance to next page (i.e. terminate event loop) on a <eol> */

    if (gin.keysym == PLK_Linefeed)
	exit_eventloop = TRUE;
    if (gin.keysym == PLK_Return)
	exit_eventloop = TRUE;
    if (gin.button == 3)
        exit_eventloop = TRUE;

/* Terminate on a 'Q' (not 'q', since it's too easy to hit by mistake) */

    if (gin.string[0] == 'Q') {
	pls->nopause = TRUE;
	plexit("");
    }
}


/*--------------------------------------------------------------------------*\
 * TranslateEvent()
 *
 * Fills in the PLGraphicsIn from a MouseEvent.  All keys are not yet 
 * translated correctly.
\*--------------------------------------------------------------------------*/

static void
TranslateEvent(MouseEvent *event, PLGraphicsIn *gin)
{
    gin->string[0] = '\0';

/* Translate DJGPP MouseEvent into PlPLot event */

    if (event->flags & M_KEYPRESS) {
        gin->keysym = event->key;
	if (isprint(event->key)) {
	    gin->string[0] = event->key;
	    gin->string[1] = '\0';
	}
	else {
	    switch (event->key) {
	    default: gin->keysym = event->key;
	      break;

	    case K_F1: case K_F2: case K_F3: 
	    case K_F4: case K_F5: case K_F6:
	    case K_F7: case K_F8: case K_F9: 
	    case K_F10:
	      gin->keysym = event->key - K_F1 + PLK_F1;
	      break;

	    case K_Home: case K_EHome:
	      gin->keysym = PLK_Home;
	      break;

	    case K_Up: case K_EUp:
	      gin->keysym = PLK_Up;
	      break;

	    case K_Right: case K_ERight:
	      gin->keysym = PLK_Right;
	      break;

	    case K_Down: case K_EDown:
	      gin->keysym = PLK_Down;
	      break;

	    case K_Left: case K_ELeft:
	      gin->keysym = PLK_Left;
	      break;

	    case K_PageDown: case K_EPageDown:
	      gin->keysym = PLK_Prior; 
	      break;

	    case K_PageUp: case K_EPageUp:
	      gin->keysym = PLK_Next;
	      break;

	    case K_Insert: case K_EInsert:
	      gin->keysym = PLK_Insert;
	      break;
	    }
	}
	gin->button = 0;
	gin->state = event->kbstat;
    }

    else if (event->flags & M_BUTTON_DOWN) {
      switch (event->flags) {
      case M_LEFT_DOWN:
          gin->button = 1;
	  break;
      case M_MIDDLE_DOWN:
	  gin->button = 2;
	  break;
      case M_RIGHT_DOWN:
	  gin->button = 3;
	  break;
      }
      gin->keysym = 0x20;
      gin->pX = event->x;
      gin->pY = event->y;
      gin->dX = (PLFLT) event->x / vgax;
      gin->dY = 1.0 - (PLFLT) event->y / vgay;
      gin->state = event->kbstat;
    }
}

#else
int 
pldummy_gnusvga()
{
    return 0;
}

#endif				/* GNUSVGA */
