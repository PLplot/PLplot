/* $Id$
 * $Log$
 * Revision 1.3  1994/07/19 22:34:23  mjl
 * Internal header file inclusion changed to /not/ use a search path so that
 * it will work better with makedepend.
 *
 * Revision 1.2  1994/06/30  18:37:49  mjl
 * Moved function prototypes for generic Tcl support calls into this file.
 *
 * Revision 1.1  1994/06/23  22:41:36  mjl
 * Holds function prototypes and other declarations dealing with Tcl API.
 */

/* 
 * pltcl.h
 * Maurice LeBrun
 * 21-Jun-94
 *
 * Declarations for PLplot/Tcl utility routines.
 * In general, these do not require either Tk or Tcl-DP.
 */

#ifndef __PLTCL_H__
#define __PLTCL_H__

#include <tcl.h>
#include "tclMatrix.h"

/* Function prototypes. */

/* From tclAPI.c */

/* Front-end to PLplot/Tcl API for use from Tcl commands (e.g. plframe). */

int
plTclCmd(char *cmdlist, Tcl_Interp *interp,
	 int argc, char **argv);

/* Initialization routine for PLplot-extended tclsh's (like pltcl). */

int
Pltcl_Init( Tcl_Interp *interp );

/* From tkshell.c */

/* Sets up auto_path variable */

int
pls_auto_path(Tcl_Interp *interp);

/* Tcl command -- wait until the specified condition is satisfied. */

int
plWait_Until(ClientData, Tcl_Interp *, int, char **);

/* Tcl command -- return the IP address for the current host.  */

int
plHost_ID(ClientData clientData, Tcl_Interp *interp, int argc, char **argv);

/* From tcpip.c */

/* Modified version of the "Tdp_PacketSend" command. */

int
pl_PacketSend(Tcl_Interp *interp, PLiodev *iodev, PDFstrm *pdfs);

/* Modified version of the "Tdp_PacketReceive" command. */

int
pl_PacketReceive(Tcl_Interp *interp, PLiodev *iodev, PDFstrm *pdfs);

#endif	/* __PLTCL_H__ */
