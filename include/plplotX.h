/* $Id$
 * $Log$
 * Revision 1.1  1993/12/08 06:20:38  mjl
 * Added for all code/data shared with the xwin driver.
 *
*/

/*	plplotX.h

	Holds system header includes, prototypes of xwin driver
	utility functions, and definition of the state structure.
*/

/* System headers */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/cursorfont.h>
#include <X11/keysym.h>

/* X driver utility functions */

void  PLColor_to_XColor		(PLColor *, XColor *);
void  PLColor_from_XColor	(PLColor *, XColor *);
int   pl_AreWeGrayscale		(Display *);

/* One of these holds the X driver state information */

typedef struct {
    int		exit_eventloop;
    long	init_width;
    long	init_height;

    U_INT	width, height;
    U_INT	depth, border;

    double	xscale;
    double	yscale;
    double	xscale_dev;
    double	yscale_dev;

    short xold, yold;		/* Coordinates of last point plotted */
    short xlen, ylen;		/* Lengths of device coordinate space */

    int		color;
    XColor	cmap0[16];
    XColor	bgcolor;
    XColor	fgcolor;
    XColor	curcolor;

    int		write_to_window;
    int		write_to_pixmap;

    int		is_main;
    int		screen;
    Display	*display;
    Window	window;
    GC		gc;
    Colormap	map;
    Pixmap	pixmap;
} XwDev;

