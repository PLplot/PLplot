/* $Id$
 *
 * This file is derived from the old tkshell.c, and implements the Pltk init
 * function.  It can be loaded into any Tcl7.5 interpreter, and requires some
 * .tcl library files to be in known places.  Set your environment variables
 * to make sure of this.
 * 
 * Old changes see 'tkshell.c'.  This file should be used _instead_ of
 * tkshell.c, if building the 'libPltk.so' shared library for dynamic loading
 * into Tcl.
 */

/* 
 * tkshell.c
 * Maurice LeBrun
 * 6-May-93
 *
 * A miscellaneous assortment of Tcl support functions.
 */

#include "plplot/plserver.h"

extern int Matrix_Init(Tcl_Interp* interp);

/*----------------------------------------------------------------------*\
 * Pltk_Init
 *
 * Initialization routine for extended wish'es.
 * Creates the plframe, matrix, wait_until, and host_id (w/Tcl-DP only)
 * commands.  Also sets the auto_path variable.
\*----------------------------------------------------------------------*/

int
Pltk_Init( Tcl_Interp *interp )
{
/*    "if {[catch {source -rsrc plplot.tcl}] != 0} {\n\
 */
    static char initCmd[] =
    "if {[catch {source plplot.tcl}] != 0} {\n\
        if [file exists [file join ${pllibrary} plplot.tcl]] {\n\
	    source [file join ${pllibrary} plplot.tcl]\n\
        } else {\n\
	    set msg \"can't find [file join ${pllibrary} plplot.tcl]\\n\"\n\
	    append msg \"Perhaps you need to install Plplot \\n\"\n\
	    append msg \"or set your PL_LIBRARY environment variable?\"\n\
	    error $msg\n\
        }\n\
    }";

    char *libDir;
/*     Tcl_DString path; */

    Tk_Window mainWindow = Tk_MainWindow(interp);

/* plframe -- PLplot graphing widget */

    Tcl_CreateCommand( interp, "plframe", plFrameCmd,
		       (ClientData) mainWindow, (void (*)(ClientData)) NULL);

/* host_id -- returns host IP number.  Only for use with Tcl-DP */

#ifdef PLD_dp
    Tcl_CreateCommand(interp, "host_id", plHost_ID,
	      (ClientData) NULL, (void (*) (ClientData)) NULL);
#endif

/* Set up auto_path */

    if (pls_auto_path(interp) == TCL_ERROR)
	return TCL_ERROR;

/* Save initial RGB colormap components */
/* Disabled for now */

#if 0
    {
    Display *display;
    Colormap map;

    display = Tk_Display(mainWindow);
    map = DefaultColormap(display, DefaultScreen(display));

    PLX_save_colormap(display, map);
    }
#endif

    if ( Matrix_Init(interp) == TCL_ERROR )
	return TCL_ERROR;

/*     Tcl_PkgRequire(interp,"Matrix","0.1",0); */

    Tcl_PkgProvide(interp,"Pltk","4.99");
/*     libDir = Tcl_GetPkgLibraryPath(interp,&path,"PL_LIBRARY","pltk","4.99"); */

    libDir = NULL;

#ifdef TCL_DIR
    if (libDir == NULL) {
    /*	libDir = PL_LIBRARY; */
	libDir = TCL_DIR;
    }
#endif

    Tcl_SetVar(interp, "pllibrary", libDir, TCL_GLOBAL_ONLY);

/*     Tcl_DStringFree(&path); */

    return Tcl_Eval(interp, initCmd);
}
