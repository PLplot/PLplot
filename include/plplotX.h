/* $Id$
 * $Log$
 * Revision 1.5  1994/05/07 03:20:40  mjl
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

/* One of these holds the X driver state information */

typedef struct {
    int		exit_eventloop;		/* Breaks the event loop when set */
    long	init_width;		/* Initial window width */
    long	init_height;		/* Initial window height */

    unsigned	width, height;		/* Current window dimensions */
    unsigned	depth, border;		/* window depth & border size */
    unsigned	pixels;			/* Number of colors available */

    double	xscale_init;		/* initial pixels/lx (virt. coords) */
    double	yscale_init;		/* initial pixels/ly (virt. coords) */
    double	xscale;			/* as above, but current value */
    double	yscale;			/*  (after possible resizing)  */

    short	xlen, ylen;		/* Lengths of device coord space */

    int		color;			/* Set to 1 if a color output device */
    int		ncol1;			/* Number of cmap 1 colors allocated */
    XColor	cmap0[16];		/* Color entries for cmap 0 */
    XColor	cmap1[256];		/* Color entries for cmap 1 */
    XColor	bgcolor;		/* Background color */
    XColor	fgcolor;		/* Foreground color (if grayscale) */
    XColor	curcolor;		/* Current pen color */

    int		write_to_window;	/* Set if plotting direct to window */
    int		write_to_pixmap;	/* Set if plotting to pixmap */

    int		is_main;		/* Set if the toplevel X window */
    int		screen;			/* X screen */
    Display	*display;		/* X display */
    Window	window;			/* X window id */
    XVisualInfo *vi;			/* X Visual info */
    GC		gc;			/* Graphics context */
    Colormap	map;			/* Colormap */
    Pixmap	pixmap;			/* Off-screen pixmap */

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
