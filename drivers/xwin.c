/* $Id$
 * $Log$
 * Revision 1.32  1994/04/08 11:43:40  mjl
 * Some improvements to color map 1 color allocation so that it will fail
 * less often (temporary, until custom color map support is added).  Master
 * event handler with user entry point added.  Escape function recognizes
 * the PLESC_EH command for handling pending events only, and a mouse handler
 * added (both contributed by Radey Shouman).
 *
 * Revision 1.31  1994/03/23  06:41:25  mjl
 * Added support for: color map 1 color selection, color map 0 or color map 1
 * state change (palette change), polygon fills.  Color map 1 allocator tries
 * to allocate as many colors as possible (minus a small number to be
 * friendly to TK) and reports the result.  Eventually this will be improved
 * to use a custom colormap, copying the colors used by the window manager
 * to reduce the flicker when changing focus.
 *
 * All drivers: cleaned up by eliminating extraneous includes (stdio.h and
 * stdlib.h now included automatically by plplotP.h), extraneous clears
 * of pls->fileset, pls->page, and pls->OutFile = NULL (now handled in
 * driver interface or driver initialization as appropriate).  Special
 * handling for malloc includes eliminated (no longer needed) and malloc
 * prototypes fixed as necessary.
 *
 * Revision 1.30  1994/01/25  06:18:34  mjl
 * Added double buffering capability.
*/

/*	xwin.c

	PLPLOT X-windows device driver.
*/
#ifdef XWIN

#include "plplotP.h"
#include "plplotX.h"
#include "drivers.h"
#include "plevent.h"

/* If this is kept small enough the initial allocation will always succeed
 * Subsequent copies of plplot will allocate the same colors, hopefully,
 * and the color cells will be shared.  If there is a problem, reduce 
 * NCOL1_MAX or pls->ncol1.  Will be fixed once I figure out how to
 * use custom color maps.
 */

#define NCOL1_MAX 50

/* Function prototypes */
/* INDENT OFF */

static void  Init		(PLStream *);
static void  Init_main		(PLStream *);
static void  Init_child		(PLStream *);
static void  WaitForPage	(PLStream *);
static void  HandleEvents	(PLStream *);
static void  ColorInit		(PLStream *);
static void  Cmap0Init		(PLStream *pls);
static void  Cmap1Init		(PLStream *pls);
static void  CreatePixmap	(PLStream *);
static void  fill_polygon	(PLStream *pls);
static void  Colorcpy		(XColor *, XColor *);

static void  MasterEH		(PLStream *, XEvent *);
static void  KeyEH		(PLStream *, XEvent *);
static void  MouseEH		(PLStream *, XEvent *);
static void  ExposeEH		(PLStream *, XEvent *);
static void  ResizeEH		(PLStream *, XEvent *);

static void  ExposeCmd		(PLStream *);
static void  ResizeCmd		(PLStream *, PLWindow *);
static void  RedrawCmd		(PLStream *);

/* INDENT ON */
/*----------------------------------------------------------------------*\
* plD_init_xw()
*
* Initialize device.
* X-dependent stuff done in one of the Init_* routines.
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
    pls->dev_flush = 1;		/* Want to handle our own flushes */
    pls->dev_fill0 = 1;		/* Can do solid fills */

/* Allocate and initialize device-specific data */

    if (pls->dev != NULL)
	free((void *) pls->dev);

    pls->dev = calloc(1, (size_t) sizeof(XwDev));
    if (pls->dev == NULL)
	plexit("plD_init_xw: Out of memory.");

    dev = (XwDev *) pls->dev;

/* Set up flags that determine what we are writing to */
/* If nopixmap is set, ignore db */

    if (pls->nopixmap) {
	dev->write_to_pixmap = 0;
	pls->db = 0;
    }
    else
	dev->write_to_pixmap = 1;

    dev->write_to_window = ! pls->db;

/* X-specific initialization */

    Init(pls);

/* Get ready for plotting */

    dev->xold = UNDEFINED;
    dev->yold = UNDEFINED;

    dev->xlen = xmax - xmin;
    dev->ylen = ymax - ymin;

    dev->xscale_init = (double) (dev->init_width-1) / (double) dev->xlen;
    dev->yscale_init = (double) (dev->init_height-1) / (double) dev->ylen;

    dev->xscale = dev->xscale_init;
    dev->yscale = dev->yscale_init;

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
    XwDev *dev = (XwDev *) pls->dev;
    int x1 = x1a, y1 = y1a, x2 = x2a, y2 = y2a;
    static long count = 0, max_count = 20;

    if (dev->is_main) {
	if ( (++count/max_count)*max_count == count) {
	    count = 0;
	    HandleEvents(pls);	/* Check for events */
	}
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
    XwDev *dev = (XwDev *) pls->dev;
    PLINT i;
    XPoint pts[PL_MAXPOLY];
    static long count = 0, max_count = 10;

    if (dev->is_main) {
	if ( (++count/max_count)*max_count == count) {
	    count = 0;
	    HandleEvents(pls);	/* Check for events */
	}
    }

    if (npts > PL_MAXPOLY)
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
    if (pls->db)
	ExposeCmd(pls);
	
    if ( ! pls->nopause)
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

    pls->plbuf_write = 0;
}

/*----------------------------------------------------------------------*\
* plD_state_xw()
*
* Handle change in PLStream state (color, pen width, fill attribute, etc).
\*----------------------------------------------------------------------*/

void 
plD_state_xw(PLStream *pls, PLINT op)
{
    XwDev *dev = (XwDev *) pls->dev;

    dbug_enter("plD_state_xw");

    if (dev->is_main) {
	HandleEvents(pls);	/* Check for events */
    }

    switch (op) {

    case PLSTATE_WIDTH:
	break;

    case PLSTATE_COLOR0:{
	int icol0 = pls->icol0;
	if ( ! dev->color) {
	    dev->curcolor.pixel = dev->fgcolor.pixel;
	    XSetForeground(dev->display, dev->gc, dev->curcolor.pixel);
	}
	else {
	    if (icol0 == PL_RGB_COLOR) {
		PLColor_to_XColor(&pls->curcolor, &dev->curcolor);

		if ( ! XAllocColor(dev->display, dev->map, &dev->curcolor)) {
		    fprintf(stderr, "Warning: could not allocate color\n");
		    dev->curcolor.pixel = dev->fgcolor.pixel;
		}

		XSetForeground(dev->display, dev->gc, dev->curcolor.pixel);
	    }
	    else
		XSetForeground(dev->display, dev->gc, dev->cmap0[icol0].pixel);
	}
	break;
    }

    case PLSTATE_COLOR1:{
	int icol1 = (pls->icol1 * (dev->ncol1-1)) / (pls->ncol1-1);
	if ( ! dev->color) {
	    dev->curcolor.pixel = dev->fgcolor.pixel;
	    XSetForeground(dev->display, dev->gc, dev->curcolor.pixel);
	}
	else {
	    XSetForeground(dev->display, dev->gc, dev->cmap1[icol1].pixel);
	}
	break;
    }
    }
}

/*----------------------------------------------------------------------*\
* plD_esc_xw()
*
* Escape function.
*
* Functions:
*
*	PLESC_EXPOSE	Force an expose
*	PLESC_RESIZE	Force a resize
*	PLESC_REDRAW	Force a redraw
*	PLESC_FLUSH	Flush X event buffer
*	PLESC_FILL	Fill polygon
*	PLESC_EH	Handle events only
*
\*----------------------------------------------------------------------*/

void
plD_esc_xw(PLStream *pls, PLINT op, void *ptr)
{
    XwDev *dev = (XwDev *) pls->dev;

    dbug_enter("plD_esc_xw");

    switch (op) {
    case PLESC_EXPOSE:
	ExposeCmd(pls);
	break;

    case PLESC_RESIZE:
	ResizeCmd(pls, (PLWindow *) ptr);
	break;

    case PLESC_REDRAW:
	RedrawCmd(pls);
	break;

    case PLESC_FLUSH:
	HandleEvents(pls);
	XFlush(dev->display);
	break;

    case PLESC_FILL:
	fill_polygon(pls);
	break;

    case PLESC_EH:
	HandleEvents(pls);
	break;
    }
}

/*----------------------------------------------------------------------*\
* fill_polygon()
*
* Fill polygon described in points pls->dev_x[] and pls->dev_y[].
* Only solid color fill supported.
\*----------------------------------------------------------------------*/

static void
fill_polygon(PLStream *pls)
{
    XwDev *dev = (XwDev *) pls->dev;
    XPoint pts[PL_MAXPOLY];
    static long count = 0, max_count = 10;
    int i, outline_only=0;

    if (dev->is_main) {
	if ( (++count/max_count)*max_count == count) {
	    count = 0;
	    HandleEvents(pls);	/* Check for events */
	}
    }

    if (pls->dev_npts > PL_MAXPOLY)
	plexit("Error -- too many points in polygon\n");

    for (i = 0; i < pls->dev_npts; i++) {
	pts[i].x = dev->xscale * pls->dev_x[i];
	pts[i].y = dev->yscale * (dev->ylen - pls->dev_y[i]);
    }

/*
 * For an interesting effect during fills, turn on outline_only.
 * Mostly for debugging.
 */

    if (outline_only) {
	if (dev->write_to_window)
	    XDrawLines(dev->display, dev->window, dev->gc, pts, pls->dev_npts,
		       CoordModeOrigin);

	if (dev->write_to_pixmap)
	    XDrawLines(dev->display, dev->pixmap, dev->gc, pts, pls->dev_npts,
		       CoordModeOrigin);
    }
    else {
	if (dev->write_to_window)
	    XFillPolygon(dev->display, dev->window, dev->gc,
			 pts, pls->dev_npts, Nonconvex, CoordModeOrigin);

	if (dev->write_to_pixmap)
	    XFillPolygon(dev->display, dev->pixmap, dev->gc,
			 pts, pls->dev_npts, Nonconvex, CoordModeOrigin);
    }
}

/*----------------------------------------------------------------------*\
* Init()
*
* Xlib initialization routine.
*
* Controlling routine for X window creation and/or initialization.
* The user may customize the window in the following ways:
*
* display:	pls->OutFile (use plsfnam() or -display option) 
* size:		pls->xlength, pls->ylength (use plspage() or -geo option)
* bg color:	pls->bgcolor (use plscolbg() or -bg option)
\*----------------------------------------------------------------------*/

static void
Init(PLStream *pls)
{
    XwDev *dev = (XwDev *) pls->dev;
    Window root;
    int x, y;

    dbug_enter("Init");

    dev->display = XOpenDisplay(pls->FileName);
    if (dev->display == NULL) {
	fprintf(stderr, "Can't open display\n");
	exit(1);
    }

    dev->screen = DefaultScreen(dev->display);

/* If not plotting into a child window, need to create main window now */

    if (pls->window_id == 0)
	Init_main(pls);
    else
	Init_child(pls);

/* Create color map */

#ifdef CUSTOM_COLOR_MAP
/* Right now this just results in a core dump somewhere. */

    dev->map = XCreateColormap(dev->display, dev->window,
			       DirectColor, AllocNone);
#else
    dev->map = DefaultColormap(dev->display, dev->screen);
#endif

/* Default color values */

    ColorInit(pls);

/* Get initial drawing area dimensions */

    (void) XGetGeometry(dev->display, dev->window, &root, &x, &y,
			&dev->width, &dev->height, &dev->border, &dev->depth);

    dev->init_width = dev->width;
    dev->init_height = dev->height;

/* Create pixmap for holding plot image (for expose events). */

    if (dev->write_to_pixmap) 
	CreatePixmap(pls);

/* Set drawing color */

    plD_state_xw(pls, PLSTATE_COLOR0);
}

/*----------------------------------------------------------------------*\
* Init_main()
*
* Xlib initialization routine for main window.
*
* Creates window & GC, sets up event handlers, and finally maps the window
* and waits for exposure.
\*----------------------------------------------------------------------*/

static void
Init_main(PLStream *pls)
{
    XwDev *dev = (XwDev *) pls->dev;

    Window root;
    XSizeHints hint;
    int x, y;
    U_INT width, height, border, depth;
    char header[80];
    Cursor cross_cursor;
    XEvent the_event;

    dbug_enter("Init_main");

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
/* Why is the window manager ignoring the x & y values??? */

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
* Init_child()
*
* Xlib initialization routine for child window.
*
* The window ID is stored as pls->window_id.  If 0, the normal
* startup is used.
\*----------------------------------------------------------------------*/

static void
Init_child(PLStream *pls)
{
    XwDev *dev = (XwDev *) pls->dev;

    dbug_enter("Init_child");

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

    if ( ! dev->is_main) {
	HandleEvents(pls);	/* Check for events */
	return;
    }

    while ( ! dev->exit_eventloop) {
	XNextEvent(dev->display, &event);
	MasterEH(pls, &event);
    }
    dev->exit_eventloop = FALSE;
}

/*----------------------------------------------------------------------*\
* HandleEvents()
*
* Just a front-end to MasterEH(), for use when not actually waiting for
* an event but only checking the event queue.  
\*----------------------------------------------------------------------*/

static void
HandleEvents(PLStream *pls)
{
    XwDev *dev = (XwDev *) pls->dev;
    XEvent event;

    if (XCheckMaskEvent(dev->display, ButtonPressMask | KeyPressMask |
			ExposureMask | StructureNotifyMask, &event))
	MasterEH(pls, &event);
}

/*----------------------------------------------------------------------*\
* MasterEH()
*
* Master X event handler routine.
* Redirects control to routines to handle:
*    - keyboard events
*    - mouse events
*    - expose events
*    - resize events
*
* By supplying a master event handler, the user can take over all event
* processing.  If events other than those trapped by plplot need handling,
* just call XSelectInput with the appropriate flags.  The default PLPlot
* event handling can be modified arbitrarily by changing the event struct.
\*----------------------------------------------------------------------*/

static void
MasterEH(PLStream *pls, XEvent *event)
{
    XwDev *dev = (XwDev *) pls->dev;

    if (dev->MasterEH != NULL)
	(*dev->MasterEH) (pls, event);

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

/* Call user keypress event handler */
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

    PLMouse mouse;

    dbug_enter("MouseEH");

    mouse.button = event->xbutton.button;
    mouse.state = event->xbutton.state;
    mouse.x = (PLFLT)event->xbutton.x / pls->xlength;
    mouse.y = 1.0 - (PLFLT)event->xbutton.y / pls->ylength;
      
/* Call user event handler */
/* Since this is called first, the user can disable all plplot internal
   event handling by setting mouse.button to 0 */

    if (pls->MouseEH != NULL)
	(*pls->MouseEH) (&mouse, pls->MouseEH_data, &dev->exit_eventloop);

    switch (mouse.button) {
    case Button1:
	break;

    case Button2:
	printf("%d\t%d\n", mouse.x, mouse.y);
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
* Front end to ExposeCmd() to deal with wierdnesses of Xlib.
\*----------------------------------------------------------------------*/

static void
ExposeEH(PLStream *pls, XEvent *event)
{
    XwDev *dev = (XwDev *) pls->dev;

    dbug_enter("ExposeEH");

/* Handle expose */

    ExposeCmd(pls);

/* Remove extraneous expose events from the event queue */

    while (XCheckMaskEvent(dev->display, ExposureMask, event))
	;
}

/*----------------------------------------------------------------------*\
* ResizeEH()
*
* Event handler routine for resize events.
* Front end to ResizeCmd() to deal with wierdnesses of Xlib.
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

    ResizeCmd(pls, &newwin);

/* Remove extraneous expose events from the event queue */
/* These are not needed since we've redrawn the whole plot */

    while (XCheckMaskEvent(dev->display, ExposureMask, event))
	;
}

/*----------------------------------------------------------------------*\
* ExposeCmd()
*
* Event handler routine for expose events.
* These are "pure" exposures (no resize), so don't need to clear window.
\*----------------------------------------------------------------------*/

static void
ExposeCmd(PLStream *pls)
{
    XwDev *dev = (XwDev *) pls->dev;

    dbug_enter("ExposeCmd");

/* Return if plD_init_xw hasn't been called yet */

    if (dev == NULL) {
	plwarn("ExposeCmd: Illegal call -- driver uninitialized");
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
* ResizeCmd()
*
* Event handler routine for resize events.
* Note: this function is callable from the outside world.
\*----------------------------------------------------------------------*/

static void
ResizeCmd(PLStream *pls, PLWindow *window)
{
    XwDev *dev = (XwDev *) pls->dev;

    dbug_enter("ResizeCmd");

/* Return if plD_init_xw hasn't been called yet */

    if (dev == NULL) {
	plwarn("ResizeCmd: Illegal call -- driver uninitialized");
	return;
    }

/* Return if pointer to window not specified. */

    if (window == NULL) {
	plwarn("ResizeCmd: Illegal call -- window pointer uninitialized");
	return;
    }

/* Reset current window bounds */

    dev->width = window->width;
    dev->height = window->height;

    dev->xscale = (double) (dev->width-1) / (double) dev->init_width;
    dev->yscale = (double) (dev->height-1) / (double) dev->init_height;

    dev->xscale = dev->xscale * dev->xscale_init;
    dev->yscale = dev->yscale * dev->yscale_init;

/* Need to regenerate pixmap copy of window using new dimensions */

    if (dev->write_to_pixmap) {
	XSync(dev->display, 0);
	XFreePixmap(dev->display, dev->pixmap);
	CreatePixmap(pls);
    }

/* Now do a redraw using the new size */

    RedrawCmd(pls);
}

/*----------------------------------------------------------------------*\
* RedrawCmd()
*
* Handles page redraw without resize (pixmap does not get reallocated).
* Calling this makes sure all necessary housekeeping gets done.
\*----------------------------------------------------------------------*/

static void
RedrawCmd(PLStream *pls)
{
    XwDev *dev = (XwDev *) pls->dev;

    dbug_enter("RedrawCmd");

/* Return if plD_init_xw hasn't been called yet */

    if (dev == NULL) {
	plwarn("RedrawCmd: Illegal call -- driver uninitialized");
	return;
    }

/* Initialize & redraw to pixmap.  Then fake an expose. */

    if (dev->write_to_pixmap)
	dev->write_to_window = 0;

    XSync(dev->display, 0);
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

    dev->write_to_window = ! pls->db;
}

/*----------------------------------------------------------------------*\
* CreatePixmapErrorHandler()
*
* Error handler used in CreatePixmap() to catch errors in allocating
* storage for pixmap.  This way we can nicely substitute redraws for
* pixmap copies if the server has insufficient memory.
\*----------------------------------------------------------------------*/

static unsigned char CreatePixmapStatus;

static int
CreatePixmapErrorHandler(Display *display, XErrorEvent *error)
{
    if (error->error_code == BadAlloc) {
	CreatePixmapStatus = error->error_code;
    }
    else
	fprintf(stderr, "Something bad just happened.\n");

    return(1);
}	

/*----------------------------------------------------------------------*\
* CreatePixmap()
*
* This routine creates a pixmap, doing error trapping in case there
* isn't enough memory on the server.
\*----------------------------------------------------------------------*/

static void
CreatePixmap(PLStream *pls)
{
    XwDev *dev = (XwDev *) pls->dev;
    int (*oldErrorHandler)();

    oldErrorHandler = XSetErrorHandler(CreatePixmapErrorHandler);

    CreatePixmapStatus = Success;
#ifdef DEBUG
    fprintf(stderr, "Creating pixmap of width %d, height %d, depth %d\n",
	    dev->width, dev->height, dev->depth);
#endif
    dev->pixmap = XCreatePixmap(dev->display, dev->window,
				dev->width, dev->height, dev->depth);
    XSync(dev->display, 0);
    if (CreatePixmapStatus != Success) {
	dev->write_to_pixmap = 0;
	dev->write_to_window = 1;
	pls->db = 0;
	fprintf(stderr, "\n\
Warning: pixmap could not be allocated (insufficient memory on server).\n\
Driver will redraw the entire plot to handle expose events.\n");
    }

    XSetErrorHandler(oldErrorHandler);
}

/*----------------------------------------------------------------------*\
* ColorInit()
*
* Does all color initialization.
\*----------------------------------------------------------------------*/

static void
ColorInit(PLStream *pls)
{
    XwDev *dev = (XwDev *) pls->dev;
    int gslevbg, gslevfg;

/*
* Default is color IF the user hasn't specified and IF the output device is
* not grayscale.  
*/

    if (pls->colorset)
	dev->color = pls->color;
    else {
	pls->color = 1;
	dev->color = ! pl_AreWeGrayscale(dev->display);
    }

/*
* Allocate background color.
*
* Background defaults to black on color screens, white on grayscale (many
* grayscale monitors have poor contrast, and black-on-white looks better).
* Note that black & white allocations should never fail.
*/

    if ( ! dev->color && ! pls->bgcolorset) {
	pls->bgcolor.r = 0xFF;
	pls->bgcolor.g = 0xFF;
	pls->bgcolor.b = 0xFF;
    }
    gslevbg = ((long) pls->bgcolor.r + (long) pls->bgcolor.g +
	       (long) pls->bgcolor.b) / 3;

    PLColor_to_XColor(&pls->bgcolor, &dev->bgcolor);

    if ( ! XAllocColor(dev->display, dev->map, &dev->bgcolor)) {
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

    if (gslevbg <= 0x7F)
	gslevfg = 0xFF;
    else
	gslevfg = 0;

    pls->fgcolor.r = gslevfg;
    pls->fgcolor.g = gslevfg;
    pls->fgcolor.b = gslevfg;

    PLColor_to_XColor(&pls->fgcolor, &dev->fgcolor);

    if ( ! XAllocColor(dev->display, dev->map, &dev->fgcolor)) {
	fprintf(stderr, "Can't allocate foreground color\n");
	exit(1);
    }

/* Allocate colors in cmap 0 & 1 */

    if (dev->color) {
	Cmap0Init(pls);
	Cmap1Init(pls);
    }
}

/*----------------------------------------------------------------------*\
* Cmap0Init()
*
* Initializes cmap 0
\*----------------------------------------------------------------------*/

static void
Cmap0Init(PLStream *pls)
{
    XwDev *dev = (XwDev *) pls->dev;
    int i;

    for (i = 0; i < pls->ncol0; i++) {

	PLColor_to_XColor(&pls->cmap0[i], &dev->cmap0[i]);

	if ( ! XAllocColor(dev->display, dev->map, &dev->cmap0[i]))
	    Colorcpy(&dev->cmap0[i], &dev->fgcolor);
    }
}

/*----------------------------------------------------------------------*\
* Cmap1Init()
*
* Initializes cmap 1
*
* There is a basic chicken-and-egg problem here:
* (a) the palette isn't known until the number of colors is available
*     since a smooth color variation is used.
* (b) the number of colors depends on the palette, because colors
*     already allocated do not consume a new color cell slot.
*
* The way to handle this is to start with some initial target for number
* of colors.  Some considerations: if your display supports 256 colors
* it's best to stay well below that since otherwise you will run out of
* unallocated color cells.  Fortunately, beyond 100 or so colors is
* getting pretty good if you get them all.  Unfortunately there's no
* guarantee you WILL get them all (i.e. they are distinct) unless you
* install your own color map, since X will return the closest color
* without error.  Eventually the X driver will support custom color maps,
* but the interaction with TK will have to be worked out in detail.
*
* If the initial allocation should fail, I just free all the allocated
* colors and try again using the number of colors I succeeded in
* allocating the first time as the limit (minus a small number).  This is
* done in a loop that terminates after 5 tries.  There is no way of
* telling a priori how many colors you will get (I usually get 50-70, and
* sometimes more).
* 
\*----------------------------------------------------------------------*/

static void
Cmap1Init(PLStream *pls)
{
    XwDev *dev = (XwDev *) pls->dev;
    PLColor newcolor;
    unsigned long pixels[NCOL1_MAX];
    int i, itry, ncol1_max;

/* Use substantially less than max colors so that TK has some */

    dev->ncol1 = MIN(NCOL1_MAX, pls->ncol1);
    dev->ncol1 = MAX(dev->ncol1, 2);

/* Initialize dev->cmap1 by interpolation, then allocate them for real */
/* It should NOT fail, but put in failsafe procedure just in case */

    for (itry=0;;itry++) {
	if (itry > 5)
	    plexit("Cannot allocate color map 1");

	for (i = 0; i < dev->ncol1; i++) {

	    plcol_interp(pls, &newcolor, i, dev->ncol1);
	    PLColor_to_XColor(&newcolor, &dev->cmap1[i]);

	    if ( ! XAllocColor(dev->display, dev->map, &dev->cmap1[i])) 
		break;

	    pixels[i] = dev->cmap1[i].pixel;
	}

/* If we got all we asked for, return */

	if (i == dev->ncol1)
	    break;

/* Failed, so need to free the allocated colors and try again */

	XFreeColors(dev->display, dev->map, pixels, i, 0);
	dev->ncol1 = MAX(i-5, 1);
    }
    fprintf(stderr, "Allocated %d colors in cmap1\n", i);
}

/*----------------------------------------------------------------------*\
* void Colorcpy()
*
* Self-explanatory.
\*----------------------------------------------------------------------*/

static void
Colorcpy(XColor *xcolor1, XColor *xcolor2)
{
    xcolor1->red = xcolor2->red;
    xcolor1->green = xcolor2->green;
    xcolor1->blue = xcolor2->blue;
    xcolor1->pixel = xcolor2->pixel;
}

/*----------------------------------------------------------------------*\
* void PLColor_to_XColor()
*
* Copies the supplied PLColor to an XColor, padding with bits as necessary
* (a PLColor uses 8 bits for color storage, while an XColor uses 16 bits).
* The argument types follow the same order as in the function name.
\*----------------------------------------------------------------------*/

#define ToXColor(a) (((0xFF & (a)) << 8) | (a))
#define ToPLColor(a) (((U_LONG) a) >> 8)

void
PLColor_to_XColor(PLColor *plcolor, XColor *xcolor)
{
    xcolor->red   = ToXColor(plcolor->r);
    xcolor->green = ToXColor(plcolor->g);
    xcolor->blue  = ToXColor(plcolor->b);
}

/*----------------------------------------------------------------------*\
* void PLColor_from_XColor()
*
* Copies the supplied XColor to a PLColor, stripping off bits as
* necessary.  See the previous routine for more info.
\*----------------------------------------------------------------------*/

void
PLColor_from_XColor(PLColor *plcolor, XColor *xcolor)
{
    plcolor->r = ToPLColor(xcolor->red);
    plcolor->g = ToPLColor(xcolor->green);
    plcolor->b = ToPLColor(xcolor->blue);
}

/*----------------------------------------------------------------------*\
* int pl_AreWeGrayscale(Display *display)
*
* gmf 11-8-91; Courtesy of Paul Martz of Evans and Sutherland. 
\*----------------------------------------------------------------------*/

int
pl_AreWeGrayscale(Display *display)
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
