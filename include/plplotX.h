/* $Id$
 * $Log$
 * Revision 1.2  1994/03/23 07:06:22  mjl
 * Properly wrapped and commented.
 *
 * Revision 1.1  1993/12/08  06:20:38  mjl
 * Added for all code/data shared with the xwin driver.
 *
*/

/*	plplotX.h

	Holds system header includes, prototypes of xwin driver
	utility functions, and definition of the state structure.
*/

#ifndef __PLPLOTX_H__
#define __PLPLOTX_H__

#include <plplot.h>

/* System headers */

#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/cursorfont.h>
#include <X11/keysym.h>

/* X driver utility functions */

#ifdef __cplusplus
extern "C" {
#endif

void  PLColor_to_XColor		(PLColor *, XColor *);
void  PLColor_from_XColor	(PLColor *, XColor *);
int   pl_AreWeGrayscale		(Display *);

#ifdef __cplusplus
}
#endif

/* One of these holds the X driver state information */

typedef struct {
    int		exit_eventloop;		/* Breaks the event loop when set */
    long	init_width;		/* Initial window width */
    long	init_height;		/* Initial window height */

    U_INT	width, height;		/* Current window dimensions */
    U_INT	depth, border;		/* window depth & border size */

    double	xscale_init;		/* initial pixels/lx (virt. coords) */
    double	yscale_init;		/* initial pixels/ly (virt. coords) */
    double	xscale;			/* as above, but current value */
    double	yscale;			/*  (after possible resizing)  */

    short xold, yold;			/* Coords of last point plotted */
    short xlen, ylen;			/* Lengths of device coord space */

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
    GC		gc;			/* Graphics context */
    Colormap	map;			/* Colormap */
    Pixmap	pixmap;			/* Off-screen pixmap */
} XwDev;

#endif	/* __PLPLOTX_H__ */
