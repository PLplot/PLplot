/* $Id$
 * $Log$
 * Revision 1.6.4.1  2000/07/27 15:54:30  airwin
 * AWI: apply initial matrix.patch for TEA-based approach
 *
 * Revision 1.6  1995/06/02 20:30:03  mjl
 * Made C++ friendly.
 *
 * Revision 1.5  1995/06/01  21:19:14  mjl
 * Split off IPC routine prototypes (not needed by user) into tcpip.h.
 *
 * Revision 1.4  1995/05/07  03:00:25  mjl
 * Added prototype for pltclMain().
 *
 * Revision 1.3  1994/07/19  22:34:23  mjl
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
 * These should not require either Tk or Tcl-DP.
 */

#ifndef __PLTCL_H__
#define __PLTCL_H__

#include "plplot.h"
#include <tcl.h>
#define USE_MATRIX_STUBS
#include "tclMatrix.h"

#ifdef __cplusplus
extern "C" {
#endif

/* tclMain.c */
/* Main program for a Tcl-based shell that reads Tcl commands from stdin. */

int
pltclMain(int argc, char **argv, char *RcFileName,
	  int (*AppInit)(Tcl_Interp *interp));

/* tclAPI.c */
/* Front-end to PLplot/Tcl API for use from Tcl commands (e.g. plframe). */

int
plTclCmd(char *cmdlist, Tcl_Interp *interp,
	 int argc, char **argv);

/* Initialization routine for PLplot-extended tclsh's (like pltcl). */
#if defined(BUILD_Pltk)
int
Pltcl_Init( Tcl_Interp *interp );
int 
PlbasicInit( Tcl_Interp *interp );

#else

/*
 * Windows needs to know which symbols to export.  Unix does not.
 * BUILD_Pltcl should be undefined for Unix.
 */

#ifdef BUILD_Pltcl
#undef TCL_STORAGE_CLASS
#define TCL_STORAGE_CLASS DLLEXPORT
#endif /* BUILD_Pltcl */

/*
 * Only the _Init function is exported, when building a shared lib.
 */

EXTERN int
Pltcl_Init( Tcl_Interp *interp );

EXTERN int 
PlbasicInit( Tcl_Interp *interp );

#endif

/* tkshell.c */
/* Sets up auto_path variable */

int
pls_auto_path(Tcl_Interp *interp);

/* Tcl command -- wait until the specified condition is satisfied. */

int
plWait_Until(ClientData, Tcl_Interp *, int, char **);

/* Tcl command -- return the IP address for the current host.  */

int
plHost_ID(ClientData clientData, Tcl_Interp *interp, int argc, char **argv);

#ifdef __cplusplus
}
#endif

#endif	/* __PLTCL_H__ */
