/* $Id$
 * $Log$
 * Revision 1.4  2002/07/11 08:52:03  vincentdarley
 * resolve name conflict between tk, tk-x-plat
 *
 * Revision 1.3  2002/07/10 09:52:38  vincentdarley
 * resolve name clashes, and sync pltools.tcl
 *
 * Revision 1.2  2002/07/04 15:14:54  vincentdarley
 * handle plplot version info directly
 *
 * Revision 1.1  2002/07/02 09:03:44  vincentdarley
 * x-platform tk code
 *
 * Revision 1.2.4.1  2000/07/28 17:48:10  airwin
 * AWI: pltk patch changes to existing files
 *
 * Revision 1.2  1998/11/19 04:32:46  furnish
 * Hack out a dumb little diagnostic which has been being printed for a
 * couple of years, but whose utility has long since expired.
 *
 * Revision 1.1  1996/10/25  19:21:21  furnish
 * New package bootstrap facility, contributed by Vince Darley, further
 * modified by Geoff Furnish.  Still needs work.
 *
 *
 * Revision 1.25  1996/11/7 vince

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

#ifdef BUILD_Plplot
#undef TCL_STORAGE_CLASS
#define TCL_STORAGE_CLASS DLLEXPORT
#endif /* BUILD_Vfs */

/*----------------------------------------------------------------------*\
 * Plplotter_Init
 *
 * Initialization routine for extended wish'es.
 * Creates the plframe, wait_until, and host_id (w/Tcl-DP only)
 * commands.  The more basic Plplot-Tcl initialization is handled by
 * the Plbasicinit function called from here.
\*----------------------------------------------------------------------*/

EXTERN int
Plplotter_Init( Tcl_Interp *interp )
{
#ifdef USE_TCL_STUBS
    /* 
     * We hard-wire 8.1 here, rather than TCL_VERSION, TK_VERSION because
     * we really don't mind which version of Tcl, Tk we use as long as it
     * is 8.1 or newer.  Otherwise if we compiled against 8.2, we couldn't
     * be loaded into 8.1
     */
    Tcl_InitStubs(interp,"8.1",0);
#endif
#ifdef USE_TK_STUBS
    Tk_InitStubs(interp,"8.1",0);
#endif
    /* This must be before any other Tcl related calls */
    if (PlbasicInit(interp) != TCL_OK) {
	return TCL_ERROR;
    }

    /* 
     * Note, the old technique of:
     *    main = Tk_MainWindow(interp);
     * and then passing main to 'plframe' as the clientdata can
     * cause unusual problems, probably due to bugs in Tk on
     * some platforms, when the 'main window' doesn't yet exist
     * properly by the time we are called.  Since plframe only
     * uses the value in one place (each time a new frame is
     * created), we simply use 'Tk_MainWindow' in plframe, and
     * avoid the startup problems.
     */

/* plframe -- PLplot graphing widget */

    Tcl_CreateCommand( interp, "plframe", plPlotterCmd,
		       (ClientData) NULL, (Tcl_CmdDeleteProc*) NULL);

/* host_id -- returns host IP number.  Only for use with Tcl-DP */

#ifdef PLD_dp
    Tcl_CreateCommand(interp, "host_id", plHost_ID,
	      (ClientData) NULL, (Tcl_CmdDeleteProc*) NULL);
#endif

    Tcl_PkgProvide(interp,"Plplotter",PLPLOT_VERSION);
    return TCL_OK;
}

