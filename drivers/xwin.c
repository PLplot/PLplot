/* $Id$
 * $Log$
 * Revision 1.44  1994/07/18 20:28:46  mjl
 * Fixed a cast.
 *
 * Revision 1.43  1994/07/01  22:38:04  mjl
 * All display-dependent data broken off into a separate structure.  The X
 * driver now saves only one of these XwDisplay structs for each physical
 * X display.  These are then shared between all PLplot streams.  This
 * ensures that using multiple streams -> multiple X windows does not cause
 * abnormal termination due to running out of resources since most are now
 * shared (allocated colors, graphics contexts, etc).  The drawback is that
 * colors are no longer independent between windows, if created by a single
 * process (but this can also be an advantage).
 *
 * Revision 1.42  1994/06/17  21:21:11  mjl
 * Fixed background color for mono displays, should now come up white (much
 * easier to see).  Middle mouse button press now returns world coordinates
 * at cursor, but only correct for last plot on the page (submitted by
 * Mark Glover glover@zombi.res.utc.com).
 *
 * Revision 1.41  1994/06/09  20:05:58  mjl
 * Fixed Visual handling to support grayscale and mono displays.  Enabled
 * exporting of colormap hack flag for other files (plframe.c) that need it.
 *
 * Revision 1.40  1994/05/27  22:19:17  mjl
 * Window now placed properly when offsets (+x+y) are specified.  Thanks
 * to Mark S. Glover (glover@zombi.res.utc.com) for the fix.
*/

/*	xwin.c

	PLPLOT X-windows device driver.
*/
#ifdef XWIN

#include "plplotP.h"
#include "plplotX.h"
#include "drivers.h"
#include "plevent.h"

/* Set constants for dealing with colormap.  In brief:
 *
 * ccmap		When set, turns on custom color map
 *
 * XWM_COLORS		Number of low "pixel" values to copy.  
 * CMAP0_COLORS		Color map 0 entries.  
 * CMAP1_COLORS		Color map 1 entries.  
 * MAX_COLORS		Maximum colors period.
 *
 * See Init_CustomCmap() and  Init_DefaultCmap() for more info.
 * Set ccmap at your own risk -- still under development.
 */

int plplot_ccmap = 0;

#define XWM_COLORS 70
#define CMAP0_COLORS 16
#define CMAP1_COLORS 100
#define MAX_COLORS 256

/* Variables to hold RGB components of given colormap. */
/* Used in an ugly hack to get past some X11R5 and TK limitations. */

static int  sxwm_colors_set;
static XColor sxwm_colors[256];

/* Keep pointers to all the displays in use */

static XwDisplay *xwDisplay[PLXDISPLAYS];

/* Function prototypes */

static void  Init		(PLStream *pls);
static void  Init_main		(PLStream *pls);
static void  Init_Colors	(PLStream *pls);
static void  Init_CustomCmap	(PLStream *pls);
static void  Init_DefaultCmap	(PLStream *pls);
static void  Map_main		(PLStream *pls);
static void  WaitForPage	(PLStream *pls);
static void  HandleEvents	(PLStream *pls);
static void  ColorInit		(PLStream *pls);
static void  Cmap0Init		(PLStream *pls);
static void  Cmap1Init		(PLStream *pls);
static void  CreatePixmap	(PLStream *pls);
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
    XwDisplay *xwd = (XwDisplay *) dev->xwd;

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
	XDrawLine(xwd->display, dev->window, xwd->gc, x1, y1, x2, y2);

    if (dev->write_to_pixmap)
	XDrawLine(xwd->display, dev->pixmap, xwd->gc, x1, y1, x2, y2);
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
    XwDisplay *xwd = (XwDisplay *) dev->xwd;

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
	XDrawLines(xwd->display, dev->window, xwd->gc, pts, npts,
		   CoordModeOrigin);

    if (dev->write_to_pixmap)
	XDrawLines(xwd->display, dev->pixmap, xwd->gc, pts, npts,
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
    XwDisplay *xwd = (XwDisplay *) dev->xwd;

    dbug_enter("plD_eop_xw");

    XFlush(xwd->display);
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
    XwDisplay *xwd = (XwDisplay *) dev->xwd;

    dbug_enter("plD_bop_xw");

    if (dev->write_to_window) {
	XClearWindow(xwd->display, dev->window);
    }
    if (dev->write_to_pixmap) {
	XSetForeground(xwd->display, xwd->gc, xwd->cmap0[0].pixel);
	XFillRectangle(xwd->display, dev->pixmap, xwd->gc, 0, 0,
		       dev->width, dev->height);
	XSetForeground(xwd->display, xwd->gc, xwd->curcolor.pixel);
    }
    XSync(xwd->display, 0);
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
    XwDisplay *xwd = (XwDisplay *) dev->xwd;

    dbug_enter("plD_tidy_xw");

    XFreeGC(xwd->display, xwd->gc);
    if (dev->is_main) {
	XDestroyWindow(xwd->display, dev->window);
	if (dev->write_to_pixmap) 
	    XFreePixmap(xwd->display, dev->pixmap);
	XCloseDisplay(xwd->display);
    }

    xwd->count--;
    if (xwd->count == 0) 
	free_mem(xwDisplay[xwd->ixwd]);

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
    XwDisplay *xwd = (XwDisplay *) dev->xwd;

    dbug_enter("plD_state_xw");

    if (dev->is_main) {
	HandleEvents(pls);	/* Check for events */
    }

    switch (op) {

    case PLSTATE_WIDTH:
	break;

    case PLSTATE_COLOR0:{
	int icol0 = pls->icol0;
	if ( ! xwd->color) {
	    xwd->curcolor.pixel = xwd->fgcolor.pixel;
	    XSetForeground(xwd->display, xwd->gc, xwd->curcolor.pixel);
	}
	else {
	    if (icol0 == PL_RGB_COLOR) {
		PLColor_to_XColor(&pls->curcolor, &xwd->curcolor);

		if ( ! XAllocColor(xwd->display, xwd->map, &xwd->curcolor)) {
		    fprintf(stderr, "Warning: could not allocate color\n");
		    xwd->curcolor.pixel = xwd->fgcolor.pixel;
		}

		XSetForeground(xwd->display, xwd->gc, xwd->curcolor.pixel);
	    }
	    else
		XSetForeground(xwd->display, xwd->gc, xwd->cmap0[icol0].pixel);
	}
	break;
    }

    case PLSTATE_COLOR1:{
	int icol1 = (pls->icol1 * (xwd->ncol1-1)) / (pls->ncol1-1);
	if ( ! xwd->color) {
	    xwd->curcolor.pixel = xwd->fgcolor.pixel;
	    XSetForeground(xwd->display, xwd->gc, xwd->curcolor.pixel);
	}
	else {
	    XSetForeground(xwd->display, xwd->gc, xwd->cmap1[icol1].pixel);
	}
	break;
    }

    case PLSTATE_CMAP0:
	Cmap0Init(pls);
	break;

    case PLSTATE_CMAP1:
	Cmap1Init(pls);
	break;

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
    XwDisplay *xwd = (XwDisplay *) dev->xwd;

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
	XFlush(xwd->display);
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
    XwDisplay *xwd = (XwDisplay *) dev->xwd;
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
	    XDrawLines(xwd->display, dev->window, xwd->gc, pts, pls->dev_npts,
		       CoordModeOrigin);

	if (dev->write_to_pixmap)
	    XDrawLines(xwd->display, dev->pixmap, xwd->gc, pts, pls->dev_npts,
		       CoordModeOrigin);
    }
    else {
	if (dev->write_to_window)
	    XFillPolygon(xwd->display, dev->window, xwd->gc,
			 pts, pls->dev_npts, Nonconvex, CoordModeOrigin);

	if (dev->write_to_pixmap)
	    XFillPolygon(xwd->display, dev->pixmap, xwd->gc,
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
 * bg color:	pls->cmap0[0] (use plscolbg() or -bg option)
\*----------------------------------------------------------------------*/

static void
Init(PLStream *pls)
{
    XwDev *dev = (XwDev *) pls->dev;
    XwDisplay *xwd;

    Window root;
    int i, x, y;

    dbug_enter("Init");

/* See if display matches any already in use, and if so use that */

    dev->xwd = NULL;
    for (i = 0; i < PLXDISPLAYS; i++) {
	if (xwDisplay[i] == NULL)
	    continue;

	if (strcmp(xwDisplay[i]->displayName, pls->FileName) == 0) {
	    dev->xwd = xwDisplay[i];
	    break;
	}
    }

/* If no display matched, create a new one */

    if (dev->xwd == NULL) {
	dev->xwd = (XwDisplay *) calloc(1, (size_t) sizeof(XwDisplay));
	if (dev->xwd == NULL)
	    plexit("Init: Out of memory.");

	for (i = 0; i < PLXDISPLAYS; i++) {
	    if (xwDisplay[i] == NULL)
		break;
	}
	if (i == PLXDISPLAYS) 
	    plexit("Init: Out of xwDisplay's.");

	xwDisplay[i] = xwd = (XwDisplay *) dev->xwd;
	xwd->count = 1;
    }
    else {
	xwd = (XwDisplay *) dev->xwd;
	xwd->count++;
    }
    xwd->ixwd = i;
	
/* Open display, initialize color map and color values */

    if (xwd->count == 1) {
	xwd->display = XOpenDisplay(pls->FileName);
	if (xwd->display == NULL) {
	    fprintf(stderr, "Can't open display\n");
	    exit(1);
	}
	xwd->displayName = pls->FileName;
	xwd->screen = DefaultScreen(xwd->display);

	Init_Colors(pls);
    }

/* If not plotting into a child window, need to create main window now */

    if (pls->window_id == 0) {
	dev->is_main = TRUE;
	Init_main(pls);
    }
    else {
	dev->is_main = FALSE;
	dev->window = pls->window_id;
    }
    XSetWindowColormap( xwd->display, dev->window, xwd->map );

/* GC creation and initialization */

    if (xwd->count == 1) {
	xwd->gc = XCreateGC(xwd->display, dev->window, 0, 0);
    }

/* Get initial drawing area dimensions */

    (void) XGetGeometry(xwd->display, dev->window, &root, &x, &y,
			&dev->width, &dev->height, &dev->border, &xwd->depth);

    dev->init_width = dev->width;
    dev->init_height = dev->height;

/* Create pixmap for holding plot image (for expose events). */

    if (dev->write_to_pixmap) 
	CreatePixmap(pls);

/* Set drawing color */

    plD_state_xw(pls, PLSTATE_COLOR0);

    XSetWindowBackground(xwd->display, dev->window, xwd->cmap0[0].pixel);
    XSetBackground(xwd->display, xwd->gc, xwd->cmap0[0].pixel);

/* If main window, need to map it and wait for exposure */

    if (dev->is_main) 
	Map_main(pls);
}

/*----------------------------------------------------------------------*\
 * Init_main()
 *
 * Create main window using standard Xlib calls.
\*----------------------------------------------------------------------*/

static void
Init_main(PLStream *pls)
{
    XwDev *dev = (XwDev *) pls->dev;
    XwDisplay *xwd = (XwDisplay *) dev->xwd;

    Window root;
    XSizeHints hint;
    int x, y;
    U_INT width, height, border, depth;
    char header[80];
    Cursor cross_cursor;

    dbug_enter("Init_main");

/* Get root window geometry */

    (void) XGetGeometry(xwd->display, DefaultRootWindow(xwd->display),
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
    hint.flags = USPosition | USSize;

/* Window title */

    sprintf(header, "plplot");

/* Window creation */

    dev->window =
	XCreateWindow( xwd->display,
		       DefaultRootWindow(xwd->display),
		       hint.x, hint.y, hint.width, hint.height,
		       dev->border, xwd->depth,
		       InputOutput, xwd->visual,
		       0, NULL );

    XSetStandardProperties(xwd->display, dev->window, header, header,
			   None, 0, 0, &hint);

/* Set cursor to crosshair */

    cross_cursor = XCreateFontCursor(xwd->display, XC_crosshair);
    XDefineCursor(xwd->display, dev->window, cross_cursor);
}

/*----------------------------------------------------------------------*\
 * Map_main()
 *
 * Sets up event handlers, maps main window and waits for exposure.
\*----------------------------------------------------------------------*/

static void
Map_main(PLStream *pls)
{
    XwDev *dev = (XwDev *) pls->dev;
    XwDisplay *xwd = (XwDisplay *) dev->xwd;

    XEvent the_event;

    dbug_enter("Map_main");

/* Input event selection */

    XSelectInput(xwd->display, dev->window,
		 ButtonPressMask | KeyPressMask |
		 ExposureMask | StructureNotifyMask);

/* Window mapping */

    XMapRaised(xwd->display, dev->window);

/* Wait for exposure */
/* Remove extraneous expose events from the event queue */

    for (;;) {
	XNextEvent(xwd->display, &the_event);
	if (the_event.type == Expose) {
	    while (XCheckMaskEvent(xwd->display, ExposureMask, &the_event));
	    break;
	}
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
    XwDev *dev = (XwDev *) pls->dev;
    XwDisplay *xwd = (XwDisplay *) dev->xwd;

    XEvent event;

    dbug_enter("WaitForPage");

    if ( ! dev->is_main) {
	HandleEvents(pls);	/* Check for events */
	return;
    }

    while ( ! dev->exit_eventloop) {
	XNextEvent(xwd->display, &event);
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
    XwDisplay *xwd = (XwDisplay *) dev->xwd;

    XEvent event;

    if (XCheckMaskEvent(xwd->display, ButtonPressMask | KeyPressMask |
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
    float xcoord, ycoord;

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
	printf("%f\t%f\n", mouse.x, mouse.y);
        xcoord=((event->xbutton.x+1-(pls->vpdxmi)*(pls->xlength))/
                ((pls->vpdxma-pls->vpdxmi)*pls->xlength))*pls->vpwxma;
        ycoord=((pls->ylength - event->xbutton.y+1-(pls->vpdymi)*(pls->ylength))/
                ((pls->vpdyma-pls->vpdymi)*pls->ylength))*pls->vpwyma;
	printf("%f\t%f\n", xcoord, ycoord);
#ifdef DEBUG
        printf("[xwin.c:MouseEH:] world X coord = %f\n",
               xcoord);
        printf("[xwin.c:MouseEH:] world Y coord = %f\n",
               ycoord);
#endif /* DEBUG */
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
    XwDisplay *xwd = (XwDisplay *) dev->xwd;

    dbug_enter("ExposeEH");

/* Handle expose */

    ExposeCmd(pls);

/* Remove extraneous expose events from the event queue */

    while (XCheckMaskEvent(xwd->display, ExposureMask, event))
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
    XwDisplay *xwd = (XwDisplay *) dev->xwd;

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

    while (XCheckMaskEvent(xwd->display, ExposureMask, event))
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
    XwDisplay *xwd = (XwDisplay *) dev->xwd;

    dbug_enter("ExposeCmd");

/* Return if plD_init_xw hasn't been called yet */

    if (dev == NULL) {
	plwarn("ExposeCmd: Illegal call -- driver uninitialized");
	return;
    }

/* Usual case: refresh window from pixmap */

    XSync(xwd->display, 0);
    if (dev->write_to_pixmap) {
	XCopyArea(xwd->display, dev->pixmap, dev->window, xwd->gc, 0, 0,
		  dev->width, dev->height, 0, 0);
	XSync(xwd->display, 0);
    }
    else {
	plRemakePlot(pls);
	XFlush(xwd->display);
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
    XwDisplay *xwd = (XwDisplay *) dev->xwd;

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
	XSync(xwd->display, 0);
	XFreePixmap(xwd->display, dev->pixmap);
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
    XwDisplay *xwd = (XwDisplay *) dev->xwd;

    dbug_enter("RedrawCmd");

/* Return if plD_init_xw hasn't been called yet */

    if (dev == NULL) {
	plwarn("RedrawCmd: Illegal call -- driver uninitialized");
	return;
    }

/* Initialize & redraw to pixmap.  Then fake an expose. */

    if (dev->write_to_pixmap)
	dev->write_to_window = 0;

    XSync(xwd->display, 0);
    plD_bop_xw(pls);
    plRemakePlot(pls);

    XSync(xwd->display, 0);
    if (dev->write_to_pixmap) {
	XCopyArea(xwd->display, dev->pixmap, dev->window, xwd->gc, 0, 0,
		  dev->width, dev->height, 0, 0);
	XSync(xwd->display, 0);
    }
    else {
	plRemakePlot(pls);
	XFlush(xwd->display);
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
    XwDisplay *xwd = (XwDisplay *) dev->xwd;

    int (*oldErrorHandler)();

    oldErrorHandler = XSetErrorHandler(CreatePixmapErrorHandler);

    CreatePixmapStatus = Success;
#ifdef DEBUG
    fprintf(stderr, "Creating pixmap of width %d, height %d, depth %d\n",
	    dev->width, dev->height, xwd->depth);
#endif
    dev->pixmap = XCreatePixmap(xwd->display, dev->window,
				dev->width, dev->height, xwd->depth);
    XSync(xwd->display, 0);
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
 * Init_Colors()
 *
 * Does all color initialization.
\*----------------------------------------------------------------------*/

static void
Init_Colors(PLStream *pls)
{
    XwDev *dev = (XwDev *) pls->dev;
    XwDisplay *xwd = (XwDisplay *) dev->xwd;

    Colormap default_map;
    PLColor fgcolor;
    int gslevbg, gslevfg;
    XVisualInfo vTemplate, *visualList;
    int visuals_matched;

    dbug_enter("Init_Colors");

/* Grab default color map */

    default_map = DefaultColormap(xwd->display, xwd->screen);

/* Get visual info */
/* Try for an 8 plane display, if unavailable go for the default */

    vTemplate.screen = xwd->screen;
    vTemplate.depth = 8;

    visualList = XGetVisualInfo( xwd->display,
				 VisualScreenMask | VisualDepthMask,
				 &vTemplate, &visuals_matched );

    if (visuals_matched) {
	xwd->visual = visualList->visual;	/* Choose first match. */
	xwd->depth = vTemplate.depth;
    }
    else {
	xwd->visual = DefaultVisual( xwd->display, xwd->screen );
	xwd->depth = DefaultDepth( xwd->display, xwd->screen );
    }

/*
 * Figure out if we have a color display or not.
 * Default is color IF the user hasn't specified and IF the output device is
 * not grayscale.  
 */

    if (pls->colorset)
	xwd->color = pls->color;
    else {
	pls->color = 1;
	xwd->color = ! pl_AreWeGrayscale(xwd->display);
    }

/*
 * Set background color.
 *
 * Background defaults to black on color screens, white on grayscale (many
 * grayscale monitors have poor contrast, and black-on-white looks better).
 */

    if ( ! xwd->color) {
	pls->cmap0[0].r = pls->cmap0[0].g = pls->cmap0[0].b = 0xFF;
    }
    gslevbg = ((long) pls->cmap0[0].r +
	       (long) pls->cmap0[0].g +
	       (long) pls->cmap0[0].b) / 3;

/*
 * Set foreground color.
 *
 * Used for grayscale output, since otherwise the plots can become nearly
 * unreadable (i.e. if colors get mapped onto grayscale values).  In this
 * case it becomes the grayscale level for all draws, and is taken to be
 * black if the background is light, and white if the background is dark.
 * Note that white/black allocations never fail.
 */

    if (gslevbg > 0x7F) 
	gslevfg = 0;
    else 
	gslevfg = 0xFF;

    fgcolor.r = fgcolor.g = fgcolor.b = gslevfg;

    PLColor_to_XColor(&fgcolor, &xwd->fgcolor);

/* If we're not on a color system, just allocate bg & fg then return */

    if ( ! xwd->color) {
	PLColor_to_XColor(&pls->cmap0[0], &xwd->cmap0[0]);
	XAllocColor(xwd->display, default_map, &xwd->cmap0[0]);
	XAllocColor(xwd->display, default_map, &xwd->fgcolor);
	return;
    }

/* Create custom color map and initialize cmap0 */

    if (plplot_ccmap) 
	Init_CustomCmap(pls);
    else 
	Init_DefaultCmap(pls);
}

/*----------------------------------------------------------------------*\
 * Init_CustomCmap()
 *
 * Initializes custom color map and all the cruft that goes with it.
 *
 * Assuming all color X displays do 256 colors, the breakdown is as follows:
 *
 * XWM_COLORS	Number of low "pixel" values to copy.  These are typically
 *		allocated first, thus are in use by the window manager. I
 *		copy them to reduce flicker.
 *
 * CMAP0_COLORS	Color map 0 entries.  I allocate these both in the default
 *		colormap and the custom colormap to reduce flicker.
 *
 * CMAP1_COLORS	Color map 1 entries.  There should be as many as practical
 *		available for smooth shading.  On the order of 100 is 
 *		pretty reasonable.  You don't really need all 256.  
 *
 * It's important to leave some extra colors unallocated for Tk.  In 
 * particular the palette tools require a fair amount.  I recommend leaving
 * at least 40 or so free.  
\*----------------------------------------------------------------------*/

static void
Init_CustomCmap(PLStream *pls)
{
    XwDev *dev = (XwDev *) pls->dev;
    XwDisplay *xwd = (XwDisplay *) dev->xwd;

    Colormap default_map;
    XColor xwm_colors[MAX_COLORS];
    int i, j, npixels;
    unsigned long plane_masks[1];
    unsigned long pixel, pixels[MAX_COLORS];

    dbug_enter("Init_Colormap");

/* Grab default color map */

    default_map = DefaultColormap(xwd->display, xwd->screen);

/* Determine current default colors */

    for (i = 0; i < MAX_COLORS; i++) {
	xwm_colors[i].pixel = i;
    }
    XQueryColors(xwd->display, default_map, xwm_colors, MAX_COLORS);

/* Allocate cmap0 colors in the default colormap.
 * The custom cmap0 colors are later stored at the same pixel values.
 * This is a really cool trick to reduce the flicker when changing colormaps.
 */

    npixels = MIN(pls->ncol0, MAX_COLORS);
    while(1) {
	if (XAllocColorCells(xwd->display, default_map, False,
			     plane_masks, 0, pixels, npixels))
	    break;
	npixels--;
	if (npixels == 0)
	    plexit("couldn't allocate any colors");
    }

    xwd->ncol0 = npixels;
    for (i = 0; i < xwd->ncol0; i++) {

	PLColor_to_XColor(&pls->cmap0[i], &xwd->cmap0[i]);

	xwd->cmap0[i].pixel = pixels[i];
	XStoreColor(xwd->display, default_map, &xwd->cmap0[i]);
    }
    XAllocColor(xwd->display, default_map, &xwd->fgcolor);

/* Create color map */

    xwd->map = XCreateColormap( xwd->display, DefaultRootWindow(xwd->display),
				xwd->visual, AllocNone );

/* Now allocate all colors so we can fill the ones we want to copy */

    npixels = MAX_COLORS;
    while(1) {
	if (XAllocColorCells(xwd->display, xwd->map, False,
			     plane_masks, 0, pixels, npixels))
	    break;
	npixels--;
	if (npixels == 0)
	    plexit("couldn't allocate any colors");
    }

/* Fill the low colors since those are in use by the window manager */

    for (i = 0; i < XWM_COLORS; i++) {
	XStoreColor(xwd->display, xwd->map, &xwm_colors[i]);
	pixels[xwm_colors[i].pixel] = 0;
    }

/* Fill the ones we will use in cmap0 */

    for (i = 0; i < xwd->ncol0; i++) {
	XStoreColor(xwd->display, xwd->map, &xwd->cmap0[i]);
	pixels[xwd->cmap0[i].pixel] = 0;
    }

/* Finally, if the colormap was saved by an external agent, see if there are
 * any differences from the current default map and save those!  A very cool
 * (or sick, depending on how you look at it) trick to get over some X and
 * Tk limitations.
 */

    if (sxwm_colors_set) {
	for (i = 0; i < MAX_COLORS; i++) {
	    if ((xwm_colors[i].red != sxwm_colors[i].red) ||
		(xwm_colors[i].green != sxwm_colors[i].green) ||
		(xwm_colors[i].blue != sxwm_colors[i].blue) ) {

		if (pixels[i] != 0) {
		    XStoreColor(xwd->display, xwd->map, &xwm_colors[i]);
		    pixels[i] = 0;
		}
	    }
	}
    }

/* Now free the ones we're not interested in */

    for (i = 0; i < npixels; i++) {
	if (pixels[i] != 0)
	    XFreeColors(xwd->display, xwd->map, &pixels[i], 1, 0);
    }

/* Allocate colors in cmap 1 */

    npixels = MAX(2, MIN(CMAP1_COLORS, pls->ncol1));
    while(1) {
	if (XAllocColorCells(xwd->display, xwd->map, False,
			     plane_masks, 0, pixels, npixels))
	    break;
	npixels--;
	if (npixels == 0)
	    plexit("couldn't allocate any colors");
    }

    fprintf(stderr, "Allocated %d colors in cmap1\n", npixels);
    xwd->ncol1 = npixels;
    for (i = 0; i < xwd->ncol1; i++) {
	xwd->cmap1[i].pixel = pixels[i];
    }

    Cmap1Init(pls);
}

/*----------------------------------------------------------------------*\
 * Init_DefaultCmap()
 *
 * Initializes default color map and all the cruft that goes with it.
 * Have to severely limit number of colors in cmap1 otherwise TK won't
 * have enough.
\*----------------------------------------------------------------------*/

static void
Init_DefaultCmap(PLStream *pls)
{
    XwDev *dev = (XwDev *) pls->dev;
    XwDisplay *xwd = (XwDisplay *) dev->xwd;

    Colormap default_map;
    int i, j, npixels;
    unsigned long plane_masks[1];
    unsigned long pixel, pixels[MAX_COLORS];

    dbug_enter("Init_DefaultCmap");

/* Grab default color map */

    xwd->map = DefaultColormap(xwd->display, xwd->screen);

/* Allocate and assign colors in cmap 0 */

    npixels = 16;
    while(1) {
	if (XAllocColorCells(xwd->display, xwd->map, False,
			     plane_masks, 0, pixels, npixels))
	    break;
	npixels--;
	if (npixels == 0)
	    plexit("couldn't allocate any colors");
    }

    xwd->ncol0 = npixels;
    for (i = 0; i < xwd->ncol0; i++) {
	xwd->cmap0[i].pixel = pixels[i];
    }

    Cmap0Init(pls);

/* Allocate colors in cmap 1 */

    npixels = MAX(2, MIN(CMAP1_COLORS, pls->ncol1));
    while(1) {
	if (XAllocColorCells(xwd->display, xwd->map, False,
			     plane_masks, 0, pixels, npixels))
	    break;
	npixels--;
	if (npixels == 0)
	    plexit("couldn't allocate any colors");
    }

/* Now free them all and start with a reduced number */

    XFreeColors(xwd->display, xwd->map, pixels, npixels, 0);

    npixels = MAX(npixels - 30, 10);
    while(1) {
	if (XAllocColorCells(xwd->display, xwd->map, False,
			     plane_masks, 0, pixels, npixels))
	    break;
	npixels--;
	if (npixels == 0)
	    plexit("couldn't allocate any colors");
    }

    fprintf(stderr, "Allocated %d colors in cmap1\n", npixels);
    xwd->ncol1 = npixels;

/* Don't assign pixels sequentially, to avoid strange problems with xor GC's */
/* Skipping by 2 seems to do the job best */

    for (j = i = 0; i < xwd->ncol1; i++) {
	while (pixels[j] == 0) 
	    j++;

	xwd->cmap1[i].pixel = pixels[j];
	pixels[j] = 0;

	j += 2;
	if (j >= xwd->ncol1)
	    j = 0;
    }

    Cmap1Init(pls);
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
    XwDisplay *xwd = (XwDisplay *) dev->xwd;
    int i;

    for (i = 0; i < xwd->ncol0; i++) {
	PLColor_to_XColor(&pls->cmap0[i], &xwd->cmap0[i]);
	XStoreColor(xwd->display, xwd->map, &xwd->cmap0[i]);
    }
}

/*----------------------------------------------------------------------*\
 * Cmap1Init()
 *
 * Initializes cmap 1
\*----------------------------------------------------------------------*/

static void
Cmap1Init(PLStream *pls)
{
    XwDev *dev = (XwDev *) pls->dev;
    XwDisplay *xwd = (XwDisplay *) dev->xwd;

    PLColor cmap1color;
    int i;

    for (i = 0; i < xwd->ncol1; i++) {
	plcol_interp(pls, &cmap1color, i, xwd->ncol1);
	PLColor_to_XColor(&cmap1color, &xwd->cmap1[i]);
	XStoreColor(xwd->display, xwd->map, &xwd->cmap1[i]);
    }
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
    xcolor->flags = DoRed | DoGreen | DoBlue;
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
 * Determines if we're using a monochrome or grayscale device.
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

/*----------------------------------------------------------------------*\
 * void PLX_save_colormap()
 *
 * Saves RGB components of given colormap.
 * Used in an ugly hack to get past some X11R5 and TK limitations.
 * This isn't guaranteed to work under all circumstances, but hopefully
 * in the future there will be a nicer way to accomplish the same thing.
 *
 * Note: I tried using XCopyColormapAndFree to do the same thing, but under
 * HPUX 9.01/VUE/X11R5 at least it doesn't preserve the previous read-only
 * color cell allocations made by Tk.  Is this a bug?  Have to look at the
 * source to find out.
\*----------------------------------------------------------------------*/

void
PLX_save_colormap(Display *display, Colormap colormap)
{
    int i;

    if ( ! plplot_ccmap)
	return;

    sxwm_colors_set = 1;
    for (i = 0; i < 256; i++) {
	sxwm_colors[i].pixel = i;
    }
    XQueryColors(display, colormap, sxwm_colors, 256);
/*
    printf("\nAt startup, default colors are: \n\n");
    for (i = 0; i < 256; i++) {
	printf(" i: %d,  pixel: %d,  r: %d,  g: %d,  b: %d\n",
	       i, sxwm_colors[i].pixel,
	       sxwm_colors[i].red, sxwm_colors[i].green, sxwm_colors[i].blue);
    }
 */
}

#else
int
pldummy_xwin()
{
    return 0;
}

#endif				/* XWIN */
