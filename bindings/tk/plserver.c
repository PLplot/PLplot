/* $Id$
 * $Log$
 * Revision 1.7  1993/08/11 19:24:03  mjl
 * Fixed some type errors.
 *
 * Revision 1.6  1993/08/10  04:51:32  mjl
 * Fixed -mkidx option (broken in last update).
 *
*/

/* 
 * plserver.c
 * Maurice LeBrun
 * 30-Apr-93
 *
 * Plplot graphics server.
 *
 * Can be run as a child process from the plplot TK driver.
 *
 * Alternately it can be used as a generic code server (with the appropriate
 * TCL initialization) for launching applications that plot into plplot
 * graphics widgets.
 */

#include "plserver.h"

/* Externals */

extern int   plFrameCmd     	(ClientData, Tcl_Interp *, int, char **);

/* Global variables */

static Tk_Window w;			/* Main window */
static Tcl_Interp *interp;		/* Interpreter */

static char *program;			/* Name of program */

/* Command line arguments */

static char *client;			/* Name of client main window */
static char *file;			/* TCL initialization file name */
static char *display;			/* X-windows display */
static char *geometry;			/* x window dimension */
static char *auto_path;			/* addition to auto_path */

static int child;			/* set if child of TK driver */
static int mkidx;			/* Create a new tclIndex file */

Tk_ArgvInfo argTable[] = {
{"-client", TK_ARGV_STRING, (char *) NULL, (char *) &client,
     "Client to notify at startup, if any"},
{"-auto_path", TK_ARGV_STRING, (char *) NULL, (char *) &auto_path,
     "Additional directory(s) to autoload"},
{"-f", TK_ARGV_STRING, (char *) NULL, (char *) &file,
     "TCL initialization file name"},
{"-display", TK_ARGV_STRING, (char *) NULL, (char *) &display,
     "Display to use"},
{"-geometry", TK_ARGV_STRING, (char *) NULL, (char *) &geometry,
     "Initial window geometry"},
{"-mkidx", TK_ARGV_CONSTANT, (char *) 1, (char *) &mkidx,
     "Create new tclIndex file"},
{"-child", TK_ARGV_CONSTANT, (char *) 1, (char *) &child,
     "Set ONLY when child of plplot TK driver"},
{(char *) NULL, TK_ARGV_END, (char *) NULL, (char *) NULL,
     (char *) NULL}
};

/* Support routine prototypes */

static void  configure		(int, char **);
static void  parse_cmdline	(int *, char **);
static void  abort_session	(char *);
static void  set_auto_path	(void);
static void  NotifyClient	(ClientData);
static void  tcl_cmd		(char *);
static int   tcl_eval		(char *);

/*----------------------------------------------------------------------*\
* main
*
* Main program for server process.
\*----------------------------------------------------------------------*/

int
main(int argc, char **argv)
{
    program = argv[0];

#ifdef DEBUG
    fprintf(stderr, "%s -- PID: %d, PGID: %d, PPID: %d\n",
	    program, getpid(), getpgrp(), getppid());
#endif

/* Instantiate a TCL interpreter. */

    interp = Tcl_CreateInterp();

/* Parse command-line arguments. */

    parse_cmdline(&argc, argv);

/* Initialize top level window */

    if (tk_toplevel(&w, interp, display, argv[0], 0))
	abort_session("");

/* Initialize stuff known to interpreter */

    configure(argc, argv);

/* Run TK startup code */

    if (tk_source(w, interp, "$tk_library/wish.tcl"))
	abort_session("");

/* Create new tclIndex file -- a convenience */

    if (mkidx != NULL) {
	tcl_cmd("auto_mkindex . *.tcl");
	client = NULL;
	abort_session("");
    }

/* Set up auto_path */

    set_auto_path();

/* 
* Run user startup code.
*
* Note you can change the name of the initialization proc here if desired.
* This is useful for wrapping plserver_init in your own code.  If the
* "source" command had some sort of path mechanism I wouldn't have to go
* through all these contortions.
*/
    if (tk_source(w, interp, file))
	abort_session("");

/* Configure main window, default settings.  Autoloaded. */

    tcl_cmd("$plserver_init_proc");

/* Send notification to client if necessary */

    if (client != NULL) 
	Tk_DoWhenIdle(NotifyClient, (ClientData) client);

/* Main loop */

    Tk_MainLoop();

/* Normal clean up */

    Tcl_DeleteInterp(interp);

    exit(0);
}

/*----------------------------------------------------------------------*\
* abort_session
*
* Terminates with an error, doing whatever cleanup appears to be necessary.
\*----------------------------------------------------------------------*/

static void
abort_session(char *errmsg)
{
    dbug_enter("abort_session");

    if (*errmsg != '\0')
	fprintf(stderr, "%s\n", errmsg);

/* If client exists, tell it to self destruct */

    if (client != NULL)
	tcl_cmd("send $client after 1 abort");

/* If main window exists, blow it away */

    if (w != NULL)
	tcl_cmd("destroy .");

    exit(1);
}

/*----------------------------------------------------------------------*\
* parse_cmdline
*
* Handles parsing of command line and some diagnostic output.
\*----------------------------------------------------------------------*/

static void
parse_cmdline(int *p_argc, char **argv)
{
    dbug_enter("parse_cmdline");

    if (Tk_ParseArgv(interp, w, p_argc, argv, argTable, 0) != TCL_OK) {
	fprintf(stderr, "%s\n", interp->result);
	abort_session("");
    }

#ifdef DEBUG
    fprintf(stderr, "\
	\n In server, client:   %s\
	\n            display:  %s\
	\n            geometry: %s\
	\n            file:     %s\
	\n",
	    client, display, geometry, file);
#endif
}

/*----------------------------------------------------------------------*\
* configure
*
* Does global variable & command initialization, mostly for interpreter.
\*----------------------------------------------------------------------*/

static void
configure(int argc, char **argv)
{
    char *args, buf[20];

    dbug_enter("configure");

/* Use main window name as program name, now that we have it */

    program = Tk_Name(w);

/* Tell interpreter about commands. */

    Tcl_CreateCommand(interp, "plframe", plFrameCmd,
                      (ClientData) w, (void (*)()) NULL);

/* Default initialization proc */

    Tcl_SetVar(interp, "plserver_init_proc", "plserver_init", 0);

/* Now variable information. */
/* For uninitialized variables it's better to skip the Tcl_SetVar. */

    if (geometry != NULL)
	Tcl_SetVar(interp, "geometry", geometry, 0);

/* Pass client variable to interpreter if set.  Sometimes useful. */

    if (client != NULL)
	Tcl_SetVar(interp, "client", client, 0);

/* Tell interpreter about plserver's ancestry */

    if (child != 0)
	Tcl_SetVar(interp, "child", "1", 0);

/* Store leftover arguments into the Tcl variables "argc" and "argv". */

    args = Tcl_Merge(argc-1, argv+1);
    Tcl_SetVar(interp, "argv", args, TCL_GLOBAL_ONLY);
    ckfree(args);
    sprintf(buf, "%d", argc-1);
    Tcl_SetVar(interp, "argc", buf, TCL_GLOBAL_ONLY);
}

/*----------------------------------------------------------------------*\
* set_autopath
*
* Sets up auto_path variable
* Note: there is no harm in adding extra directories, even if they don't
* actually exist (aside from a slight increase in processing time when
* the autoloaded proc is first found).
\*----------------------------------------------------------------------*/

static void
set_auto_path(void)
{
    char *buf, *ptr;
#ifdef DEBUG
    char *path;
#endif

    dbug_enter("set_auto_path");
    buf = malloc(256 * sizeof(char));

/* Add /usr/local/plplot/tcl */

    Tcl_SetVar(interp, "dir", "/usr/local/plplot/tcl", TCL_GLOBAL_ONLY);
    tcl_cmd("set auto_path \"$dir $auto_path\"");
#ifdef DEBUG
    fprintf(stderr, "plserver: adding %s to auto_path\n", "/usr/local/plplot");
    path = Tcl_GetVar(interp, "auto_path", 0);
    fprintf(stderr, "plserver: auto_path is %s\n", path);
#endif

/* Add $HOME/tcl */

    ptr = getenv("HOME");
    if (ptr != NULL) {
	strcpy(buf, ptr);
	strcat(buf, "/tcl");
	Tcl_SetVar(interp, "dir", buf, 0);
	tcl_cmd("set auto_path \"$dir $auto_path\"");
#ifdef DEBUG
	fprintf(stderr, "plserver: adding %s to auto_path\n", buf);
	path = Tcl_GetVar(interp, "auto_path", 0);
	fprintf(stderr, "plserver: auto_path is %s\n", path);
#endif

    }

/* Add $PL_LIBRARY */

    ptr = getenv("PL_LIBRARY");
    if (ptr != NULL) {
	Tcl_SetVar(interp, "dir", ptr, 0);
	tcl_cmd("set auto_path \"$dir $auto_path\"");
#ifdef DEBUG
	fprintf(stderr, "plserver: adding %s to auto_path\n", ptr);
	path = Tcl_GetVar(interp, "auto_path", 0);
	fprintf(stderr, "plserver: auto_path is %s\n", path);
#endif
    }

/* Add cwd */

    if (getcwd(buf, 256) == NULL) {
	abort_session("could not determine cwd");
    }
    Tcl_SetVar(interp, "dir", buf, 0);
    tcl_cmd("set auto_path \"$dir $auto_path\"");

#ifdef DEBUG
    fprintf(stderr, "plserver: adding %s to auto_path\n", buf);
    path = Tcl_GetVar(interp, "auto_path", 0);
    fprintf(stderr, "plserver: auto_path is %s\n", path);
#endif

/* Add user-specified directory(s) */

    if (auto_path != NULL) {
	Tcl_SetVar(interp, "dir", auto_path, 0);
	tcl_cmd("set auto_path \"$dir $auto_path\"");
#ifdef DEBUG
	fprintf(stderr, "plserver: adding %s to auto_path\n", auto_path);
	path = Tcl_GetVar(interp, "auto_path", 0);
	fprintf(stderr, "plserver: auto_path is %s\n", path);
#endif

    }

    free ((void *) buf);
}

/*----------------------------------------------------------------------*\
* NotifyClient
*
* Sends client program notification
\*----------------------------------------------------------------------*/

static void
NotifyClient(ClientData clientData)
{
    char *client = (char *) clientData;

    Tcl_SetVar(interp, "plserver", program, 0);
    Tcl_SetVar(interp, "client", client, 0);
#ifdef DEBUG
    fprintf(stderr, "$plserver: %s, $client: %s\n",
	    Tcl_GetVar(interp, "plserver", 0),
	    Tcl_GetVar(interp, "client", 0));
#endif
    tcl_cmd("send $client [list set plserver $plserver]");
}

/*----------------------------------------------------------------------*\
* tcl_cmd
*
* Evals the specified command, aborting on an error.
\*----------------------------------------------------------------------*/

static void
tcl_cmd(char *cmd)
{
    dbug_enter("tcl_cmd");
#ifdef DEBUG_ENTER
    fprintf(stderr, "plserver: evaluating command %s\n", cmd);
#endif

    if (tcl_eval(cmd)) {
	fprintf(stderr, "plserver: TCL command \"%s\" failed:\n\t %s\n",
		cmd, interp->result);
	abort_session("");
    }
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
tcl_eval(char *cmd)
{
    if (cmdbuf == NULL) 
	cmdbuf = (char *) malloc(cmdbuf_len);

    if (strlen(cmd) >= cmdbuf_len) {
	free((void *) cmdbuf);
	cmdbuf_len = strlen(cmd) + 20;
	cmdbuf = (char *) malloc(cmdbuf_len);
    }

    strcpy(cmdbuf, cmd);
    return(Tcl_Eval(interp, cmdbuf, 0, (char **) NULL));
}
