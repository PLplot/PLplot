/* $Id$
 * $Log$
 * Revision 1.1  1995/06/01 21:25:40  mjl
 * Added to hold all definitions & prototypes specific to the Tk driver.
 * There shouldn't be much need for the user to access this now, but it's
 * here if you want it.
 *
 *
 * Old log entries from plplotTK.h:
 *
 * Revision 1.6  1995/05/07  02:29:59  mjl
 * Added include of pltk.h, dp.h (if DP extensions requested), and added new
 * variables updatecmd and child_pid to state structure.
 *
 * Revision 1.5  1995/03/16  23:42:04  mjl
 * Old cursorX, cursorY variables replaced with a PLGraphicsIn structure.
 * Also variables instr, max_instr, and locate_mode added.
 *
 * Revision 1.4  1994/11/02  19:55:41  mjl
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

/*	pltkd.h

	Holds system header includes, prototypes of TK driver
	utility functions, and definition of the state structure.

	This file is set up so that the determined user can get access
	to the tk driver data without too much hassle.  The only drawback
	is that plplotP.h must be included to get all of the necessary
	system header files and configuration settings.
*/

#ifndef __PLTKD_H__
#define __PLTKD_H__

#include "plplotP.h"
#include "pltk.h"
#include "pdf.h"

#include <tk.h>
#ifdef PLD_dp
#include <dp.h>
#endif

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
    char	*updatecmd;		/* Name of update command */
    pid_t	child_pid;		/* PID for child process */
    int		instr;			/* Instruction timer */
    int		max_instr;		/* Limit before issuing an update */
    int		locate_mode;		/* Set while in locate mode */
    PLGraphicsIn gin;			/* Graphics input structure */
} TkDev;

#endif	/* __PLTKD_H__ */
