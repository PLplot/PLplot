/* $Id$
 * $Log$
 * Revision 1.2  1993/07/16 22:01:29  mjl
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

#include "plplotP.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "plstream.h"

#include <tk.h>

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
