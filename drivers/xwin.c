/* $Id$

	PLplot X-windows device driver.
*/
#include "plplot/plDevs.h"

#define DEBUG

#ifdef PLD_xwin
#define NEED_PLDEBUG
#include "plplot/plplotP.h"
#include "plplot/plxwd.h"
#include "plplot/drivers.h"
#include "plplot/plevent.h"

static int synchronize = 0;	/* change to 1 for X synchronized operation */
                                /* Use "-drvopt sync" cmd line option to set. */

static int nobuffered = 0;      /* make it a buffered device by default */
                                /* use "-drvopt buffered=0" to make it unbuffered */ 

/* When USE_DEFAULT_VISUAL is defined, DefaultVisual() is used to get the
 * visual.  Otherwise, the visual is obtained using XGetVisualInfo() to make a
 * match.  USE_DEFAULT_VISUAL is set by the configure script.
 */

/*#define HACK_STATICCOLOR*/

/* Number of instructions to skip between querying the X server for events */

#define MAX_INSTR 20

/* Pixels/mm */

/* The xwin driver uses the xscale and yscale values to convert from virtual
 * to real pixels using the current size in pixels of the display window.
 * We define PHYSICAL here so that PLplot core knows about this rescaling
 * and mm values are converted to virtual pixels at a ratio consistent with
 * a constant ratio of DPMM real pixels per mm. */
#define PHYSICAL	1
/* Desktop monitors of reasonable quality typically have 0.25 mm spacing 
 * between dots which corresponds to 4.0 dots per mm. */
#define DPMM		4.0

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

/* plplot_ccmap is statically defined in plxwd.h.  Note that
 * plframe.c also includes that header and uses the variable. */

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

/* Driver entry and dispatch setup */

void plD_dispatch_init_xw	( PLDispatchTable *pdt );

void plD_init_xw		(PLStream *);
void plD_line_xw		(PLStream *, short, short, short, short);
void plD_polyline_xw		(PLStream *, short *, short *, PLINT);
void plD_eop_xw			(PLStream *);
void plD_bop_xw			(PLStream *);
void plD_tidy_xw		(PLStream *);
void plD_state_xw		(PLStream *, PLINT);
void plD_esc_xw			(PLStream *, PLINT, void *);

/* Initialization */

static void  Init		(PLStream *pls);
static void  InitMain		(PLStream *pls);
static void  InitColors		(PLStream *pls);
static void  AllocCustomMap	(PLStream *pls);
static void  AllocCmap0		(PLStream *pls);
static void  AllocCmap1		(PLStream *pls);
static void  MapMain		(PLStream *pls);
static void  CreatePixmap	(PLStream *pls);

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
static void  ConfigBufferingCmd (PLStream *pls, PLBufferingCB *ptr );
static void  GetCursorCmd	(PLStream *pls, PLGraphicsIn *ptr);
static void  FillPolygonCmd	(PLStream *pls);
static void  XorMod		(PLStream *pls, PLINT *mod);
static void  DrawImage          (PLStream *pls);

/* Miscellaneous */

static void  StoreCmap0		(PLStream *pls);
static void  StoreCmap1		(PLStream *pls);
static void  imageops           (PLStream *pls, int *ptr);

static DrvOpt xwin_options[] = {{"sync", DRV_INT, &synchronize, "Synchronized X server operation (0|1)"},
				{"nobuffered", DRV_INT, &nobuffered, "Sets unbuffered operation (0|1)"},
				{NULL, DRV_INT, NULL, NULL}};

void plD_dispatch_init_xw( PLDispatchTable *pdt )
{
    pdt->pl_MenuStr  = "X-Window (Xlib)";
    pdt->pl_DevName  = "xwin";
    pdt->pl_type     = plDevType_Interactive;
    pdt->pl_seq      = 5;
    pdt->pl_init     = (plD_init_fp)     plD_init_xw;
    pdt->pl_line     = (plD_line_fp)     plD_line_xw;
    pdt->pl_polyline = (plD_polyline_fp) plD_polyline_xw;
    pdt->pl_eop      = (plD_eop_fp)      plD_eop_xw;
    pdt->pl_bop      = (plD_bop_fp)      plD_bop_xw;
    pdt->pl_tidy     = (plD_tidy_fp)     plD_tidy_xw;
    pdt->pl_state    = (plD_state_fp)    plD_state_xw;
    pdt->pl_esc      = (plD_esc_fp)      plD_esc_xw;
}

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
    PLFLT pxlx, pxly;
    int xmin = 0;
    int xmax = PIXELS_X - 1;
    int ymin = 0;
    int ymax = PIXELS_Y - 1;

    dbug_enter("plD_init_xw");

    pls->termin = 1;		/* Is an interactive terminal */
    pls->dev_flush = 1;		/* Handle our own flushes */
    pls->dev_fill0 = 1;		/* Handle solid fills */
    pls->plbuf_write = 1; 	/* Activate plot buffer */
    pls->dev_fastimg = 1;       /* is a fast image device */
    pls->dev_xor = 1;           /* device support xor mode */

    plParseDrvOpts(xwin_options);

    if (nobuffered)
      pls->plbuf_write = 0;	/* desactivate plot buffer */

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
    pxlx = DPMM/dev->xscale;
    pxly = DPMM/dev->yscale;
#else
    pxlx = (double) PIXELS_X / LPAGE_X;
    pxly = (double) PIXELS_Y / LPAGE_Y;
#endif

    plP_setpxl(pxlx, pxly);
    plP_setphy(xmin, xmax, ymin, ymax);
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
 * XorMod()
 *
 * Enter xor mode ( mod != 0) or leave it ( mode = 0)
\*--------------------------------------------------------------------------*/

static void
XorMod(PLStream *pls, PLINT *mod)
{
    XwDev *dev = (XwDev *) pls->dev;
    XwDisplay *xwd = (XwDisplay *) dev->xwd;

    if (*mod == 0)
      XSetFunction(xwd->display, dev->gc, GXcopy);
    else
      XSetFunction(xwd->display, dev->gc, GXxor);
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
	XFlush(xwd->display);
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
        XSetLineAttributes( xwd->display, dev->gc, pls->width, 
			    LineSolid, CapRound, JoinMiter);
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
 * 	PLESC_XORMOD 	set/reset xor mode
 * 	PLESC_IMAGE     draw the image in a fast way
 *      PLESC_IMAGEOPS: image related operations:
 * 	   ZEROW2D  	disable writing to display
 * 	   ZEROW2B  	disable writing to buffer
 * 	   ONEW2D 	enable  writing to display
 * 	   ONEW2B 	enable  writing to buffer
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

  case PLESC_XORMOD:
    XorMod(pls, (PLINT *) ptr);
    break;

  case PLESC_DOUBLEBUFFERING:
    ConfigBufferingCmd(pls, (PLBufferingCB *) ptr );
    break;

  case PLESC_IMAGE:
    DrawImage(pls);
    break;

  case PLESC_IMAGEOPS:
    imageops(pls, (PLINT *) ptr);
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
    if (dev->locate_mode) {
      dev->locate_mode = 0;
      DestroyXhairs(pls);
    }
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

    (void) XGetGeometry(xwd->display, DefaultRootWindow(xwd->display),
			&root, &x, &y, &width, &height, &border, &depth);

/* Set window size */
/* Need to be careful to set XSizeHints flags correctly */

    hint.flags = 0;
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
    dev->border = 5;

/* Set window position if specified by the user. */
/* Otherwise leave up to the window manager */

    if (pls->xoffset != 0 || pls->yoffset != 0) {
	hint.flags |= USPosition;
	hint.x = (int) pls->xoffset;
	hint.y = (int) pls->yoffset;
    }

/* Window title */

    if (plsc->plwindow){    /* allow -plwindow to specify wm decoration name */
       sprintf(header, "%s", plsc->plwindow);
    }
    else if(plsc->program) {
       sprintf(header, "%s", plsc->program); /* else program name */
    }
    else
       sprintf(header,"%s","Plplot");

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
	ButtonMotionMask     | /* drag */
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
	if (event->xmotion.state)
	  ButtonEH(pls, event); /* drag */
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
			     PointerMotionMask, &event));

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

    if (dev->drawing_xhairs)
	UpdateXhairs(pls);

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
    } else {
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
				 ExposureMask | StructureNotifyMask, event));
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
			     ExposureMask | StructureNotifyMask, event));
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
\*--------------------------------------------------------------------------*/

static void
ResizeCmd(PLStream *pls, PLDisplay *pldis)
{
    XwDev *dev = (XwDev *) pls->dev;
    XwDisplay *xwd = (XwDisplay *) dev->xwd;
    int write_to_window = dev->write_to_window;

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
	PLFLT pxlx = DPMM/dev->xscale;
	PLFLT pxly = DPMM/dev->yscale;
	plP_setpxl(pxlx, pxly);
    }
#endif

/* Note: the following order MUST be obeyed -- if you instead redraw into
 * the window and then copy it to the pixmap, off-screen parts of the window
 * may contain garbage which is then transferred to the pixmap (and thus
 * will not go away after an expose).  
 */

/* Resize pixmap using new dimensions */

    if (dev->write_to_pixmap) {
	dev->write_to_window = 0;
	XFreePixmap(xwd->display, dev->pixmap);
	CreatePixmap(pls);
    }

/* Initialize & redraw (to pixmap, if available). */

    plD_bop_xw(pls);
    plRemakePlot(pls);
    XSync(xwd->display, 0);

/* If pixmap available, fake an expose */

    if (dev->write_to_pixmap) {
	dev->write_to_window = write_to_window;
	XCopyArea(xwd->display, dev->pixmap, dev->window, dev->gc, 0, 0,
		  dev->width, dev->height, 0, 0);
	XSync(xwd->display, 0);
    }
}

/*--------------------------------------------------------------------------*\
 * ConfigBufferingCmd()
 *
 * Allows a widget to manipulate the double buffering support in the
 * xwin dirver.
\*--------------------------------------------------------------------------*/

static void ConfigBufferingCmd( PLStream *pls, PLBufferingCB *ptr )
{
    XwDev *dev = (XwDev *) pls->dev;

    switch (ptr->cmd) {

    case PLESC_DOUBLEBUFFERING_ENABLE:
	dev->write_to_window = 0;
	pls->db = 1;
	break;

    case PLESC_DOUBLEBUFFERING_DISABLE:
	dev->write_to_window = 1;
	pls->db = 0;
	break;

    case PLESC_DOUBLEBUFFERING_QUERY:
	ptr->result = pls->db;
	break;

    default:
	printf( "Unrecognized buffering request ignored.\n" );
	break;
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
    CreatePixmapStatus = error->error_code;
    if (error->error_code != BadAlloc) {
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

    if (xwd->rw_cmap) {
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
    else {
	if (pls->verbose)
	    fprintf( stderr, "Attempting to allocate r/o colors in cmap0.\n" );

	for (i = 1; i < pls->ncol0; i++) {
	    int r;
	    XColor c;
	    PLColor_to_XColor(&pls->cmap0[i], &c);
	    r = XAllocColor( xwd->display, xwd->map, &c );
	    if (pls->verbose)
		fprintf( stderr, "i=%d, r=%d, pixel=%d\n", i, r, (int) c.pixel );
	    if ( r )
		xwd->cmap0[i] = c;
            else
            {
                XColor screen_def, exact_def;

                if (pls->verbose)
                    fprintf( stderr,
                             "color alloc failed, trying by name: %s.\n",
                             pls->cmap0[i].name );

            /* Hmm, didn't work, try another approach. */
                r = XAllocNamedColor( xwd->display, xwd->map,
                                      pls->cmap0[i].name,
                                      &screen_def, &exact_def );

/*                 xwd->cmap0[i] = screen_def; */

                if (r) {
                    if (pls->verbose)
                        fprintf( stderr, "yes, got a color by name.\n" );
                    xwd->cmap0[i] = screen_def;
                } else {
                    r = XAllocNamedColor( xwd->display, xwd->map,
                                          "white",
                                          &screen_def, &exact_def );
                    if (r)
                        xwd->cmap0[i] = screen_def;
                    else
                        printf( "Can't find white?! Giving up...\n" );
                }
            }
	}
	xwd->ncol0 = i;

	if (pls->verbose)
	    fprintf( stderr, "Allocated %d colors in cmap0.\n", xwd->ncol0 );
    }
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

    if (xwd->rw_cmap) {
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
		    "Warning: unable to allocate sufficient colors in cmap1.\n");
	    return;
	} 
	else {
	    xwd->ncol1 = npixels;
	    if (pls->verbose)
		fprintf(stderr, "AllocCmap1 (xwin.c): Allocated %d colors in cmap1.\n", npixels);
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
    else {
	int i, r, ncolors;
	PLColor cmap1color;
	XColor xcol;

	if (pls->verbose)
	    fprintf( stderr, "Attempting to allocate r/o colors in cmap1.\n" );

	switch(xwd->visual->class) {
	case TrueColor:
	    ncolors = 200;
	    break;
	default:
	    ncolors = 50;
	}

	for( i = 0; i < ncolors; i++ ) {
	    plcol_interp( pls, &cmap1color, i, ncolors );
	    PLColor_to_XColor( &cmap1color, &xcol );
	    
	    r = XAllocColor( xwd->display, xwd->map, &xcol );
	    if (pls->verbose)
		fprintf(stderr, "i=%d, r=%d, pixel=%d\n", i, r, (int) xcol.pixel );
	    if ( r )
		xwd->cmap1[i] = xcol;
	    else
		break;

	}
	if (i < ncolors) {
	    xwd->ncol1 = -1;
	    fprintf(stderr,
		    "Warning: unable to allocate sufficient colors in cmap1\n");
	    return;
	} 
	else {
	    xwd->ncol1 = ncolors;
	    if (pls->verbose)
		fprintf(stderr, "AllocCmap1 (xwin.c): Allocated %d colors in cmap1\n", ncolors );
	}
    }
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
	if (xwd->rw_cmap)
	  XStoreColor(xwd->display, xwd->map, &xwd->cmap0[i]);
	else
	  XAllocColor( xwd->display, xwd->map, &xwd->cmap0[i]);
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
	if (xwd->rw_cmap)
	  XStoreColor(xwd->display, xwd->map, &xwd->cmap1[i]);
	else 
	  XAllocColor(xwd->display, xwd->map, &xwd->cmap1[i]);
    }
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


/*--------------------------------------------------------------------------*\
 * GetImageErrorHandler()
 *
 * Error handler used in XGetImage() to catch errors when pixmap or window
 * are not completely viewable.
\*--------------------------------------------------------------------------*/

static int
GetImageErrorHandler(Display *display, XErrorEvent *error)
{
    if (error->error_code != BadMatch) {
	char buffer[256];
	XGetErrorText(display, error->error_code, buffer, 256);
	fprintf(stderr, "Error in XGetImage: %s.\n", buffer);
    }
    return 1;
}	

/*--------------------------------------------------------------------------*\
 * DrawImage()
 *
 * Fill polygon described in points pls->dev_x[] and pls->dev_y[].
 * Only solid color fill supported.
\*--------------------------------------------------------------------------*/

static void
DrawImage(PLStream *pls)
{

  XwDev *dev = (XwDev *) pls->dev;
  XwDisplay *xwd = (XwDisplay *) dev->xwd;
  XImage *ximg = NULL;
  XColor curcolor;
  PLINT xmin, xmax, ymin, ymax, icol1;

  int (*oldErrorHandler)();

  float mlr, mtb;
  float blt, brt, brb, blb;
  float left, right;
  int kx, ky;
  int nx, ny, ix, iy;
  int i, corners[4], r[4];

  struct {
    float x, y;
  } Ppts[4];
 
  CheckForEvents(pls);
  
  xmin = dev->xscale * plsc->imclxmin;
  xmax = dev->xscale * plsc->imclxmax;
  ymin = dev->yscale * plsc->imclymin;
  ymax = dev->yscale * plsc->imclymax;
  
  nx = pls->dev_nptsX;
  ny = pls->dev_nptsY;

  /* XGetImage() call fails if either the pixmap or window is not fully viewable! */
  oldErrorHandler = XSetErrorHandler(GetImageErrorHandler);

  XFlush(xwd->display); 
  if (dev->write_to_pixmap)
    ximg =  XGetImage( xwd->display, dev->pixmap, 0, 0, dev->width, dev->height,
    		       AllPlanes, ZPixmap);

  if (dev->write_to_window)
    ximg =  XGetImage( xwd->display, dev->window, 0, 0, dev->width, dev->height,
		       AllPlanes, ZPixmap);

  XSetErrorHandler(oldErrorHandler);

  if (ximg == NULL) {
    plabort("Can't get image, the window must be partly obscured.");
    return;
  }

  if (xwd->ncol1 == 0)
    AllocCmap1(pls);
  if (xwd->ncol1 < 2)
    return;

  /* translate array for rotation */
  switch ((int)(plsc->diorot - 4.*floor(plsc->diorot/4.))) {
  case 0:
    r[0]=0; r[1]=1; r[2]=2; r[3]=3; break;
  case 1:
    r[0]=1; r[1]=2; r[2]=3; r[3]=0; break;
  case 2: 
    r[0]=2; r[1]=3; r[2]=0; r[3]=1; break;
  case 3:
    r[0]=3; r[1]=0; r[2]=1; r[3]=2;
  }

  /* after rotation and coordinate translation, each fill
     lozangue will have coordinates (Ppts), slopes (m...)
     and y intercepts (b...):

           Ppts[3]
             /\
    mlr,blt /  \ mtb,brt
           /    \ 
   Ppts[0]<      > Ppts[2]
           \    /
    mtb,blt \  / mlr,brb
             \/
           Ppts[1]
  */

  /* slope of left/right and top/bottom edges */
  mlr = (dev->yscale * (plsc->dev_iy[1] - plsc->dev_iy[0])) /
    (dev->xscale * (plsc->dev_ix[1] - plsc->dev_ix[0]));

  mtb = (dev->yscale * (plsc->dev_iy[ny] - plsc->dev_iy[0])) /
    (dev->xscale * (plsc->dev_ix[ny] - plsc->dev_ix[0]));

  for(ix = 0; ix < nx-1; ix++) {
    for(iy = 0; iy < ny-1; iy++) {
      corners[0] = ix*ny+iy; /* [ix][iy] */
      corners[1] = (ix+1)*ny+iy; /* [ix+1][iy] */
      corners[2] = (ix+1)*ny+iy+1; /* [ix+1][iy+1] */
      corners[3] = ix*ny+iy+1; /* [ix][iy+1] */

      for (i=0; i<4; i++) {
	Ppts[i].x = dev->xscale * (plsc->dev_ix[corners[r[i]]]);
	Ppts[i].y = dev->yscale * (plsc->dev_iy[corners[r[i]]]);
      }

      /* if any corner is inside the draw area */
      if (Ppts[0].x >= xmin || Ppts[2].x <= xmax ||
	  Ppts[1].y >= ymin || Ppts[3].y <= ymax) {

	Ppts[0].x = MAX(Ppts[0].x, xmin);
	Ppts[2].x = MIN(Ppts[2].x, xmax);
	Ppts[1].y = MAX(Ppts[1].y, ymin);
	Ppts[3].y = MIN(Ppts[3].y, ymax);
	
	/* the Z array has size (nx-1)*(ny-1) */
	icol1 = plsc->dev_z[ix*(ny-1)+iy];

	/* only plot points within zmin/zmax range */
	if (icol1 < plsc->dev_zmin || icol1 > plsc->dev_zmax)
	  continue;

	icol1 = icol1/(float)USHRT_MAX * (xwd->ncol1-1);
	if (xwd->color)
	  curcolor = xwd->cmap1[icol1];
	else 
	  curcolor = xwd->fgcolor;

	/* Fill square between current and next points. */

	/* If the fill area is a single dot, accelerate the fill. */	   
	if ( (fabs(Ppts[2].x - Ppts[0].x) == 1) &&
	    (fabs(Ppts[3].y - Ppts[1].y) == 1)) {
	  XPutPixel(ximg, Ppts[0].x, dev->height-1 - Ppts[0].y, curcolor.pixel);

	  /* integer rotate, accelerate */
	} else if (plsc->diorot == floor(plsc->diorot)) {
	  for( ky = Ppts[1].y; ky < Ppts[3].y; ky++)
	    for( kx = Ppts[0].x; kx < Ppts[2].x; kx++)
	      XPutPixel(ximg, kx, dev->height-1 - ky, curcolor.pixel);

	  /* lozangue, scanline fill it */
	} else {

	  /* y interception point of left/right top/bottom edges */	    
	  blt = Ppts[0].y - mlr * Ppts[0].x;
	  brb = Ppts[2].y - mlr * Ppts[2].x;
	    
	  brt = Ppts[2].y - mtb * Ppts[2].x;
	  blb = Ppts[0].y - mtb * Ppts[0].x;

	  for( ky = Ppts[1].y; ky < Ppts[3].y; ky++) {
	    left = MAX(((ky-blt)/mlr), ((ky-blb)/mtb));
	    right = MIN(((ky-brt)/mtb), ((ky-brb)/mlr));
	    for( kx = Ppts[0].x; kx < Ppts[2].x; kx++) {
	      if (kx >= rint(left) && kx <= rint(right)) {
		XPutPixel(ximg, kx, dev->height-1 - ky, curcolor.pixel);
	      }
	    }
	  }
	}
      }
    }
  }

  if (dev->write_to_pixmap)
    XPutImage( xwd->display, dev->pixmap, dev->gc, ximg, 0, 0, 0, 0, dev->width, dev->height);
    
  if (dev->write_to_window)
    XPutImage( xwd->display, dev->window, dev->gc, ximg, 0, 0,
	       0, 0, dev->width, dev->height);

  XDestroyImage(ximg);
}

static void
imageops(PLStream *pls, int *ptr)
{

  XwDev *dev = (XwDev *) pls->dev;
  XwDisplay *xwd = (XwDisplay *) dev->xwd;

  /* TODO: store/revert to/from previous state */

  switch (*ptr) {
  case ZEROW2D:
    dev->write_to_window = 0;
    break;

  case ONEW2D:
    dev->write_to_window = 1;
    break;

  case ZEROW2B:
    dev->write_to_pixmap = 0;
    break;

  case ONEW2B:
    XFlush(xwd->display);
    dev->write_to_pixmap = 1;
    break;
  }
}

#else
int
pldummy_xwin()
{
    return 0;
}

#endif				/* PLD_xwin */
