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
static int dummy;
#ifdef XWIN

#include "plplot.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "dispatch.h"
#ifdef VMS
#include "decw$include:xlib.h"
#include "decw$include:xutil.h"
#else
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/cursorfont.h>
#endif

/* Function prototypes */

static void	xw_Xinit	(PLStream *);
static void	WaitForPage	(PLStream *);
static void	EventHandler	(PLStream *pls, XEvent *event);
static void	xwesc_ancol	(PLStream *pls, pleNcol *col);
static void	xwesc_rgb	(PLStream *pls, pleRGB *cols);
static int	AreWeMonochrome (Display *);
static void	color_def 	(PLStream *, int, char *);
static int	alloc_named_color( PLStream *pls, XColor *color, char *name);

/* top level declarations */

/* Stuff to handle reentrancy for multiple streams/devices.
   This is still under development so use caution. Right now there
   is no multiple device capability beyond that of multiple streams. */

#define PL_NDEV_XW 10	/* Max number of X sessions active */
#define PL_NDEVICES 10	/* Max number of devices/stream active */

typedef struct {
    int			advance_page;
    int			monochrome;
    long		init_width;
    long		init_height;
    long		cur_width;
    long		cur_height;
    double		xscale;
    double		yscale;
    double		xscale_dev;
    double		yscale_dev;
    int			ignore_next_expose;
    int			in_main_loop;
    U_LONG		foreground;
    U_LONG		background;

    int			screen;
    Display		*display;
    Window		window;
    XEvent		theEvent;
    GC			gc;
    Colormap		map;
    XColor		colors[17];
} XwDev;

static XwDev xwdev[PL_NDEV_XW];
static PLDev pldev[PL_NDEV_XW];

static int idev = -1;
static int devtable[PL_NSTREAMS][PL_NDEVICES];

/* Miscellaneous global definitions */

static int swap_background = 0;

#define PIXELS_X	32767		/* Number of virtual pixels in x */
#define PIXELS_Y	32767		/* Number of virtual pixels in y */

static PLFLT lpage_x = 238.0;		/* Page length in x in virtual mm */
static PLFLT lpage_y = 178.0;		/* Page length in y in virtual mm */

/*----------------------------------------------------------------------*\
* xw_init()
*
* Initialize device.
* X-dependent stuff done in xw_Xinit().  You can set the display by
* calling plsfile() with the display name as the (string) argument.
\*----------------------------------------------------------------------*/

void 
xw_init (PLStream *pls)
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
    pls->color = 1;
    pls->width = 1;
    pls->bytecnt = 0;
    pls->page = 0;
    pls->plbuf_enable = 1;

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

    xwd->xscale_dev = (double) xwd->init_width  / (double) pld->xlen;
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
xw_line (PLStream *pls, PLINT x1a, PLINT y1a, PLINT x2a, PLINT y2a)
{
    int x1 = x1a, y1 = y1a, x2 = x2a, y2 = y2a;
    int id = devtable[pls->ipls][pls->ipld];
    XwDev * xwd = &(xwdev[id]);
    PLDev * pld = &(pldev[id]);

    y1 = pld->ylen - y1;
    y2 = pld->ylen - y2;

    x1 = x1 * xwd->xscale;
    x2 = x2 * xwd->xscale;
    y1 = y1 * xwd->yscale;
    y2 = y2 * xwd->yscale;

    XDrawLine(xwd->display, xwd->window, xwd->gc, x1, y1, x2, y2);
}

/*----------------------------------------------------------------------*\
* xw_clear()
*
* Clear page.  User must click left mouse button to continue.
\*----------------------------------------------------------------------*/

void 
xw_clear (PLStream *pls)
{
    int id = devtable[pls->ipls][pls->ipld];
    XwDev * xwd = &(xwdev[id]);

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
xw_page (PLStream *pls)
{
    pls->page++;
}

/*----------------------------------------------------------------------*\
* xw_adv()
*
* Advance to the next page.
\*----------------------------------------------------------------------*/

void 
xw_adv (PLStream *pls)
{
    xw_clear(pls);
    xw_page(pls);
}

/*----------------------------------------------------------------------*\
* xw_tidy()
*
* Close graphics file
\*----------------------------------------------------------------------*/

void 
xw_tidy (PLStream *pls)
{
    int id = devtable[pls->ipls][pls->ipld];
    XwDev * xwd = &(xwdev[id]);

    xw_clear(pls);
    XFreeGC(xwd->display, xwd->gc);
    XDestroyWindow(xwd->display, xwd->window);
    XCloseDisplay(xwd->display);

    pls->fileset = 0;
    pls->page = 0;
    pls->OutFile = NULL;
    idev--;
}

/*----------------------------------------------------------------------*\
* xw_color()
*
* Set pen color.
\*----------------------------------------------------------------------*/

void 
xw_color (PLStream *pls)
{
    int id = devtable[pls->ipls][pls->ipld];
    XwDev * xwd = &(xwdev[id]);
    XColor curcolor;

    if (xwd->monochrome) 
	curcolor.pixel = (xwd->colors)[15].pixel;
    else {
	if (pls->color >= 0 && pls->color <= 16)
	    curcolor.pixel = (xwd->colors)[pls->color].pixel;
	else
	    curcolor.pixel = (xwd->colors)[15].pixel;
    }
    XSetForeground(xwd->display, xwd->gc, curcolor.pixel);
}

/*----------------------------------------------------------------------*\
* xw_text()
*
* Switch to text mode.
\*----------------------------------------------------------------------*/

void 
xw_text (PLStream *pls)
{
}

/*----------------------------------------------------------------------*\
* xw_graph()
*
* Switch to graphics mode.
\*----------------------------------------------------------------------*/

void 
xw_graph (PLStream *pls)
{
}

/*----------------------------------------------------------------------*\
* xw_width()
*
* Set pen width.
\*----------------------------------------------------------------------*/

void 
xw_width (PLStream *pls)
{
}

/*----------------------------------------------------------------------*\
* xw_esc()
*
* Escape function.
\*----------------------------------------------------------------------*/

void 
xw_esc (PLStream *pls, PLINT op, char *ptr)
{
    int id = devtable[pls->ipls][pls->ipld];
    XwDev * xwd = &(xwdev[id]);

    switch (op) {
    case PL_SET_RGB:
	xwesc_rgb(pls, (pleRGB *) ptr);
	break;

    case PL_ALLOC_NCOL:
	xwesc_ancol(pls, (pleNcol *) ptr);
	break;
    }
}

/*----------------------------------------------------------------------*\
* Main initialization routine.
\*----------------------------------------------------------------------*/

static void 
xw_Xinit (PLStream *pls)
{
    int id = devtable[pls->ipls][pls->ipld];
    XwDev * xwd = &(xwdev[id]);

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
	pls->xlength = width-border*2;
    if (pls->ylength > height)
	pls->ylength = height-border*2;

    if (pls->xoffset == 0)
	pls->xoffset = width / 20;
    if (pls->yoffset == 0)
	pls->yoffset = height / 20;

    xwd->screen 	= DefaultScreen(xwd->display);
    xwd->map		= DefaultColormap(xwd->display, xwd->screen);
    xwd->monochrome	= AreWeMonochrome( xwd->display );
    if (xwd->monochrome)
	swap_background = 1;

    if (!swap_background) {
	xwd->background = BlackPixel(xwd->display, xwd->screen);
	xwd->foreground = WhitePixel(xwd->display, xwd->screen);
    }
    else {
	xwd->background = WhitePixel(xwd->display, xwd->screen);
	xwd->foreground = BlackPixel(xwd->display, xwd->screen);
    }

/* Default color values */

    if (!xwd->monochrome) {
        color_def( pls, 0, "coral" );
        color_def( pls, 1, "red" );
        color_def( pls, 2, "yellow" );
        color_def( pls, 3, "green" );
        color_def( pls, 4, "aquamarine" );
        color_def( pls, 5, "pink" );
        color_def( pls, 6, "wheat" );
        color_def( pls, 7, "grey" );
        color_def( pls, 8, "brown" );
        color_def( pls, 9, "blue" );
        color_def( pls, 10, "BlueViolet" );
        color_def( pls, 11, "cyan" );
        color_def( pls, 12, "turquoise" );
        color_def( pls, 13, "magenta" );
        color_def( pls, 14, "salmon" );
    }

/* Default foreground/background */

    if (!swap_background) {
	color_def( pls, 15, "white" );
	color_def( pls, 16, "black" );
    }
    else {
	color_def( pls, 15, "black" );
	color_def( pls, 16, "white" );
    }

/* Default program-specified window position and size */

    hint.x      = (int) pls->xoffset;
    hint.y      = (int) pls->yoffset;
    hint.width  = (int) pls->xlength;
    hint.height = (int) pls->ylength;
    hint.flags  = PPosition | PSize;

/* Window title */

    if (idev == 0) 
	sprintf(header, "plplot");
    else
	sprintf(header, "plplot_%d", idev);

/* Window creation */
/* Why is X ignoring the x & y values??? */

    xwd->window = 
	XCreateSimpleWindow(xwd->display,
			    DefaultRootWindow(xwd->display),
			    hint.x, hint.y, hint.width, hint.height,
			    border, xwd->foreground, xwd->background);

    XSetStandardProperties(xwd->display, xwd->window, header, header,
			   None, 0, 0, &hint);

/* GC creation and initialization */

    xwd->gc = XCreateGC(xwd->display, xwd->window, 0, 0);
    
/* set cursor to crosshair */
 
   cross_cursor = XCreateFontCursor(xwd->display, XC_crosshair);
   XDefineCursor(xwd->display, xwd->window, cross_cursor);    

/* input event selection */

    XSelectInput(xwd->display, xwd->window,
		 ButtonPressMask | KeyPressMask |
		 ExposureMask | StructureNotifyMask);

/* window mapping */

    XMapRaised(xwd->display, xwd->window);

    XSetBackground(xwd->display, xwd->gc, xwd->background);
    xw_color(pls);

/* wait for exposure */

    while (1) {
	XNextEvent(xwd->display, &xwd->theEvent);
	if (xwd->theEvent.xexpose.count == 0)
	    break;
    }

/* Get initial drawing area dimensions */

    (void) XGetGeometry(xwd->display, xwd->window,
			&root, &x, &y, &width, &height,
			&border_width, &depth);

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
    XwDev * xwd = &(xwdev[id]);

    if (pls->nopause) return;

    while (!xwd->advance_page) {
	XNextEvent(xwd->display, &xwd->theEvent);
	EventHandler(pls, &xwd->theEvent);
    }
    xwd->advance_page=FALSE;
}

/*----------------------------------------------------------------------*\
* EventHandler()
*
* Event handler routine to various X events.
* On:
*   Button1: nothing for now
*   Button2: spit out device space coordinates
*   Button3: set page advance flag for later processing
*   Expose:  refresh if necessary
*   Resize:  recompute window bounds and refresh
\*----------------------------------------------------------------------*/

static void
EventHandler ( PLStream *pls, XEvent *event )
{
    int id = devtable[pls->ipls][pls->ipld];
    XwDev * xwd = &(xwdev[id]);
    int old_width, old_height;

    switch (event->type)
      {
	case ButtonPress:
	  switch (event->xbutton.button)
	    {
	      case Button1:
		break;

	      case Button2:
		printf("%d\t%d\n", event->xbutton.x, event->xbutton.y);
		break;

	      case Button3:
		xwd->advance_page=TRUE;
		return;
	    }
	  break;

	case Expose:
	  if (event->xexpose.count == 0) {
	      if (!xwd->ignore_next_expose) {
		  XFlush(xwd->display);
		  plRemakePlot(pls);
	      }
	      else {
		  xwd->ignore_next_expose = FALSE;
	      }
	  }
	  break;

	case ConfigureNotify:
	  old_width  = xwd->cur_width;
	  old_height = xwd->cur_height;

	  xwd->cur_width  = event->xconfigure.width;
	  xwd->cur_height = event->xconfigure.height;

	  xwd->xscale = (double) xwd->cur_width  / (double) xwd->init_width;
	  xwd->yscale = (double) xwd->cur_height / (double) xwd->init_height;

	  xwd->xscale = xwd->xscale * xwd->xscale_dev;
	  xwd->yscale = xwd->yscale * xwd->yscale_dev;

	  if (old_width != xwd->cur_width || old_height != xwd->cur_height) {
	      XFlush(xwd->display);
	      plRemakePlot(pls);
	  }

	  if (xwd->cur_width > old_width || xwd->cur_height > old_height)
	    xwd->ignore_next_expose = TRUE;

	  break;
      }
}

/*----------------------------------------------------------------------*\
* Color allocation routines.
\*----------------------------------------------------------------------*/

static void 
color_def (PLStream *pls, int icolor, char *name)
{
    int id = devtable[pls->ipls][pls->ipld];
    XwDev * xwd = &(xwdev[id]);

    if (alloc_named_color(pls, xwd->colors+icolor, name)) {
	if (icolor == 15 || icolor == 16) {
	    fprintf(stderr, "Can't allocate foreground/background colors\n");
	    exit(1);
	}
    }
}

static int
alloc_named_color( PLStream *pls, XColor *color, char *name)
{
    int id = devtable[pls->ipls][pls->ipld];
    XwDev * xwd = &(xwdev[id]);
    XColor xcolor;

    if (XAllocNamedColor(xwd->display, xwd->map,
			 name, &xcolor, color) == 0)
    {
	fprintf(stderr, "Can't allocate color %s\n", name);
	fprintf(stderr, "Using current foreground color instead\n");
	color = xwd->colors+15;
	return(1);
    }
    return(0);
}

/*----------------------------------------------------------------------*\
* Escape function: Allocate named color.
\*----------------------------------------------------------------------*/

static void 
xwesc_ancol (PLStream *pls, pleNcol *col)
{
    int id = devtable[pls->ipls][pls->ipld];
    XwDev * xwd = &(xwdev[id]);

    if (xwd->monochrome) {
	if (!strcmp(col->name, "white"))
	    ;
	else if (!strcmp(col->name, "black"))
	    ;
	else
	    return;
    }
    alloc_named_color(pls, xwd->colors+col->icolor, col->name);
}

/*----------------------------------------------------------------------*\
* Escape function: Set rgb color.
\*----------------------------------------------------------------------*/

static void 
xwesc_rgb (PLStream *pls, pleRGB *cols)
{
    int id = devtable[pls->ipls][pls->ipld];
    XwDev * xwd = &(xwdev[id]);
    XColor color;

    if (xwd->monochrome) return;

    color.red   = MIN(65535, MAX(0, (int) (65535. * cols->red)));
    color.green = MIN(65535, MAX(0, (int) (65535. * cols->green)));
    color.blue  = MIN(65535, MAX(0, (int) (65535. * cols->blue)));
    XAllocColor(xwd->display, xwd->map, &color);
    XSetForeground(xwd->display, xwd->gc, color.pixel);
}

/*----------------------------------------------------------------------*\
* Misc. support routines.
\*----------------------------------------------------------------------*/

/* gmf 11-8-91; Courtesy of Paul Martz of Evans and Sutherland. */

static int AreWeMonochrome ( Display *display )
{
#if defined(__cplusplus) || defined(c_plusplus)
#define THING c_class
#else
#define THING class
#endif

    XVisualInfo *visuals;
    int nitems, i;

    /* get a list of info on the visuals available */
    visuals = XGetVisualInfo (display, 0, NULL, &nitems);

    /* check the list looking for non-monochrome visual classes */
    for (i=0; i<nitems; i++)
        if ((visuals[i].THING != GrayScale) &&
            (visuals[i].THING != StaticGray))
            return (0);

    /* if we got this far, only StaticGray and GrayScale classes available */
    return (1);
}

#endif	/* XWIN */
