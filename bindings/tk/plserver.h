/* $Id$
 * $Log$
 * Revision 1.16  1994/06/16 19:07:08  mjl
 * Include-guarded.  Now includes file tclMatrix.h, to pick up the new matrix
 * command support.  Prototype for plframe() moved here from plplot.h since
 * it shouldn't be necessary for the user to explicitly refer to it any more
 * (in lieu of using Pltk_Init()).
 *
 */

/* 
 * plserver.h
 * Maurice LeBrun
 * 6-May-93
 *
 * Declarations for plserver and associated files.  
 */

#ifndef __PLSERVER_H__
#define __PLSERVER_H__

#include <plplotP.h>
#include <plplotTK.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <math.h>
#include <ctype.h>

#include "tclMatrix.h"

/* State info for the rendering code */

typedef struct {
    char  *client;			/* Name of client main window */
    PDFstrm *pdfs;			/* PDF stream descriptor */
    PLiodev *iodev;			/* I/O info */
    int   nbytes;			/* data bytes waiting to be read */
    int   at_bop, at_eop;		/* bop/eop condition flags */

    short xmin, xmax, ymin, ymax;	/* Data minima and maxima */
    PLFLT xold, yold;			/* Endpoints of last line plotted */
} PLRDev;

/* External function prototypes. */
/* Note that tcl command functions are added during Pltk_Init and don't */
/* need to be called directly by the user */

/* from plframe.c */

int
plFrameCmd(ClientData clientData, Tcl_Interp *interp,
	   int argc, char **argv);

/* from tkshell.c */

/* Create top level window */

int
pltk_toplevel(Tk_Window *w, Tcl_Interp *interp,
	      char *display, char *basename, char *classname);

/* Run a script */

int
pltk_source(Tk_Window w, Tcl_Interp *interp, char *script);

/* Sets up auto_path variable */

int
pls_auto_path(Tcl_Interp *interp);

/* Tcl command -- wait until the specified condition is satisfied. */

int
plWait_Until(ClientData, Tcl_Interp *, int, char **);

/* from plr.c */

/* Set default state parameters before anyone else has a chance to. */

void
plr_start(PLRDev *plr);

/* Read & process commands until "nbyte_max" bytes have been read. */

int
plr_process(PLRDev *plr);

/* From tcpip.c */

/* Modified version of the "Tdp_PacketReceive" command. */

int
pl_PacketReceive(Tcl_Interp *interp, PLiodev *iodev, PDFstrm *pdfs);

/* Modified version of the "Tdp_PacketSend" command. */

int
pl_PacketSend(Tcl_Interp *interp, PLiodev *iodev, PDFstrm *pdfs);

/* Tcl command -- return the IP address for the current host.  */

int
plHost_ID(ClientData clientData, Tcl_Interp *interp, int argc, char **argv);

#endif	/* __PLSERVER_H__ */
