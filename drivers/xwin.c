/* $Id$
   $Log$
   Revision 1.2  1992/07/31 06:06:48  mjl
   Swapped background/foreground colors for monochrome X output.

 * Revision 1.1  1992/05/20  21:32:46  furnish
 * Initial checkin of the whole PLPLOT project.
 *
*/

/*	xwin.c

	PLPLOT X-windows device driver.
*/
#ifdef XWIN

#include <stdio.h>
#include "plplot.h"
#ifdef PLSTDC
#include <string.h>
#endif

#include "dispatch.h"
#ifdef VMS
#include "decw$include:xlib.h"
#include "decw$include:xutil.h"
#else
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#endif

/* Function prototypes */

static void	begplt		PLARGS((PLStream *));
static void 	xwesc_rgb	PLARGS((char *));
static void 	xwesc_ancol	PLARGS((char *));
static void	setcol		PLARGS((long));
static void	outplt		PLARGS((void));
static void	endplt		PLARGS((void));
static void	erase		PLARGS((void));
static void	getkey		PLARGS((int *));
static int 	getwcur		PLARGS((float *, float *));
static int	AreWeMonochrome PLARGS((Display *));
static void	color_def 	PLARGS((int, char *));
static int	alloc_named_color PLARGS((XColor *, char *));

/* top level declarations */

/* Stuff to handle reentrancy for multiple streams/devices.
   This is still under development so use caution. Right now there
   is no multiple device capability beyond that of multiple streams. */

#define PL_NDEV_XW 10	/* Max number of X sessions active */
#define PL_NDEVICES 10	/* Max number of devices/stream active */

XSizeHints myhint;
static XEvent myevent;

typedef struct {
    int monochrome;
    int myscreen;
    U_LONG myforeground, mybackground;
    Display *mydisplay;
    Window mywindow;
    GC mygc;
    Colormap mymap;
    XColor colors[17];
} XwDev;

static XwDev xwdev[PL_NDEV_XW];
static PLDev pldev[PL_NDEV_XW];

static XwDev *xwd;
static PLDev *pld;
static int id, idev = -1;
static int devtable[PL_NSTREAMS][PL_NDEVICES];

static int swap_background = 0;

/*----------------------------------------------------------------------*\
* xw_init()
*
* Initialize device.
* X-dependent stuff done in begplt().  You can set the display by
* calling plsfile() with the display name as the (string) argument.
\*----------------------------------------------------------------------*/

void 
xw_init(pls)
PLStream *pls;
{
    int hxa, hya;

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

    begplt(pls);
    hxa = 2 * (pls->xlength - 1);
    hya = 2 * (pls->ylength - 1);

    pld->xold = UNDEFINED;
    pld->yold = UNDEFINED;
    pld->xmin = 0;
    pld->xmax = hxa - 1;
    pld->ymin = 0;
    pld->ymax = hya - 1;

    setpxl(4.771 * 2, 4.653 * 2);
    setphy(0, hxa - 1, 0, hya - 1);
}

/*----------------------------------------------------------------------*\
* xw_line()
*
* Draw a line in the current color from (x1,y1) to (x2,y2).
\*----------------------------------------------------------------------*/

void 
xw_line(pls, x1a, y1a, x2a, y2a)
PLStream *pls;
PLINT x1a, y1a, x2a, y2a;
{
    int x1, y1, x2, y2;

    id = devtable[pls->ipls][pls->ipld];
    xwd = &(xwdev[id]);

    x1 = x1a / 2;
    y1 = y1a / 2;
    x2 = x2a / 2;
    y2 = y2a / 2;

    if (pls->pscale)
	plSclPhy(pls, pld, &x1, &y1, &x2, &y2);

    y1 = (pls->ylength - 1) - y1;
    y2 = (pls->ylength - 1) - y2;

    XDrawLine(xwd->mydisplay, xwd->mywindow, xwd->mygc, x1, y1, x2, y2);
}

/*----------------------------------------------------------------------*\
* xw_clear()
*
* Clear page.  User must click left mouse button to continue.
\*----------------------------------------------------------------------*/

void 
xw_clear(pls)
PLStream *pls;
{
    int intrpt, but;
    float x, y;

    id = devtable[pls->ipls][pls->ipld];
    xwd = &(xwdev[id]);

    outplt();
    if (!pls->nopause)
	but = getwcur(&x, &y);
    erase();
}

/*----------------------------------------------------------------------*\
* xw_page()
*
* Set up for the next page.
\*----------------------------------------------------------------------*/

void 
xw_page(pls)
PLStream *pls;
{
    pls->page++;
}

/*----------------------------------------------------------------------*\
* xw_adv()
*
* Advance to the next page.
\*----------------------------------------------------------------------*/

void 
xw_adv(pls)
PLStream *pls;
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
xw_tidy(pls)
PLStream *pls;
{
    int intrpt, but;
    float x, y;

    id = devtable[pls->ipls][pls->ipld];
    xwd = &(xwdev[id]);

    outplt();
    if (!pls->nopause)
	but = getwcur(&x, &y);
    endplt();
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
xw_color(pls)
PLStream *pls;
{
    id = devtable[pls->ipls][pls->ipld];
    xwd = &(xwdev[id]);

    setcol(pls->color);
}

/*----------------------------------------------------------------------*\
* xw_text()
*
* Switch to text mode.
\*----------------------------------------------------------------------*/

void 
xw_text(pls)
PLStream *pls;
{
}

/*----------------------------------------------------------------------*\
* xw_graph()
*
* Switch to graphics mode.
\*----------------------------------------------------------------------*/

void 
xw_graph(pls)
PLStream *pls;
{
}

/*----------------------------------------------------------------------*\
* xw_width()
*
* Set pen width.
\*----------------------------------------------------------------------*/

void
xw_width (pls)
PLStream *pls;
{
}

/*----------------------------------------------------------------------*\
* xw_esc()
*
* Escape function.
\*----------------------------------------------------------------------*/

void 
xw_esc(pls, op, ptr)
PLStream *pls;
PLINT op;
char *ptr;
{
    id = devtable[pls->ipls][pls->ipld];
    xwd = &(xwdev[id]);

    switch (op) {
    case PL_SET_RGB:
	xwesc_rgb(ptr);
	break;

    case PL_ALLOC_NCOL:
	xwesc_ancol(ptr);
	break;
    }
}

/*----------------------------------------------------------------------*\
* Main initialization routine.
\*----------------------------------------------------------------------*/

static void
begplt(pls)
PLStream *pls;
{
    Window root;
    int x, y;
    U_INT zwidth, zheight;
    U_INT border_width, myborder;
    U_INT depth;
    Status status;
    char header[80];

    xwd->mydisplay = XOpenDisplay(pls->FileName);
    if (xwd->mydisplay == NULL) {
	fprintf(stderr, "Can't open display\n");
	exit(1);
    }
    myborder = 5;
    status = XGetGeometry(xwd->mydisplay, DefaultRootWindow(xwd->mydisplay),
		     &root, &x, &y, &zwidth, &zheight, &border_width, &depth);

    if (pls->xlength == 0)
	pls->xlength = 7 * zwidth / 8;
    if (pls->ylength == 0)
	pls->ylength = 7 * zheight / 8;
    if (pls->xlength > zwidth)
	pls->xlength = zwidth-myborder*2;
    if (pls->ylength > zheight)
	pls->ylength = zheight-myborder*2;

    if (pls->xoffset == 0)
	pls->xoffset = zwidth / 20;
    if (pls->yoffset == 0)
	pls->yoffset = zheight / 20;

    xwd->myscreen	= DefaultScreen(xwd->mydisplay);
    xwd->mymap		= DefaultColormap(xwd->mydisplay, xwd->myscreen);
    xwd->monochrome	= AreWeMonochrome( xwd->mydisplay );
    if (xwd->monochrome)
	swap_background = 1;

    if (!swap_background) {
	xwd->mybackground	= BlackPixel(xwd->mydisplay, xwd->myscreen);
	xwd->myforeground	= WhitePixel(xwd->mydisplay, xwd->myscreen);
    }
    else {
	xwd->mybackground	= WhitePixel(xwd->mydisplay, xwd->myscreen);
	xwd->myforeground	= BlackPixel(xwd->mydisplay, xwd->myscreen);
    }

/* Default color values */

    if (!xwd->monochrome) {
        color_def(0, "coral");
        color_def(1, "red");
        color_def(2, "yellow");
        color_def(3, "green");
        color_def(4, "aquamarine");
        color_def(5, "pink");
        color_def(6, "wheat");
        color_def(7, "grey");
        color_def(8, "brown");
        color_def(9, "blue");
        color_def(10, "BlueViolet");
        color_def(11, "cyan");
        color_def(12, "turquoise");
        color_def(13, "magenta");
        color_def(14, "salmon");
    }

/* Default foreground/background */

    if (!swap_background) {
	color_def(15, "white");
	color_def(16, "black");
    }
    else {
	color_def(15, "black");
	color_def(16, "white");
    }

/* Default program-specified window position and size */

    myhint.x	= pls->xoffset;
    myhint.y	= pls->yoffset;
    myhint.width = pls->xlength;
    myhint.height = pls->ylength;
    myhint.flags = PPosition | PSize;

/* Window title */

    if (idev == 0) 
	sprintf(header, "plplot");
    else
	sprintf(header, "plplot_%d", idev);

/* Window creation */
/* Why is X ignoring the x & y values??? */

    xwd->mywindow = 
	XCreateSimpleWindow(xwd->mydisplay,
			    DefaultRootWindow(xwd->mydisplay),
			    myhint.x, myhint.y, myhint.width, myhint.height,
			    myborder, xwd->myforeground, xwd->mybackground);

    XSetStandardProperties(xwd->mydisplay, xwd->mywindow, header, header,
			   None, 0, 0, &myhint);

/* GC creation and initialization */

    xwd->mygc = XCreateGC(xwd->mydisplay, xwd->mywindow, 0, 0);

/* input event selection */

    XSelectInput(xwd->mydisplay, xwd->mywindow,
		 ButtonPressMask | KeyPressMask | ExposureMask);

/* window mapping */

    XMapRaised(xwd->mydisplay, xwd->mywindow);

    XSetBackground(xwd->mydisplay, xwd->mygc, xwd->mybackground);
    setcol(15);

/* wait for exposure */

    while (1) {
	XNextEvent(xwd->mydisplay, &myevent);
	if (myevent.xexpose.count == 0)
	    break;
    }
}

/*----------------------------------------------------------------------*\
* Color allocation routines.
\*----------------------------------------------------------------------*/

static void
color_def(icolor, name)
int icolor;
char *name;
{
    if (alloc_named_color(xwd->colors+icolor, name)) {
	if (icolor == 15 || icolor == 16) {
	    fprintf(stderr, "Can't allocate foreground/background colors\n");
	    exit(1);
	}
    }
}

static int
alloc_named_color(color, name)
XColor *color;
char *name;
{
    XColor xcolor;

    if (XAllocNamedColor(xwd->mydisplay, xwd->mymap, name, &xcolor, color) == 0) {
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
xwesc_ancol(ptr)
char *ptr;
{
    int icolor;
    char *name;
    pleNcol *col = (pleNcol *) ptr;

    icolor = col->icolor;
    name   = col->name;

    if (xwd->monochrome) {
	if (!strcmp(name, "white"))
	    ;
	else if (!strcmp(name, "black"))
	    ;
	else
	    return;
    }
    alloc_named_color(xwd->colors+icolor, name);
}

/*----------------------------------------------------------------------*\
* Escape function: Set rgb color.
\*----------------------------------------------------------------------*/

static void 
xwesc_rgb(ptr)
char *ptr;
{
    XColor color;
    pleRGB *cols = (pleRGB *) ptr;

    if (xwd->monochrome) return;

    color.red   = MIN(65535, MAX(0, (int) (65535. * cols->red)));
    color.green = MIN(65535, MAX(0, (int) (65535. * cols->green)));
    color.blue  = MIN(65535, MAX(0, (int) (65535. * cols->blue)));
    XAllocColor(xwd->mydisplay, xwd->mymap, &color);
    XSetForeground(xwd->mydisplay, xwd->mygc, color.pixel);
}

/*----------------------------------------------------------------------*\
* Set color.
\*----------------------------------------------------------------------*/

static void
setcol(icol)
long icol;
{
    XColor curcolor;

    if (xwd->monochrome) 
	curcolor.pixel = (xwd->colors)[15].pixel;
    else {
	switch (icol) {
	case 0:
	    curcolor.pixel = (xwd->colors)[0].pixel;
	    break;
	case 1:
	    curcolor.pixel = (xwd->colors)[1].pixel;
	    break;
	case 2:
	    curcolor.pixel = (xwd->colors)[2].pixel;
	    break;
	case 3:
	    curcolor.pixel = (xwd->colors)[3].pixel;
	    break;
	case 4:
	    curcolor.pixel = (xwd->colors)[4].pixel;
	    break;
	case 5:
	    curcolor.pixel = (xwd->colors)[5].pixel;
	    break;
	case 6:
	    curcolor.pixel = (xwd->colors)[6].pixel;
	    break;
	case 7:
	    curcolor.pixel = (xwd->colors)[7].pixel;
	    break;
	case 8:
	    curcolor.pixel = (xwd->colors)[8].pixel;
	    break;
	case 9:
	    curcolor.pixel = (xwd->colors)[9].pixel;
	    break;
	case 10:
	    curcolor.pixel = (xwd->colors)[10].pixel;
	    break;
	case 11:
	    curcolor.pixel = (xwd->colors)[11].pixel;
	    break;
	case 12:
	    curcolor.pixel = (xwd->colors)[12].pixel;
	    break;
	case 13:
	    curcolor.pixel = (xwd->colors)[13].pixel;
	    break;
	case 14:
	    curcolor.pixel = (xwd->colors)[14].pixel;
	    break;
	case 15:
	    curcolor.pixel = (xwd->colors)[15].pixel;
	    break;
	case 16:
	    curcolor.pixel = (xwd->colors)[16].pixel;
	    break;
	default:
	    curcolor.pixel = (xwd->colors)[15].pixel;
	    break;
	}
    }
    XSetForeground(xwd->mydisplay, xwd->mygc, curcolor.pixel);
}

/*----------------------------------------------------------------------*\
* Misc. support routines.
\*----------------------------------------------------------------------*/

static void
outplt()
{
    XFlush(xwd->mydisplay);
}

static void
endplt()
{
    int intrpt;
    int ncb;
    int ic;
    XFlush(xwd->mydisplay);
    XFreeGC(xwd->mydisplay, xwd->mygc);
    XDestroyWindow(xwd->mydisplay, xwd->mywindow);
    XCloseDisplay(xwd->mydisplay);
}

static void
erase()
{
    int intrpt;
    intrpt = 0;
    XFlush(xwd->mydisplay);
    XClearWindow(xwd->mydisplay, xwd->mywindow);
    return;
}

static void
getkey(intrpt)
int *intrpt;
{
    int ic;
    ic = getchar();
    if (ic > 31 || ic < 1)
	*intrpt = 1;
    else
	*intrpt = 0;
}

static int 
getwcur(x, y)
float *x, *y;
{
    int nbut;
    while (1) {
	XNextEvent(xwd->mydisplay, &myevent);
	if (myevent.type != ButtonPress)
	    continue;
	nbut = myevent.xbutton.button;
	if (nbut == 2)
	    nbut = 3;
	else if (nbut == 3)
	    nbut = 2;
	return (nbut);
    }
}

/* gmf 11-8-91; Courtesy of Paul Martz of Evans & Sutherland. */

static int
AreWeMonochrome (display)
Display     *display;
{
    XVisualInfo *visuals;
    int nitems, i;

    /* get a list of info on the visuals available */
    visuals = XGetVisualInfo (display, 0, NULL, &nitems);

    /* check the list looking for non-monochrome visual classes */
    for (i=0; i<nitems; i++)
        if ((visuals[i].class != GrayScale) &&
            (visuals[i].class != StaticGray))
            return (0);

    /* if we got this far, only StaticGray and GrayScale classes available */
    return (1);
}
#endif	/* XWIN */
