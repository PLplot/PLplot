/* $Id$
 * Revision 1.6  1992/10/22  17:05:01  mjl
 * Fixed warnings, errors generated when compling with HP C++.
 *
 * Revision 1.5  1992/10/22  16:07:10  gray
 * added crude support to have mouse button 2 return cursor postition to
 * stdout
 *
 * Revision 1.4  1992/09/30  18:25:01  furnish
 * Massive cleanup to irradicate garbage code.  Almost everything is now
 * prototyped correctly.  Builds on HPUX, SUNOS (gcc), AIX, and UNICOS.
 *
 * Revision 1.3  1992/09/29  04:44:52  furnish
 * Massive clean up effort to remove support for garbage compilers (K&R).
 *
 * Revision 1.2  1992/07/31  06:06:48  mjl
 * Swapped background/foreground colors for monochrome X output.
 *
 * Revision 1.1  1992/05/20  21:32:46  furnish
 * Initial checkin of the whole PLPLOT project.
 *
*/

/*	xwin.c

	PLPLOT X-windows device driver.
*/
#ifdef XWIN

#include "plplot.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/cursorfont.h>
#include <X11/keysym.h>

#include "drivers.h"
#include "plevent.h"

/* Function prototypes */
/* INDENT OFF */

static void	xw_Xinit	( PLStream * );
static void	WaitForPage	( PLStream * );
static void	EventHandler	( PLStream *pls, XEvent *event );
static void	xw_colini	( PLStream *pls );
static int	AreWeMonochrome ( Display * );
static void	PLColor_to_XColor(XColor *, PLColor *);
static void	XColor_to_XColor (XColor *, XColor *);

static void	KeyEH		(PLStream *, XEvent *);
static void	MouseEH		(PLStream *, XEvent *);
static void	ExposeEH	(PLStream *, XEvent *);
static void	ResizeEH	(PLStream *, XEvent *);

/* top level declarations */

/* Stuff to handle reentrancy for multiple streams/devices.
   This is still under development so use caution. Right now there
   is no multiple device capability beyond that of multiple streams. */

#define PL_NDEV_XW 10	/* Max number of X sessions active */
#define PL_NDEVICES 10	/* Max number of devices/stream active */

typedef struct {
    int			exit_eventloop;
    long		init_width;
    long		init_height;
    long		cur_width;
    long		cur_height;
    double		xscale;
    double		yscale;
    double		xscale_dev;
    double		yscale_dev;

    int			monochrome;
    XColor		cmap0[16];
    XColor		fgcolor;
    XColor		curcolor;

    int			screen;
    Display		*display;
    Window		window;
    XEvent		theEvent;
    GC			gc;
    Colormap		map;
} XwDev;

static XwDev xwdev[PL_NDEV_XW];
static PLDev pldev[PL_NDEV_XW];

static int idev = -1;
static int devtable[PL_NSTREAMS][PL_NDEVICES];

/* Miscellaneous global definitions */

#define PIXELS_X	8191		/* Number of virtual pixels in x */
#define PIXELS_Y	8191		/* Number of virtual pixels in y */

static PLFLT lpage_x = 238.0;		/* Page length in x in virtual mm */
static PLFLT lpage_y = 178.0;		/* Page length in y in virtual mm */

/* INDENT ON */
/*----------------------------------------------------------------------*\
* xw_init()
*
* Initialize device.
* X-dependent stuff done in xw_Xinit().  You can set the display by
* calling plsfile() with the display name as the (string) argument.
\*----------------------------------------------------------------------*/

void
xw_init(PLStream *pls)
{
    XwDev *xwd;
    PLDev *pld;
    int id;

    if (++idev == PL_NDEV_XW)
	plexit("Exceeded maximum number of active X sessions.");

    devtable[pls->ipls][pls->ipld] = idev;

    pld = &(pldev[idev]);
    xwd = &(xwdev[idev]);

    pls->termin = 1;		/* is an interactive terminal */
    pls->icol0 = 1;
    pls->width = 1;
    pls->bytecnt = 0;
    pls->page = 0;
    pls->plbuf_enable++;

    xw_Xinit(pls);

    pld->xold = UNDEFINED;
    pld->yold = UNDEFINED;
    pld->xmin = 0;
    pld->xmax = PIXELS_X;
    pld->ymin = 0;
    pld->ymax = PIXELS_Y;
    pld->xlen = pld->xmax - pld->xmin;
    pld->ylen = pld->ymax - pld->ymin;

    pld->pxlx = pld->xlen / lpage_x;
    pld->pxly = pld->ylen / lpage_y;

    xwd->xscale_dev = (double) xwd->init_width / (double) pld->xlen;
    xwd->yscale_dev = (double) xwd->init_height / (double) pld->ylen;

    xwd->xscale = xwd->xscale * xwd->xscale_dev;
    xwd->yscale = xwd->yscale * xwd->yscale_dev;

    setpxl(pld->pxlx, pld->pxly);
    setphy(pld->xmin, pld->xmax, pld->ymin, pld->ymax);
}

/*----------------------------------------------------------------------*\
* xw_line()
*
* Draw a line in the current color from (x1,y1) to (x2,y2).
\*----------------------------------------------------------------------*/

void
xw_line(PLStream *pls, PLSHORT x1a, PLSHORT y1a, PLSHORT x2a, PLSHORT y2a)
{
    int x1 = x1a, y1 = y1a, x2 = x2a, y2 = y2a;
    int id = devtable[pls->ipls][pls->ipld];
    XwDev *xwd = &(xwdev[id]);
    PLDev *pld = &(pldev[id]);

    y1 = pld->ylen - y1;
    y2 = pld->ylen - y2;

    x1 = x1 * xwd->xscale;
    x2 = x2 * xwd->xscale;
    y1 = y1 * xwd->yscale;
    y2 = y2 * xwd->yscale;

    XDrawLine(xwd->display, xwd->window, xwd->gc, x1, y1, x2, y2);
}

/*----------------------------------------------------------------------*\
* xw_polyline()
*
* Draw a polyline in the current color from (x1,y1) to (x2,y2).
\*----------------------------------------------------------------------*/

void
xw_polyline(PLStream *pls, PLSHORT *xa, PLSHORT *ya, PLINT npts)
{
    PLINT i;
    XPoint pts[PL_MAXPOLYLINE];
    int id = devtable[pls->ipls][pls->ipld];
    XwDev *xwd = &(xwdev[id]);
    PLDev *pld = &(pldev[id]);

    if (npts > PL_MAXPOLYLINE)
	plexit("Error -- too many points in polyline\n");

    for (i = 0; i < npts; i++) {
	pts[i].x = xwd->xscale * xa[i];
	pts[i].y = xwd->yscale * (pld->ylen - ya[i]);
    }

    XDrawLines(xwd->display, xwd->window, xwd->gc, pts, npts, CoordModeOrigin);
}

/*----------------------------------------------------------------------*\
* xw_clear()
*
* Clear page.  User must click left mouse button to continue.
\*----------------------------------------------------------------------*/

void
xw_clear(PLStream *pls)
{
    int id = devtable[pls->ipls][pls->ipld];
    XwDev *xwd = &(xwdev[id]);

    XFlush(xwd->display);
    WaitForPage(pls);
    XClearWindow(xwd->display, xwd->window);
}

/*----------------------------------------------------------------------*\
* xw_page()
*
* Set up for the next page.
\*----------------------------------------------------------------------*/

void
xw_page(PLStream *pls)
{
    pls->page++;
}

/*----------------------------------------------------------------------*\
* xw_tidy()
*
* Close graphics file
\*----------------------------------------------------------------------*/

void
xw_tidy(PLStream *pls)
{
    int id = devtable[pls->ipls][pls->ipld];
    XwDev *xwd = &(xwdev[id]);

    XFreeGC(xwd->display, xwd->gc);
    XDestroyWindow(xwd->display, xwd->window);
    XCloseDisplay(xwd->display);

    pls->fileset = 0;
    pls->page = 0;
    pls->plbuf_enable--;
    pls->OutFile = NULL;
    idev--;
}

/*----------------------------------------------------------------------*\
* xw_color()
*
* Set pen color.
\*----------------------------------------------------------------------*/

void
xw_color(PLStream *pls)
{
    int id = devtable[pls->ipls][pls->ipld];
    XwDev *xwd = &(xwdev[id]);
    int icol0 = pls->icol0;

    if (icol0 == PL_RGB_COLOR) {
	xwd->curcolor.red = (pls->curcolor.r << 8);
	xwd->curcolor.green = (pls->curcolor.g << 8);
	xwd->curcolor.blue = (pls->curcolor.b << 8);

	if (!XAllocColor(xwd->display, xwd->map, &xwd->curcolor))
	    xwd->curcolor.pixel = xwd->fgcolor.pixel;

	XSetForeground(xwd->display, xwd->gc, xwd->curcolor.pixel);
    }
    else
	XSetForeground(xwd->display, xwd->gc, xwd->cmap0[icol0].pixel);
}

/*----------------------------------------------------------------------*\
* xw_text()
*
* Switch to text mode.
\*----------------------------------------------------------------------*/

void
xw_text(PLStream *pls)
{
}

/*----------------------------------------------------------------------*\
* xw_graph()
*
* Switch to graphics mode.
\*----------------------------------------------------------------------*/

void
xw_graph(PLStream *pls)
{
}

/*----------------------------------------------------------------------*\
* xw_width()
*
* Set pen width.
\*----------------------------------------------------------------------*/

void
xw_width(PLStream *pls)
{
}

/*----------------------------------------------------------------------*\
* xw_esc()
*
* Escape function.
\*----------------------------------------------------------------------*/

void
xw_esc(PLStream *pls, PLINT op, char *ptr)
{
    int id = devtable[pls->ipls][pls->ipld];
    XwDev *xwd = &(xwdev[id]);

    switch (op) {
      case 0:
	break;
    }
}

/*----------------------------------------------------------------------*\
* Main initialization routine.
\*----------------------------------------------------------------------*/

static void
xw_Xinit(PLStream *pls)
{
    int id = devtable[pls->ipls][pls->ipld];
    XwDev *xwd = &(xwdev[id]);

    Window root;
    XSizeHints hint;
    int x, y;
    U_INT width, height;
    U_INT border_width, border;
    U_INT depth;
    char header[80];
    Cursor cross_cursor;

    xwd->display = XOpenDisplay(pls->FileName);
    if (xwd->display == NULL) {
	fprintf(stderr, "Can't open display\n");
	exit(1);
    }
    border = 5;
    (void) XGetGeometry(xwd->display, DefaultRootWindow(xwd->display),
			&root, &x, &y, &width, &height,
			&border_width, &depth);

    if (pls->xlength == 0)
	pls->xlength = 7 * width / 8;
    if (pls->ylength == 0)
	pls->ylength = 7 * height / 8;
    if (pls->xlength > width)
	pls->xlength = width - border * 2;
    if (pls->ylength > height)
	pls->ylength = height - border * 2;

    if (pls->xoffset == 0)
	pls->xoffset = width / 20;
    if (pls->yoffset == 0)
	pls->yoffset = height / 20;

    xwd->screen = DefaultScreen(xwd->display);
    xwd->map = DefaultColormap(xwd->display, xwd->screen);

/* Default color values */

    xw_colini(pls);

/* Default program-specified window position and size */

    hint.x = (int) pls->xoffset;
    hint.y = (int) pls->yoffset;
    hint.width = (int) pls->xlength;
    hint.height = (int) pls->ylength;
    hint.flags = PPosition | PSize;

/* Window title */

    if (idev == 0)
	sprintf(header, "plplot");
    else
	sprintf(header, "plplot_%d", idev);

/* Window creation */
/* Why is the server ignoring the x & y values??? */

    xwd->window =
	XCreateSimpleWindow(xwd->display,
			    DefaultRootWindow(xwd->display),
			    hint.x, hint.y, hint.width, hint.height,
			    border, xwd->fgcolor.pixel, xwd->cmap0[0].pixel);

    XSetStandardProperties(xwd->display, xwd->window, header, header,
			   None, 0, 0, &hint);

/* GC creation and initialization */

    xwd->gc = XCreateGC(xwd->display, xwd->window, 0, 0);

/* Set cursor to crosshair */

    cross_cursor = XCreateFontCursor(xwd->display, XC_crosshair);
    XDefineCursor(xwd->display, xwd->window, cross_cursor);

/* Input event selection */

    XSelectInput(xwd->display, xwd->window,
		 ButtonPressMask | KeyPressMask |
		 ExposureMask | StructureNotifyMask);

/* Window mapping */

    XMapRaised(xwd->display, xwd->window);

    XSetBackground(xwd->display, xwd->gc, xwd->cmap0[0].pixel);
    xw_color(pls);

/* Wait for exposure */
/* Also need to remove extraneous expose events from the event queue */

    for (;;) {
	XNextEvent(xwd->display, &xwd->theEvent);
	if (xwd->theEvent.type == Expose) {
	    while (XCheckMaskEvent(xwd->display, ExposureMask, &xwd->theEvent))
		;
	    break;
	}
    }

/* Get initial drawing area dimensions */

    (void) XGetGeometry(xwd->display, xwd->window, &root, &x, &y,
			&width, &height, &border_width, &depth);

    xwd->init_width = width;
    xwd->init_height = height;

    xwd->cur_width = xwd->init_width;
    xwd->cur_height = xwd->init_height;

    xwd->xscale = (double) xwd->cur_width / (double) xwd->init_width;
    xwd->yscale = (double) xwd->cur_height / (double) xwd->init_height;
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
    int id = devtable[pls->ipls][pls->ipld];
    XwDev *xwd = &(xwdev[id]);

    if (pls->nopause)
	return;

    while (!xwd->exit_eventloop) {
	XNextEvent(xwd->display, &xwd->theEvent);
	EventHandler(pls, &xwd->theEvent);
    }
    xwd->exit_eventloop = FALSE;
}

/*----------------------------------------------------------------------*\
* EventHandler()
*
* Event handler routine to various X events.
* Just redirects control to routines to handle:
*    - keyboard events
*    - mouse events
*    - expose events
*    - resize events
\*----------------------------------------------------------------------*/

static void
EventHandler(PLStream *pls, XEvent *event)
{
    switch (event->type) {
      case KeyPress:
	KeyEH(pls, event);
	break;

      case ButtonPress:
	MouseEH(pls, event);
	break;

      case Expose:
	ExposeEH(pls, event);
	break;

      case ConfigureNotify:
	ResizeEH(pls, event);
	break;
    }
}

/*----------------------------------------------------------------------*\
* KeyEH()
*
* Event handler routine for keyboard events.
\*----------------------------------------------------------------------*/

static void
KeyEH(PLStream *pls, XEvent *event)
{
    int id = devtable[pls->ipls][pls->ipld];
    XwDev *xwd = &(xwdev[id]);

    KeySym keysym;
    PLKey key;
    int count;
    XComposeStatus cs;

/* Translate event into string */

    count = XLookupString((XKeyEvent *) event, key.string, PL_NKEYSTRING,
			  &keysym, &cs);
    key.string[count] = '\0';
    key.code = (U_LONG) keysym;

#ifdef DEBUG
    printf("Keysym %x, translation: %s\n", keysym, key.string);
#endif

/* Set key attributes */
/* INDENT OFF */

    key.isKeypadKey       = IsKeypadKey(keysym);
    key.isCursorKey       = IsCursorKey(keysym);
    key.isPFKey           = IsPFKey(keysym);
    key.isFunctionKey     = IsFunctionKey(keysym);
    key.isMiscFunctionKey = IsMiscFunctionKey(keysym);
    key.isModifierKey     = IsModifierKey(keysym);

/* INDENT ON */
/* Call user event handler */
/* Since this is called first, the user can disable all plplot internal
   event handling by setting key.code to 0 and key.string to '\0' */

    if (pls->KeyEH != NULL)
	(*pls->KeyEH) (&key, pls->KeyEH_data, &xwd->exit_eventloop);

/* Handle internal events */

/* Advance to next page (i.e. terminate event loop) on a <eol> */
/* Check for both <CR> and <LF> for portability, also a <Page Down> */

    if (key.code == PLK_Return ||
	key.code == PLK_Linefeed ||
	key.code == PLK_Next)
	xwd->exit_eventloop = TRUE;

/* Terminate on a 'Q' (not 'q', since it's too easy to hit by mistake) */

    if (key.string[0] == 'Q') {
	pls->nopause = TRUE;
	plexit("");
    }
}

/*----------------------------------------------------------------------*\
* MouseEH()
*
* Event handler routine for mouse events.
* On:
*   Button1: nothing for now
*   Button2: spit out device space coordinates
*   Button3: set page advance flag for later processing
\*----------------------------------------------------------------------*/

static void
MouseEH(PLStream *pls, XEvent *event)
{
    int id = devtable[pls->ipls][pls->ipld];
    XwDev *xwd = &(xwdev[id]);

    switch (event->xbutton.button) {
    case Button1:
	break;

    case Button2:
	printf("%d\t%d\n", event->xbutton.x, event->xbutton.y);
	break;

    case Button3:
	xwd->exit_eventloop = TRUE;
	break;
    }
}

/*----------------------------------------------------------------------*\
* ExposeEH()
*
* Event handler routine for expose events.
* These are "pure" exposures (no resize), so don't need to clear window.
\*----------------------------------------------------------------------*/

static void
ExposeEH(PLStream *pls, XEvent *event)
{
    int id = devtable[pls->ipls][pls->ipld];
    XwDev *xwd = &(xwdev[id]);

    XFlush(xwd->display);
    plRemakePlot(pls);

/* Remove extraneous expose events from the event queue */

    while (XCheckMaskEvent(xwd->display, ExposureMask, event))
	;
}

/*----------------------------------------------------------------------*\
* ResizeEH()
*
* Event handler routine for resize events.
\*----------------------------------------------------------------------*/

static void
ResizeEH(PLStream *pls, XEvent *event)
{
    int id = devtable[pls->ipls][pls->ipld];
    XwDev *xwd = &(xwdev[id]);
    int old_width, old_height;

    old_width = xwd->cur_width;
    old_height = xwd->cur_height;

    xwd->cur_width = event->xconfigure.width;
    xwd->cur_height = event->xconfigure.height;

    xwd->xscale = (double) xwd->cur_width / (double) xwd->init_width;
    xwd->yscale = (double) xwd->cur_height / (double) xwd->init_height;

    xwd->xscale = xwd->xscale * xwd->xscale_dev;
    xwd->yscale = xwd->yscale * xwd->yscale_dev;

/* Only need to refresh if size is actually changed */

    if (old_width == xwd->cur_width && old_height == xwd->cur_height)
	return;

    XFlush(xwd->display);
    XClearWindow(xwd->display, xwd->window);
    plRemakePlot(pls);

/* Remove extraneous expose events from the event queue */

    while (XCheckMaskEvent(xwd->display, ExposureMask, event))
	;
}

/*----------------------------------------------------------------------*\
* xw_colini()
*
* Does all color initialization.
\*----------------------------------------------------------------------*/

static void
xw_colini(PLStream *pls)
{
    int id = devtable[pls->ipls][pls->ipld];
    XwDev *xwd = &(xwdev[id]);
    int i, gslevbg, gslevfg;

    xwd->monochrome = AreWeMonochrome(xwd->display);

/* Default is color IF the user hasn't specified and IF the output device
   is not monochrome or grayscale. */

    if (!pls->colorset) {
	if (xwd->monochrome)
	    pls->color = 0;
	else
	    pls->color = 1;
    }

/*
* Allocate background color.
* Note that black & white allocations should never fail.
*/

    gslevbg = ((float) pls->cmap0[0].r +
	       (float) pls->cmap0[0].g +
	       (float) pls->cmap0[0].b ) / 3.;

    if (!XAllocColor(xwd->display, xwd->map, &xwd->cmap0[0])) {
	if (gslevbg < 128)
	    gslevbg = 0;
	else
	    gslevbg = 255;

	pls->cmap0[0].r = gslevbg;
	pls->cmap0[0].g = gslevbg;
	pls->cmap0[0].b = gslevbg;

	PLColor_to_XColor(&xwd->cmap0[0], &pls->cmap0[0]);

	if (!XAllocColor(xwd->display, xwd->map, &xwd->cmap0[0])) {
	    fprintf(stderr, "Can't allocate background color\n");
	    exit(1);
	}
    }

/*
* Foreground color.
* Normally there is no use for a "foreground color", since a palette with
* a variety of colors is used.  But for grayscale output it makes sense to
* use it, otherwise the plots can become nearly unreadable (i.e. if colors
* get mapped onto grayscale values).  In this case it becomes the grayscale
* level for all draws, and is taken to be black if the background is light,
* and white if the background is dark.  Also, the X initialization routines
* require you to input a foreground color, although this is irrelevant if
* a color palette is being used.
*/

    if (gslevbg < 128)
	gslevfg = 255;
    else
	gslevfg = 0;

    pls->fgcolor.r = gslevfg;
    pls->fgcolor.g = gslevfg;
    pls->fgcolor.b = gslevfg;

    PLColor_to_XColor(&xwd->fgcolor, &pls->fgcolor);

    if (!XAllocColor(xwd->display, xwd->map, &xwd->fgcolor)) {
	fprintf(stderr, "Can't allocate foreground color\n");
	exit(1);
    }

    if (xwd->monochrome) {
	for (i = 1; i < pls->ncol0; i++) {
	    pls->cmap0[i].r = pls->fgcolor.r;
	    pls->cmap0[i].g = pls->fgcolor.g;
	    pls->cmap0[i].b = pls->fgcolor.b;
	}
    }

/* Allocate colors in palette 0 */

    for (i = 1; i < pls->ncol0; i++) {

	PLColor_to_XColor(&xwd->cmap0[i], &pls->cmap0[i]);

	if (!XAllocColor(xwd->display, xwd->map, &xwd->cmap0[i]))
	    XColor_to_XColor(&xwd->cmap0[i], &xwd->fgcolor);
    }
}

/*----------------------------------------------------------------------*\
* void PLColor_to_XColor()
*
* Self-explanatory.
\*----------------------------------------------------------------------*/

static void
PLColor_to_XColor(XColor * xcolor, PLColor *plcolor)
{
    xcolor->red = plcolor->r << 8;
    xcolor->green = plcolor->g << 8;
    xcolor->blue = plcolor->b << 8;
}

/*----------------------------------------------------------------------*\
* void XColor_to_XColor()
*
* Self-explanatory.
\*----------------------------------------------------------------------*/

static void
XColor_to_XColor(XColor * xcolor1, XColor * xcolor2)
{
    xcolor1->red = xcolor2->red;
    xcolor1->green = xcolor2->green;
    xcolor1->blue = xcolor2->blue;
    xcolor1->pixel = xcolor2->pixel;
}

/*----------------------------------------------------------------------*\
* Misc. support routines.
\*----------------------------------------------------------------------*/

/* gmf 11-8-91; Courtesy of Paul Martz of Evans and Sutherland. */

static int
AreWeMonochrome(Display * display)
{
#if defined(__cplusplus) || defined(c_plusplus)
#define THING c_class
#else
#define THING class
#endif

    XVisualInfo *visuals;
    int nitems, i;

    /* get a list of info on the visuals available */
    visuals = XGetVisualInfo(display, 0, NULL, &nitems);

    /* check the list looking for non-monochrome visual classes */
    for (i = 0; i < nitems; i++)
	if ((visuals[i].THING != GrayScale) &&
	    (visuals[i].THING != StaticGray))
	    return (0);

    /* if we got this far, only StaticGray and GrayScale classes available */
    return (1);
}

#else
int
pldummy_xwin()
{
    return 0;
}

#endif				/* XWIN */
