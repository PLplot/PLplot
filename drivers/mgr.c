/* $Id$
 * $Log$
 * Revision 1.1  1993/08/03 03:21:55  mjl
 * Added contributions from Sergio Fanchiotti for use under Linux.
 *
*/

/*
	mgr.c
	S. Fanchiotti (Using gnusvga.c by Geoffrey Furnish)
	4 May 1993
	
	This file constitutes the driver for an MGR window under Linux
	using the GNU CC compiler.

	Things to note:
         The idea of an event handler as done for xterm.c should be used
	 as it is more elegant and also it can be used to set the event
	 codes. In this way no recompilation would be ncessary if other
	 routines set up messages for the same event.
*/
#ifdef MGR			/* Only compile if MGR support is needed */

#include "plplotP.h"
#include <stdlib.h>
#include <string.h>
/* The next one is highly dependant on the MGR installation!!! */
#include "/usr/mgr/include/term.h"
#include "drivers.h"

/* Function prototypes */
/* INDENT OFF */

static void SetEvent	(void);
static void GetKey	(PLStream *);
static void mgr_text	(PLStream *pls);
static void mgr_graph	(PLStream *pls);

/* INDENT ON */

static PLINT mgrx = 639;
static PLINT mgry = 479;

/* A flag to tell us whether we are in text or graphics mode */

#define TEXT_MODE 0
#define GRAPHICS_MODE 1

/* gmf; should probably query this on start up... Maybe later. */

static int col = 1;
static int totcol = 16;

#define CLEAN 0
#define DIRTY 1

static page_state;

/*----------------------------------------------------------------------*\
* plD_init_mgr()
*
* Initialize device.
\*----------------------------------------------------------------------*/

void
plD_init_mgr(PLStream *pls)
{
    PLDev *dev;
    int Xw, Yw;			/* Dummy vars */

/* Check if we are running in a MGR window */
    if (strcmp(getenv("TERM"), "mgr"))
	plexit("plD_init_mgr: TERM enviroment var not set to \"mgr\".");

/* Ok, now set up the device parameters */
    pls->termin = 1;		/* is an interactive terminal */
    pls->icol0 = 1;
    pls->color = 0;		/* No color implemented in Linux MGR (Yet!) */
    pls->width = 1;
    pls->bytecnt = 0;
    pls->page = 0;
    pls->graphx = TEXT_MODE;

    if (!pls->colorset)
	pls->color = BLACK;	/* As defined in window.h */

/* Allocate and initialize device-specific data */

    dev = plAllocDev(pls);

    m_setup(M_FLUSH | M_MODEOK);/* Initialize function library */
    m_push(P_ALL);		/* Store terminal state in a stack */

    /* Now we get the dimensions of the window */
    m_setnoecho();		/* MGR not echoing to screen */
    get_size(&Xw, &Yw, &mgrx, &mgry);
    mgrx -= 1;
    mgry -= 1;

    dev->xold = UNDEFINED;
    dev->yold = UNDEFINED;
    dev->xmin = 0;
    dev->xmax = mgrx;
    dev->ymin = 0;
    dev->ymax = mgry;

    dev->xlen = dev->xmax - dev->xmin;
    dev->ylen = dev->ymax - dev->ymin;

    setpxl(2.5, 2.5);		/* My best guess.  Seems to work okay. */

    setphy(0, mgrx, 0, mgry);

    SetEvent();			/* Tell MGR to inform us of changes */
    m_setecho();		/* In case there is some text interaction */
}

/*----------------------------------------------------------------------*\
* plD_line_mgr()
*
* Draw a line in the current color from (x1,y1) to (x2,y2).
\*----------------------------------------------------------------------*/

void
plD_line_mgr(PLStream *pls, short x1a, short y1a, short x2a, short y2a)
{
    int x1 = x1a, y1 = y1a, x2 = x2a, y2 = y2a;

    y1 = mgry - y1;
    y2 = mgry - y2;

    m_line(x1, y1, x2, y2);

    page_state = DIRTY;
}

/*----------------------------------------------------------------------*\
* plD_polyline_mgr()
*
* Draw a polyline in the current color.
\*----------------------------------------------------------------------*/

void
plD_polyline_mgr(PLStream *pls, short *xa, short *ya, PLINT npts)
{
    PLINT i;

    for (i = 0; i < npts - 1; i++)
	plD_line_mgr(pls, xa[i], ya[i], xa[i + 1], ya[i + 1]);
}

/*----------------------------------------------------------------------*\
* plD_eop_mgr()
*
* End of page.
\*----------------------------------------------------------------------*/

void
plD_eop_mgr(PLStream *pls)
{
    if (page_state == DIRTY)
	GetKey(pls);

    m_clear();			/* just clean it */

    page_state = CLEAN;
}

/*----------------------------------------------------------------------*\
* plD_bop_mgr()
*
* Set up for the next page.
* Advance to next family file if necessary (file output).
\*----------------------------------------------------------------------*/

void
plD_bop_mgr(PLStream *pls)
{
    pls->page++;
    plD_eop_mgr(pls);
}

/*----------------------------------------------------------------------*\
* plD_tidy_mgr()
*
* Close graphics file or otherwise clean up.
\*----------------------------------------------------------------------*/

void
plD_tidy_mgr(PLStream *pls)
{
    m_ttyreset();		/* Reset tty mode */
    m_popall();			/* Restores original state of window (see
				   plD_init_mgr) */
    pls->fileset = 0;
    pls->page = 0;
    pls->OutFile = NULL;
}

/*----------------------------------------------------------------------*\
* plD_state_mgr()
*
* Handle change in PLStream state (color, pen width, fill attribute, etc).
\*----------------------------------------------------------------------*/

void
plD_state_mgr(PLStream *pls, PLINT op)
{
    switch (op) {

      case PLSTATE_WIDTH:
	break;

      case PLSTATE_COLOR0:
	if (pls->color) {
	    col = (pls->icol0) % totcol;	/* Color modulo # of colors
						   available */
	    m_fcolor(col);	/* Useless in monochrome MGR */
	}
	break;

      case PLSTATE_COLOR1:
	break;
    }
}

/*----------------------------------------------------------------------*\
* plD_esc_mgr()
*
* Escape function.
\*----------------------------------------------------------------------*/

void
plD_esc_mgr(PLStream *pls, PLINT op, void *ptr)
{
    switch (op) {

      case PLESC_TEXT:
	mgr_text(pls);
	break;

      case PLESC_GRAPH:
	mgr_graph(pls);
	break;
    }
}

/*----------------------------------------------------------------------*\
* mgr_text()
*
* Switch to text mode.
\*----------------------------------------------------------------------*/

static void
mgr_text(PLStream *pls)
{
    if (pls->graphx == GRAPHICS_MODE) {

	m_setecho();		/* Echo characters again  */
	m_ttyreset();		/* Reset tty mode */

	pls->graphx = TEXT_MODE;
    }
}

/*----------------------------------------------------------------------*\
* mgr_graph()
*
* Switch to graphics mode.
\*----------------------------------------------------------------------*/

static void
mgr_graph(PLStream *pls)
{
    if (pls->graphx == TEXT_MODE) {

	m_ttyset();		/* Enable MGR commands */
	m_setcursor(CS_INVIS);	/* make cursor invisible */
	m_setnoecho();		/* Don't write to terminal MGR's messages */
	m_setmode(M_ABS);	/* We will use absolute window coordinates. */

	pls->graphx = GRAPHICS_MODE;
	page_state = CLEAN;
    }
}

/*----------------------------------------------------------------------*\
* SetEvent(void)
*
* Setup MGR event messages format and load menus.
\*----------------------------------------------------------------------*/

static void
SetEvent(void)
{
    /* Load menu in Middle and Right buttons */
    m_loadmenu(0, "|Next page|Non-stop|Quit|N||Q|");
    m_loadmenu(1, "|Next page|Non-stop|Quit|N||Q|");
    m_selectmenu(0);		/* Middle button */
    m_selectmenu2(1);		/* Right button */

    m_setevent(RESHAPE, "");	/* If a window reshape occurs an Ctrl-R char is
				   passed to the client */
}

/*----------------------------------------------------------------------*\
* GetKey(PLStream *)
*
* Check if MGR did something to the window and act accordingly. It needs
* to wait for a key to be pressed though.
\*----------------------------------------------------------------------*/

static void
GetKey(PLStream *pls)
{
    PLDev *dev = (PLDev *) pls->dev;
    static char ch;
    static int again, Xw, Yw;	/* Dummy vars */

    if (pls->nopause)
	return;			/* Just ignore anything and go on! */

    do {
	again = FALSE;
	ch = m_getchar();	/* Ignore ESC character */
	switch (ch) {
	  case '':		/* Checkout window dimensions again */
	    get_size(&Xw, &Yw, &mgrx, &mgry);
	    mgrx -= 1;
	    mgry -= 1;
	    /* Inform of the change to the plplot routines */
	    dev->xmax = mgrx;
	    dev->ymax = mgry;
	    dev->xlen = dev->xmax - dev->xmin;
	    dev->ylen = dev->ymax - dev->ymin;
	    setphy(0, mgrx, 0, mgry);	/* A hack */
	    again = TRUE;
	    break;
	  case '':		/* Run non stop */
	    pls->nopause = TRUE;
	    break;
	  case 'q':		/* Sort of ``cut it off now!'' */
	  case 'Q':
	  case 'x':
	    pls->nopause = TRUE;
	    plexit("");		/* Abort the thing */
	    break;
	}
    } while (again);
}

#else
int
pldummy_mgr()
{
    return 0;
}

#endif				/* MGR */
