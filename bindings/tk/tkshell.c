/* $Id$
 * $Log$
 * Revision 1.21  1994/07/22 22:21:16  mjl
 * Eliminated a gcc -Wall warning.
 *
 * Revision 1.20  1994/07/19  22:31:50  mjl
 * All device drivers: enabling macro renamed to PLD_<driver>, where <driver>
 * is xwin, ps, etc.  See plDevs.h for more detail.  All internal header file
 * inclusion changed to /not/ use a search path so that it will work better
 * with makedepend.
 *
 * Revision 1.19  1994/06/30  18:44:18  mjl
 * Cleaning up, also moved Tk-driver-specific functions to tk.c
 *
 * Revision 1.18  1994/06/23  22:37:30  mjl
 * Minor cosmetic changes.
 *
 * Revision 1.17  1994/06/16  19:15:17  mjl
 * Moved the Tk initialization function for the tk driver into tk.c.  Changed
 * Pltk_Init to include by default some of the other Tcl commands used by
 * the PLplot/Tk driver.  Turned set_auto_path() into pls_auto_path() and
 * made it global.
 *
 * Revision 1.16  1994/06/09  20:27:35  mjl
 * Hacked out direct widget support; this was moved to plframe.c.
*/

/* 
 * tkshell.c
 * Maurice LeBrun
 * 6-May-93
 *
 * A miscellaneous assortment of Tcl support functions.
 */

#include "plserver.h"
#include <unistd.h>

/* Static functions */

/* Evals the specified command, aborting on an error. */

static int
tcl_cmd(Tcl_Interp *interp, char *cmd);

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
    Tk_Window main;

    main = Tk_MainWindow(interp);

/* plframe -- PLplot graphing widget */

    Tcl_CreateCommand(interp, "plframe", plFrameCmd,
                      (ClientData) main, (void (*)(ClientData)) NULL);

/* matrix -- matrix support command */

    Tcl_CreateCommand(interp, "matrix", Tcl_MatrixCmd,
                      (ClientData) main, (void (*)(ClientData)) NULL);

/* wait_until -- waits for a specific condition to arise */
/* Can be used with either Tcl-DP or TK */

    Tcl_CreateCommand(interp, "wait_until", plWait_Until,
		      (ClientData) NULL, (void (*) (ClientData)) NULL);

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

    display = Tk_Display(main);
    map = DefaultColormap(display, DefaultScreen(display));

    PLX_save_colormap(display, map);
    }
#endif
    return TCL_OK;
}

/*----------------------------------------------------------------------*\
 * pls_auto_path
 *
 * Sets up auto_path variable
 * Note: there is no harm in adding extra directories, even if they don't
 * actually exist (aside from a slight increase in processing time when
 * the autoloaded proc is first found).
\*----------------------------------------------------------------------*/

int
pls_auto_path(Tcl_Interp *interp)
{
    char *buf, *ptr=NULL, *dn;
#ifdef DEBUG
    char *path;
#endif

    dbug_enter("set_auto_path");
    buf = (char *) malloc(256 * sizeof(char));

/* Add TCL_DIR */

#ifdef TCL_DIR
    Tcl_SetVar(interp, "dir", TCL_DIR, TCL_GLOBAL_ONLY);
    if (tcl_cmd(interp, "set auto_path \"$dir $auto_path\"") == TCL_ERROR)
	return TCL_ERROR;
#ifdef DEBUG
    fprintf(stderr, "adding %s to auto_path\n", TCL_DIR);
    path = Tcl_GetVar(interp, "auto_path", 0);
    fprintf(stderr, "auto_path is %s\n", path);
#endif
#endif

/* Add $HOME/tcl */

    dn = getenv("HOME");
    if (dn != NULL) {
	plGetName(dn, "tcl", "", &ptr);
	Tcl_SetVar(interp, "dir", ptr, 0);
	if (tcl_cmd(interp, "set auto_path \"$dir $auto_path\"") == TCL_ERROR)
	    return TCL_ERROR;
#ifdef DEBUG
	fprintf(stderr, "adding %s to auto_path\n", ptr);
	path = Tcl_GetVar(interp, "auto_path", 0);
	fprintf(stderr, "auto_path is %s\n", path);
#endif
    }

/* Add $(PLPLOT_DIR)/tcl */

    dn = getenv("PLPLOT_DIR");
    if (dn != NULL) {
	plGetName(dn, "tcl", "", &ptr);
	Tcl_SetVar(interp, "dir", ptr, 0);
	if (tcl_cmd(interp, "set auto_path \"$dir $auto_path\"") == TCL_ERROR)
	    return TCL_ERROR;
#ifdef DEBUG
	fprintf(stderr, "adding %s to auto_path\n", ptr);
	path = Tcl_GetVar(interp, "auto_path", 0);
	fprintf(stderr, "auto_path is %s\n", path);
#endif
    }

/* Add cwd */

    if (getcwd(buf, 256) == NULL) 
	return TCL_ERROR;

    Tcl_SetVar(interp, "dir", buf, 0);
    if (tcl_cmd(interp, "set auto_path \"$dir $auto_path\"") == TCL_ERROR)
	return TCL_ERROR;

#ifdef DEBUG
    fprintf(stderr, "adding %s to auto_path\n", buf);
    path = Tcl_GetVar(interp, "auto_path", 0);
    fprintf(stderr, "auto_path is %s\n", path);
#endif

    free_mem(buf);
    free_mem(ptr);

    return TCL_OK;
}

/*----------------------------------------------------------------------*\
 * tcl_cmd
 *
 * Evals the specified command, aborting on an error.
\*----------------------------------------------------------------------*/

static int
tcl_cmd(Tcl_Interp *interp, char *cmd)
{
    int result;

    result = Tcl_VarEval(interp, cmd, (char **) NULL);
    if (result != TCL_OK) {
	fprintf(stderr, "TCL command \"%s\" failed:\n\t %s\n",
		cmd, interp->result);
    }
    return result;
}

/*----------------------------------------------------------------------*\
 * plWait_Until
 *
 * Tcl command -- wait until the specified condition is satisfied.
 * Processes all events while waiting.
 *
 * This command is more capable than tkwait, and has the added benefit
 * of working with Tcl-DP as well.  Example usage:
 *
 *	wait_until {[info exists foobar]}
 *
 * Note the [info ...] command must be protected by braces so that it
 * isn't actually evaluated until passed into this routine.
\*----------------------------------------------------------------------*/

int
plWait_Until(ClientData clientData, Tcl_Interp *interp, int argc, char **argv)
{
    int result = 0;

    dbug_enter("plWait_Until");

    for (;;) {
	if (Tcl_ExprBoolean(interp, argv[1], &result)) {
	    fprintf(stderr, "wait_until command \"%s\" failed:\n\t %s\n",
		    argv[1], interp->result);
	    break;
	}
	if (result)
	    break;

	Tk_DoOneEvent(0);
    }
    return TCL_OK;
}
