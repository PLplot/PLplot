/* $Id$
 * 
 *	Main program for Tcl-interface to PLplot.  Allows interpretive
 *	execution of plotting primitives without regard to output driver.
 *
 * Maurice LeBrun
 * IFS, University of Texas at Austin
 * 19-Jun-1994
 *
 */

#include "plplot/plplotP.h"
#include "plplot/pltcl.h"

static void
plErrorHandler(Tcl_Interp *interp, int code, int tty);

static int
AppInit(Tcl_Interp *interp);

extern void (*tclErrorHandler)(Tcl_Interp *interp, int code, int tty);

/*--------------------------------------------------------------------------*\
 * main --
 *
 * Just a stub routine to call pltclMain.  The latter is nice to have when
 * building extended tclsh's, since then you don't have to rely on sucking
 * the Tcl main out of libtcl (which doesn't work correctly on all
 * systems/compilers/linkers/etc).  Hopefully in the future Tcl will
 * supply a sufficiently capable tclMain() type function that can be used
 * instead.
\*--------------------------------------------------------------------------*/

int
main(int argc, char **argv)
{
    exit(pltclMain(argc, argv, NULL, AppInit));
}

/*--------------------------------------------------------------------------*\
 * plExitCmd
 *
 * PLplot/Tcl extension command -- handle exit.
 * The reason for overriding the normal exit command is so we can tell
 * the PLplot library to clean up.
\*--------------------------------------------------------------------------*/

static int
plExitCmd(ClientData clientData, Tcl_Interp *interp, int argc, char **argv)
{

/* Print error message if one given */

    if (interp->result != NULL && interp->result[0] != '\0')
	fprintf(stderr, "%s\n", interp->result);

    plspause(0);
    plend();

    Tcl_UnsetVar(interp, "tcl_prompt1", 0);
    Tcl_Eval(interp, "tclexit");

    return TCL_OK;
}

/*--------------------------------------------------------------------------*\
 * prPromptCmd
 *
 * PLplot/Tcl extension command -- print the prompt.
 * Allows much more flexible setting of the prompt.
\*--------------------------------------------------------------------------*/

static int
prPromptCmd(ClientData clientData, Tcl_Interp *interp, int argc, char **argv)
{
    PLStream *pls;
    char prompt[80];

    plgpls(&pls);

    if (pls->ipls == 0) 
	sprintf(prompt, "pltext; puts -nonewline \"pltcl> \"; flush stdout");
    else
	sprintf(prompt, "pltext; puts -nonewline \"pltcl_%d> \"; flush stdout", pls->ipls);

    Tcl_VarEval(interp, prompt, 0);

    return TCL_OK;
}

/*
 *--------------------------------------------------------------------------
 *
 * AppInit --
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
 *--------------------------------------------------------------------------
 */

static int
AppInit(Tcl_Interp *interp)
{
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

    if (Tcl_Init(interp) == TCL_ERROR) {
	return TCL_ERROR;
    }
#ifdef HAVE_ITCL
    if (Itcl_Init(interp) == TCL_ERROR) {
	return TCL_ERROR;
    }
#endif
    if (Pltcl_Init(interp) == TCL_ERROR) {
	return TCL_ERROR;
    }

/* Application-specific startup.  That means: for use in pltcl ONLY. */

/* Rename "exit" to "tclexit", and insert custom exit handler */

    Tcl_VarEval(interp, "rename exit tclexit", (char *) NULL);

    Tcl_CreateCommand(interp, "exit", plExitCmd,
                      (ClientData) NULL, (void (*)(ClientData)) NULL);

    Tcl_CreateCommand(interp, "pr_prompt", prPromptCmd,
                      (ClientData) NULL, (void (*)(ClientData)) NULL);

/* Custom prompt, to make sure we are in text mode when entering commands */

    Tcl_SetVar(interp, "tcl_prompt1", "pr_prompt", 0);

/* Also before an error is printed.  Can't use normal call mechanism here */
/* because it trashes the interp->result string. */

    tclErrorHandler = plErrorHandler;

    return TCL_OK;
}

/*
 *--------------------------------------------------------------------------
 *
 * plErrorHandler --
 *
 *	Handles error conditions while parsing.  Modified from the version
 *	in tclMain.c to ensure we are on the text screen before issuing
 *	the error message, otherwise it may disappear.
 *
 * Results:
 *	None.
 *
 * Side effects:
 *	Error info is printed to stdout, if interactive, otherwise stderr.
 *
 *--------------------------------------------------------------------------
 */

static void
plErrorHandler(Tcl_Interp *interp, int code, int tty)
{
    pltext();
    if (tty)
	printf("%s\n", interp->result);
    else
	fprintf(stderr, "%s\n", interp->result);
}
