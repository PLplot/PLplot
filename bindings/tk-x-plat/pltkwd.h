/* $Id$
 * $Log$
 * Revision 1.3  2002/07/28 22:41:15  airwin
 * Split tkwin.c into tkwin_common.c and tkwin.c to remove cross-linking issue
 * and allow tkwin.c to act like a normal driver.  So the last "special" version
 * of drivers is gone.
 *
 * Reconfigured library dependencies and link lines so that we now have reasonably
 * rational hierarchical dependencies and linking on Linux.
 *
 * Revision 1.2  2002/07/10 09:52:38  vincentdarley
 * resolve name clashes, and sync pltools.tcl
 *
 * Revision 1.1  2002/07/02 09:03:44  vincentdarley
 * x-platform tk code
 *
 * Revision 1.1.2.1  2000/07/28 19:12:23  airwin
 * AWI: added files from pltk patch
 *
 * Revision 1.1  1995/06/01  21:45:26  mjl
 * Created to hold definitions and prototypes specific to the X-driver.  The
 * user shouldn't need to access this file but it's here just in case.
 *
 *
 * Old entries from plplotX.h:
 *
 * Revision 1.17  1995/05/26  20:20:13  mjl
 * Changed plstream.h -> plstrm.h everywhere.
 *
 * Revision 1.16  1995/04/12  21:10:26  mjl
 * Made the ordinary graphics context and the current color a device-dependent
 * quantity rather than a display-dependent one, to fix problems with color
 * selection when plotting to separate streams/xwindows.  Thanks to Radey
 * Shouman for pointing it out.
 *
 * Revision 1.15  1995/03/16  23:43:46  mjl
 * Old cursorX, cursorY variables replaced with a PLGraphicsIn structure.
 * Also variables added for dealing with graphic crosshairs.
 *
 * Revision 1.14  1994/11/02  19:55:50  mjl
 * Added cursorX, cursorY for holding coordinates at mouse click.
 *
 * Revision 1.13  1994/10/11  18:59:57  mjl
 * Added prototypes for plD_open_xw() and plX_setBGFG().
*/

/*	plxwd.h

	Holds system header includes, prototypes of xwin driver
	utility functions, and definition of the state structure.
*/

#ifndef __PLTKWD_H__
#define __PLTKWD_H__

#include "plplot.h"
#include "plstrm.h"

/* System headers */

#ifndef MAC_TCL
#ifndef __MWERKS__
#include <sys/types.h>
#endif
#endif

#include <tk.h>
#ifdef MAC_TCL
#else
/* These are pulled in by 'tkMacPort.h' */
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/cursorfont.h>
#include <X11/keysym.h>
#endif
/* Specify max number of displays in use */

#define PLTKDISPLAYS 100

/* Set constants for dealing with colormap.  In brief:
 *
 * ccmap                When set, turns on custom color map
 *
 * See Init_CustomCmap() and  Init_DefaultCmap() for more info.
 * Set ccmap at your own risk -- still under development.
 */

static int plplot_tkwin_ccmap = 0;

/* One of these holds the display info, shared by all streams on a given */
/* display */

typedef struct {
    int		nstreams;		/* Number of streams using display */
    int		ixwd;			/* Specifies tkwDisplay number */
    char	*displayName;		/* Name of X display */
    int		screen;			/* X screen */
    Display	*display;		/* X display */
    Visual	*visual;		/* X Visual */
    GC		gcXor;			/* Graphics context for XOR draws */
    Colormap	map;			/* Colormap */
    unsigned	depth;			/* display depth */
    int		color;			/* Set to 1 if a color output device */
    int		ncol0;			/* Number of cmap 0 colors allocated */
    int		ncol1;			/* Number of cmap 1 colors allocated */
    XColor	cmap0[16];		/* Color entries for cmap 0 */
    XColor	cmap1[256];		/* Color entries for cmap 1 */
    XColor	fgcolor;		/* Foreground color (if grayscale) */
    Tk_Cursor	xhair_cursor;		/* Crosshair cursor */
} TkwDisplay;

/* One of these holds the X driver state information */

typedef struct {
    TkwDisplay	*tkwd;			/* Pointer to display info */

    Window	window;			/* X window id */
    Pixmap	pixmap;			/* Off-screen pixmap */
    GC		gc;			/* Graphics context */
    XColor	curcolor;		/* Current pen color */

    long	event_mask;		/* Event mask */
    int		flags;		        /* 1 = delete, 2 = exit event loop */
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

    int		instr;			/* Instruction timer */
    int		max_instr;		/* Limit before X server is queried */

    PLGraphicsIn gin;			/* Graphics input structure */

    int		locate_mode;		/* Set while in locate mode */
    int		drawing_xhairs;		/* Set during xhair draws */
    XPoint	xhair_x[2], xhair_y[2];	/* Crosshair lines */

    void (*MasterEH) (PLStream *, XEvent *);	/* Master X event handler */
} TkwDev;

/*--------------------------------------------------------------------------*\
 *		Function Prototypes
\*--------------------------------------------------------------------------*/

#ifdef __cplusplus
extern "C" {
#endif

/* Performs basic driver initialization. */

void
plD_open_tkwin(PLStream *pls);

/* Copies the supplied PLColor to an XColor */

void
PLColor_to_TkColor(PLColor *plcolor, XColor *xcolor);

/* Copies the supplied XColor to a PLColor */

void
PLColor_from_TkColor(PLColor *plcolor, XColor *xcolor);

/* Same but also tells me if the color changed */
int
PLColor_from_TkColor_Changed(PLColor *plcolor, XColor *xcolor);

/* Set background & foreground colors.  */

void
pltkwin_setBGFG(PLStream *pls);

#ifdef __cplusplus
}
#endif

#endif	/* __PLTKWD_H__ */
