/* $Id$
 * $Log$
 * Revision 1.37  1996/06/26 21:35:15  furnish
 * Various hacks to support Tcl 7.5 and Tk 4.1.
 *
 * Revision 1.36  1995/06/01  21:24:27  mjl
 * Is now [incr Tcl] aware, if HAVE_ITCL is defined (done during configure).
 *
 * Revision 1.35  1995/05/06  17:11:30  mjl
 * Improved debugging output and fixed a bug in the exit handling.
 *
 * Revision 1.34  1995/04/12  08:06:15  mjl
 * Offloaded the C code for cleaning up from plserver.c into the proc
 * plserver_link_end in plserver.tcl.  The Tcl code was modified to better
 * handshake with the client (plplot TK driver) program.
 *
 * Revision 1.33  1995/03/16  23:13:57  mjl
 * Fixed the Copyright message and general cleaning up.
 *
 * Revision 1.32  1994/09/27  21:56:50  mjl
 * Minor exit handling fix for Tk communication.
 *
 * Revision 1.31  1994/09/23  07:40:19  mjl
 * Now does a bit more complete cleanup at exit when communicating with the
 * Tcl-DP driver, to avoid potential problems.
 *
 * Revision 1.30  1994/09/18  07:14:58  mjl
 * Changed the syntax for pltkMain() in order for it to work better with
 * shared libraries.  In particular, Tcl_AppInit is no longer external but
 * passed as a function pointer.
 *
 * Revision 1.29  1994/08/25  04:00:27  mjl
 * Fixed some error output; elminates spurious <RET> at end.
 *
 * Revision 1.28  1994/07/19  22:31:43  mjl
 * All device drivers: enabling macro renamed to PLD_<driver>, where <driver>
 * is xwin, ps, etc.  See plDevs.h for more detail.  All internal header file
 * inclusion changed to /not/ use a search path so that it will work better
 * with makedepend.
 *
 * Revision 1.27  1994/06/30  18:45:02  mjl
 * Minor changes to pass gcc -Wall without warnings and other cleaning up.
 *
 * Revision 1.26  1994/06/23  22:33:53  mjl
 * Fixed bug introduced in last update concerning exit handling for DP style
 * communication.
 *
 * Revision 1.25  1994/06/16  19:04:32  mjl
 * Massively restructured.  Is now just a front-end to the pltkMain()
 * function.  Structured along the preferred lines for extended wish'es.
*/

/* 
    plserver.c
    Copyright 1993, 1994, 1995
    Maurice LeBrun			mjl@dino.ph.utexas.edu
    Institute for Fusion Studies	University of Texas at Austin
 
    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public
    License along with this library; if not, write to the Free
    Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 
    PLplot graphics server.
 
    Just a front-end to the pltkMain() function.  Structured along the
    preferred lines for extended wish'es.  Is typically run as a child
    process from the PLplot TK driver to render output.  Can use either TK
    send or Tcl-DP RPC for communication, depending on how it is invoked.
 
    Note that plserver can be used the same way as wish or dpwish, as it
    contains the functionality of each of these (except the -notk Tcl-DP
    command-line option is not supported).  
*/
/*
*/

#include "plserver.h"

/* Application-specific command-line options */
/* Variable declarations */

static char *client_name;	/* Name of client main window */
static char *auto_path;		/* addition to auto_path */
static int child;		/* set if child of TK driver */
static int dp;			/* set if using Tcl-DP to communicate */
static char *client_host;	/* Host id for client */
static char *client_port;	/* Communications port id for client */

static Tk_ArgvInfo argTable[] = {
    {"-client_name", TK_ARGV_STRING, (char *) NULL, (char *) &client_name,
	 "Client main window name to connect to"},
    {"-client_host", TK_ARGV_STRING, (char *) NULL, (char *) &client_host,
	 "Client host to connect to"},
    {"-client_port", TK_ARGV_STRING, (char *) NULL, (char *) &client_port,
	 "Client port (Tcl-DP) to connect to"},
    {"-auto_path", TK_ARGV_STRING, (char *) NULL, (char *) &auto_path,
	 "Additional directory(s) to autoload"},
    {"-child", TK_ARGV_CONSTANT, (char *) 1, (char *) &child,
	 "Set ONLY when child of PLplot TK driver"},
    {(char *) NULL, TK_ARGV_END, (char *) NULL, (char *) NULL,
	 (char *) NULL}
};

/* PLplot/Tk extension command -- handle exit. */

static int
plExitCmd(ClientData clientData, Tcl_Interp *interp, int argc, char **argv);

/* Evals the specified command, aborting on an error. */

static void
tcl_cmd(Tcl_Interp *interp, char *cmd);

/* Application-specific startup */

static int
AppInit(Tcl_Interp *interp);

/*--------------------------------------------------------------------------*\
 * main --
 *
 * Just a stub routine to call pltkMain.  The latter is nice to have
 * when building extended wishes, since then you don't have to rely on
 * sucking the Tk main out of libtk (which doesn't work correctly on all
 * systems/compilers/linkers/etc).  Hopefully in the future Tk will
 * supply a sufficiently capable tkMain() type function that can be used
 * instead. 
\*--------------------------------------------------------------------------*/

int
main(int argc, char **argv)
{
    int i, myargc = argc;
    char *myargv[20];
    Tcl_Interp *interp;
    char *helpmsg = "Command-specific options:";

#ifdef DEBUG
    fprintf(stderr, "Program %s called with arguments :\n", argv[0]);
    for (i = 1; i < argc; i++) {
	fprintf(stderr, "%s ", argv[i]);
    }
    fprintf(stderr, "\n");
#endif

/* Create interpreter just for argument parsing */

    interp = Tcl_CreateInterp();

/* Save arglist to get around tk_ParseArgv limitations */

    for (i = 0; i < argc; i++) {
	myargv[i] = argv[i];
    }

/* Parse args */
/* Examine the result string to see if an error return is really an error */

    if (Tk_ParseArgv(interp, (Tk_Window) NULL, &argc, argv, argTable, 
		     TK_ARGV_NO_DEFAULTS) != TCL_OK) {
	fprintf(stderr, "\n(plserver) %s\n\n", interp->result);
	fprintf(stderr, "\
The client_<xxx> and -child options should not be used except via the\n\
PLplot/Tk driver.\n\n(wish) ");
	if (strncmp(interp->result, helpmsg, strlen(helpmsg)))
	    exit(1);
    }

/* No longer need interpreter */

#if TCL_MAJOR_VERSION < 7 || ( TCL_MAJOR_VERSION == 7 && TCL_MINOR_VERSION < 5 )
    Tcl_DeleteInterp(interp);
#endif

/* Call pltkMain() with original argc/argv list, to make sure -h is seen */
/* Does not return until program exit */

    exit(pltkMain(myargc, myargv, NULL, AppInit));
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

#if TCL_MAJOR_VERSION < 7 || ( TCL_MAJOR_VERSION == 7 && TCL_MINOR_VERSION < 5 )
    if (Tcl_Init(interp) == TCL_ERROR) {
	return TCL_ERROR;
    }
    if (main && Tk_Init(interp) == TCL_ERROR) {
	return TCL_ERROR;
    }
#endif
#ifdef HAVE_ITCL
    if (Itcl_Init(interp) == TCL_ERROR) {
	return TCL_ERROR;
    }
#endif
#ifdef PLD_dp
    if (Tdp_Init(interp) == TCL_ERROR) {
	return TCL_ERROR;
    }
#endif
    if (Pltk_Init(interp) == TCL_ERROR) {
	return TCL_ERROR;
    }

/* Application-specific startup.  That means: for use in plserver ONLY. */

/* Pass child variable to interpreter if set. */

    if (child != 0)
	Tcl_SetVar(interp, "child", "1", 0);

/* If client_name is set, TK send is being used to communicate. */
/* If client_port is set, Tcl-DP RPC is being used to communicate. */
/* The "dp" variable determines which style communication is used */

    if (client_name != NULL) {
	Tcl_SetVar(interp, "client_name", client_name, 0);
	dp = 0; tcl_cmd(interp, "set dp 0");
    }
    else if (client_port != NULL) {
#ifdef PLD_dp
	Tcl_SetVar(interp, "client_port", client_port, 0);
	if (client_host != NULL)
	    Tcl_SetVar(interp, "client_host", client_host, 0);
	dp = 1; tcl_cmd(interp, "set dp 1");
#else
	Tcl_AppendResult(interp,
			 "no Tcl-DP support in this version of plserver",
			 (char *) NULL);
	return TCL_ERROR;
#endif
    }

/* Add user-specified directory(s) to auto_path */

    if (auto_path != NULL) {
	Tcl_SetVar(interp, "dir", auto_path, 0);
	tcl_cmd(interp, "set auto_path \"$dir $auto_path\"");
    }

/* Rename "exit" to "tkexit", and insert custom exit handler */

    tcl_cmd(interp, "rename exit tkexit");

    Tcl_CreateCommand(interp, "exit", plExitCmd,
                      (ClientData) main, (void (*)(ClientData)) NULL);

    return TCL_OK;
}

/*--------------------------------------------------------------------------*\
 * plExitCmd
 *
 * PLplot/Tk extension command -- handle exit.
 * The reason for overriding the normal exit command is so we can tell the
 * client to abort.
\*--------------------------------------------------------------------------*/

static int
plExitCmd(ClientData clientData, Tcl_Interp *interp, int argc, char **argv)
{
    int value = 0;

/* Print error message if one given */

    if (interp->result != NULL && interp->result[0] != '\0')
	fprintf(stderr, "%s\n", interp->result);

/* Best to check the syntax before proceeding */

    if ((argc != 1) && (argc != 2)) {
	Tcl_AppendResult(interp, "wrong # args: should be \"", argv[0],
			 " ?returnCode?\"", (char *) NULL);
	return TCL_ERROR;
    }
    if ((argc != 1) && (Tcl_GetInt(interp, argv[1], &value) != TCL_OK)) {
	Tcl_AppendResult(interp, "non-integer return code: \"", argv[1],
			 "\"", (char *) NULL);
	return TCL_ERROR;
    }

/* If client exists, tell it to self destruct */

    Tcl_VarEval(interp, "plserver_link_end", (char **) NULL);

/* Now really exit */

    return Tcl_VarEval(interp, "tkexit", argv[1], (char **) NULL);
}

/*--------------------------------------------------------------------------*\
* tcl_cmd
*
* Evals the specified command, aborting on an error.
\*--------------------------------------------------------------------------*/

static void
tcl_cmd(Tcl_Interp *interp, char *cmd)
{
    int result;

    dbug_enter("tcl_cmd");
    pldebug("tcl_cmd", "evaluating command %s\n", cmd);

    result = Tcl_VarEval(interp, cmd, (char **) NULL);
    if (result != TCL_OK) {
	Tcl_Eval(interp, "exit");
	exit(1);
    }
}
