/* $Id$
 * $Log$
 * Revision 1.1  1994/06/23 22:41:36  mjl
 * Holds function prototypes and other declarations dealing with Tcl API.
 *
 */

/* 
 * pltcl.h
 * Maurice LeBrun
 * 21-Jun-94
 *
 * Declarations for pltcl and associated files.  
 */

#ifndef __PLTCL_H__
#define __PLTCL_H__

#include <plplotP.h>
#include <tcl.h>
#include <tclMatrix.h>

/* Front-end to PLplot/Tcl API for use from the plframe widget. */

int
plTclCmd(char *xcmdlist, Tcl_Interp *interp,
	 int argc, char **argv);

/* Initialization routine for extended wish'es. */

int
Pltcl_Init( Tcl_Interp *interp );

#endif	/* __PLTCL_H__ */
