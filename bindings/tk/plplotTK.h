/* $Id$
 * $Log$
 * Revision 1.4  1994/11/02 19:55:41  mjl
 * Added cursorX, cursorY for holding coordinates at mouse click.
 *
 * Revision 1.3  1994/07/19  22:34:22  mjl
 * Internal header file inclusion changed to /not/ use a search path so that
 * it will work better with makedepend.
 *
 * Revision 1.2  1994/06/30  18:36:37  mjl
 * Minor change to what header files are included.
 *
 * Revision 1.1  1994/04/08  12:21:18  mjl
 * Added to allow access to TK driver internals by determined users.
*/

/*	plplotTK.h

	Holds system header includes, prototypes of TK driver
	utility functions, and definition of the state structure.
*/

#ifndef __PLPLOTTK_H__
#define __PLPLOTTK_H__

/* Several of the PLplot header files must be included first */
/* I only do this conditionally because of the search path */

#ifndef __PLPLOT_H__
#include <plplot.h>
#endif
#ifndef __PDF_H__
#include <pdf.h>
#endif

#include <tk.h>

/* One of these holds the TK driver state information */

typedef struct {
    Tk_Window	w;			/* Main window */
    Tcl_Interp	*interp;		/* Interpreter */
    PLINT	xold, yold;		/* Coordinates of last point plotted */
    int		exit_eventloop;		/* Break out of event loop */
    int		pass_thru;		/* Skip normal error termination */
    char	*cmdbuf;		/* Command buffer */
    int		cmdbuf_len;		/* and its length */
    PLiodev	*iodev;			/* I/O device info */
    float	cursorX, cursorY;	/* coordinates for mouse event */
} TkDev;

#endif	/* __PLPLOTTK_H__ */
