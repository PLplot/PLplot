/* $Id$
 * $Log$
 * Revision 1.3.4.1  2000/07/28 17:48:10  airwin
 * AWI: pltk patch changes to existing files
 *
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

/*
 * Windows needs to know which symbols to export.  Unix does not.
 * BUILD_Pltk should be undefined for Unix.
 */

#ifdef BUILD_Pltk
#undef TCL_STORAGE_CLASS
#define TCL_STORAGE_CLASS DLLEXPORT
#endif /* BUILD_Pltk */

#ifdef __cplusplus
extern "C" {
#endif

/* Generic initialization routine for extended wish'es. */

/*
 * Only the _Init function is exported, when building a shared lib.
 */
EXTERN int
Pltk_Init( Tcl_Interp *interp );

/* Main program for wish, with PLplot extensions. */

int
pltkMain(int argc, char **argv, char *RcFileName,
	 int (*AppInit)(Tcl_Interp *interp));

#ifdef __cplusplus
}
#endif

#endif	/* __PLTK_H__ */
