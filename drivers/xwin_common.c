/* $Id$

	functions which are required both by xwin.c and plframe.c
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

/* When USE_DEFAULT_VISUAL is defined, DefaultVisual() is used to get the
 * visual.  Otherwise, the visual is obtained using XGetVisualInfo() to make a
 * match.  USE_DEFAULT_VISUAL is set by the configure script.
 */

/*#define HACK_STATICCOLOR*/

/* Number of instructions to skip between querying the X server for events */

#define MAX_INSTR 20

/* Set constants for dealing with colormap.  In brief:
 * MAX_COLORS		Maximum colors period.
 */

#define MAX_COLORS 256

/* Keep pointers to all the displays in use */

static XwDisplay *xwDisplay[PLXDISPLAYS];

/* Function prototypes */

/* Initialization */

static void  GetVisual		(PLStream *pls);
static void  AllocBGFG		(PLStream *pls);

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

#ifndef USE_DEFAULT_VISUAL
    {
	XVisualInfo vTemplate, *visualList;

/* Try for an 8 plane display, if unavailable go for the default */

	vTemplate.screen = xwd->screen;
	vTemplate.depth = 8;

	visualList = XGetVisualInfo( xwd->display,
				     VisualScreenMask | VisualDepthMask,
				     &vTemplate, &visuals_matched );

#ifdef HACK_STATICCOLOR
	if (visuals_matched) {
	    int i, found = 0;
	    printf( "visuals_matched = %d\n", visuals_matched );
	    for( i=0; i < visuals_matched && !found; i++ ) {
		Visual *v = visualList[i].visual;
		printf( "Checking visual %d: ", i );
		switch( v->class ) {
		case PseudoColor:
		    printf( "PseudoColor\n" );
		    break;
		case GrayScale:
		    printf( "GrayScale\n" );
		    break;
		case DirectColor:
		    printf( "DirectColor\n" );
		    break;
		case TrueColor:
		    printf( "TrueColor\n" );
		    break;
		case StaticColor:
		    printf( "StaticColor\n" );
		    break;
		case StaticGray:
		    printf( "StaticGray\n" );
		    break;
		default:
		    printf( "Unknown.\n" );
		    break;
		}
		if (v->class == StaticColor) {
		    xwd->visual = v;
		    xwd->depth = visualList[i].depth;
		    found = 1;
		}
	    }
	    if (!found) {
		printf( "Unable to get a StaticColor visual.\n" );
		exit(1);
	    }
	    printf( "Found StaticColor visual, depth=%d\n", xwd->depth );
	}
#else
	if (visuals_matched) {
	    xwd->visual = visualList->visual;	/* Choose first match. */
	    xwd->depth = vTemplate.depth;
	}
#endif /* HACK_STATICCOLOR */
    }
#endif /* USE_DEFAULT_VISUAL */

    if ( ! visuals_matched) {
	xwd->visual = DefaultVisual( xwd->display, xwd->screen );
	xwd->depth = DefaultDepth( xwd->display, xwd->screen );
    }

/* Check to see if we expect to be able to allocate r/w color cells. */

    switch(xwd->visual->class) {
    case TrueColor:
    case StaticColor:
    case StaticGray:
	xwd->rw_cmap = 0;
	break;
    default:
	xwd->rw_cmap = 1;
    }
    
/*xwd->rw_cmap = 0;*/ /* debugging hack. */

/* Just for kicks, see what kind of visual we got. */

    if (pls->verbose) {
	fprintf( stderr, "XVisual class == " );
	switch(xwd->visual->class) {
	case PseudoColor:
	    fprintf( stderr, "PseudoColor\n" );
	    break;
	case GrayScale:
	    fprintf( stderr, "GrayScale\n" );
	    break;
	case DirectColor:
	    fprintf( stderr, "DirectColor\n" );
	    break;
	case TrueColor:
	    fprintf( stderr, "TrueColor\n" );
	    break;
	case StaticColor:
	    fprintf( stderr, "StaticColor\n" );
	    break;
	case StaticGray:
	    fprintf( stderr, "StaticGray\n" );
	    break;
	default:
	    fprintf( stderr, "Unknown.\n" );
	    break;
	}
	fprintf( stderr, "xwd->rw_cmap = %d\n", xwd->rw_cmap );
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

    if ( xwd->rw_cmap &&
	 XAllocColorCells(xwd->display, xwd->map, False,
			  plane_masks, 0, pixels, 1)) {
	xwd->cmap0[0].pixel = pixels[0];
    }
    else {
	xwd->cmap0[0].pixel = BlackPixel(xwd->display, xwd->screen);
	xwd->fgcolor.pixel = WhitePixel(xwd->display, xwd->screen);
	xwd->rw_cmap = 0;
	if (pls->verbose)
	    fprintf( stderr, "Downgrading to r/o cmap.\n" );
	return;
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

    if (xwd->rw_cmap && xwd->color) {
	XStoreColor(xwd->display, xwd->map, &xwd->fgcolor);
	XStoreColor(xwd->display, xwd->map, &xwd->cmap0[0]);
    } else {
	XAllocColor(xwd->display, xwd->map, &xwd->cmap0[0]);
	XAllocColor(xwd->display, xwd->map, &xwd->fgcolor);
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
#endif				/* PLD_xwin */
