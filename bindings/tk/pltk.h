/* $Id$
 * $Log$
 * Revision 1.1  1995/05/07 03:01:18  mjl
 * Added to hold all TK-related PLplot global function prototypes.
 *
 */

/* 
 * pltk.h
 * Maurice LeBrun
 * 28-Apr-95
 *
 * Declarations for PLplot/TK utility routines.
 */

#ifndef __PLTK_H__
#define __PLTK_H__

#include <tk.h>

/* Generic initialization routine for extended wish'es. */

int
Pltk_Init( Tcl_Interp *interp );

/* Main program for wish, with PLplot extensions. */

int
pltkMain(int argc, char **argv, char *RcFileName,
	 int (*AppInit)(Tcl_Interp *interp));

#endif	/* __PLTK_H__ */
