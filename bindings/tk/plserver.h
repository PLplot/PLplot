/* $Id$
 * $Log$
 * Revision 1.9  1993/12/09 21:19:26  mjl
 * Changed prototype for tk_toplevel().
 *
 * Revision 1.8  1993/12/09  20:33:41  mjl
 * Eliminated unneccessary system header file inclusions.
 *
 * Revision 1.7  1993/12/08  06:18:08  mjl
 * Changed to include new plplotX.h header file.
 *
 * Revision 1.6  1993/11/19  07:31:20  mjl
 * Fixed the prototype for tk_toplevel().
 */

/* 
 * plserver.h
 * Maurice LeBrun
 * 6-May-93
 *
 * Declarations for plserver and associated files.  
 */

#include "plplotP.h"
#include "plplotX.h"
#include "plstream.h"
#include "plplotio.h"

#include <tcl.h>
#include <tk.h>

#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <math.h>
#include <ctype.h>

/* This data structure holds all state info for the rendering code */

typedef struct {
    char  *client;			/* Name of client main window */
    FILE  *file;			/* fifo or socket file descriptor */
    char  *filename;			/* Name of fifo or socket */
    char  *filetype;			/* Set to "fifo" or "socket" */

    int   nbytes;			/* data bytes waiting to be read */

    short xmin, xmax, ymin, ymax;	/* Data minima and maxima */
    PLFLT xold, yold;			/* Endpoints of last line plotted */
} PLRDev;

/* External function prototypes. */

/* from tk.c */

/* Create top level window */

int
tk_toplevel(Tk_Window *w, Tcl_Interp *interp,
	    char *display, char *basename, char *classname);

/* Run a script */

int
tk_source(Tk_Window w, Tcl_Interp *interp, char *script);

/* from plr.c */

/* Set default state parameters before anyone else has a chance to. */

void
plr_start(PLRDev *plr);

/* Read & process commands until "nbyte_max" bytes have been read. */

int
plr_process(PLRDev *plr);
