/* $Id$
 * $Log$
 * Revision 1.19  1993/07/16 22:17:00  mjl
 * Added escape function to redraw plot (unlike resize, the window size remains
 * unchanged).  Changed draw functions to only draw to the pixmap during page
 * redraws.  Pixmap is then copied to the actual window when done, for faster
 * and more smooth response.
 *
 * Revision 1.18  1993/07/01  22:05:48  mjl
 * Changed all plplot source files to include plplotP.h (private) rather than
 * plplot.h.  Rationalized namespace -- all externally-visible plplot functions
 * now start with "pl"; device driver functions start with "plD_".  X driver
 * enhanced to plot to a specified window id.  A pixmap is now used for
 * handling expose events.  External entries for doing expose, resize, and
 * redraw events were created using the escape function (for use from TK
 * plframe widget).  The X-driver device data is now dynamically allocated,
 * for use by multiple streams.  The default size of the created X window
 * has been reduced some.
 *
 * Revision 1.17  1993/03/16  06:49:24  mjl
 * Changed driver functions that check for events to do so only after a
 * specified number of calls, to reduce overhead.
 *
 * Revision 1.16  1993/03/15  21:42:14  mjl
 * Changed _clear/_page driver functions to the names _eop/_bop, to be
 * more representative of what's actually going on.  Also moved clear window
 * call to the _bop function to support plot interrupts by plrender (seeks
 * before the plot is completed).
 *
 * Revision 1.15  1993/03/06  05:00:39  mjl
 * Fixed a bug in foreground plotting for grayscale devices when the bg color
 * was set to black.
 *
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
 * Swapped background/foreground colors for grayscale X output.
 *
 * Revision 1.1  1992/05/20  21:32:46  furnish
 * Initial checkin of the whole PLPLOT project.
 *
*/

/*	xwin.c

	PLPLOT X-windows device driver.
*/
#ifdef XWIN
#define USE_PIXMAP

#include "plplotP.h"
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

static void  xw_Xinit		(PLStream *);
static void  xw_Xinit_main	(PLStream *);
static void  xw_Xinit_child	(PLStream *);
static void  WaitForPage	(PLStream *);
static void  HandleEvents	(PLStream *);
static void  xw_colini		(PLStream *);
static int   AreWeGrayscale	(Display *);
static void  PLColor_to_XColor	(XColor *, PLColor *);
static void  XColorcpy		(XColor *, XColor *);

static void  EventHandler	(PLStream *, XEvent *);
static void  KeyEH		(PLStream *, XEvent *);
static void  MouseEH		(PLStream *, XEvent *);
static void  ExposeEH		(PLStream *, XEvent *);
static void  ResizeEH		(PLStream *, XEvent *);
static void  xw_expose		(PLStream *);
static void  xw_resize		(PLStream *, PLWindow *);
static void  xw_redraw		(PLStream *);

/* Struct to hold device-specific info. */

typedef struct {
    int		exit_eventloop;
    long	init_width;
    long	init_height;

    U_INT	width, height;
    U_INT	depth, border;

    double	xscale;
    double	yscale;
    double	xscale_dev;
    double	yscale_dev;

    short xold, yold;		/* Coordinates of last point plotted */
    short xlen, ylen;		/* Lengths of device coordinate space */

    int		color;
    XColor	cmap0[16];
    XColor	bgcolor;
    XColor	fgcolor;
    XColor	curcolor;

    int		write_to_window;
    int		write_to_pixmap;

    int		is_main;
    int		screen;
    Display	*display;
    Window	window;
    GC		gc;
    Colormap	map;
    Pixmap	pixmap;
} XwDev;

/* Miscellaneous global definitions */

static PLFLT lpage_x = 238.0;		/* Page length in x in virtual mm */
static PLFLT lpage_y = 178.0;		/* Page length in y in virtual mm */

/* INDENT ON */
/*----------------------------------------------------------------------*\
* plD_init_xw()
*
* Initialize device.
* X-dependent stuff done in one of the xw_Xinit_* routines.
\*----------------------------------------------------------------------*/

void
plD_init_xw(PLStream *pls)
{
    XwDev *dev;
    int xmin = 0;
    int xmax = PIXELS_X - 1;
    int ymin = 0;
    int ymax = PIXELS_Y - 1;

    float pxlx = (double) PIXELS_X / (double) LPAGE_X;
    float pxly = (double) PIXELS_Y / (double) LPAGE_Y;

    dbug_enter("plD_init_xw");

    pls->termin = 1;		/* is an interactive terminal */
    pls->icol0 = 1;
    pls->width = 1;
    pls->bytecnt = 0;
    pls->page = 0;
    pls->plbuf_write = 1;

/* Allocate and initialize device-specific data */

    pls->dev = malloc((size_t) sizeof(XwDev));
    if (pls->dev == NULL)
	plexit("plD_init_xw: Out of memory.");

    memset(pls->dev, 0, sizeof(XwDev));
    dev = (XwDev *) pls->dev;

    dev->write_to_window = 1;
    dev->write_to_pixmap = 1;

/* X-specific initialization */

    xw_Xinit(pls);

/* Get ready for plotting */

    dev->xold = UNDEFINED;
    dev->yold = UNDEFINED;

    dev->xlen = xmax - xmin;
    dev->ylen = ymax - ymin;

    dev->xscale_dev = (double) (dev->init_width-1) / (double) dev->xlen;
    dev->yscale_dev = (double) (dev->init_height-1) / (double) dev->ylen;

    dev->xscale = dev->xscale_dev;
    dev->yscale = dev->yscale_dev;

    plP_setpxl(pxlx, pxly);
    plP_setphy(xmin, xmax, ymin, ymax);
}

/*----------------------------------------------------------------------*\
* plD_line_xw()
*
* Draw a line in the current color from (x1,y1) to (x2,y2).
\*----------------------------------------------------------------------*/

void
plD_line_xw(PLStream *pls, short x1a, short y1a, short x2a, short y2a)
{
    int x1 = x1a, y1 = y1a, x2 = x2a, y2 = y2a;
    XwDev *dev = (XwDev *) pls->dev;
    static long count = 0, max_count = 20;

    if ( (++count/max_count)*max_count == count) {
	count = 0;
	HandleEvents(pls);	/* Check for events */
    }

    y1 = dev->ylen - y1;
    y2 = dev->ylen - y2;

    x1 = x1 * dev->xscale;
    x2 = x2 * dev->xscale;
    y1 = y1 * dev->yscale;
    y2 = y2 * dev->yscale;

    if (dev->write_to_window)
	XDrawLine(dev->display, dev->window, dev->gc, x1, y1, x2, y2);

    if (dev->write_to_pixmap)
	XDrawLine(dev->display, dev->pixmap, dev->gc, x1, y1, x2, y2);
}

/*----------------------------------------------------------------------*\
* plD_polyline_xw()
*
* Draw a polyline in the current color from (x1,y1) to (x2,y2).
\*----------------------------------------------------------------------*/

void
plD_polyline_xw(PLStream *pls, short *xa, short *ya, PLINT npts)
{
    PLINT i;
    XPoint pts[PL_MAXPOLYLINE];
    XwDev *dev = (XwDev *) pls->dev;
    static long count = 0, max_count = 10;

    if ( (++count/max_count)*max_count == count) {
	count = 0;
	HandleEvents(pls);	/* Check for events */
    }

    if (npts > PL_MAXPOLYLINE)
	plexit("Error -- too many points in polyline\n");

    for (i = 0; i < npts; i++) {
	pts[i].x = dev->xscale * xa[i];
	pts[i].y = dev->yscale * (dev->ylen - ya[i]);
    }

    if (dev->write_to_window)
	XDrawLines(dev->display, dev->window, dev->gc, pts, npts,
		   CoordModeOrigin);

    if (dev->write_to_pixmap)
	XDrawLines(dev->display, dev->pixmap, dev->gc, pts, npts,
		   CoordModeOrigin);
}

/*----------------------------------------------------------------------*\
* plD_eop_xw()
*
* End of page.  User must click left mouse button to continue.
\*----------------------------------------------------------------------*/

void
plD_eop_xw(PLStream *pls)
{
    XwDev *dev = (XwDev *) pls->dev;

    dbug_enter("plD_eop_xw");

    XFlush(dev->display);
    WaitForPage(pls);
}

/*----------------------------------------------------------------------*\
* plD_bop_xw()
*
* Set up for the next page.
\*----------------------------------------------------------------------*/

void
plD_bop_xw(PLStream *pls)
{
    XwDev *dev = (XwDev *) pls->dev;

    dbug_enter("plD_bop_xw");

    if (dev->write_to_window) {
	XClearWindow(dev->display, dev->window);
    }
    if (dev->write_to_pixmap) {
	XSetForeground(dev->display, dev->gc, dev->bgcolor.pixel);
	XFillRectangle(dev->display, dev->pixmap, dev->gc, 0, 0,
		       dev->width, dev->height);
	XSetForeground(dev->display, dev->gc, dev->curcolor.pixel);
    }
    XSync(dev->display, 0);
    pls->page++;
}

/*----------------------------------------------------------------------*\
* plD_tidy_xw()
*
* Close graphics file
\*----------------------------------------------------------------------*/

void
plD_tidy_xw(PLStream *pls)
{
    XwDev *dev = (XwDev *) pls->dev;

    dbug_enter("plD_tidy_xw");

    XFreeGC(dev->display, dev->gc);
    if (dev->is_main) {
	XDestroyWindow(dev->display, dev->window);
	if (dev->write_to_pixmap) 
	    XFreePixmap(dev->display, dev->pixmap);
	XCloseDisplay(dev->display);
    }

    pls->fileset = 0;
    pls->page = 0;
    pls->plbuf_write = 0;
    pls->OutFile = NULL;
    free((void *) pls->dev);
}

/*----------------------------------------------------------------------*\
* plD_color_xw()
*
* Set pen color.
\*----------------------------------------------------------------------*/

void
plD_color_xw(PLStream *pls)
{
    XwDev *dev = (XwDev *) pls->dev;
    int icol0 = pls->icol0;

    HandleEvents(pls);	/* Check for events */

    if (!dev->color) {
	dev->curcolor.pixel = dev->fgcolor.pixel;
	XSetForeground(dev->display, dev->gc, dev->curcolor.pixel);
    }
    else {
	if (icol0 == PL_RGB_COLOR) {
	    dev->curcolor.red = (pls->curcolor.r << 8);
	    dev->curcolor.green = (pls->curcolor.g << 8);
	    dev->curcolor.blue = (pls->curcolor.b << 8);

	    if (!XAllocColor(dev->display, dev->map, &dev->curcolor))
		dev->curcolor.pixel = dev->fgcolor.pixel;

	    XSetForeground(dev->display, dev->gc, dev->curcolor.pixel);
	}
	else
	    XSetForeground(dev->display, dev->gc, dev->cmap0[icol0].pixel);
    }
}

/*----------------------------------------------------------------------*\
* plD_text_xw()
*
* Switch to text mode.
\*----------------------------------------------------------------------*/

void
plD_text_xw(PLStream *pls)
{
    HandleEvents(pls);	/* Check for events */
}

/*----------------------------------------------------------------------*\
* plD_graph_xw()
*
* Switch to graphics mode.
\*----------------------------------------------------------------------*/

void
plD_graph_xw(PLStream *pls)
{
    HandleEvents(pls);	/* Check for events */
}

/*----------------------------------------------------------------------*\
* plD_width_xw()
*
* Set pen width.
\*----------------------------------------------------------------------*/

void
plD_width_xw(PLStream *pls)
{
}

/*----------------------------------------------------------------------*\
* plD_esc_xw()
*
* Escape function.
\*----------------------------------------------------------------------*/

void
plD_esc_xw(PLStream *pls, PLINT op, void *ptr)
{
    dbug_enter("plD_esc_xw");

    switch (op) {
      case PL_EXPOSE:
	xw_expose(pls);
	break;

      case PL_RESIZE:
	xw_resize(pls, (PLWindow *) ptr);
	break;

      case PL_REDRAW:
	xw_redraw(pls);
	break;
    }
}

/*----------------------------------------------------------------------*\
* xw_Xinit()
*
* Xlib initialization routine.
*
* Controlling routine for X window creation and/or initialization.
* The user may customize the window in the following ways:
*
* display:	pls->OutFile (use plsfnam() or -display option) 
* size:		pls->xlength, pls->ylength (use plspage() or -geo option)
* bg color:	pls->bgcol (use plsbgcol() or -bg option)
\*----------------------------------------------------------------------*/

static void
xw_Xinit(PLStream *pls)
{
    XwDev *dev = (XwDev *) pls->dev;
    Window root;
    int x, y;

    dbug_enter("xw_Xinit");

    dev->display = XOpenDisplay(pls->FileName);
    if (dev->display == NULL) {
	fprintf(stderr, "Can't open display\n");
	exit(1);
    }

    dev->screen = DefaultScreen(dev->display);
    dev->map = DefaultColormap(dev->display, dev->screen);

/* Default color values */

    xw_colini(pls);

/* If not plotting into a child window, need to create main window now */

    if (pls->window_id == 0)
	xw_Xinit_main(pls);
    else
	xw_Xinit_child(pls);

/* Get initial drawing area dimensions */

    (void) XGetGeometry(dev->display, dev->window, &root, &x, &y,
			&dev->width, &dev->height, &dev->border, &dev->depth);

    dev->init_width = dev->width;
    dev->init_height = dev->height;

/* Create pixmap for holding plot image (for expose events). */

    if (dev->write_to_pixmap) 
	dev->pixmap = XCreatePixmap(dev->display, dev->window,
				    dev->width, dev->height, dev->depth);

/* Set drawing color */

    plD_color_xw(pls);
}

/*----------------------------------------------------------------------*\
* xw_Xinit_main()
*
* Xlib initialization routine for main window.
*
* Creates window & GC, sets up event handlers, and finally maps the window
* and waits for exposure.
\*----------------------------------------------------------------------*/

static void
xw_Xinit_main(PLStream *pls)
{
    XwDev *dev = (XwDev *) pls->dev;

    Window root;
    XSizeHints hint;
    int x, y;
    U_INT width, height, border, depth;
    char header[80];
    Cursor cross_cursor;
    XEvent the_event;

    dbug_enter("xw_Xinit_main");

    dev->is_main = TRUE;

/* Get root window geometry */

    (void) XGetGeometry(dev->display, DefaultRootWindow(dev->display),
			&root, &x, &y, &width, &height, &border, &depth);

    dev->border = 5;
    if (pls->xlength == 0)
	pls->xlength = width * 0.75;
    if (pls->ylength == 0)
	pls->ylength = height * 0.75;
    if (pls->xlength > width)
	pls->xlength = width - dev->border * 2;
    if (pls->ylength > height)
	pls->ylength = height - dev->border * 2;

    if (pls->xoffset == 0)
	pls->xoffset = width / 20;
    if (pls->yoffset == 0)
	pls->yoffset = height / 20;

/* Default program-specified window position and size */

    hint.x = (int) pls->xoffset;
    hint.y = (int) pls->yoffset;
    hint.width = (int) pls->xlength;
    hint.height = (int) pls->ylength;
    hint.flags = PPosition | PSize;

/* Window title */

    sprintf(header, "plplot");

/* Window creation */
/* Why is the server ignoring the x & y values??? */

    dev->window =
	XCreateSimpleWindow(dev->display,
			    DefaultRootWindow(dev->display),
			    hint.x, hint.y, hint.width, hint.height,
			    dev->border,
			    dev->fgcolor.pixel, dev->bgcolor.pixel);

    XSetStandardProperties(dev->display, dev->window, header, header,
			   None, 0, 0, &hint);

/* GC creation and initialization */

    dev->gc = XCreateGC(dev->display, dev->window, 0, 0);

/* Set cursor to crosshair */

    cross_cursor = XCreateFontCursor(dev->display, XC_crosshair);
    XDefineCursor(dev->display, dev->window, cross_cursor);

/* Input event selection */

    XSelectInput(dev->display, dev->window,
		 ButtonPressMask | KeyPressMask |
		 ExposureMask | StructureNotifyMask);

/* Window mapping */

    XMapRaised(dev->display, dev->window);

    XSetBackground(dev->display, dev->gc, dev->bgcolor.pixel);

/* Wait for exposure */
/* Also need to remove extraneous expose events from the event queue */

    for (;;) {
	XNextEvent(dev->display, &the_event);
	if (the_event.type == Expose) {
	    while (XCheckMaskEvent(dev->display, ExposureMask, &the_event));
	    break;
	}
    }
}

/*----------------------------------------------------------------------*\
* xw_Xinit_child()
*
* Xlib initialization routine for child window.
*
* The window ID is stored as pls->window_id.  If 0, the normal
* startup is used.
\*----------------------------------------------------------------------*/

static void
xw_Xinit_child(PLStream *pls)
{
    XwDev *dev = (XwDev *) pls->dev;

    dbug_enter("xw_Xinit_child");

    dev->is_main = FALSE;
    dev->window = pls->window_id;

/* GC creation and initialization */

    dev->gc = XCreateGC(dev->display, dev->window, 0, 0);
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
    XwDev *dev = (XwDev *) pls->dev;
    XEvent event;

    dbug_enter("WaitForPage");

    if (pls->nopause || ! dev->is_main)
	return;

    while (!dev->exit_eventloop) {
	XNextEvent(dev->display, &event);
	EventHandler(pls, &event);
    }
    dev->exit_eventloop = FALSE;
}

/*----------------------------------------------------------------------*\
* HandleEvents()
*
* Just a front-end to EventHandler(), for use when not actually waiting for
* an event but only checking the event queue.  Right now the ONLY event we
* check for is a KeyPress or ButtonPress, since the plot buffer logic does
* not support multiple writes of the plot buffer in a single page.
\*----------------------------------------------------------------------*/

static void
HandleEvents(PLStream *pls)
{
    XwDev *dev = (XwDev *) pls->dev;
    XEvent event;

    if ( ! dev->is_main)
	return;

    if (XCheckMaskEvent(dev->display, ButtonPressMask | KeyPressMask, &event))
	EventHandler(pls, &event);
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
    XwDev *dev = (XwDev *) pls->dev;

    KeySym keysym;
    PLKey key;
    int count;
    XComposeStatus cs;

    dbug_enter("KeyEH");

/* Translate event into string */

    count = XLookupString((XKeyEvent *) event, key.string, PL_NKEYSTRING,
			  &keysym, &cs);
    key.string[count] = '\0';
    key.code = (U_LONG) keysym;

#ifdef DEBUG
    printf("Keysym %x, translation: %s\n", keysym, key.string);
#endif

/* Call user event handler */
/* Since this is called first, the user can disable all plplot internal
   event handling by setting key.code to 0 and key.string to '\0' */

    if (pls->KeyEH != NULL)
	(*pls->KeyEH) (&key, pls->KeyEH_data, &dev->exit_eventloop);

/* Handle internal events */

/* Advance to next page (i.e. terminate event loop) on a <eol> */
/* Check for both <CR> and <LF> for portability, also a <Page Down> */

    if (key.code == PLK_Return ||
	key.code == PLK_Linefeed ||
	key.code == PLK_Next)
	dev->exit_eventloop = TRUE;

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
    XwDev *dev = (XwDev *) pls->dev;

    dbug_enter("MouseEH");

    switch (event->xbutton.button) {
      case Button1:
	break;

      case Button2:
	printf("%d\t%d\n", event->xbutton.x, event->xbutton.y);
	break;

      case Button3:
	dev->exit_eventloop = TRUE;
	break;
    }
}

/*----------------------------------------------------------------------*\
* ExposeEH()
*
* Event handler routine for expose events.
* Front end to xw_expose() to deal with wierdnesses of Xlib.
\*----------------------------------------------------------------------*/

static void
ExposeEH(PLStream *pls, XEvent *event)
{
    XwDev *dev = (XwDev *) pls->dev;

    dbug_enter("ExposeEH");

/* Handle expose */

    xw_expose(pls);

/* Remove extraneous expose events from the event queue */

    while (XCheckMaskEvent(dev->display, ExposureMask, event));
}

/*----------------------------------------------------------------------*\
* ResizeEH()
*
* Event handler routine for resize events.
* Front end to xw_resize() to deal with wierdnesses of Xlib.
\*----------------------------------------------------------------------*/

static void
ResizeEH(PLStream *pls, XEvent *event)
{
    XwDev *dev = (XwDev *) pls->dev;
    PLWindow newwin;

    dbug_enter("ResizeEH");

    newwin.width = event->xconfigure.width;
    newwin.height = event->xconfigure.height;

/* Only need to resize if size is actually changed */

    if (newwin.width == dev->width && newwin.height == dev->height)
	return;

    xw_resize(pls, &newwin);

/* Remove extraneous expose events from the event queue */
/* These are not needed since we've redrawn the whole plot */

    while (XCheckMaskEvent(dev->display, ExposureMask, event));
}

/*----------------------------------------------------------------------*\
* xw_expose()
*
* Event handler routine for expose events.
* These are "pure" exposures (no resize), so don't need to clear window.
* Note: this function is callable from the outside world.
\*----------------------------------------------------------------------*/

static void
xw_expose(PLStream *pls)
{
    XwDev *dev = (XwDev *) pls->dev;

    dbug_enter("xw_expose");

/* Return if plD_init_xw hasn't been called yet */

    if (dev == NULL) {
	plwarn("xw_expose: Illegal call -- driver uninitialized");
	return;
    }

/* Usual case: refresh window from pixmap */

    XSync(dev->display, 0);
    if (dev->write_to_pixmap) {
	XCopyArea(dev->display, dev->pixmap, dev->window, dev->gc, 0, 0,
		  dev->width, dev->height, 0, 0);
	XSync(dev->display, 0);
    }
    else {
	plRemakePlot(pls);
	XFlush(dev->display);
    }
}

/*----------------------------------------------------------------------*\
* xw_resize()
*
* Event handler routine for resize events.
* Note: this function is callable from the outside world.
\*----------------------------------------------------------------------*/

static void
xw_resize(PLStream *pls, PLWindow *window)
{
    XwDev *dev = (XwDev *) pls->dev;

    dbug_enter("xw_resize");

/* Return if plD_init_xw hasn't been called yet */

    if (dev == NULL) {
	plwarn("xw_resize: Illegal call -- driver uninitialized");
	return;
    }

/* Reset current window bounds */

    dev->width = window->width;
    dev->height = window->height;

    dev->xscale = (double) (dev->width-1) / (double) dev->init_width;
    dev->yscale = (double) (dev->height-1) / (double) dev->init_height;

    dev->xscale = dev->xscale * dev->xscale_dev;
    dev->yscale = dev->yscale * dev->yscale_dev;

/* Need to regenerate pixmap copy of window using new dimensions */

    if (dev->write_to_pixmap) {
	XFreePixmap(dev->display, dev->pixmap);
	dev->pixmap = XCreatePixmap(dev->display, dev->window,
				    dev->width, dev->height, dev->depth);
    }

/* Now do a redraw using the new size */

    xw_redraw(pls);
}

/*----------------------------------------------------------------------*\
* xw_redraw()
*
* Handles page redraw without resize (pixmap does not get reallocated).
* Calling this makes sure all necessary housekeeping gets done.
\*----------------------------------------------------------------------*/

static void
xw_redraw(PLStream *pls)
{
    XwDev *dev = (XwDev *) pls->dev;

    dbug_enter("xw_redraw");

/* Return if plD_init_xw hasn't been called yet */

    if (dev == NULL) {
	plwarn("xw_redraw: Illegal call -- driver uninitialized");
	return;
    }

/* Initialize & redraw to pixmap.  Then fake an expose. */

    if (dev->write_to_pixmap)
	dev->write_to_window = 0;

    plD_bop_xw(pls);
    plRemakePlot(pls);

    XSync(dev->display, 0);
    if (dev->write_to_pixmap) {
	XCopyArea(dev->display, dev->pixmap, dev->window, dev->gc, 0, 0,
		  dev->width, dev->height, 0, 0);
	XSync(dev->display, 0);
    }
    else {
	plRemakePlot(pls);
	XFlush(dev->display);
    }
    dev->write_to_window = 1;
}

/*----------------------------------------------------------------------*\
* xw_colini()
*
* Does all color initialization.
\*----------------------------------------------------------------------*/

static void
xw_colini(PLStream *pls)
{
    XwDev *dev = (XwDev *) pls->dev;
    int i, gslevbg, gslevfg;

/*
* Default is color IF the user hasn't specified and IF the output device is
* not grayscale.  
*/

    if (pls->colorset)
	dev->color = pls->color;
    else {
	pls->color = 1;
	dev->color = !AreWeGrayscale(dev->display);
    }

/*
* Allocate background color.
*
* Background defaults to black on color screens, white on grayscale (many
* grayscale monitors have poor contrast, and black-on-white looks better).
* Note that black & white allocations should never fail.
*/

    if (!dev->color && !pls->bgcolorset) {
	pls->bgcolor.r = 255;
	pls->bgcolor.g = 255;
	pls->bgcolor.b = 255;
    }

    gslevbg = ((float) pls->bgcolor.r +
	       (float) pls->bgcolor.g +
	       (float) pls->bgcolor.b) / 3.;

    if (!dev->color) {
	if (gslevbg < 128)
	    gslevbg = 0;
	else
	    gslevbg = 255;

	pls->bgcolor.r = gslevbg;
	pls->bgcolor.g = gslevbg;
	pls->bgcolor.b = gslevbg;
    }

    PLColor_to_XColor(&dev->bgcolor, &pls->bgcolor);

    if (!XAllocColor(dev->display, dev->map, &dev->bgcolor)) {
	fprintf(stderr, "Can't allocate background color\n");
	exit(1);
    }

/*
* Foreground color.
*
* Normally there is no use for a "foreground color", since a palette with a
* variety of colors is used.  But for grayscale output it makes sense to use
* it, otherwise the plots can become nearly unreadable (i.e. if colors get
* mapped onto grayscale values).  In this case it becomes the grayscale level
* for all draws, and is taken to be black if the background is light, and
* white if the background is dark.  We also use the foreground color for (a)
* input to XCreateSimpleWindow (although the choice is basically irrelevant
* since a color palette is being used), and (b) as the color to use if the
* call to XAllocColor fails at runtime.
*/

    if (gslevbg < 128)
	gslevfg = 255;
    else
	gslevfg = 0;

    pls->fgcolor.r = gslevfg;
    pls->fgcolor.g = gslevfg;
    pls->fgcolor.b = gslevfg;

    PLColor_to_XColor(&dev->fgcolor, &pls->fgcolor);

    if (!XAllocColor(dev->display, dev->map, &dev->fgcolor)) {
	fprintf(stderr, "Can't allocate foreground color\n");
	exit(1);
    }

/* Allocate colors in palette 0 */

    if (dev->color) {
	for (i = 0; i < pls->ncol0; i++) {

	    PLColor_to_XColor(&dev->cmap0[i], &pls->cmap0[i]);

	    if (!XAllocColor(dev->display, dev->map, &dev->cmap0[i]))
		XColorcpy(&dev->cmap0[i], &dev->fgcolor);
	}
    }
}

/*----------------------------------------------------------------------*\
* void PLColor_to_XColor()
*
* Self-explanatory.
\*----------------------------------------------------------------------*/

static void
PLColor_to_XColor(XColor *xcolor, PLColor *plcolor)
{
    xcolor->red = plcolor->r << 8;
    xcolor->green = plcolor->g << 8;
    xcolor->blue = plcolor->b << 8;
}

/*----------------------------------------------------------------------*\
* void XColorcpy()
*
* Self-explanatory.
\*----------------------------------------------------------------------*/

static void
XColorcpy(XColor *xcolor1, XColor *xcolor2)
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
AreWeGrayscale(Display *display)
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
