/* $Id$
 * $Log$
 * Revision 1.9  1994/07/01 22:38:09  mjl
 * All display-dependent data broken off into a separate structure.  The X
 * driver now saves only one of these XwDisplay structs for each physical
 * X display.  These are then shared between all PLplot streams.  This
 * ensures that using multiple streams -> multiple X windows does not cause
 * abnormal termination due to running out of resources since most are now
 * shared (allocated colors, graphics contexts, etc).  The drawback is that
 * colors are no longer independent between windows, if created by a single
 * process (but this can also be an advantage).
 *
 * Revision 1.8  1994/06/09  20:30:50  mjl
 * Changed to storing the Visual instead of VisualInfo pointer.
 *
 * Revision 1.7  1994/05/23  22:09:24  mjl
 * Eliminated the "pixels" var as it is no longer being used.
 *
 * Revision 1.6  1994/05/16  21:26:53  mjl
 * Added ncol0 for storing number of colors allocated in cmap0.
 *
 * Revision 1.5  1994/05/07  03:20:40  mjl
 * Added entry for: XVisualInfo *vi to XwDev struct, and prototype for new
 * global function PLX_save_colormap.
 *
 * Revision 1.4  1994/04/30  16:16:32  mjl
 * Eliminated unused variables xold, yold, from XwDev definition.
 *
 * Revision 1.3  1994/04/08  12:15:16  mjl
 * Added CADDR_T_HACK -- define this and the typedef of caddr_t will be
 * made here for braindead systems.  Added prototype for user specified
 * X event handler (MasterEH).  Include of plstream.h now uses the
 * search path.
 *
 * Revision 1.2  1994/03/23  07:06:22  mjl
 * Properly wrapped and commented.
 *
 * Revision 1.1  1993/12/08  06:20:38  mjl
 * Added for all code/data shared with the xwin driver.
*/

/*	plplotX.h

	Holds system header includes, prototypes of xwin driver
	utility functions, and definition of the state structure.
*/

#ifndef __PLPLOTX_H__
#define __PLPLOTX_H__

#include <plplot.h>
#include <plstream.h>

/* System headers */

#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/cursorfont.h>
#include <X11/keysym.h>

#ifdef CADDR_T_HACK
typedef char * caddr_t;
#endif

/* Specify max number of displays in use */

#define PLXDISPLAYS 100

/* One of these holds the display info, shared by all streams on a given */
/* display */

typedef struct {
    int		count;			/* Number of streams using display */
    int		ixwd;			/* Specifies xwDisplay number */
    char	*displayName;		/* Name of X display */
    int		screen;			/* X screen */
    Display	*display;		/* X display */
    Visual	*visual;		/* X Visual */
    GC		gc;			/* Graphics context */
    Colormap	map;			/* Colormap */
    unsigned	depth;			/* display depth */
    int		color;			/* Set to 1 if a color output device */
    int		ncol0;			/* Number of cmap 0 colors allocated */
    int		ncol1;			/* Number of cmap 1 colors allocated */
    XColor	cmap0[16];		/* Color entries for cmap 0 */
    XColor	cmap1[256];		/* Color entries for cmap 1 */
    XColor	fgcolor;		/* Foreground color (if grayscale) */
    XColor	curcolor;		/* Current pen color */
} XwDisplay;

/* One of these holds the X driver state information */

typedef struct {
    XwDisplay	*xwd;			/* Pointer to display info */

    int		is_main;		/* Set if the toplevel X window */
    Window	window;			/* X window id */
    Pixmap	pixmap;			/* Off-screen pixmap */

    int		exit_eventloop;		/* Breaks the event loop when set */
    long	init_width;		/* Initial window width */
    long	init_height;		/* Initial window height */

    unsigned	width, height, border;	/* Current window dimensions */

    double	xscale_init;		/* initial pixels/lx (virt. coords) */
    double	yscale_init;		/* initial pixels/ly (virt. coords) */
    double	xscale;			/* as above, but current value */
    double	yscale;			/*  (after possible resizing)  */

    short	xlen, ylen;		/* Lengths of device coord space */

    int		write_to_window;	/* Set if plotting direct to window */
    int		write_to_pixmap;	/* Set if plotting to pixmap */

    void (*MasterEH) (PLStream *, XEvent *);	/* Master X event handler */
} XwDev;

/*----------------------------------------------------------------------*\
*		Function Prototypes
\*----------------------------------------------------------------------*/

#ifdef __cplusplus
extern "C" {
#endif

/* Copies the supplied PLColor to an XColor */

void
PLColor_to_XColor(PLColor *plcolor, XColor *xcolor);

/* Copies the supplied XColor to a PLColor */

void
PLColor_from_XColor(PLColor *plcolor, XColor *xcolor);

/* Determines if we're using a monochrome or grayscale device */

int
pl_AreWeGrayscale(Display *display);

/* Saves RGB components of given colormap */

void
PLX_save_colormap(Display *display, Colormap map);

#ifdef __cplusplus
}
#endif

#endif	/* __PLPLOTX_H__ */
