/* $Id$
   $Log$
   Revision 1.10  1993/02/27 21:07:40  mjl
   Fixed the last bug :-) in text/graph window switching.

 * Revision 1.9  1993/02/27  08:44:38  mjl
 * Fixed some long-standing bugs in switching between text and graphics windows
 * (didn't affect xterms, but did affect certain vt100/tek emulators).
 *
 * Revision 1.8  1993/02/27  04:46:42  mjl
 * Fixed errors in ordering of header file inclusion.  "plplot.h" should
 * always be included first.
 *
 * Revision 1.7  1993/02/26  06:23:21  mjl
 * Changed char->int in input parameter to EventHandler.
 *
 * Revision 1.6  1993/02/22  23:11:04  mjl
 * Eliminated the gradv() driver calls, as these were made obsolete by
 * recent changes to plmeta and plrender.  Also eliminated page clear commands
 * from grtidy() -- plend now calls grclr() and grtidy() explicitly.
 *
 * Revision 1.5  1993/01/23  05:41:55  mjl
 * Changes to support new color model, polylines, and event handler support
 * (interactive devices only).
 *
 * Revision 1.4  1992/11/07  07:48:50  mjl
 * Fixed orientation operation in several files and standardized certain startup
 * operations. Fixed bugs in various drivers.
 *
 * Revision 1.3  1992/10/22  17:04:59  mjl
 * Fixed warnings, errors generated when compling with HP C++.
 *
 * Revision 1.2  1992/09/29  04:44:51  furnish
 * Massive clean up effort to remove support for garbage compilers (K&R).
 *
 * Revision 1.1  1992/05/20  21:32:45  furnish
 * Initial checkin of the whole PLPLOT project.
 *
*/

/*	xterm.c

	PLPLOT xterm device driver.
*/
#ifdef XTERM

#include "plplot.h"
#include <stdio.h>
#include "drivers.h"
#include "plevent.h"

/* top level declarations */
/* INDENT OFF */

#define TEKX   1023
#define TEKY    779

/* Graphics control characters. */

#define ETX  3
#define BEL  7
#define FF   12
#define CAN  24
#define ESC  27
#define GS   29
#define US   31

/* Static function prototypes */

static void	WaitForPage	(PLStream *);
static void	EventHandler	(PLStream *, int);

/* (dev) will get passed in eventually, so this looks weird right now */

static PLDev device;
static PLDev *dev = &device;
static exit_eventloop = 0;

/* INDENT ON */
/*----------------------------------------------------------------------*\
* xte_init()
*
* Initialize device.
\*----------------------------------------------------------------------*/

void 
xte_init (PLStream *pls)
{
    pls->termin = 1;
    pls->icol0 = 1;
    pls->color = 0;
    pls->width = 1;
    pls->bytecnt = 0;
    pls->page = 0;

    dev->xold = UNDEFINED;
    dev->yold = UNDEFINED;
    dev->xmin = 0;
    dev->xmax = TEKX * 16;
    dev->ymin = 0;
    dev->ymax = TEKY * 16;

    /* set device resolution in dots/mm */

    setpxl((PLFLT) (4.771 * 16), (PLFLT) (4.653 * 16));

    /* set page size using setphy(xmin,xmax,ymin,ymax) */
    /* plplot assumes that the min coordinates are in the lower left */

    setphy(dev->xmin, dev->xmax, dev->ymin, dev->ymax);

    printf("%c[?38h", ESC);	/* open graphics window */
    printf("%c", GS);		/* set to vector mode */
    printf("%c%c", ESC, FF);

    pls->graphx = GRAPHICS_MODE;
}

/*----------------------------------------------------------------------*\
* xte_line()
*
* Draw a line in the current color from (x1,y1) to (x2,y2).
\*----------------------------------------------------------------------*/

void 
xte_line (PLStream *pls, PLSHORT x1a, PLSHORT y1a, PLSHORT x2a, PLSHORT y2a)
{
    int x1 = x1a, y1 = y1a, x2 = x2a, y2 = y2a;
    int hy, ly, hx, lx;

    if (pls->pscale)
	plSclPhy(pls, dev, &x1, &y1, &x2, &y2);

    x1 >>= 4;
    y1 >>= 4;
    x2 >>= 4;
    y2 >>= 4;

/* If continuation of previous line just send new point */

    if (x1 == dev->xold && y1 == dev->yold) {
	hy = y2 / 32 + 32;
	ly = y2 - (y2 / 32) * 32 + 96;
	hx = x2 / 32 + 32;
	lx = x2 - (x2 / 32) * 32 + 64;
	printf("%c%c%c%c", hy, ly, hx, lx);
    }
    else {
	printf("%c", GS);
	hy = y1 / 32 + 32;
	ly = y1 - (y1 / 32) * 32 + 96;
	hx = x1 / 32 + 32;
	lx = x1 - (x1 / 32) * 32 + 64;
	printf("%c%c%c%c", hy, ly, hx, lx);
	hy = y2 / 32 + 32;
	ly = y2 - (y2 / 32) * 32 + 96;
	hx = x2 / 32 + 32;
	lx = x2 - (x2 / 32) * 32 + 64;
	printf("%c%c%c%c", hy, ly, hx, lx);
    }
    dev->xold = x2;
    dev->yold = y2;
}

/*----------------------------------------------------------------------*\
* xte_polyline()
*
* Draw a polyline in the current color.
\*----------------------------------------------------------------------*/

void 
xte_polyline (PLStream *pls, PLSHORT *xa, PLSHORT *ya, PLINT npts)
{
    PLINT i;

    for (i=0; i<npts-1; i++) 
      xte_line( pls, xa[i], ya[i], xa[i+1], ya[i+1] );
}

/*----------------------------------------------------------------------*\
* xte_clear()
*
* Clear page.  User must hit a <CR> to continue.
\*----------------------------------------------------------------------*/

void 
xte_clear (PLStream *pls)
{
    putchar(BEL);
    fflush(stdout);
    WaitForPage(pls);
    printf("%c%c", ESC, FF);
}

/*----------------------------------------------------------------------*\
* xte_page()
*
* Set up for the next page.
\*----------------------------------------------------------------------*/

void 
xte_page (PLStream *pls)
{
    dev->xold = UNDEFINED;
    dev->yold = UNDEFINED;
    pls->page++;
}

/*----------------------------------------------------------------------*\
* xte_tidy()
*
* Close graphics file
\*----------------------------------------------------------------------*/

void 
xte_tidy (PLStream *pls)
{
    fflush(stdout);
    xte_graph(pls);

    pls->graphx = TEXT_MODE;
    printf("%c%c", US, CAN);
    printf("%c%c", ESC, ETX);
    printf("%c%c", US, CAN);

    fflush(stdout);
    pls->fileset = 0;
    pls->page = 0;
    pls->OutFile = NULL;
}

/*----------------------------------------------------------------------*\
* xte_color()
*
* Set pen color.
\*----------------------------------------------------------------------*/

void 
xte_color (PLStream *pls)
{
}

/*----------------------------------------------------------------------*\
* xte_text()
*
* Switch to text mode.
\*----------------------------------------------------------------------*/

void 
xte_text (PLStream *pls)
{
    if (pls->graphx == GRAPHICS_MODE) {
	pls->graphx = TEXT_MODE;
	printf("%c%c", US, CAN);
	printf("%c%c", ESC, ETX);
	printf("%c%c", US, CAN);
    }
}

/*----------------------------------------------------------------------*\
* xte_graph()
*
* Switch to graphics mode.
\*----------------------------------------------------------------------*/

void 
xte_graph (PLStream *pls)
{
    if (pls->graphx == TEXT_MODE) {
	pls->graphx = GRAPHICS_MODE;
	printf("%c[?38h", ESC);
    }
}

/*----------------------------------------------------------------------*\
* xte_width()
*
* Set pen width.
\*----------------------------------------------------------------------*/

void 
xte_width (PLStream *pls)
{
}

/*----------------------------------------------------------------------*\
* xte_esc()
*
* Escape function.
\*----------------------------------------------------------------------*/

void 
xte_esc (PLStream *pls, PLINT op, char *ptr)
{
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
    int input_char;

    if (pls->nopause)
	return;

    while (!exit_eventloop) {
	input_char = getchar();
	EventHandler(pls, input_char);
    }
    exit_eventloop = FALSE;
}

/*----------------------------------------------------------------------*\
* EventHandler()
*
* Event handler routine for xterm.  
* Just reacts to keyboard input.
\*----------------------------------------------------------------------*/

static void
EventHandler(PLStream *pls, int input_char)
{
    PLKey key;

    key.code = 0;
    key.string[0] = '\0';

/* Translate keystroke into a PLKey */

    if (isprint(input_char)) {
	key.string[0] = input_char;
	key.string[1] = '\0';
    }
    else {
	switch (input_char) {
	case 0x08:
	    key.code = PLK_BackSpace;
	    break;

	case 0x09:
	    key.code = PLK_Tab;
	    break;

	case 0x0A:
	    key.code = PLK_Linefeed;
	    break;

	case 0x0D:
	    key.code = PLK_Return;
	    break;

	case 0x1B:
	    key.code = PLK_Escape;
	    break;

	case 0xFF:
	    key.code = PLK_Delete;
	    break;
	}
    }

#ifdef DEBUG
    printf("Keycode %x, string: %s\n", key.code, key.string);
#endif

/* Call user event handler */
/* Since this is called first, the user can disable all plplot internal
   event handling by setting key.code to 0 and key.string to '\0' */

    if (pls->KeyEH != NULL)
	(*pls->KeyEH) (&key, pls->KeyEH_data, &exit_eventloop);

/* Handle internal events */

/* Advance to next page (i.e. terminate event loop) on a <eol> */

    if (key.code == PLK_Linefeed)
	exit_eventloop = TRUE;

/* Terminate on a 'Q' (not 'q', since it's too easy to hit by mistake) */

    if (key.string[0] == 'Q') {
	pls->nopause = TRUE;
	plexit("");
    }
}

#else
int pldummy_xterm() {return 0;}
#endif	/* XTERM */
 
