/* $Id$
 * $Log$
 * Revision 1.3  1995/06/01 21:24:59  mjl
 * Added include of pltcl.h.
 *
 * Revision 1.2  1995/05/08  21:29:47  furnish
 * Fix bogus C++ unfriendly header.
 *
 * Revision 1.1  1995/05/07  03:01:18  mjl
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

#include "pltcl.h"
#include <tk.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Generic initialization routine for extended wish'es. */

int
Pltk_Init( Tcl_Interp *interp );

/* Main program for wish, with PLplot extensions. */

int
pltkMain(int argc, char **argv, char *RcFileName,
	 int (*AppInit)(Tcl_Interp *interp));

#ifdef __cplusplus
}
#endif

#endif	/* __PLTK_H__ */
