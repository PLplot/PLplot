/* $Id$
 * $Log$
 * Revision 1.10  1994/03/22 23:17:39  furnish
 * Avoid collision with user code when he wants to make a custom wish
 * combined with PLPLOT.
 *
 * Revision 1.9  1994/01/15  17:48:31  mjl
 * Added built-in Tcl commands: wait_until (for general use -- waits until
 * the specified condition evaluates to true) and host_id (Tcl-DP only).
 *
 * Revision 1.8  1993/12/21  10:32:04  mjl
 * Moved a part of the set_auto_path function back into plserver.c where
 * it belonged (adding directories to auto_path based on an input flag).
 *
 * Revision 1.7  1993/12/15  09:05:43  mjl
 * Added functions Tcl_AppInit() and set_autoload(), to be shared by both
 * the plserver and tk driver interpreter startup code.  Changes to
 * Tcl_AppInit() to support Tcl-DP style communication (taken from Tcl-DP
 * source distribution).
 *
 * Revision 1.6  1993/12/09  21:20:12  mjl
 * Simplified tk_toplevel() and eliminated obsolete functions.
 *
 * Revision 1.5  1993/11/19  07:55:44  mjl
 * Added missing CVS id and log fields.
*/

/* 
 * tkshell.c
 * Maurice LeBrun
 * 6-May-93
 *
 * Functions to handle creation & initialization of TCL interpreter and
 * main window.  
 */

#include "plserver.h"
#ifdef TCL_DP
extern int Tdp_Init			_ANSI_ARGS_((Tcl_Interp *interp));
#endif

static int tcl_cmd	(Tcl_Interp *interp, char *cmd);
static int tcl_eval	(Tcl_Interp *interp, char *cmd);

/*----------------------------------------------------------------------*\
*
* tk_toplevel --
*
*	Create top level window without mapping it.
*
* Results:
*	Returns 1 on error.
*
* Side effects:
*	Returns window ID as *w.
*
\*----------------------------------------------------------------------*/

int
tk_toplevel(Tk_Window *w, Tcl_Interp *interp,
	    char *display, char *basename, char *classname)
{
    char *new_name;
    static char wcmd[] = "wm withdraw .";

/*
* Determine server name.  If it contains any forward slashes ("/"), only
* use the part following the last "/" so that name can be loaded with 
* argv[0] by caller.
*/
    new_name = strrchr(basename, '/');
    if (new_name != NULL) 
	basename = ++new_name;

    new_name = strrchr(classname, '/');
    if (new_name != NULL) 
	classname = ++new_name;

/* Create the main window without mapping it */

#if (TK_MAJOR_VERSION <= 3) && (TK_MINOR_VERSION <= 3)
    *w = Tk_CreateMainWindow(interp, display, basename);
#else
    *w = Tk_CreateMainWindow(interp, display, basename, classname);
#endif
    if (*w == NULL) {
	fprintf(stderr, "%s\n", (interp)->result);
	return(1);
    }

    Tcl_VarEval(interp, wcmd, (char *) NULL);

    return(0);
}

/*----------------------------------------------------------------------*\
*
* tk_source --
*
*	Run a script.
*
* Results:
*	Returns 1 on error.
*
* Side effects:
*	None.
*
\*----------------------------------------------------------------------*/

int
tk_source(Tk_Window w, Tcl_Interp *interp, char *script)
{
    int result;
    char *msg;

/* Execute TCL/TK initialization script. */

    if (script != NULL) {
	result = Tcl_VarEval(interp, "source ", script, (char *) NULL);

	if (result != TCL_OK) {
	    msg = Tcl_GetVar(interp, "errorInfo", TCL_GLOBAL_ONLY);
	    if (msg == NULL) {
		msg = interp->result;
	    }
	    fprintf(stderr, "%s\n", msg);
	    return(1);
	}
    }
    return(0);
}

/*
 *----------------------------------------------------------------------
 *
 * Tcl_AppInit --
 *
 *	This procedure performs application-specific initialization.
 *	Most applications, especially those that incorporate additional
 *	packages, will have their own version of this procedure.
 *
 * Results:
 *	Returns a standard Tcl completion code, and leaves an error
 *	message in interp->result if an error occurs.
 *
 * Side effects:
 *	Depends on the startup script.
 *
 *----------------------------------------------------------------------
 */

int
plTcl_AppInit(Tcl_Interp *interp)
{
    Tk_Window main;

    main = Tk_MainWindow(interp);

    /*
     * Call the init procedures for included packages.  Each call should
     * look like this:
     *
     * if (Mod_Init(interp) == TCL_ERROR) {
     *     return TCL_ERROR;
     * }
     *
     * where "Mod" is the name of the module.
     */

#if (TK_MAJOR_VERSION <= 3) && (TK_MINOR_VERSION <= 2)
    if (tk_source(main, interp, "$tk_library/wish.tcl")) {
	return TCL_ERROR;
    }
#else
    if (Tcl_Init(interp) == TCL_ERROR) {
	return TCL_ERROR;
    }
    if (main && Tk_Init(interp) == TCL_ERROR) {
	return TCL_ERROR;
    }
#endif

    /*
     * Call Tcl_CreateCommand for application-specific commands, if
     * they weren't already created by the init procedures called above.
     */

    Tcl_CreateCommand(interp, "wait_until", Wait_Until,
		      (ClientData) NULL, (void (*) (ClientData)) NULL);

#ifdef TCL_DP
    if (Tdp_Init(interp) == TCL_ERROR) {
	return TCL_ERROR;
    }
    Tcl_CreateCommand(interp, "host_id", Host_ID,
		      (ClientData) NULL, (void (*) (ClientData)) NULL);
#endif

    /*
     * Specify a user-specific startup file to invoke if the application
     * is run interactively.  Typically the startup file is "~/.apprc"
     * where "app" is the name of the application.  If this line is deleted
     * then no user-specific startup file will be run under any conditions.
     */


/* Set up auto_path */

    if (set_auto_path(interp) == TCL_ERROR)
	return TCL_ERROR;

    return TCL_OK;
}

/*----------------------------------------------------------------------*\
* set_autopath
*
* Sets up auto_path variable
* Note: there is no harm in adding extra directories, even if they don't
* actually exist (aside from a slight increase in processing time when
* the autoloaded proc is first found).
\*----------------------------------------------------------------------*/

int
set_auto_path(Tcl_Interp *interp)
{
    char *buf, *ptr=NULL, *dn;
#ifdef DEBUG
    char *path;
#endif

    dbug_enter("set_auto_path");
    buf = (char *) malloc(256 * sizeof(char));

/* Add INSTALL_DIR/tcl */

#ifdef INSTALL_DIR
    plGetName(INSTALL_DIR, "tcl", "", &ptr);
    Tcl_SetVar(interp, "dir", ptr, TCL_GLOBAL_ONLY);
    if (tcl_cmd(interp, "set auto_path \"$dir $auto_path\"") == TCL_ERROR)
	return TCL_ERROR;
#ifdef DEBUG
    fprintf(stderr, "adding %s to auto_path\n", ptr);
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
	fprintf(stderr, "adding %s to auto_path\n", buf);
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
    dbug_enter("tcl_cmd");
#ifdef DEBUG_ENTER
    fprintf(stderr, "evaluating command %s\n", cmd);
#endif

    if (tcl_eval(interp, cmd)) {
	fprintf(stderr, "TCL command \"%s\" failed:\n\t %s\n",
		cmd, interp->result);
	return TCL_ERROR;
    }
    return TCL_OK;
}

/*----------------------------------------------------------------------*\
* tcl_eval
*
* Evals the specified string, returning the result.
* Use a static string buffer to hold the command, to ensure it's in
* writable memory (grrr...).
\*----------------------------------------------------------------------*/

static char *cmdbuf = NULL;
static int cmdbuf_len = 100;

static int
tcl_eval(Tcl_Interp *interp, char *cmd)
{
    if (cmdbuf == NULL) 
	cmdbuf = (char *) malloc(cmdbuf_len);

    if (strlen(cmd) >= cmdbuf_len) {
	free((void *) cmdbuf);
	cmdbuf_len = strlen(cmd) + 20;
	cmdbuf = (char *) malloc(cmdbuf_len);
    }

    strcpy(cmdbuf, cmd);
    return(Tcl_VarEval(interp, cmdbuf, (char **) NULL));
}

/*----------------------------------------------------------------------*\
* Wait_Until
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
Wait_Until(ClientData clientData, Tcl_Interp *interp, int argc, char **argv)
{
    int result = 0;

    dbug_enter("Wait_Until");

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

