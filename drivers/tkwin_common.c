/* $Id$
 * Functions which are required both by tkwin.c and plplotter.c*/
#include "plplot/plDevs.h"

#define DEBUG

#ifdef PLD_tkwin
#define NEED_PLDEBUG
#include "plplot/plplotP.h"
#include "plplot/pltkwd.h"
#include "plplot/drivers.h"
#include "plplot/plevent.h"

#define _TCLINT
#ifdef USE_TCL_STUBS
/* Unfortunately, tkInt.h ends up loading Malloc.h under Windows */
/* So we have to deal with this mess */
    #undef malloc 
    #undef free
    #undef realloc
    #undef calloc
#if defined(__WIN32__) || defined (MAC_TCL)
#include <tkInt.h>
#else
#include <tk.h>
#endif
    #define malloc ckalloc
    #define free(m) ckfree((char*)m)
    #define realloc ckrealloc
    #define calloc ckcalloc
#else
#if defined(__WIN32__) || defined (MAC_TCL)
#include <tkInt.h>
#else
#include <tk.h>
#endif
#endif

#ifdef ckalloc
#undef ckalloc
#define ckalloc malloc
#endif
#ifdef ckfree
#undef ckfree
#define ckfree free
#endif
#ifdef free
#undef free
#endif

void *	ckcalloc(size_t nmemb, size_t size);

/* 
 * We want to use the 'pure Tk' interface.  On Unix we can use
 * some direct calls to X instead of Tk, if we want, although
 * that code hasn't been tested for some time.  So this define
 * is required on Windows/MacOS and perhaps optional on Unix.
 */
#define USE_TK

#ifdef __WIN32__
#define XSynchronize(display, bool) {display->request++;}
#define XSync(display, bool) {display->request++;}
#define XFlush(display)
#endif


/* Dummy definition of PlPlotter containing first few fields */
typedef struct PlPlotter {
  Tk_Window tkwin;  /* Window that embodies the frame. NULL
		     * means that the window has been destroyed
		     * but the data structures haven't yet been
		     * cleaned up.
		     */
  Display *display;  /* Display containing widget. Used, among
		      * other things, so that resources can be
		      * freed even after tkwin has gone away. 
		      */
  Tcl_Interp *interp;  /* Interpreter associated with
			* widget. Used to delete widget
			* command. 
			*/
} PlPlotter;

void CopyColour(XColor* from, XColor* to);
static int  pltk_AreWeGrayscale(PlPlotter *plf);

static int synchronize = 0; /* change to 1 for synchronized operation */
/* for debugging only */

/* Number of instructions to skip between querying the X server for events */

#define MAX_INSTR 20

/* Set constants for dealing with colormap.  In brief:
 * MAX_COLORS           Maximum colors period.
 */

#define MAX_COLORS 256

/* Variables to hold RGB components of given colormap. */
/* Used in an ugly hack to get past some X11R5 and TK limitations. */

static int sxwm_colors_set;
static XColor sxwm_colors[MAX_COLORS];

/* Keep pointers to all the displays in use */

static TkwDisplay *tkwDisplay[PLTKDISPLAYS];

#if !defined(MAC_TCL) && !defined(__WIN32__)
static unsigned char CreatePixmapStatus;
static int CreatePixmapErrorHandler(Display *display, XErrorEvent *error);
#endif

/* Function prototypes */
/* Initialization */

static void GetVisual  (PLStream *pls);
static void AllocBGFG  (PLStream *pls);
void plD_open_tkwin             (PLStream *pls);

/*--------------------------------------------------------------------------*\
 * plD_open_tkwin()
 *
 * Performs basic driver initialization, without actually opening or
 * modifying a window. May be called by the outside world before plinit
 * in case the caller needs early access to the driver internals (not
 * very common -- currently only used externally by plplotter).
\*--------------------------------------------------------------------------*/

void
plD_open_tkwin(PLStream *pls)
{
    TkwDev *dev;
    TkwDisplay *tkwd;
    int i;
    
    dbug_enter("plD_open_tkw");
    
    /* Allocate and initialize device-specific data */
    
    if (pls->dev != NULL)
	plwarn("plD_open_tkw: device pointer is already set");
    
    pls->dev = (TkwDev*) calloc(1, (size_t) sizeof(TkwDev));
    if (pls->dev == NULL)
	plexit("plD_init_tkw: Out of memory.");
    
    dev = (TkwDev *) pls->dev;
    
    /* Variables used in querying the X server for events */
    
    dev->instr = 0;
    dev->max_instr = MAX_INSTR;

    /* See if display matches any already in use, and if so use that */
    
    dev->tkwd = NULL;
    for (i = 0; i < PLTKDISPLAYS; i++) {
	if (tkwDisplay[i] == NULL) {
	    continue;
	}
	else if (pls->FileName == NULL && tkwDisplay[i]->displayName == NULL) {
	    dev->tkwd = tkwDisplay[i];
	    break;
	}
	else if (pls->FileName == NULL || tkwDisplay[i]->displayName == NULL) {
	    continue;
	}
	else if (strcmp(tkwDisplay[i]->displayName, pls->FileName) == 0) {
	    dev->tkwd = tkwDisplay[i];
	    break;
	}
    }

    /* If no display matched, create a new one */
    
    if (dev->tkwd == NULL) {
	dev->tkwd = (TkwDisplay *) calloc(1, (size_t) sizeof(TkwDisplay));
	if (dev->tkwd == NULL)
	    plexit("Init: Out of memory.");
	
	for (i = 0; i < PLTKDISPLAYS; i++) {
	    if (tkwDisplay[i] == NULL)
		break;
	}
	if (i == PLTKDISPLAYS) 
	    plexit("Init: Out of tkwDisplay's.");
	
	tkwDisplay[i] = tkwd = (TkwDisplay *) dev->tkwd;
	tkwd->nstreams = 1;
	
	/* 
	 * If we don't have a tk widget we're being called on, then
	 * abort operations now 
	 */
	if (pls->plPlotterPtr == NULL) {
	    fprintf(stderr, "No tk plframe widget to connect to\n");
	    exit(1);
	}
	/* Old version for MacOS Tk8.0 */
	/* 
	 * char deflt[] = "Macintosh:0";
	 * pls->FileName = deflt;
	 * tkwd->display = (Display*) TkpOpenDisplay(pls->FileName);
	 */

	/* Open display */
#if defined(MAC_TCL) || defined(__WIN32__)
	if(!pls->FileName) {
	    /* 
	     * Need to strdup because Tk has allocated the screen name,
	     * but we will actually 'free' it later ourselves, and therefore
	     * need to own the memory.
	     */
	    pls->FileName = strdup(TkGetDefaultScreenName(NULL,NULL));
	}
	tkwd->display = pls->plPlotterPtr->display;
#else
	tkwd->display = XOpenDisplay(pls->FileName);
#endif
	if (tkwd->display == NULL) {
	    fprintf(stderr, "Can't open display\n");
	    exit(1);
	}
	tkwd->displayName = pls->FileName;
	tkwd->screen = DefaultScreen(tkwd->display);
	if (synchronize) {
	    XSynchronize(tkwd->display, 1);
	}
	/* Get colormap and visual */
	
	tkwd->map = Tk_Colormap(pls->plPlotterPtr->tkwin);
	GetVisual(pls);
	
	/*
	 * Figure out if we have a color display or not.
	 * Default is color IF the user hasn't specified and IF the output device is
	 * not grayscale. 
	 */

	if (pls->colorset)
	    tkwd->color = pls->color;
	else {
	    pls->color = 1;
	    tkwd->color = ! pltk_AreWeGrayscale(pls->plPlotterPtr);
	}
	
	/* Allocate & set background and foreground colors */
	
	AllocBGFG(pls);
	pltkwin_setBGFG(pls);
    }
    
    /* Display matched, so use existing display data */
    
    else {
	tkwd = (TkwDisplay *) dev->tkwd;
	tkwd->nstreams++;
    }
    tkwd->ixwd = i;
}

/*--------------------------------------------------------------------------*\
 * GetVisual()
 *
 * Get visual info. In order to safely use a visual other than that of
 * the parent (which hopefully is that returned by DefaultVisual), you
 * must first find (using XGetRGBColormaps) or create a colormap matching
 * this visual and then set the colormap window attribute in the
 * XCreateWindow attributes and valuemask arguments. I don't do this
 * right now, so this is turned off by default.
\*--------------------------------------------------------------------------*/

static void
GetVisual(PLStream *pls)
{
    int depth;
    TkwDev *dev = (TkwDev *) pls->dev;
    TkwDisplay *tkwd = (TkwDisplay *) dev->tkwd;
    
    dbug_enter("GetVisual");
    
    tkwd->visual = Tk_GetVisual(pls->plPlotterPtr->interp, 
				pls->plPlotterPtr->tkwin, 
				"best",
				&depth, NULL);
    tkwd->depth = depth;
    
}

/*--------------------------------------------------------------------------*\
 * AllocBGFG()
 *
 * Allocate background & foreground colors. If possible, I choose pixel
 * values such that the fg pixel is the xor of the bg pixel, to make
 * rubber-banding easy to see.
\*--------------------------------------------------------------------------*/

static void
AllocBGFG(PLStream *pls)
{
    TkwDev *dev = (TkwDev *) pls->dev;
    TkwDisplay *tkwd = (TkwDisplay *) dev->tkwd;
    
#ifndef USE_TK
    int i, j, npixels;
    unsigned long plane_masks[1], pixels[MAX_COLORS];
#endif
    
    dbug_enter("AllocBGFG");
    
    /* If not on a color system, just return */

    if ( ! tkwd->color) 
	return;
#ifndef USE_TK
    /* Allocate r/w color cell for background */
    
    if (XAllocColorCells(tkwd->display, tkwd->map, False,
			 plane_masks, 0, pixels, 1)) {
	tkwd->cmap0[0].pixel = pixels[0];
    } else {
	plexit("couldn't allocate background color cell");
    }
    
    /* Allocate as many colors as we can */
    
    npixels = MAX_COLORS;
    for (;;) {
	if (XAllocColorCells(tkwd->display, tkwd->map, False,
			     plane_masks, 0, pixels, npixels))
	break;
	npixels--;
	if (npixels == 0)
	    break;
    }

    /* Find the color with pixel = xor of the bg color pixel. */
    /* If a match isn't found, the last pixel allocated is used. */
    
    for (i = 0; i < npixels-1; i++) {
	if (pixels[i] == (~tkwd->cmap0[0].pixel & 0xFF))
	    break;
    }

    /* Use this color cell for our foreground color. Then free the rest. */
    
    tkwd->fgcolor.pixel = pixels[i];
    for (j = 0; j < npixels; j++) {
	if (j != i)
	    XFreeColors(tkwd->display, tkwd->map, &pixels[j], 1, 0);
    }
#endif
}

/*--------------------------------------------------------------------------*\
 * pltkwin_setBGFG()
 *
 * Set background & foreground colors. Foreground over background should
 * have high contrast.
\*--------------------------------------------------------------------------*/

void
pltkwin_setBGFG(PLStream *pls)
{
    TkwDev *dev = (TkwDev *) pls->dev;
    TkwDisplay *tkwd = (TkwDisplay *) dev->tkwd;
    PLColor fgcolor;
    int gslevbg, gslevfg;
    
    dbug_enter("pltkwin_setBGFG");

    /*
     * Set background color.
     *
     * Background defaults to black on color screens, white on grayscale (many
     * grayscale monitors have poor contrast, and black-on-white looks better).
     */

    if ( ! tkwd->color) {
	pls->cmap0[0].r = pls->cmap0[0].g = pls->cmap0[0].b = 0xFF;
    }
    gslevbg = ((long) pls->cmap0[0].r +
	       (long) pls->cmap0[0].g +
	       (long) pls->cmap0[0].b) / 3;

    PLColor_to_TkColor(&pls->cmap0[0], &tkwd->cmap0[0]);
    
    /*
     * Set foreground color.
     *
     * Used for grayscale output, since otherwise the plots can become nearly
     * unreadable (i.e. if colors get mapped onto grayscale values). In this
     * case it becomes the grayscale level for all draws, and is taken to be
     * black if the background is light, and white if the background is dark.
     * Note that white/black allocations never fail.
     */

    if (gslevbg > 0x7F) 
	gslevfg = 0;
    else 
	gslevfg = 0xFF;

    fgcolor.r = fgcolor.g = fgcolor.b = gslevfg;
    
    PLColor_to_TkColor(&fgcolor, &tkwd->fgcolor);

    /* Now store */
#ifndef USE_TK
    if (tkwd->color) {
	XStoreColor(tkwd->display, tkwd->map, &tkwd->fgcolor);
	XStoreColor(tkwd->display, tkwd->map, &tkwd->cmap0[0]);
    } else {
	XAllocColor(tkwd->display, tkwd->map, &tkwd->cmap0[0]);
	XAllocColor(tkwd->display, tkwd->map, &tkwd->fgcolor);
    }
#else
    Tkw_StoreColor(pls, tkwd, &tkwd->cmap0[0]);
    Tkw_StoreColor(pls, tkwd, &tkwd->fgcolor);
#endif
}

void CopyColour(XColor* from, XColor* to) {
    to->pixel = from->pixel;
    to->red = from->red;
    to->blue = from->blue;
    to->green = from->green;
    to->flags = from->flags;
}
      
void Tkw_StoreColor(PLStream* pls, TkwDisplay* tkwd, XColor* col) {
    XColor *xc;
#ifndef USE_TK
    XStoreColor(tkwd->display, tkwd->map, col);
#else
    /* We're probably losing memory here */
    xc = Tk_GetColorByValue(pls->plPlotterPtr->tkwin,col);
    CopyColour(xc,col);
#endif 
}

/*--------------------------------------------------------------------------*\
 * void PLColor_to_TkColor()
 *
 * Copies the supplied PLColor to an XColor, padding with bits as necessary
 * (a PLColor uses 8 bits for color storage, while an XColor uses 16 bits).
 * The argument types follow the same order as in the function name.
\*--------------------------------------------------------------------------*/

#define ToXColor(a) (((0xFF & (a)) << 8) | (a))
#define ToPLColor(a) (((U_LONG) a) >> 8)

void
PLColor_to_TkColor(PLColor *plcolor, XColor *xcolor)
{
    xcolor->red = ToXColor(plcolor->r);
    xcolor->green = ToXColor(plcolor->g);
    xcolor->blue = ToXColor(plcolor->b);
    xcolor->flags = DoRed | DoGreen | DoBlue;
}

/*--------------------------------------------------------------------------*\
 * void PLColor_from_TkColor()
 *
 * Copies the supplied XColor to a PLColor, stripping off bits as
 * necessary. See the previous routine for more info.
\*--------------------------------------------------------------------------*/

void
PLColor_from_TkColor(PLColor *plcolor, XColor *xcolor)
{
    plcolor->r = (unsigned char) ToPLColor(xcolor->red);
    plcolor->g = (unsigned char) ToPLColor(xcolor->green);
    plcolor->b = (unsigned char) ToPLColor(xcolor->blue);
}

/*--------------------------------------------------------------------------*\
 * void PLColor_from_TkColor_Changed()
 *
 * Copies the supplied XColor to a PLColor, stripping off bits as
 * necessary. See the previous routine for more info.
 *
 * Returns 1 if the color was different from the old one.
\*--------------------------------------------------------------------------*/

int
PLColor_from_TkColor_Changed(PLColor *plcolor, XColor *xcolor)
{
    int changed = 0;
    int color;
    color = ToPLColor(xcolor->red);
    
    if(plcolor->r != color) {
        changed = 1;
        plcolor->r = color;
    }
    color = ToPLColor(xcolor->green);
    if(plcolor->g != color) {
        changed = 1;
        plcolor->g = color;
    }
    color = ToPLColor(xcolor->blue);
    if(plcolor->b != color) {
        changed = 1;
        plcolor->b = color;
    }
    return changed;
}

/*--------------------------------------------------------------------------*\
 * int pltk_AreWeGrayscale(PlPlotter *plf)
 *
 * Determines if we're using a monochrome or grayscale device.
 * gmf 11-8-91; Courtesy of Paul Martz of Evans and Sutherland. 
 * Changed July 1996 by Vince: now uses Tk to check the enclosing PlPlotter
\*--------------------------------------------------------------------------*/

static int
pltk_AreWeGrayscale(PlPlotter *plf)
{
#if defined(__cplusplus) || defined(c_plusplus)
#define THING c_class
#else
#define THING class
#endif

    Visual* visual;
    /* get the window's Visual */
    visual = Tk_Visual(plf->tkwin);
    if ((visual->THING != GrayScale) && (visual->THING != StaticGray))
	return (0);
    /* if we got this far, only StaticGray and GrayScale classes available */
    return (1);
}

#endif    /* PLD_tkwin */

void *	ckcalloc(size_t nmemb, size_t size) {
    long *ptr;
    long *p;
    size *= nmemb;
    ptr = (long*) malloc(size);
    if (!ptr)
	return(0);

#if !__POWERPC__
    
    for (size = (size / sizeof(long)) + 1, p = ptr; --size;)
	*p++ = 0;
    
#else
	
    for (size = (size / sizeof(long)) + 1, p = ptr - 1; --size;)
	*++p = 0;

#endif
	
    return(ptr);
}
