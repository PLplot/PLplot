/* $Id$
 * $Log$
 * Revision 1.4  1993/08/18 19:04:08  mjl
 * Added include of file "plplotio.h".
 *
 * Revision 1.3  1993/08/03  01:48:08  mjl
 * Eliminated dependence on internal Tcl/TK header files.
 *
 * Revision 1.2  1993/07/16  22:01:29  mjl
 * Eliminated obsolete variables from renderer state struct.
 *
 * Revision 1.1  1993/07/02  06:58:32  mjl
 * The new TCL/TK driver!  Yes it's finally here!  YAAAAAAAAYYYYYYY!!!
 *
 */

/* 
 * plserver.h
 * Maurice LeBrun
 * 6-May-93
 *
 * Declarations for plserver and associated files.  
 */

/* This is always included first */

#include "plplotP.h"

/* System headers */

#include <stdio.h>
#include <ctype.h>
#include <fcntl.h>
#include <math.h>
#include <pwd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/file.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <unistd.h>

#include <X11/Xlib.h>
#include <X11/cursorfont.h>
#include <X11/keysym.h>
#include <X11/Xatom.h>
#include <X11/Xproto.h>
#include <X11/Xresource.h>
#include <X11/Xutil.h>

#include <tk.h>

#include "plstream.h"
#include "plplotio.h"

/* Macro settings */

#define NOMAP		0x01		/* Do not map toplevel window */

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

int tk_toplevel 	(Tk_Window *, Tcl_Interp *, char *, char *, int);
int tk_source		(Tk_Window, Tcl_Interp *, char *);

/* from plr.c */

void	plr_start	(PLRDev *);
int	plr_process	(PLRDev *);
