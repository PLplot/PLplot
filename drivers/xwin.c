/* $Id$
 * $Log$
 * Revision 1.65  1995/06/23 02:56:11  mjl
 * Fix to crossing event handling after leaving locate mode.  Inserted
 * experimental code for using physical window dimensions in the library.
 * Still needs work.
 *
 * Revision 1.64  1995/06/01  21:37:23  mjl
 * Changes to header file inclusion.
 *
 * Revision 1.63  1995/05/19  22:16:58  mjl
 * Undid some of the damage from the last commit.  It turns out that while
 * writes to the screen first look better on a resize, writes to the pixmap
 * first (which is blitted to the screen when the write is finished) look
 * better on a redraw, e.g. when scrolling.  So now the code is no longer
 * shared between them to allow the order of operations to be different in
 * each case.
 *
 * Revision 1.62  1995/05/15  21:54:10  mjl
 * Fixed problem with color allocation that was causing code death on
 * monochrome X terminals.  Reordered resize operations so that window is
 * drawn to first, then pixmap copied after that (more natural looking).
 *
 * Revision 1.61  1995/05/08  20:25:51  mjl
 * Split the crossing event handler into separate enter and leave event
 * handlers to better keep track of whether we should be drawing graphic
 * crosshairs or not.  Now works properly under all combinations of expose,
 * pointer in or out of window, and with or without a pixmap, as far as I can
 * tell.  Added more cool stuff to be done when -debug is specified.
 *
 * Revision 1.60  1995/05/06  17:06:07  mjl
 * Changed debugging output to use new pldebug() function and improved.
 * Reduced max number of cmap1 colors to try to allocate to reduce conflicts
 * with other programs (like Mosaic).  Added event handling for EnterNotify
 * and LeaveNotify events, to draw or eliminate crosshairs (if being drawn),
 * respectively.
 *
 * Revision 1.59  1995/04/12  21:10:17  mjl
 * Made the ordinary graphics context and the current color a device-dependent
 * quantity rather than a display-dependent one, to fix problems with color
 * selection when plotting to separate streams/xwindows.  Thanks to Radey
 * Shouman for pointing it out.
 *
 * Revision 1.58  1995/03/22  17:46:38  mjl
 * Moved initialization of the write_to_pixmap and write_to_window vars to the
 * regular driver initialization part, to give plframe users a way to specify
 * the nopixmap option and have it actually work.
 *
 * Revision 1.57  1995/03/11  21:41:30  mjl
 * All drivers: eliminated unnecessary variable initializations, other cleaning
 * up.  Changed structure for graphics input to a PLGraphicsIn (was PLCursor).
 * Substantially rewrote input loop to handle new locate mode (type 'L' while
 * at a page break).  Implemented true graphic crosshairs for use while locate
 * mode is active.
 *
 * Revision 1.56  1995/01/06  07:47:09  mjl
 * Replaced calls to XCheckMaskEvent with calls to XCheckWindowEvent, since
 * there may be more than one event handler active per display (suggested by
 * Radey Shouman).  All drivers: pls->width now more sensibly handled.  If
 * the driver supports multiple widths, it first checks to see if it has been
 * initialized already (e.g. from the command line) before initializing it.
 * For drivers that don't support multiple widths, pls->width is ignored.
 *
 * Revision 1.55  1994/11/02  19:48:06  mjl
 * Added locator handling.  When escape function is called with a PLESC_GETC
 * operation, the cursor switches to crosshair and it sits in the X event
 * loop, waiting for a left mouse click.  The coordinates are stored in the
 * data structure passed to the escape function, and the cursor reverts to
 * the default (now unchanged from the system default).
 *
 * Revision 1.54  1994/10/11  18:43:33  mjl
 * Restructured X initialization, and added the function plD_open_xw() to
 * allow early access (before calling plinit) to the X driver data
 * structures.  Currently used by the plframe widget to ensure that fg/bg
 * colors are consistent in both the widget and the driver.  The fg color is
 * now set dynamically so that rubber-band drawing will always give a good
 * contrast to the background color.
 *
 * Revision 1.53  1994/09/18  07:14:01  mjl
 * Fixed yet more stupid bugs that cause alloc/free or strcmp problems on
 * some systems.
 *
 * Revision 1.52  1994/09/16  19:14:57  mjl
 * Color allocation changed: now cmap1 entries aren't allocated until the
 * first time a cmap1 color is requested.  This will help prevent running out
 * of colors when many PLplot X windows from independent processes are active
 * (as long as cmap1 isn't being used).
 *
 * Revision 1.51  1994/09/16  06:27:51  mjl
 * Fixed problem with if-test for display name introduced last update.
 *
 * Revision 1.50  1994/09/16  05:35:45  mjl
 * Inserted workaround for cases where NULL was being used as argument to
 * strcmp (some systems don't like it).
 *
 * Revision 1.49  1994/08/25  03:58:15  mjl
 * Change to use default visual for now, since otherwise the current
 * procedure results in a BadMatch when calling XCreateWindow on some systems
 * (Suns).  To really get it right, XGetRGBColormaps or something similar
 * must be used to pair the visual with a compatible colormap.
*/

/*	xwin.c

	PLplot X-windows device driver.
*/
#include "plDevs.h"

#define DEBUG

#ifdef PLD_xwin
#include "plplotP.h"
#include "plxwd.h"
#include "drivers.h"
#include "plevent.h"

static int synchronize = 0;	/* change to 1 for synchronized operation */
				/* for debugging only */

/* When DEFAULT_VISUAL is 1, DefaultVisual() is used to get the visual.
 * Otherwise, the visual is obtained using XGetVisualInfo() to make a
 * match.
 */

#define DEFAULT_VISUAL 1

/* Number of instructions to skip between querying the X server for events */

#define MAX_INSTR 20

/* Pixels/mm */

#define PHYSICAL	0		/* Enables physical scaling.. */
#define DPMM		2.88		/* ..just experimental for now */

/* These need to be distinguished since the handling is slightly different. */

#define LOCATE_INVOKED_VIA_API		1
#define LOCATE_INVOKED_VIA_DRIVER	2

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
#define CMAP1_COLORS 50
#define MAX_COLORS 256

/* Variables to hold RGB components of given colormap. */
/* Used in an ugly hack to get past some X11R5 and TK limitations. */

static int  sxwm_colors_set;
static XColor sxwm_colors[MAX_COLORS];

/* Keep pointers to all the displays in use */

static XwDisplay *xwDisplay[PLXDISPLAYS];

/* Function prototypes */
/* Initialization */

static void  Init		(PLStream *pls);
static void  InitMain		(PLStream *pls);
static void  InitColors		(PLStream *pls);
static void  AllocCustomMap	(PLStream *pls);
static void  AllocCmap0		(PLStream *pls);
static void  AllocCmap1		(PLStream *pls);
static void  MapMain		(PLStream *pls);
static void  CreatePixmap	(PLStream *pls);
static void  GetVisual		(PLStream *pls);
static void  AllocBGFG		(PLStream *pls);

/* Routines to poll the X-server */

static void  WaitForPage	(PLStream *pls);
static void  CheckForEvents	(PLStream *pls);
static void  HandleEvents	(PLStream *pls);

/* Event handlers */

static void  MasterEH		(PLStream *pls, XEvent *event);
static void  ExposeEH		(PLStream *pls, XEvent *event);
static void  ResizeEH		(PLStream *pls, XEvent *event);
static void  MotionEH		(PLStream *pls, XEvent *event);
static void  EnterEH		(PLStream *pls, XEvent *event);
static void  LeaveEH		(PLStream *pls, XEvent *event);
static void  KeyEH		(PLStream *pls, XEvent *event);
static void  ButtonEH		(PLStream *pls, XEvent *event);
static void  LookupXKeyEvent	(PLStream *pls, XEvent *event);
static void  LookupXButtonEvent	(PLStream *pls, XEvent *event);

static void  ProcessKey		(PLStream *pls);
static void  LocateKey		(PLStream *pls);
static void  ProcessButton	(PLStream *pls);
static void  LocateButton	(PLStream *pls);
static void  Locate		(PLStream *pls);

/* Routines for manipulating graphic crosshairs */

static void  CreateXhairs	(PLStream *pls);
static void  DestroyXhairs	(PLStream *pls);
static void  DrawXhairs		(PLStream *pls, int x0, int y0);
static void  UpdateXhairs	(PLStream *pls);

/* Escape function commands */

static void  ExposeCmd		(PLStream *pls, PLDisplay *ptr);
static void  RedrawCmd		(PLStream *pls);
static void  ResizeCmd		(PLStream *pls, PLDisplay *ptr);
static void  GetCursorCmd	(PLStream *pls, PLGraphicsIn *ptr);
static void  FillPolygonCmd	(PLStream *pls);

/* Miscellaneous */

static void  StoreCmap0		(PLStream *pls);
static void  StoreCmap1		(PLStream *pls);

/*--------------------------------------------------------------------------*\
 * plD_init_xw()
 *
 * Initialize device.
 * X-dependent stuff done in plD_open_xw() and Init().
\*--------------------------------------------------------------------------*/

void
plD_init_xw(PLStream *pls)
{
    XwDev *dev;
    float pxlx, pxly;
    int xmin = 0;
    int xmax = PIXELS_X - 1;
    int ymin = 0;
    int ymax = PIXELS_Y - 1;

    dbug_enter("plD_init_xw");

    pls->termin = 1;		/* Is an interactive terminal */
    pls->dev_flush = 1;		/* Handle our own flushes */
    pls->dev_fill0 = 1;		/* Handle solid fills */
    pls->plbuf_write = 1;	/* Activate plot buffer */

/* The real meat of the initialization done here */

    if (pls->dev == NULL)
	plD_open_xw(pls);

    dev = (XwDev *) pls->dev;

    Init(pls);

/* Get ready for plotting */

    dev->xlen = xmax - xmin;
    dev->ylen = ymax - ymin;

    dev->xscale_init = dev->init_width  / (double) dev->xlen;
    dev->yscale_init = dev->init_height / (double) dev->ylen;

    dev->xscale = dev->xscale_init;
    dev->yscale = dev->yscale_init;

#if PHYSICAL
    pxlx = (double) PIXELS_X / dev->width  * DPMM;
    pxly = (double) PIXELS_Y / dev->height * DPMM;
#else
    pxlx = (double) PIXELS_X / LPAGE_X;
    pxly = (double) PIXELS_Y / LPAGE_Y;
#endif

    plP_setpxl(pxlx, pxly);
    plP_setphy(xmin, xmax, ymin, ymax);
}

/*--------------------------------------------------------------------------*\
 * plD_open_xw()
 *
 * Performs basic driver initialization, without actually opening or
 * modifying a window.  May be called by the outside world before plinit
 * in case the caller needs early access to the driver internals (not
 * very common -- currently only used by the plframe widget).
\*--------------------------------------------------------------------------*/

void
plD_open_xw(PLStream *pls)
{
    XwDev *dev;
    XwDisplay *xwd;
    int i;

    dbug_enter("plD_open_xw");

/* Allocate and initialize device-specific data */

    if (pls->dev != NULL)
	plwarn("plD_open_xw: device pointer is already set");

    pls->dev = calloc(1, (size_t) sizeof(XwDev));
    if (pls->dev == NULL)
	plexit("plD_init_xw: Out of memory.");

    dev = (XwDev *) pls->dev;

/* Variables used in querying the X server for events */

    dev->instr = 0;
    dev->max_instr = MAX_INSTR;

/* See if display matches any already in use, and if so use that */

    dev->xwd = NULL;
    for (i = 0; i < PLXDISPLAYS; i++) {
	if (xwDisplay[i] == NULL) {
	    continue;
	}
	else if (pls->FileName == NULL && xwDisplay[i]->displayName == NULL) {
	    dev->xwd = xwDisplay[i];
	    break;
	}
	else if (pls->FileName == NULL || xwDisplay[i]->displayName == NULL) {
	    continue;
	}
	else if (strcmp(xwDisplay[i]->displayName, pls->FileName) == 0) {
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
	xwd->nstreams = 1;

/* Open display */

	xwd->display = XOpenDisplay(pls->FileName);
	if (xwd->display == NULL) {
	    fprintf(stderr, "Can't open display\n");
	    exit(1);
	}
	xwd->displayName = pls->FileName;
	xwd->screen = DefaultScreen(xwd->display);
	if (synchronize) 
	    XSynchronize(xwd->display, 1);

/* Get colormap and visual */

	xwd->map = DefaultColormap(xwd->display, xwd->screen);

	GetVisual(pls);

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

/* Allocate & set background and foreground colors */

	AllocBGFG(pls);
	plX_setBGFG(pls);
    }

/* Display matched, so use existing display data */

    else {
	xwd = (XwDisplay *) dev->xwd;
	xwd->nstreams++;
    }
    xwd->ixwd = i;
}

/*--------------------------------------------------------------------------*\
 * plD_line_xw()
 *
 * Draw a line in the current color from (x1,y1) to (x2,y2).
\*--------------------------------------------------------------------------*/

void
plD_line_xw(PLStream *pls, short x1a, short y1a, short x2a, short y2a)
{
    XwDev *dev = (XwDev *) pls->dev;
    XwDisplay *xwd = (XwDisplay *) dev->xwd;

    int x1 = x1a, y1 = y1a, x2 = x2a, y2 = y2a;

    CheckForEvents(pls);

    y1 = dev->ylen - y1;
    y2 = dev->ylen - y2;

    x1 = x1 * dev->xscale;
    x2 = x2 * dev->xscale;
    y1 = y1 * dev->yscale;
    y2 = y2 * dev->yscale;

    if (dev->write_to_window)
	XDrawLine(xwd->display, dev->window, dev->gc, x1, y1, x2, y2);

    if (dev->write_to_pixmap)
	XDrawLine(xwd->display, dev->pixmap, dev->gc, x1, y1, x2, y2);
}

/*--------------------------------------------------------------------------*\
 * plD_polyline_xw()
 *
 * Draw a polyline in the current color from (x1,y1) to (x2,y2).
\*--------------------------------------------------------------------------*/

void
plD_polyline_xw(PLStream *pls, short *xa, short *ya, PLINT npts)
{
    XwDev *dev = (XwDev *) pls->dev;
    XwDisplay *xwd = (XwDisplay *) dev->xwd;

    PLINT i;
    XPoint pts[PL_MAXPOLY];

    if (npts > PL_MAXPOLY)
	plexit("plD_polyline_xw: Too many points in polyline\n");

    CheckForEvents(pls);

    for (i = 0; i < npts; i++) {
	pts[i].x = dev->xscale * xa[i];
	pts[i].y = dev->yscale * (dev->ylen - ya[i]);
    }

    if (dev->write_to_window)
	XDrawLines(xwd->display, dev->window, dev->gc, pts, npts,
		   CoordModeOrigin);

    if (dev->write_to_pixmap)
	XDrawLines(xwd->display, dev->pixmap, dev->gc, pts, npts,
		   CoordModeOrigin);
}

/*--------------------------------------------------------------------------*\
 * plD_eop_xw()
 *
 * End of page.  User must hit return (or third mouse button) to continue.
\*--------------------------------------------------------------------------*/

void
plD_eop_xw(PLStream *pls)
{
    XwDev *dev = (XwDev *) pls->dev;
    XwDisplay *xwd = (XwDisplay *) dev->xwd;

    dbug_enter("plD_eop_xw");

    XFlush(xwd->display);
    if (pls->db)
	ExposeCmd(pls, NULL);
	
    if (dev->is_main && ! pls->nopause)
	WaitForPage(pls);
}

/*--------------------------------------------------------------------------*\
 * plD_bop_xw()
 *
 * Set up for the next page.
\*--------------------------------------------------------------------------*/

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
	XSetForeground(xwd->display, dev->gc, xwd->cmap0[0].pixel);
	XFillRectangle(xwd->display, dev->pixmap, dev->gc, 0, 0,
		       dev->width, dev->height);
	XSetForeground(xwd->display, dev->gc, dev->curcolor.pixel);
    }
    XSync(xwd->display, 0);
    pls->page++;
}

/*--------------------------------------------------------------------------*\
 * plD_tidy_xw()
 *
 * Close graphics file
\*--------------------------------------------------------------------------*/

void
plD_tidy_xw(PLStream *pls)
{
    XwDev *dev = (XwDev *) pls->dev;
    XwDisplay *xwd = (XwDisplay *) dev->xwd;

    dbug_enter("plD_tidy_xw");

    if (dev->is_main) {
	XDestroyWindow(xwd->display, dev->window);
	if (dev->write_to_pixmap) 
	    XFreePixmap(xwd->display, dev->pixmap);
    }

    xwd->nstreams--;
    if (xwd->nstreams == 0) {
	int ixwd = xwd->ixwd;
	XFreeGC(xwd->display, dev->gc);
	XCloseDisplay(xwd->display);
	free_mem(xwDisplay[ixwd]);
    }
    pls->plbuf_write = 0;
}

/*--------------------------------------------------------------------------*\
 * plD_state_xw()
 *
 * Handle change in PLStream state (color, pen width, fill attribute, etc).
\*--------------------------------------------------------------------------*/

void 
plD_state_xw(PLStream *pls, PLINT op)
{
    XwDev *dev = (XwDev *) pls->dev;
    XwDisplay *xwd = (XwDisplay *) dev->xwd;

    dbug_enter("plD_state_xw");

    CheckForEvents(pls);

    switch (op) {

    case PLSTATE_WIDTH:
	break;

    case PLSTATE_COLOR0:{
	int icol0 = pls->icol0;
	if (xwd->color) {
	    if (icol0 == PL_RGB_COLOR) {
		PLColor_to_XColor(&pls->curcolor, &dev->curcolor);
		if ( ! XAllocColor(xwd->display, xwd->map, &dev->curcolor)) {
		    fprintf(stderr, "Warning: could not allocate color\n");
		    dev->curcolor.pixel = xwd->fgcolor.pixel;
		}
	    } else {
		dev->curcolor = xwd->cmap0[icol0];
	    }
	    XSetForeground(xwd->display, dev->gc, dev->curcolor.pixel);
	}
	else {
	    dev->curcolor = xwd->fgcolor;
	    XSetForeground(xwd->display, dev->gc, dev->curcolor.pixel);
	}
	break;
    }

    case PLSTATE_COLOR1:{
	int icol1;

	if (xwd->ncol1 == 0)
	    AllocCmap1(pls);

	if (xwd->ncol1 < 2)
	    break;

	icol1 = (pls->icol1 * (xwd->ncol1-1)) / (pls->ncol1-1);
	if (xwd->color) 
	    dev->curcolor = xwd->cmap1[icol1];
	else 
	    dev->curcolor = xwd->fgcolor;

	XSetForeground(xwd->display, dev->gc, dev->curcolor.pixel);
	break;
    }

    case PLSTATE_CMAP0:
	plX_setBGFG(pls);
	StoreCmap0(pls);
	break;

    case PLSTATE_CMAP1:
	StoreCmap1(pls);
	break;
    }
}

/*--------------------------------------------------------------------------*\
 * plD_esc_xw()
 *
 * Escape function.
 *
 * Functions:
 *
 *	PLESC_EH	Handle pending events
 *	PLESC_EXPOSE	Force an expose
 *	PLESC_FILL	Fill polygon
 *	PLESC_FLUSH	Flush X event buffer
 *	PLESC_GETC	Get coordinates upon mouse click
 *	PLESC_REDRAW	Force a redraw
 *	PLESC_RESIZE	Force a resize
\*--------------------------------------------------------------------------*/

void
plD_esc_xw(PLStream *pls, PLINT op, void *ptr)
{
    XwDev *dev = (XwDev *) pls->dev;
    XwDisplay *xwd = (XwDisplay *) dev->xwd;

    dbug_enter("plD_esc_xw");

    switch (op) {
    case PLESC_EH:
	HandleEvents(pls);
	break;

    case PLESC_EXPOSE:
	ExposeCmd(pls, (PLDisplay *) ptr);
	break;

    case PLESC_FILL:
	FillPolygonCmd(pls);
	break;

    case PLESC_FLUSH:
	HandleEvents(pls);
	XFlush(xwd->display);
	break;

    case PLESC_GETC:
	GetCursorCmd(pls, (PLGraphicsIn *) ptr);
	break;

    case PLESC_REDRAW:
	RedrawCmd(pls);
	break;

    case PLESC_RESIZE:
	ResizeCmd(pls, (PLDisplay *) ptr);
	break;
    }
}

/*--------------------------------------------------------------------------*\
 * GetCursorCmd()
 *
 * Waits for a graphics input event and returns coordinates.
\*--------------------------------------------------------------------------*/

static void
GetCursorCmd(PLStream *pls, PLGraphicsIn *ptr)
{
    XwDev *dev = (XwDev *) pls->dev;
    XwDisplay *xwd = (XwDisplay *) dev->xwd;
    XEvent event;
    PLGraphicsIn *gin = &(dev->gin);

/* Initialize */

    plGinInit(gin);
    dev->locate_mode = LOCATE_INVOKED_VIA_API;
    CreateXhairs(pls);

/* Run event loop until a point is selected */

    while (gin->pX < 0 && dev->locate_mode) {
	XWindowEvent(xwd->display, dev->window, dev->event_mask, &event);
	MasterEH(pls, &event);
    }
    *ptr = *gin;
}

/*--------------------------------------------------------------------------*\
 * FillPolygonCmd()
 *
 * Fill polygon described in points pls->dev_x[] and pls->dev_y[].
 * Only solid color fill supported.
\*--------------------------------------------------------------------------*/

static void
FillPolygonCmd(PLStream *pls)
{
    XwDev *dev = (XwDev *) pls->dev;
    XwDisplay *xwd = (XwDisplay *) dev->xwd;
    XPoint pts[PL_MAXPOLY];
    int i;

    if (pls->dev_npts > PL_MAXPOLY)
	plexit("FillPolygonCmd: Too many points in polygon\n");

    CheckForEvents(pls);

    for (i = 0; i < pls->dev_npts; i++) {
	pts[i].x = dev->xscale * pls->dev_x[i];
	pts[i].y = dev->yscale * (dev->ylen - pls->dev_y[i]);
    }

/* Fill polygons */

    if (dev->write_to_window)
	XFillPolygon(xwd->display, dev->window, dev->gc,
		     pts, pls->dev_npts, Nonconvex, CoordModeOrigin);

    if (dev->write_to_pixmap)
	XFillPolygon(xwd->display, dev->pixmap, dev->gc,
		     pts, pls->dev_npts, Nonconvex, CoordModeOrigin);

/* If in debug mode, draw outline of boxes being filled */

#ifdef DEBUG
    if (plsc->debug) {
	XSetForeground(xwd->display, dev->gc, xwd->fgcolor.pixel);
	if (dev->write_to_window)
	    XDrawLines(xwd->display, dev->window, dev->gc, pts, pls->dev_npts,
		       CoordModeOrigin);

	if (dev->write_to_pixmap)
	    XDrawLines(xwd->display, dev->pixmap, dev->gc, pts, pls->dev_npts,
		       CoordModeOrigin);

	XSetForeground(xwd->display, dev->gc, dev->curcolor.pixel);
    }
#endif
}

/*--------------------------------------------------------------------------*\
 * Init()
 *
 * Xlib initialization routine.
 *
 * Controlling routine for X window creation and/or initialization.
 * The user may customize the window in the following ways:
 *
 * display:	pls->OutFile (use plsfnam() or -display option) 
 * size:	pls->xlength, pls->ylength (use plspage() or -geo option)
 * bg color:	pls->cmap0[0] (use plscolbg() or -bg option)
\*--------------------------------------------------------------------------*/

static void
Init(PLStream *pls)
{
    XwDev *dev = (XwDev *) pls->dev;
    XwDisplay *xwd = (XwDisplay *) dev->xwd;

    Window root;
    int x, y;

    dbug_enter("Init");

/* If not plotting into a child window, need to create main window now */

    if (pls->window_id == 0) {
	dev->is_main = TRUE;
	InitMain(pls);
    }
    else {
	dev->is_main = FALSE;
	dev->window = pls->window_id;
    }

/* Initialize colors */

    InitColors(pls);
    XSetWindowColormap( xwd->display, dev->window, xwd->map );

/* Set up GC for ordinary draws */

    if ( ! dev->gc) 
	dev->gc = XCreateGC(xwd->display, dev->window, 0, 0);

/* Set up GC for rubber-band draws */

    if ( ! xwd->gcXor) {
	XGCValues gcValues;
	unsigned long mask;

	gcValues.background = xwd->cmap0[0].pixel;
	gcValues.foreground = 0xFF;
	gcValues.function = GXxor;
	mask = GCForeground | GCBackground | GCFunction;

	xwd->gcXor = XCreateGC(xwd->display, dev->window, mask, &gcValues);
    }

/* Get initial drawing area dimensions */

    (void) XGetGeometry(xwd->display, dev->window, &root, &x, &y,
			&dev->width, &dev->height, &dev->border, &xwd->depth);

    dev->init_width  = dev->width;
    dev->init_height = dev->height;

/* Set up flags that determine what we are writing to */
/* If nopixmap is set, ignore db */

    if (pls->nopixmap) {
	dev->write_to_pixmap = 0;
	pls->db = 0;
    }
    else {
	dev->write_to_pixmap = 1;
    }
    dev->write_to_window = ! pls->db;

/* Create pixmap for holding plot image (for expose events). */

    if (dev->write_to_pixmap) 
	CreatePixmap(pls);

/* Set drawing color */

    plD_state_xw(pls, PLSTATE_COLOR0);

    XSetWindowBackground(xwd->display, dev->window, xwd->cmap0[0].pixel);
    XSetBackground(xwd->display, dev->gc, xwd->cmap0[0].pixel);

/* If main window, need to map it and wait for exposure */

    if (dev->is_main) 
	MapMain(pls);
}

/*--------------------------------------------------------------------------*\
 * InitMain()
 *
 * Create main window using standard Xlib calls.
\*--------------------------------------------------------------------------*/

static void
InitMain(PLStream *pls)
{
    XwDev *dev = (XwDev *) pls->dev;
    XwDisplay *xwd = (XwDisplay *) dev->xwd;

    Window root;
    XSizeHints hint;
    int x, y;
    U_INT width, height, border, depth;
    char header[80];

    dbug_enter("InitMain");

/* Get root window geometry */
/*
    (void) XGetGeometry(xwd->display, DefaultRootWindow(xwd->display),
			&root, &x, &y, &width, &height, &border, &depth);
			*/
    width = DisplayWidth(xwd->display, 0);
    height = DisplayHeight(xwd->display, 0);

    dev->border = 5;
    hint.flags = 0;

/* Set window size */
/* Need to be careful to set XSizeHints flags correctly */

    if (pls->xlength == 0 && pls->ylength == 0)
	hint.flags |= PSize;
    else
	hint.flags |= USSize;

    if (pls->xlength == 0) pls->xlength = width  * 0.75;
    if (pls->ylength == 0) pls->ylength = height * 0.75;

    if (pls->xlength > width)  pls->xlength = width  - dev->border * 2;
    if (pls->ylength > height) pls->ylength = height - dev->border * 2;

    hint.width  = (int) pls->xlength;
    hint.height = (int) pls->ylength;

/* Set window position if specified by the user. */
/* Otherwise leave up to the window manager */

    if (pls->xoffset != 0 || pls->yoffset != 0) {
	hint.flags |= USPosition;
	hint.x = (int) pls->xoffset;
	hint.y = (int) pls->yoffset;
    }

/* Window title */

    sprintf(header, "PLplot");

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
}

/*--------------------------------------------------------------------------*\
 * MapMain()
 *
 * Sets up event handlers, maps main window and waits for exposure.
\*--------------------------------------------------------------------------*/

static void
MapMain(PLStream *pls)
{
    XwDev *dev = (XwDev *) pls->dev;
    XwDisplay *xwd = (XwDisplay *) dev->xwd;
    XEvent event;

    dbug_enter("MapMain");

/* Input event selection */

    dev->event_mask = 
	ButtonPressMask      |
	KeyPressMask         |
	ExposureMask         |
	StructureNotifyMask;

    XSelectInput(xwd->display, dev->window, dev->event_mask);

/* Window mapping */

    XMapRaised(xwd->display, dev->window);

/* Wait for exposure */
/* Remove extraneous expose events from the event queue */

    for (;;) {
	XWindowEvent(xwd->display, dev->window, dev->event_mask, &event);
	if (event.type == Expose) {
	    while (XCheckWindowEvent(xwd->display, dev->window,
				     ExposureMask, &event))
		;
	    break;
	}
    }
}

/*--------------------------------------------------------------------------*\
 * WaitForPage()
 *
 * This routine waits for the user to advance the plot, while handling
 * all other events.
\*--------------------------------------------------------------------------*/

static void
WaitForPage(PLStream *pls)
{
    XwDev *dev = (XwDev *) pls->dev;
    XwDisplay *xwd = (XwDisplay *) dev->xwd;
    XEvent event;

    dbug_enter("WaitForPage");

    while ( ! dev->exit_eventloop) {
	XWindowEvent(xwd->display, dev->window, dev->event_mask, &event);
	MasterEH(pls, &event);
    }
    dev->exit_eventloop = FALSE;
}

/*--------------------------------------------------------------------------*\
 * CheckForEvents()
 *
 * A front-end to HandleEvents(), which is only called if certain conditions
 * are satisfied:
 *
 * - must be the creator of the main window (i.e. PLplot is handling the
 *   X event loop by polling).
 * - must not be in the middle of a plot redisplay (else the order of event
 *   handling can become circuitous).
 * - only query X for events and process them every dev->max_instr times
 *   this function is called (good for performance since querying X is a
 *   nontrivial performance hit).
\*--------------------------------------------------------------------------*/

static void
CheckForEvents(PLStream *pls)
{
    XwDev *dev = (XwDev *) pls->dev;

    if (dev->is_main &&
	! pls->plbuf_read &&
	++dev->instr % dev->max_instr == 0) {

	dev->instr = 0;
	HandleEvents(pls);
    }
}

/*--------------------------------------------------------------------------*\
 * HandleEvents()
 *
 * Just a front-end to MasterEH(), for use when not actually waiting for an
 * event but only checking the event queue.  
\*--------------------------------------------------------------------------*/

static void
HandleEvents(PLStream *pls)
{
    XwDev *dev = (XwDev *) pls->dev;
    XwDisplay *xwd = (XwDisplay *) dev->xwd;
    XEvent event;

    while (XCheckWindowEvent(xwd->display, dev->window,
			     dev->event_mask, &event))
	MasterEH(pls, &event);
}

/*--------------------------------------------------------------------------*\
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
 * processing.  If events other than those trapped by PLplot need handling,
 * just call XSelectInput with the appropriate flags.  The default PLplot
 * event handling can be modified arbitrarily by changing the event struct.
\*--------------------------------------------------------------------------*/

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
	ButtonEH(pls, event);
	break;

    case Expose:
	ExposeEH(pls, event);
	break;

    case ConfigureNotify:
	ResizeEH(pls, event);
	break;

    case MotionNotify:
	MotionEH(pls, event);
	break;

    case EnterNotify:
	EnterEH(pls, event);
	break;

    case LeaveNotify:
	LeaveEH(pls, event);
	break;
    }
}

/*--------------------------------------------------------------------------*\
 * KeyEH()
 *
 * Event handler routine for keyboard events.
\*--------------------------------------------------------------------------*/

static void
KeyEH(PLStream *pls, XEvent *event)
{
    XwDev *dev = (XwDev *) pls->dev;

    dbug_enter("KeyEH");

    LookupXKeyEvent(pls, event);
    if (dev->locate_mode)
	LocateKey(pls);
    else
	ProcessKey(pls);
}

/*--------------------------------------------------------------------------*\
 * ButtonEH()
 *
 * Event handler routine for ButtonPress events.
\*--------------------------------------------------------------------------*/

static void
ButtonEH(PLStream *pls, XEvent *event)
{
    XwDev *dev = (XwDev *) pls->dev;

    dbug_enter("ButtonEH");

    LookupXButtonEvent(pls, event);
    if (dev->locate_mode)
	LocateButton(pls);
    else
	ProcessButton(pls);
}

/*--------------------------------------------------------------------------*\
 * LookupXKeyEvent()
 *
 * Fills in the PLGraphicsIn from an XKeyEvent.  The PLGraphicsIn keysym is
 * the same as the X keysym for all cases except for control keys that have
 * ASCII equivalents, i.e.:
 *
 * Name		      X-keysym	ASCII	Ctrl-key
 * ----		      --------	-----	--------
 * XK_BackSpace        0xFF08   0x08      ^H
 * XK_Tab              0xFF09   0x09      ^I
 * XK_Linefeed         0xFF0A   0x0A      ^J
 * XK_Return           0xFF0D   0x0D      ^M
 * XK_Escape           0xFF1B   0x1B      ^[
 * XK_Delete           0xFFFF   0xFF     (none)
 *
 * The ASCII representation of these characters is used for the PLGraphicsIn
 * keysym to simplify code logic.  It turns out that the X keysyms are
 * identical to the ASCII values with the upper 8 bits set.
\*--------------------------------------------------------------------------*/

static void
LookupXKeyEvent(PLStream *pls, XEvent *event)
{
    XwDev *dev = (XwDev *) pls->dev;
    PLGraphicsIn *gin = &(dev->gin);
    XKeyEvent *keyEvent = (XKeyEvent *) event;
    KeySym keysym;
    int nchars, ncmax = PL_MAXKEY-1;
    XComposeStatus cs;

    gin->pX = keyEvent->x;
    gin->pY = keyEvent->y;
    gin->dX = (PLFLT) keyEvent->x / (dev->width - 1);
    gin->dY = 1.0 - (PLFLT) keyEvent->y / (dev->height - 1);

    gin->state = keyEvent->state;

    nchars = XLookupString(keyEvent, gin->string, ncmax, &keysym, &cs);
    gin->string[nchars] = '\0';

    pldebug("LookupXKeyEvent",
	    "Keysym %x, translation: %s\n", keysym, gin->string);

    switch (keysym) {

    case XK_BackSpace:
    case XK_Tab:
    case XK_Linefeed:
    case XK_Return:
    case XK_Escape:
    case XK_Delete:
	gin->keysym = 0xFF & keysym;
	break;

    default:
	gin->keysym = keysym;
    }
}

/*--------------------------------------------------------------------------*\
 * LookupXButtonEvent()
 *
 * Fills in the PLGraphicsIn from an XButtonEvent.
\*--------------------------------------------------------------------------*/

static void
LookupXButtonEvent(PLStream *pls, XEvent *event)
{
    XwDev *dev = (XwDev *) pls->dev;
    PLGraphicsIn *gin = &(dev->gin);
    XButtonEvent *buttonEvent = (XButtonEvent *) event;

    pldebug("LookupXButtonEvent",
	    "Button: %d, x: %d, y: %d\n",
	    buttonEvent->button, buttonEvent->x, buttonEvent->y);

    gin->pX = buttonEvent->x;
    gin->pY = buttonEvent->y;
    gin->dX = (PLFLT) buttonEvent->x / (dev->width - 1);
    gin->dY = 1.0 - (PLFLT) buttonEvent->y / (dev->height - 1);

    gin->button = buttonEvent->button;
    gin->state = buttonEvent->state;
    gin->keysym = 0x20;
}

/*--------------------------------------------------------------------------*\
 * ProcessKey()
 *
 * Process keyboard events other than locate input.
\*--------------------------------------------------------------------------*/

static void
ProcessKey(PLStream *pls)
{
    XwDev *dev = (XwDev *) pls->dev;
    PLGraphicsIn *gin = &(dev->gin);

    dbug_enter("ProcessKey");

/* Call user keypress event handler.  Since this is called first, the user
 * can disable all internal event handling by setting key.keysym to 0.
*/
    if (pls->KeyEH != NULL)
	(*pls->KeyEH) (gin, pls->KeyEH_data, &dev->exit_eventloop);

/* Handle internal events */

    switch (gin->keysym) {

    case PLK_Return:
    case PLK_Linefeed:
    case PLK_Next:
    /* Advance to next page (i.e. terminate event loop) on a <eol> */
    /* Check for both <CR> and <LF> for portability, also a <Page Down> */
	dev->exit_eventloop = TRUE;
	break;

    case 'Q':
    /* Terminate on a 'Q' (not 'q', since it's too easy to hit by mistake) */
	pls->nopause = TRUE;
	plexit("");
	break;

    case 'L':
    /* Begin locate mode */
	dev->locate_mode = LOCATE_INVOKED_VIA_DRIVER;
	CreateXhairs(pls);
	break;
    }
}

/*--------------------------------------------------------------------------*\
 * ProcessButton()
 *
 * Process ButtonPress events other than locate input.
 * On:
 *   Button1: nothing (except when in locate mode, see ButtonLocate)
 *   Button2: nothing
 *   Button3: set page advance flag
\*--------------------------------------------------------------------------*/

static void
ProcessButton(PLStream *pls)
{
    XwDev *dev = (XwDev *) pls->dev;
    PLGraphicsIn *gin = &(dev->gin);

    dbug_enter("ProcessButton");

/* Call user event handler.  Since this is called first, the user can
 * disable all PLplot internal event handling by setting gin->button to 0.
*/
    if (pls->ButtonEH != NULL)
	(*pls->ButtonEH) (gin, pls->ButtonEH_data, &dev->exit_eventloop);

/* Handle internal events */

    switch (gin->button) {
    case Button3:
	dev->exit_eventloop = TRUE;
	break;
    }
}

/*--------------------------------------------------------------------------*\
 * LocateKey()
 *
 * Front-end to locate handler for KeyPress events.
 * Provides for a number of special effects:
 *
 *  <Escape>		Ends locate mode
 *  <Cursor>		Moves cursor one pixel in the specified direction
 *  <Mod-Cursor>	Accelerated cursor movement (5x for each modifier)
\*--------------------------------------------------------------------------*/

static void
LocateKey(PLStream *pls)
{
    XwDev *dev = (XwDev *) pls->dev;
    XwDisplay *xwd = (XwDisplay *) dev->xwd;
    PLGraphicsIn *gin = &(dev->gin);

/* End locate mode on <Escape> */

    if (gin->keysym == PLK_Escape) {
	dev->locate_mode = 0;
	DestroyXhairs(pls);
	plGinInit(gin);
    }

/* Ignore modifier keys */

    else if (IsModifierKey(gin->keysym)) {
	plGinInit(gin);
    }

/* Now handle cursor keys */

    else if (IsCursorKey(gin->keysym)) {
	int x1, y1, dx = 0, dy = 0;
	int xmin = 0, xmax = dev->width - 1, ymin = 0, ymax = dev->height - 1;

	switch (gin->keysym) {
	case PLK_Left:
	    dx = -1;
	    break;
	case PLK_Right:
	    dx = 1;
	    break;
	case PLK_Up:
	    dy = -1;
	    break;
	case PLK_Down:
	    dy = 1;
	    break;
	}

    /* Each modifier key added increases the multiplication factor by 5 */

    /* Shift */

	if (gin->state & 0x01) {
	    dx *= 5;
	    dy *= 5;
	}

    /* Caps Lock */

	if (gin->state & 0x02) {
	    dx *= 5;
	    dy *= 5;
	}

    /* Control */

	if (gin->state & 0x04) {
	    dx *= 5;
	    dy *= 5;
	}

    /* Alt */

	if (gin->state & 0x08) {
	    dx *= 5;
	    dy *= 5;
	}

    /* Bounds checking so that we don't send cursor out of window */

	x1 = gin->pX + dx;
	y1 = gin->pY + dy;

	if (x1 < xmin) dx = xmin - gin->pX;
	if (y1 < ymin) dy = ymin - gin->pY;
	if (x1 > xmax) dx = xmax - gin->pX;
	if (y1 > ymax) dy = ymax - gin->pY;

    /* Engage... */

	XWarpPointer(xwd->display, dev->window, None, 0, 0, 0, 0, dx, dy);
	plGinInit(gin);
    }

/* Call ordinary locate handler */

    else {
	Locate(pls);
    }
}

/*--------------------------------------------------------------------------*\
 * LocateButton()
 *
 * Front-end to locate handler for ButtonPress events.
 * Only passes control to Locate() for Button1 presses.
\*--------------------------------------------------------------------------*/

static void
LocateButton(PLStream *pls)
{
    XwDev *dev = (XwDev *) pls->dev;
    PLGraphicsIn *gin = &(dev->gin);

    switch (gin->button) {

    case Button1:
	Locate(pls);
	break;
    }
}

/*--------------------------------------------------------------------------*\
 * Locate()
 *
 * Handles locate mode events.
 *
 * In locate mode: move cursor to desired location and select by pressing a
 * key or by clicking on the mouse (if available).  Typically the world
 * coordinates of the selected point are reported.
 *
 * There are two ways to enter Locate mode -- via the API, or via a driver
 * command.  The API entry point is the call plGetCursor(), which initiates
 * locate mode and does not return until input has been obtained.  The
 * driver entry point is by entering a 'L' while the driver is waiting for
 * events.  
 *
 * Locate mode input is reported in one of three ways:
 * 1. Through a returned PLGraphicsIn structure, when user has specified a
 *    locate handler via (*pls->LocateEH).
 * 2. Through a returned PLGraphicsIn structure, when locate mode is invoked
 *    by a plGetCursor() call.
 * 3. Through writes to stdout, when locate mode is invoked by a driver
 *    command and the user has not supplied a locate handler.
 *
 * Hitting <Escape> will at all times end locate mode.  Other keys will
 * typically be interpreted as locator input.  Selecting a point out of
 * bounds will end locate mode unless the user overrides with a supplied
 * Locate handler.
\*--------------------------------------------------------------------------*/

static void
Locate(PLStream *pls)
{
    XwDev *dev = (XwDev *) pls->dev;
    PLGraphicsIn *gin = &(dev->gin);

/* Call user locate mode handler if provided */

    if (pls->LocateEH != NULL)
	(*pls->LocateEH) (gin, pls->LocateEH_data, &dev->locate_mode);

/* Use default procedure */

    else {

    /* Try to locate cursor */

	if (plTranslateCursor(gin)) {

	/* If invoked by the API, we're done */
	/* Otherwise send report to stdout */

	    if (dev->locate_mode == LOCATE_INVOKED_VIA_DRIVER) {
		pltext();
		if (gin->keysym < 0xFF && isprint(gin->keysym)) 
		    printf("%f %f %c\n", gin->wX, gin->wY, gin->keysym);
		else
		    printf("%f %f 0x%02x\n", gin->wX, gin->wY, gin->keysym);

		plgra();
	    }
	}
	else {

	/* Selected point is out of bounds, so end locate mode */

	    dev->locate_mode = 0;
	    DestroyXhairs(pls);
	}
    }
}

/*--------------------------------------------------------------------------*\
 * MotionEH()
 *
 * Event handler routine for MotionNotify events.
 * If drawing crosshairs, the first and last draws must be done "by hand".
\*--------------------------------------------------------------------------*/

static void
MotionEH(PLStream *pls, XEvent *event)
{
    XwDev *dev = (XwDev *) pls->dev;
    XMotionEvent *motionEvent = (XMotionEvent *) event;

    if (dev->drawing_xhairs) {
	DrawXhairs(pls, motionEvent->x, motionEvent->y);
    }
}

/*--------------------------------------------------------------------------*\
 * EnterEH()
 *
 * Event handler routine for EnterNotify events.  Only called if drawing
 * crosshairs -- a draw must be done here to start off the new set.
\*--------------------------------------------------------------------------*/

static void
EnterEH(PLStream *pls, XEvent *event)
{
    XwDev *dev = (XwDev *) pls->dev;
    XCrossingEvent *crossingEvent = (XCrossingEvent *) event;

    DrawXhairs(pls, crossingEvent->x, crossingEvent->y);
    dev->drawing_xhairs = 1;
}

/*--------------------------------------------------------------------------*\
 * LeaveEH()
 *
 * Event handler routine for EnterNotify or LeaveNotify events.
 * If drawing crosshairs, a draw must be done here to start off the new
 * set or erase the last set.
\*--------------------------------------------------------------------------*/

static void
LeaveEH(PLStream *pls, XEvent *event)
{
    XwDev *dev = (XwDev *) pls->dev;

    UpdateXhairs(pls);
    dev->drawing_xhairs = 0;
}

/*--------------------------------------------------------------------------*\
 * CreateXhairs()
 *
 * Creates graphic crosshairs at current pointer location.
\*--------------------------------------------------------------------------*/

static void
CreateXhairs(PLStream *pls)
{
    XwDev *dev = (XwDev *) pls->dev;
    XwDisplay *xwd = (XwDisplay *) dev->xwd;
    Window root, child;
    int root_x, root_y, win_x, win_y;
    unsigned int mask;
    XEvent event;

/* Get a crosshair cursor and switch to it. */

    if (!xwd->xhair_cursor)
	xwd->xhair_cursor = XCreateFontCursor(xwd->display, XC_crosshair);

    XDefineCursor(xwd->display, dev->window, xwd->xhair_cursor);

/* Find current pointer location and draw graphic crosshairs if pointer is */
/* inside our window */

    if (XQueryPointer(xwd->display, dev->window, &root, &child,
		      &root_x, &root_y, &win_x, &win_y, &mask)) {

	if (win_x >= 0 && win_x < dev->width &&
	    win_y >= 0 && win_y < dev->height) {
	    DrawXhairs(pls, win_x, win_y);
	    dev->drawing_xhairs = 1;
	}
    }

/* Sync the display and then throw away all pending motion events */

    XSync(xwd->display, 0);
    while (XCheckWindowEvent(xwd->display, dev->window,
			     PointerMotionMask, &event))
	;

/* Catch PointerMotion and crossing events so we can update them properly */

    dev->event_mask |= PointerMotionMask | EnterWindowMask | LeaveWindowMask;
    XSelectInput(xwd->display, dev->window, dev->event_mask);
}

/*--------------------------------------------------------------------------*\
 * DestroyXhairs()
 *
 * Destroys graphic crosshairs.
\*--------------------------------------------------------------------------*/

static void
DestroyXhairs(PLStream *pls)
{
    XwDev *dev = (XwDev *) pls->dev;
    XwDisplay *xwd = (XwDisplay *) dev->xwd;

/* Switch back to boring old pointer */

    XUndefineCursor(xwd->display, dev->window);

/* Don't catch PointerMotion or crossing events any more */

    dev->event_mask &=
	~PointerMotionMask & ~EnterWindowMask & ~LeaveWindowMask;
    XSelectInput(xwd->display, dev->window, dev->event_mask);

/* This draw removes the last set of graphic crosshairs */

    UpdateXhairs(pls);
    dev->drawing_xhairs = 0;
}

/*--------------------------------------------------------------------------*\
 * DrawXhairs()
 *
 * Draws graphic crosshairs at (x0, y0).  The first draw erases the old set.
\*--------------------------------------------------------------------------*/

static void
DrawXhairs(PLStream *pls, int x0, int y0)
{
    XwDev *dev = (XwDev *) pls->dev;

    int xmin = 0, xmax = dev->width - 1;
    int ymin = 0, ymax = dev->height - 1;

    if (dev->drawing_xhairs) {
	UpdateXhairs(pls);
    }
    dev->xhair_x[0].x = xmin; dev->xhair_x[0].y = y0;
    dev->xhair_x[1].x = xmax; dev->xhair_x[1].y = y0;

    dev->xhair_y[0].x = x0; dev->xhair_y[0].y = ymin;
    dev->xhair_y[1].x = x0; dev->xhair_y[1].y = ymax;

    UpdateXhairs(pls);
}

/*--------------------------------------------------------------------------*\
 * UpdateXhairs()
 *
 * Updates graphic crosshairs.  If already there, they are erased.
\*--------------------------------------------------------------------------*/

static void
UpdateXhairs(PLStream *pls)
{
    XwDev *dev = (XwDev *) pls->dev;
    XwDisplay *xwd = (XwDisplay *) dev->xwd;

    XDrawLines(xwd->display, dev->window, xwd->gcXor, dev->xhair_x, 2,
	       CoordModeOrigin);

    XDrawLines(xwd->display, dev->window, xwd->gcXor, dev->xhair_y, 2,
	       CoordModeOrigin);
}

/*--------------------------------------------------------------------------*\
 * ExposeEH()
 *
 * Event handler routine for expose events.
 * Front end to ExposeCmd() to deal with wierdnesses of Xlib.
\*--------------------------------------------------------------------------*/

static void
ExposeEH(PLStream *pls, XEvent *event)
{
    XwDev *dev = (XwDev *) pls->dev;
    XwDisplay *xwd = (XwDisplay *) dev->xwd;
    XExposeEvent *exposeEvent = (XExposeEvent *) event;
    PLDisplay pldis;
    int redrawn;

    dbug_enter("ExposeEH");

    pldebug("ExposeEH",
	    "x = %d, y = %d, width = %d, height = %d, count = %d, pending = %d\n",
	    exposeEvent->x, exposeEvent->y,
	    exposeEvent->width, exposeEvent->height,
	    exposeEvent->count, XPending(xwd->display));

/* Handle expose */
/* If we have anything overlaid (like crosshairs), we need to refresh the */
/* entire plot in order to have a predictable outcome.  In this case we */
/* need to first clear window.  Otherwise it's better to not clear it, for a */
/* smoother redraw (unobscured regions appear to stay the same). */

    if (dev->drawing_xhairs) {
	XClearWindow(xwd->display, dev->window);
	ExposeCmd(pls, NULL);
	UpdateXhairs(pls);
	redrawn = 1;
    }
    else {
	pldis.x      = exposeEvent->x;
	pldis.y      = exposeEvent->y;
	pldis.width  = exposeEvent->width;
	pldis.height = exposeEvent->height;

	ExposeCmd(pls, &pldis);
	redrawn = ! dev->write_to_pixmap;
    }

/* If entire plot was redrawn, remove extraneous events from the queue */

    if (redrawn)
	while (XCheckWindowEvent(xwd->display, dev->window,
				 ExposureMask | StructureNotifyMask, event))
	    ;
}

/*--------------------------------------------------------------------------*\
 * ResizeEH()
 *
 * Event handler routine for resize events.
 * Front end to ResizeCmd() to deal with wierdnesses of Xlib.
\*--------------------------------------------------------------------------*/

static void
ResizeEH(PLStream *pls, XEvent *event)
{
    XwDev *dev = (XwDev *) pls->dev;
    XwDisplay *xwd = (XwDisplay *) dev->xwd;
    XConfigureEvent *configEvent = (XConfigureEvent *) event;
    PLDisplay pldis;

    dbug_enter("ResizeEH");

    pldis.width  = configEvent->width;
    pldis.height = configEvent->height;

/* Only need to resize if size is actually changed */

    if (pldis.width == dev->width && pldis.height == dev->height)
	return;

    pldebug("ResizeEH",
	    "x = %d, y = %d, pending = %d\n",
	    configEvent->width, configEvent->height, XPending(xwd->display));

/* Handle resize */

    ResizeCmd(pls, &pldis);
    if (dev->drawing_xhairs) {
	UpdateXhairs(pls);
    }

/* Remove extraneous Expose and ConfigureNotify events from the event queue */
/* Exposes do not need to be handled since we've redrawn the whole plot */

    XFlush(xwd->display);
    while (XCheckWindowEvent(xwd->display, dev->window,
			     ExposureMask | StructureNotifyMask, event))
	;
}

/*--------------------------------------------------------------------------*\
 * ExposeCmd()
 *
 * Event handler routine for expose events.
 * These are "pure" exposures (no resize), so don't need to clear window.
\*--------------------------------------------------------------------------*/

static void
ExposeCmd(PLStream *pls, PLDisplay *pldis)
{
    XwDev *dev = (XwDev *) pls->dev;
    XwDisplay *xwd = (XwDisplay *) dev->xwd;
    int x, y, width, height;

    dbug_enter("ExposeCmd");

/* Return if plD_init_xw hasn't been called yet */

    if (dev == NULL) {
	plwarn("ExposeCmd: Illegal call -- driver uninitialized");
	return;
    }

/* Exposed area.  If unspecified, the entire window is used. */

    if (pldis == NULL) {
	x      = 0;
	y      = 0;
	width  = dev->width;
	height = dev->height;
    }
    else {
	x      = pldis->x;
	y      = pldis->y;
	width  = pldis->width;
	height = pldis->height;
    }

/* Usual case: refresh window from pixmap */
/* DEBUG option: draws rectangle around refreshed region */

    XSync(xwd->display, 0);
    if (dev->write_to_pixmap) {
	XCopyArea(xwd->display, dev->pixmap, dev->window, dev->gc,
		  x, y, width, height, x, y);
	XSync(xwd->display, 0);
#ifdef DEBUG
	if (plsc->debug) {
	    XPoint pts[5];
	    int x0 = x, x1 = x+width, y0 = y, y1 = y+height;
	    pts[0].x = x0; pts[0].y = y0;
	    pts[1].x = x1; pts[1].y = y0;
	    pts[2].x = x1; pts[2].y = y1;
	    pts[3].x = x0; pts[3].y = y1;
	    pts[4].x = x0; pts[4].y = y0;

	    XDrawLines(xwd->display, dev->window, dev->gc, pts, 5,
		       CoordModeOrigin);
	}
#endif
    }
    else {
	plRemakePlot(pls);
	XFlush(xwd->display);
    }
}

/*--------------------------------------------------------------------------*\
 * ResizeCmd()
 *
 * Event handler routine for resize events.
 * Note: this function is callable from the outside world.
\*--------------------------------------------------------------------------*/

static void
ResizeCmd(PLStream *pls, PLDisplay *pldis)
{
    XwDev *dev = (XwDev *) pls->dev;
    XwDisplay *xwd = (XwDisplay *) dev->xwd;
    int write_to_pixmap = dev->write_to_pixmap;

    dbug_enter("ResizeCmd");

/* Return if plD_init_xw hasn't been called yet */

    if (dev == NULL) {
	plwarn("ResizeCmd: Illegal call -- driver uninitialized");
	return;
    }

/* Return if pointer to window not specified. */

    if (pldis == NULL) {
	plwarn("ResizeCmd: Illegal call -- window pointer uninitialized");
	return;
    }

/* Reset current window bounds */

    dev->width  = pldis->width;
    dev->height = pldis->height;

    dev->xscale = dev->width  / (double) dev->init_width;
    dev->yscale = dev->height / (double) dev->init_height;

    dev->xscale = dev->xscale * dev->xscale_init;
    dev->yscale = dev->yscale * dev->yscale_init;

#if PHYSICAL
    {
	float pxlx = (double) PIXELS_X / dev->width  * DPMM;
	float pxly = (double) PIXELS_Y / dev->height * DPMM;
	plP_setpxl(pxlx, pxly);
    }
#endif

/* Initialize & redraw to window */
/* The ordering of this block and the next is reversed from a simple redraw; */
/* it looks better this way */

    dev->write_to_pixmap = 0;

    plD_bop_xw(pls);
    plRemakePlot(pls);
    XSync(xwd->display, 0);

    dev->write_to_pixmap = write_to_pixmap;

/* Need to regenerate pixmap copy of window using new dimensions, then */
/* copy on-screen window to pixmap.  */

    if (dev->write_to_pixmap) {
	XFreePixmap(xwd->display, dev->pixmap);
	CreatePixmap(pls);
	XCopyArea(xwd->display, dev->window, dev->pixmap, dev->gc, 0, 0,
		  dev->width, dev->height, 0, 0);
	XSync(xwd->display, 0);
    }
}

/*--------------------------------------------------------------------------*\
 * RedrawCmd()
 *
 * Handles page redraw without resize (pixmap does not get reallocated).
 * Calling this makes sure all necessary housekeeping gets done.
\*--------------------------------------------------------------------------*/

static void
RedrawCmd(PLStream *pls)
{
    XwDev *dev = (XwDev *) pls->dev;
    XwDisplay *xwd = (XwDisplay *) dev->xwd;
    int write_to_window = dev->write_to_window;

    dbug_enter("RedrawCmd");

/* Return if plD_init_xw hasn't been called yet */

    if (dev == NULL) {
	plwarn("RedrawCmd: Illegal call -- driver uninitialized");
	return;
    }

/* Initialize & redraw (to pixmap, if available). */

    if (dev->write_to_pixmap)
	dev->write_to_window = 0;

    plD_bop_xw(pls);
    plRemakePlot(pls);
    XSync(xwd->display, 0);

    dev->write_to_window = write_to_window;

/* If pixmap available, fake an expose */

    if (dev->write_to_pixmap) {
	XCopyArea(xwd->display, dev->pixmap, dev->window, dev->gc, 0, 0,
		  dev->width, dev->height, 0, 0);
	XSync(xwd->display, 0);
    }
}

/*--------------------------------------------------------------------------*\
 * CreatePixmapErrorHandler()
 *
 * Error handler used in CreatePixmap() to catch errors in allocating
 * storage for pixmap.  This way we can nicely substitute redraws for
 * pixmap copies if the server has insufficient memory.
\*--------------------------------------------------------------------------*/

static unsigned char CreatePixmapStatus;

static int
CreatePixmapErrorHandler(Display *display, XErrorEvent *error)
{
    if (error->error_code == BadAlloc) {
	CreatePixmapStatus = error->error_code;
    }
    else {
	char buffer[256];
	XGetErrorText(display, error->error_code, buffer, 256);
	fprintf(stderr, "Error in XCreatePixmap: %s.\n", buffer);
    }
    return 1;
}	

/*--------------------------------------------------------------------------*\
 * CreatePixmap()
 *
 * This routine creates a pixmap, doing error trapping in case there
 * isn't enough memory on the server.
\*--------------------------------------------------------------------------*/

static void
CreatePixmap(PLStream *pls)
{
    XwDev *dev = (XwDev *) pls->dev;
    XwDisplay *xwd = (XwDisplay *) dev->xwd;

    int (*oldErrorHandler)();

    oldErrorHandler = XSetErrorHandler(CreatePixmapErrorHandler);

    CreatePixmapStatus = Success;
    pldebug("CreatePixmap",
	    "creating pixmap: width = %d, height = %d, depth = %d\n",
	    dev->width, dev->height, xwd->depth);

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

/*--------------------------------------------------------------------------*\
 * GetVisual()
 *
 * Get visual info.  In order to safely use a visual other than that of
 * the parent (which hopefully is that returned by DefaultVisual), you
 * must first find (using XGetRGBColormaps) or create a colormap matching
 * this visual and then set the colormap window attribute in the
 * XCreateWindow attributes and valuemask arguments.  I don't do this
 * right now, so this is turned off by default.
\*--------------------------------------------------------------------------*/

static void
GetVisual(PLStream *pls)
{
    XwDev *dev = (XwDev *) pls->dev;
    XwDisplay *xwd = (XwDisplay *) dev->xwd;
    int visuals_matched = 0;

    dbug_enter("GetVisual");

#if DEFAULT_VISUAL == 0
    {
	XVisualInfo vTemplate, *visualList;

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
    }
#endif

    if ( ! visuals_matched) {
	xwd->visual = DefaultVisual( xwd->display, xwd->screen );
	xwd->depth = DefaultDepth( xwd->display, xwd->screen );
    }
}

/*--------------------------------------------------------------------------*\
 * AllocBGFG()
 *
 * Allocate background & foreground colors.  If possible, I choose pixel
 * values such that the fg pixel is the xor of the bg pixel, to make
 * rubber-banding easy to see.
\*--------------------------------------------------------------------------*/

static void
AllocBGFG(PLStream *pls)
{
    XwDev *dev = (XwDev *) pls->dev;
    XwDisplay *xwd = (XwDisplay *) dev->xwd;

    int i, j, npixels;
    unsigned long plane_masks[1], pixels[MAX_COLORS];

    dbug_enter("AllocBGFG");

/* If not on a color system, just return */

    if ( ! xwd->color) 
	return;

/* Allocate r/w color cell for background */

    if (XAllocColorCells(xwd->display, xwd->map, False,
			 plane_masks, 0, pixels, 1)) {
	xwd->cmap0[0].pixel = pixels[0];
    }
    else {
	plexit("couldn't allocate background color cell");
    }

/* Allocate as many colors as we can */

    npixels = MAX_COLORS;
    for (;;) {
	if (XAllocColorCells(xwd->display, xwd->map, False,
			     plane_masks, 0, pixels, npixels))
	    break;
	npixels--;
	if (npixels == 0)
	    break;
    }

/* Find the color with pixel = xor of the bg color pixel. */
/* If a match isn't found, the last pixel allocated is used. */

    for (i = 0; i < npixels-1; i++) {
	if (pixels[i] == (~xwd->cmap0[0].pixel & 0xFF))
	    break;
    }

/* Use this color cell for our foreground color.  Then free the rest. */

    xwd->fgcolor.pixel = pixels[i];
    for (j = 0; j < npixels; j++) {
	if (j != i)
	    XFreeColors(xwd->display, xwd->map, &pixels[j], 1, 0);
    }
}

/*--------------------------------------------------------------------------*\
 * plX_setBGFG()
 *
 * Set background & foreground colors.  Foreground over background should
 * have high contrast.
\*--------------------------------------------------------------------------*/

void
plX_setBGFG(PLStream *pls)
{
    XwDev *dev = (XwDev *) pls->dev;
    XwDisplay *xwd = (XwDisplay *) dev->xwd;

    PLColor fgcolor;
    int gslevbg, gslevfg;

    dbug_enter("plX_setBGFG");

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

    PLColor_to_XColor(&pls->cmap0[0], &xwd->cmap0[0]);

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

/* Now store */

    if (xwd->color) {
	XStoreColor(xwd->display, xwd->map, &xwd->fgcolor);
	XStoreColor(xwd->display, xwd->map, &xwd->cmap0[0]);
    } else {
	XAllocColor(xwd->display, xwd->map, &xwd->cmap0[0]);
	XAllocColor(xwd->display, xwd->map, &xwd->fgcolor);
    }
}

/*--------------------------------------------------------------------------*\
 * InitColors()
 *
 * Does all color initialization.
\*--------------------------------------------------------------------------*/

static void
InitColors(PLStream *pls)
{
    XwDev *dev = (XwDev *) pls->dev;
    XwDisplay *xwd = (XwDisplay *) dev->xwd;

    dbug_enter("InitColors");

/* Allocate and initialize color maps. */
/* Defer cmap1 allocation until it's actually used */

    if (xwd->color) {
	if (plplot_ccmap) {
	    AllocCustomMap(pls);
	}
	else {
	    AllocCmap0(pls);
	}
    }
}

/*--------------------------------------------------------------------------*\
 * AllocCustomMap()
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
 *		available for smooth shading.  On the order of 50-100 is 
 *		pretty reasonable.  You don't really need all 256,
 *		especially if all you're going to do is to print it to
 *		postscript (which doesn't have any intrinsic limitation on
 *		the number of colors).
 *
 * It's important to leave some extra colors unallocated for Tk.  In 
 * particular the palette tools require a fair amount.  I recommend leaving
 * at least 40 or so free.  
\*--------------------------------------------------------------------------*/

static void
AllocCustomMap(PLStream *pls)
{
    XwDev *dev = (XwDev *) pls->dev;
    XwDisplay *xwd = (XwDisplay *) dev->xwd;

    XColor xwm_colors[MAX_COLORS];
    int i, npixels;
    unsigned long plane_masks[1], pixels[MAX_COLORS];

    dbug_enter("AllocCustomMap");

/* Determine current default colors */

    for (i = 0; i < MAX_COLORS; i++) {
	xwm_colors[i].pixel = i;
    }
    XQueryColors(xwd->display, xwd->map, xwm_colors, MAX_COLORS);

/* Allocate cmap0 colors in the default colormap.
 * The custom cmap0 colors are later stored at the same pixel values.
 * This is a really cool trick to reduce the flicker when changing colormaps.
 */

    AllocCmap0(pls);
    XAllocColor(xwd->display, xwd->map, &xwd->fgcolor);

/* Create new color map */

    xwd->map = XCreateColormap( xwd->display, DefaultRootWindow(xwd->display),
				xwd->visual, AllocNone );

/* Now allocate all colors so we can fill the ones we want to copy */

    npixels = MAX_COLORS;
    for (;;) {
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

    AllocCmap1(pls);
}

/*--------------------------------------------------------------------------*\
 * AllocCmap0()
 *
 * Allocate & initialize cmap0 entries.
\*--------------------------------------------------------------------------*/

static void
AllocCmap0(PLStream *pls)
{
    XwDev *dev = (XwDev *) pls->dev;
    XwDisplay *xwd = (XwDisplay *) dev->xwd;

    int i, npixels;
    unsigned long plane_masks[1], pixels[MAX_COLORS];

    dbug_enter("AllocCmap0");

/* Allocate and assign colors in cmap 0 */

    npixels = pls->ncol0-1;
    for (;;) {
	if (XAllocColorCells(xwd->display, xwd->map, False,
			     plane_masks, 0, &pixels[1], npixels))
	    break;
	npixels--;
	if (npixels == 0)
	    plexit("couldn't allocate any colors");
    }

    xwd->ncol0 = npixels+1;
    for (i = 1; i < xwd->ncol0; i++) {
	xwd->cmap0[i].pixel = pixels[i];
    }

    StoreCmap0(pls);
}

/*--------------------------------------------------------------------------*\
 * AllocCmap1()
 *
 * Allocate & initialize cmap1 entries.  If using the default color map,
 * must severely limit number of colors otherwise TK won't have enough.
\*--------------------------------------------------------------------------*/

static void
AllocCmap1(PLStream *pls)
{
    XwDev *dev = (XwDev *) pls->dev;
    XwDisplay *xwd = (XwDisplay *) dev->xwd;

    int i, j, npixels;
    unsigned long plane_masks[1], pixels[MAX_COLORS];

    dbug_enter("AllocCmap1");

/* Allocate colors in cmap 1 */

    npixels = MAX(2, MIN(CMAP1_COLORS, pls->ncol1));
    for (;;) {
	if (XAllocColorCells(xwd->display, xwd->map, False,
			     plane_masks, 0, pixels, npixels))
	    break;
	npixels--;
	if (npixels == 0)
	    break;
    }

    if (npixels < 2) {
	xwd->ncol1 = -1;
	fprintf(stderr,
		"Warning: unable to allocate sufficient colors in cmap1\n");
	return;
    } 
    else {
	xwd->ncol1 = npixels;
	if (pls->verbose)
	    fprintf(stderr, "AllocCmap1 (xwin.c): Allocated %d colors in cmap1\n", npixels);
    }

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

    StoreCmap1(pls);
}

/*--------------------------------------------------------------------------*\
 * StoreCmap0()
 *
 * Stores cmap 0 entries in X-server colormap.
\*--------------------------------------------------------------------------*/

static void
StoreCmap0(PLStream *pls)
{
    XwDev *dev = (XwDev *) pls->dev;
    XwDisplay *xwd = (XwDisplay *) dev->xwd;
    int i;

    if ( ! xwd->color) 
	return;

    for (i = 1; i < xwd->ncol0; i++) {
	PLColor_to_XColor(&pls->cmap0[i], &xwd->cmap0[i]);
	XStoreColor(xwd->display, xwd->map, &xwd->cmap0[i]);
    }
}

/*--------------------------------------------------------------------------*\
 * StoreCmap1()
 *
 * Stores cmap 1 entries in X-server colormap.
\*--------------------------------------------------------------------------*/

static void
StoreCmap1(PLStream *pls)
{
    XwDev *dev = (XwDev *) pls->dev;
    XwDisplay *xwd = (XwDisplay *) dev->xwd;

    PLColor cmap1color;
    int i;

    if ( ! xwd->color) 
	return;

    for (i = 0; i < xwd->ncol1; i++) {
	plcol_interp(pls, &cmap1color, i, xwd->ncol1);
	PLColor_to_XColor(&cmap1color, &xwd->cmap1[i]);
	XStoreColor(xwd->display, xwd->map, &xwd->cmap1[i]);
    }
}

/*--------------------------------------------------------------------------*\
 * void PLColor_to_XColor()
 *
 * Copies the supplied PLColor to an XColor, padding with bits as necessary
 * (a PLColor uses 8 bits for color storage, while an XColor uses 16 bits).
 * The argument types follow the same order as in the function name.
\*--------------------------------------------------------------------------*/

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

/*--------------------------------------------------------------------------*\
 * void PLColor_from_XColor()
 *
 * Copies the supplied XColor to a PLColor, stripping off bits as
 * necessary.  See the previous routine for more info.
\*--------------------------------------------------------------------------*/

void
PLColor_from_XColor(PLColor *plcolor, XColor *xcolor)
{
    plcolor->r = ToPLColor(xcolor->red);
    plcolor->g = ToPLColor(xcolor->green);
    plcolor->b = ToPLColor(xcolor->blue);
}

/*--------------------------------------------------------------------------*\
 * int pl_AreWeGrayscale(Display *display)
 *
 * Determines if we're using a monochrome or grayscale device.
 * gmf 11-8-91; Courtesy of Paul Martz of Evans and Sutherland. 
\*--------------------------------------------------------------------------*/

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

/*--------------------------------------------------------------------------*\
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
\*--------------------------------------------------------------------------*/

void
PLX_save_colormap(Display *display, Colormap colormap)
{
    int i;

    if ( ! plplot_ccmap)
	return;

    sxwm_colors_set = 1;
    for (i = 0; i < MAX_COLORS; i++) {
	sxwm_colors[i].pixel = i;
    }
    XQueryColors(display, colormap, sxwm_colors, MAX_COLORS);
/*
    printf("\nAt startup, default colors are: \n\n");
    for (i = 0; i < MAX_COLORS; i++) {
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

#endif				/* PLD_xwin */
