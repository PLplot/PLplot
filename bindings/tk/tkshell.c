/* 
 * tkshell.c
 * Maurice LeBrun
 * 6-May-93
 *
 * Functions to handle creation & initialization of TCL interpreter and
 * main window.  Taken from source code for "wish", copyright follows:
 *
 * Copyright 1990-1992 Regents of the University of California.
 * Permission to use, copy, modify, and distribute this
 * software and its documentation for any purpose and without
 * fee is hereby granted, provided that the above copyright
 * notice appear in all copies.  The University of California
 * makes no representations about the suitability of this
 * software for any purpose.  It is provided "as is" without
 * express or implied warranty.
 */

#include "plserver.h"

/* Global variables */

static int synchronize = 0;

/* Static function prototypes */

static void	DelayedMap	(ClientData);
static void	StructureProc	(ClientData, XEvent *);

/*----------------------------------------------------------------------*\
*
* tk_toplevel --
*
*	Creates top level window.
*
* Results:
*	Returns 1 on error.
*
* Side effects:
*	Returns window ID as *w, and arranges for window to be mapped.
*
\*----------------------------------------------------------------------*/

int
tk_toplevel(Tk_Window *w, Tcl_Interp *interp,
	    char *display, char *name, int options)
{
    char *new_name;
    char wcmd[] = "wm withdraw .";

/*
* Determine server name.  If it contains any forward slashes ("/"), only
* use the part following the last "/" so that name can be loaded with 
* argv[0] by caller.
*/
    new_name = strrchr(name, '/');
    if (new_name != NULL) {
	name = ++new_name;
    }

/* Create the main window */

    *w = Tk_CreateMainWindow(interp, display, name);
    if (*w == NULL) {
	fprintf(stderr, "%s\n", (interp)->result);
	return(1);
    }

/*
* Initialize the Tk application and arrange to map the main window
* after the startup script has been executed, if any.  This way
* the script can withdraw the window so it isn't ever mapped
* at all.
*/
    if (! (options && NOMAP)) {
	Tk_SetClass(*w, "Tk");
	Tk_CreateEventHandler(*w, StructureNotifyMask, StructureProc,
			      (ClientData) *w);

	Tk_DoWhenIdle(DelayedMap, (ClientData) *w);
	if (synchronize) {
	    XSynchronize(Tk_Display(*w), True);
	}
    }
    else
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

/*----------------------------------------------------------------------*\
*
* StructureProc --
*
*	This procedure is invoked whenever a structure-related event
*	occurs on the main window.  If the window is deleted, the
*	procedure modifies "w" to record that fact.
*
* Results:
*	None.
*
* Side effects:
*	Variable "w" may get set to NULL.
*
\*----------------------------------------------------------------------*/

static void
StructureProc(ClientData clientData, XEvent *eventPtr)
{
    register Tk_Window w = (Tk_Window) clientData;

    if (eventPtr->type == DestroyNotify) {
	w = NULL;
    }
}

/*----------------------------------------------------------------------*\
*
* DelayedMap --
*
*	This procedure is invoked by the event dispatcher once the
*	startup script has been processed.  It waits for all other
*	pending idle handlers to be processed (so that all the
*	geometry information will be correct), then maps the
*	application's main window.
*
* Results:
*	None.
*
* Side effects:
*	The main window gets mapped.
*
\*----------------------------------------------------------------------*/

static void
DelayedMap(ClientData clientData)
{
    register Tk_Window w = (Tk_Window) clientData;

    while (Tk_DoOneEvent(TK_IDLE_EVENTS) != 0) {
	/* Empty loop body. */
    }
    if (w == NULL) {
	return;
    }
    Tk_MapWindow(w);
}
