/* $Id$
 * $Log$
 * Revision 1.1  1994/04/08 12:21:18  mjl
 * Added to allow access to TK driver internals by determined users.
 *
*/

/*	plplotTK.h

	Holds system header includes, prototypes of TK driver
	utility functions, and definition of the state structure.
*/

#ifndef __PLPLOTTK_H__
#define __PLPLOTTK_H__

#include <plplot.h>
#include <plplotX.h>
#include <pdf.h>
#include <plstream.h>

#include <tcl.h>
#include <tk.h>

/* One of these holds the TK driver state information */

typedef struct {
    Tk_Window w;		/* Main window */
    Tcl_Interp *interp;		/* Interpreter */
    PLINT xold, yold;		/* Coordinates of last point plotted */
    int   exit_eventloop;	/* Flag for breaking out of event loop */
    int   pass_thru;		/* Skips normal error termination when set */
    char  *cmdbuf;		/* Command buffer */
    int   cmdbuf_len;		/* and its length */
    PLiodev *iodev;		/* I/O device info */
} TkDev;

#endif	/* __PLPLOTTK_H__ */
