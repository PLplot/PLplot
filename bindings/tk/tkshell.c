/* $Id$
 * $Log$
 * Revision 1.6  1993/12/09 21:20:12  mjl
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

