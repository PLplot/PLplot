/* $Id$
 * $Log$
 * Revision 1.24  1994/05/07 03:10:33  mjl
 * Changed startup to ensure that initial RGB color map values get saved (a
 * hack to get around limitations in X11R5 and Tk).
 *
 * Revision 1.23  1994/04/08  11:51:12  mjl
 * Changed main init function name (for wish builders) from plTcl_AppInit to
 * pltk_Init, to make it more conventional.
 *
 * Revision 1.22  1994/03/22  23:17:34  furnish
 * Avoid collision with user code when he wants to make a custom wish
 * combined with PLPLOT.
 *
 * Revision 1.21  1994/01/17  21:32:14  mjl
 * Eliminated -mkidx flag as it is unnecessary.  Replace with the alias:
 * alias mktclidx	"echo 'auto_mkindex . *.tcl; destroy .' | wish"
 *
 * Revision 1.20  1994/01/15  17:38:44  mjl
 * Split off communications link initialization into a Tcl proc, for
 * flexibility.
 *
 * Revision 1.19  1993/12/22  21:26:32  mjl
 * Last commit was botched on this file.
 *
 * Revision 1.18  1993/12/21  10:21:06  mjl
 * Changed -client arg name to -client_name to be more transparent (stands
 * for client program main window name).  Substantially rewrote
 * initialization to be better suited for Tcl-DP or TK style communication.
 * Created client_cmd function for handling all messages to the client.
 *
 * Revision 1.17  1993/12/15  08:59:03  mjl
 * Changes to support Tcl-DP.  Also moved functions Tcl_AppInit() and
 * set_autopath() to tkshell.c so they could be used by the TK driver
 * initialization as well.
 *
 * Revision 1.16  1993/12/09  21:29:47  mjl
 * Fixed another bug generated in the reorganization dealing with cleanup.
 *
 * Revision 1.15  1993/12/09  20:25:57  mjl
 * Completely reorganized, using the source for wish (tkMain.c) as a starting
 * point.  Will now be able to track changes to Tcl/TK more easily, and can
 * use plserver exactly like wish (including interactive usage).  Fixed a bug
 * occasionally encountered when shutting down.
 *
 * Revision 1.14  1993/11/19  07:31:35  mjl
 * Updated to new call syntax for tk_toplevel().
 *
 * Revision 1.13  1993/09/27  20:34:25  mjl
 * Eliminated some cases of freeing unallocated memory.
 *
 * Revision 1.12  1993/09/08  18:38:24  mjl
 * Changed conditional compile for Tk 3.2 to expand for any pre-3.3 version.
 *
 * Revision 1.11  1993/09/08  02:31:34  mjl
 * Fixes to work with TK 3.3b3.  Search path for autoloaded Tcl procs now
 * follows ".", ${PLPLOT_DIR}/tcl, ${HOME}/tcl, INSTALL_DIR/tcl, where
 * INSTALL_DIR is passed in from the makefile.
*/

/* 
 * plserver.c
 * Maurice LeBrun
 * 30-Apr-93
 *
 * Plplot graphics server.
 *
 * Is typically run as a child process from the plplot TK driver to render
 * output.  Can use either TK send or Tcl-DP RPC for communication,
 * depending on how it is invoked.
 *
 * Also plserver can be used the same way as wish or dpwish, as it
 * contains the functionality of each of these (except the -notk Tcl-DP
 * command-line option is not supported).  In the source code I've changed
 * as few lines as possible from the source for "wish" in order to make it
 * easier to track future changes to Tcl/TK and Tcl-DP.  Tcl_AppInit (in
 * tkshell.c) was copied from the Tcl-DP sources and modified accordingly.
 */

#include "plserver.h"
/*
#define DEBUG
*/
/* Externals */

extern int   plFrameCmd     	(ClientData, Tcl_Interp *, int, char **);

/*
 * Global variables used by the main program:
 */

static Tk_Window mainWindow;	/* The main window for the application.  If
				 * NULL then the application no longer
				 * exists. */
static Tcl_Interp *interp;	/* Interpreter for this application. */
char *tcl_RcFileName = NULL;	/* Name of a user-specific startup script
				 * to source if the application is being run
				 * interactively (e.g. "~/.wishrc").  Set
				 * by Tcl_AppInit.  NULL means don't source
				 * anything ever. */
static Tcl_DString command;	/* Used to assemble lines of terminal input
				 * into Tcl commands. */
static int tty;			/* Non-zero means standard input is a
				 * terminal-like device.  Zero means it's
				 * a file. */
static char errorExitCmd[] = "exit 1";

/*
 * Command-line options:
 */

static int synchronize = 0;
static char *fileName = NULL;
static char *name = NULL;
static char *display = NULL;
static char *geometry = NULL;

/* plserver additions */

static char *client_name;	/* Name of client main window */
static char *auto_path;		/* addition to auto_path */
static int child;		/* set if child of TK driver */
static int pass_thru;		/* Skip normal error termination when set */
static char *cmdbuf = NULL;	/* Buffer to hold evalled commands */
static int cmdbuf_len = 100;

/* These are for supporting Tcl-DP communication */

static int dp;			/* set if using Tcl-DP to communicate */
static char *client_host;	/* Host id for client */
static char *client_port;	/* Communications port id for client */

static Tk_ArgvInfo argTable[] = {
    {"-file", TK_ARGV_STRING, (char *) NULL, (char *) &fileName,
	"File from which to read commands"},
    {"-geometry", TK_ARGV_STRING, (char *) NULL, (char *) &geometry,
	"Initial geometry for window"},
    {"-display", TK_ARGV_STRING, (char *) NULL, (char *) &display,
	"Display to use"},
    {"-name", TK_ARGV_STRING, (char *) NULL, (char *) &name,
	"Name to use for application"},
    {"-sync", TK_ARGV_CONSTANT, (char *) 1, (char *) &synchronize,
	"Use synchronous mode for display server"},

/* plserver additions */

    {"-client_name", TK_ARGV_STRING, (char *) NULL, (char *) &client_name,
	 "Client main window name to connect to"},
    {"-client_host", TK_ARGV_STRING, (char *) NULL, (char *) &client_host,
	 "Client host to connect to"},
    {"-client_port", TK_ARGV_STRING, (char *) NULL, (char *) &client_port,
	 "Client port (Tcl-DP) to connect to"},
    {"-auto_path", TK_ARGV_STRING, (char *) NULL, (char *) &auto_path,
	 "Additional directory(s) to autoload"},
    {"-child", TK_ARGV_CONSTANT, (char *) 1, (char *) &child,
	 "Set ONLY when child of plplot TK driver"},
    {(char *) NULL, TK_ARGV_END, (char *) NULL, (char *) NULL,
	 (char *) NULL}
};

/* Support routines for plserver */

static void  configure_plserver	(void);
static void  abort_session	(char *);
static void  client_cmd		(char *);
static void  tcl_cmd		(char *);
static int   tcl_eval		(char *);
static void  copybuf		(char *);

/*
 * Forward declarations for procedures defined later in this file:
 */

static void		Prompt _ANSI_ARGS_((Tcl_Interp *interp, int partial));
static void		StdinProc _ANSI_ARGS_((ClientData clientData,
			    int mask));

/*
 *----------------------------------------------------------------------
 *
 * main --
 *
 *	Main program for Wish.
 *
 * Results:
 *	None. This procedure never returns (it exits the process when
 *	it's done
 *
 * Side effects:
 *	This procedure initializes the wish world and then starts
 *	interpreting commands;  almost anything could happen, depending
 *	on the script being interpreted.
 *
 *----------------------------------------------------------------------
 */

int
main(argc, argv)
    int argc;				/* Number of arguments. */
    char **argv;			/* Array of argument strings. */
{
    char *args, *p, *msg;
    char buf[20];
    int code;

#ifdef DEBUG
{
    int i;
    fprintf(stderr, "Program %s called with arguments :\n", argv[0]);
    for (i = 1; i < argc; i++) 
	fprintf(stderr, "%s ", argv[i]);
    fprintf(stderr, "\n");
}
#endif

    interp = Tcl_CreateInterp();
#ifdef TCL_MEM_DEBUG
    Tcl_InitMemory(interp);
#endif
    Tcl_SetVar(interp, "plsend", "send", 0);

    /*
     * Parse command-line arguments.
     */

    if (Tk_ParseArgv(interp, (Tk_Window) NULL, &argc, argv, argTable, 0)
	    != TCL_OK) {
	abort_session(interp->result);
	exit(1);
    }
    if (name == NULL) {
	if (fileName != NULL) {
	    p = fileName;
	} else {
	    p = argv[0];
	}
	name = strrchr(p, '/');
	if (name != NULL) {
	    name++;
	} else {
	    name = p;
	}
    }

    /*
     * If a display was specified, put it into the DISPLAY
     * environment variable so that it will be available for
     * any sub-processes created by us.
     */

    if (display != NULL) {
	Tcl_SetVar2(interp, "env", "DISPLAY", display, TCL_GLOBAL_ONLY);
    }

    /*
     * Initialize the Tk application.
     */

    mainWindow = Tk_CreateMainWindow(interp, display, name, "Tk");
    if (mainWindow == NULL) {
	abort_session(interp->result);
	exit(1);
    }
    if (synchronize) {
	XSynchronize(Tk_Display(mainWindow), True);
    }
    Tk_GeometryRequest(mainWindow, 200, 200);

    /*
     * Make command-line arguments available in the Tcl variables "argc"
     * and "argv".  Also set the "geometry" variable from the geometry
     * specified on the command line.
     */

    args = Tcl_Merge(argc-1, argv+1);
    Tcl_SetVar(interp, "argv", args, TCL_GLOBAL_ONLY);
    ckfree(args);
    sprintf(buf, "%d", argc-1);
    Tcl_SetVar(interp, "argc", buf, TCL_GLOBAL_ONLY);
    Tcl_SetVar(interp, "argv0", (fileName != NULL) ? fileName : argv[0],
	    TCL_GLOBAL_ONLY);
    if (geometry != NULL) {
	Tcl_SetVar(interp, "geometry", geometry, TCL_GLOBAL_ONLY);
    }

    /*
     * Set the "tcl_interactive" variable.
     */

    tty = isatty(0);
    Tcl_SetVar(interp, "tcl_interactive",
	    ((fileName == NULL) && tty) ? "1" : "0", TCL_GLOBAL_ONLY);

    /*
     * Invoke application-specific initialization.
     */

    if (pltk_Init(interp) != TCL_OK) {
	abort_session(interp->result);
	Tcl_Eval(interp, "exit");
    }
    configure_plserver();

    /*
     * Set the geometry of the main window, if requested.
     */

    if (geometry != NULL) {
	code = Tcl_VarEval(interp, "wm geometry . ", geometry, (char *) NULL);
	if (code != TCL_OK) {
	    fprintf(stderr, "%s\n", interp->result);
	}
    }

    /*
     * Invoke the script specified on the command line, if any.
     * If invoked as a child process of the plplot/tk driver, configure
     * main window.
     */

    if (client_name != NULL || client_port != NULL) {
	tcl_cmd("$plserver_init_proc");
	tcl_cmd("plserver_link_init");
	tty = 0;
    }
    else if (fileName != NULL) {
	code = Tcl_VarEval(interp, "source ", fileName, (char *) NULL);
	if (code != TCL_OK) {
	    goto error;
	}
	tty = 0;
    } else {
	/*
	 * Commands will come from standard input, so set up an event
	 * handler for standard input.  If the input device is aEvaluate the
	 * .rc file, if one has been specified, set up an event handler
	 * for standard input, and print a prompt if the input
	 * device is a terminal.
	 */

	if (tcl_RcFileName != NULL) {
	    Tcl_DString buffer;
	    char *fullName;
	    FILE *f;
    
	    fullName = Tcl_TildeSubst(interp, tcl_RcFileName, &buffer);
	    if (fullName == NULL) {
		fprintf(stderr, "%s\n", interp->result);
	    } else {
		f = fopen(fullName, "r");
		if (f != NULL) {
		    code = Tcl_EvalFile(interp, fullName);
		    if (code != TCL_OK) {
			fprintf(stderr, "%s\n", interp->result);
		    }
		    fclose(f);
		}
	    }
	    Tcl_DStringFree(&buffer);
	}
	Tk_CreateFileHandler(0, TK_READABLE, StdinProc, (ClientData) 0);
	if (tty) {
	    Prompt(interp, 0);
	}
    }
    fflush(stdout);
    Tcl_DStringInit(&command);

    /*
     * Loop infinitely, waiting for commands to execute.  When there
     * are no windows left, Tk_MainLoop returns and we exit.
     */

    Tk_MainLoop();

    /*
     * Don't exit directly, but rather invoke the Tcl "exit" command.
     * This gives the application the opportunity to redefine "exit"
     * to do additional cleanup.
     */

    Tcl_Eval(interp, "exit");
    exit(1);

error:
    msg = Tcl_GetVar(interp, "errorInfo", TCL_GLOBAL_ONLY);
    if (msg == NULL) {
	msg = interp->result;
    }
    abort_session(msg);
    Tcl_Eval(interp, errorExitCmd);
    return 1;			/* Needed only to prevent compiler warnings. */
}

/*
 *----------------------------------------------------------------------
 *
 * StdinProc --
 *
 *	This procedure is invoked by the event dispatcher whenever
 *	standard input becomes readable.  It grabs the next line of
 *	input characters, adds them to a command being assembled, and
 *	executes the command if it's complete.
 *
 * Results:
 *	None.
 *
 * Side effects:
 *	Could be almost arbitrary, depending on the command that's
 *	typed.
 *
 *----------------------------------------------------------------------
 */

    /* ARGSUSED */
static void
StdinProc(clientData, mask)
    ClientData clientData;		/* Not used. */
    int mask;				/* Not used. */
{
#define BUFFER_SIZE 4000
    char input[BUFFER_SIZE+1];
    static int gotPartial = 0;
    char *cmd;
    int code, count;

    count = read(fileno(stdin), input, BUFFER_SIZE);
    if (count <= 0) {
	if (!gotPartial) {
	    if (tty) {
		abort_session("");
		Tcl_Eval(interp, "exit");
	    } else {
		Tk_DeleteFileHandler(0);
	    }
	    return;
	} else {
	    count = 0;
	}
    }
    cmd = Tcl_DStringAppend(&command, input, count);
    if (count != 0) {
	if ((input[count-1] != '\n') && (input[count-1] != ';')) {
	    gotPartial = 1;
	    goto prompt;
	}
	if (!Tcl_CommandComplete(cmd)) {
	    gotPartial = 1;
	    goto prompt;
	}
    }
    gotPartial = 0;

    /*
     * Disable the stdin file handler while evaluating the command;
     * otherwise if the command re-enters the event loop we might
     * process commands from stdin before the current command is
     * finished.  Among other things, this will trash the text of the
     * command being evaluated.
     */

    Tk_CreateFileHandler(0, 0, StdinProc, (ClientData) 0);
    code = Tcl_RecordAndEval(interp, cmd, 0);
    Tk_CreateFileHandler(0, TK_READABLE, StdinProc, (ClientData) 0);
    Tcl_DStringFree(&command);
    if (*interp->result != 0) {
	if ((code != TCL_OK) || (tty)) {
	    printf("%s\n", interp->result);
	}
    }

    /*
     * Output a prompt.
     */

    prompt:
    if (tty) {
	Prompt(interp, gotPartial);
    }
}

/*
 *----------------------------------------------------------------------
 *
 * Prompt --
 *
 *	Issue a prompt on standard output, or invoke a script
 *	to issue the prompt.
 *
 * Results:
 *	None.
 *
 * Side effects:
 *	A prompt gets output, and a Tcl script may be evaluated
 *	in interp.
 *
 *----------------------------------------------------------------------
 */

static void
Prompt(interp, partial)
    Tcl_Interp *interp;			/* Interpreter to use for prompting. */
    int partial;			/* Non-zero means there already
					 * exists a partial command, so use
					 * the secondary prompt. */
{
    char *promptCmd;
    int code;

    promptCmd = Tcl_GetVar(interp,
	partial ? "tcl_prompt2" : "tcl_prompt1", TCL_GLOBAL_ONLY);
    if (promptCmd == NULL) {
	defaultPrompt:
	if (!partial) {
	    fputs("% ", stdout);
	}
    } else {
	code = Tcl_Eval(interp, promptCmd);
	if (code != TCL_OK) {
	    Tcl_AddErrorInfo(interp,
		    "\n    (script that generates prompt)");
	    fprintf(stderr, "%s\n", interp->result);
	    goto defaultPrompt;
	}
    }
    fflush(stdout);
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

/* Safety check for out of control code */

    if (pass_thru)
	return;

    pass_thru = 1;

/* Print error message if one given */

    if (*errmsg != '\0')
	fprintf(stderr, "%s\n", errmsg);

/* If client exists, tell it to self destruct */

    if (client_name != NULL)
	client_cmd("abort");

    Tcl_Eval(interp, "exit");
}

/*----------------------------------------------------------------------*\
* configure_plserver
*
* Does global variable & command initialization for interpreter.
\*----------------------------------------------------------------------*/

static void
configure_plserver(void)
{
#ifdef DEBUG
    char *path;
#endif

    dbug_enter("configure_plserver");

/* Standard PLplot/Tk extensions. */

    Pltk_Init( interp );

/* Default initialization proc */

    Tcl_SetVar(interp, "plserver_init_proc", "plserver_init", 0);

/* Now variable information. */
/* For uninitialized variables it's better to skip the Tcl_SetVar. */

/* Pass child variable to interpreter if set. */

    if (child != 0)
	Tcl_SetVar(interp, "child", "1", 0);

/* If client_name is set, TK send is being used to communicate. */
/* If client_port is set, Tcl-DP RPC is being used to communicate. */
/* In the latter case, need to create server communications port */

    if (client_name != NULL) {
	Tcl_SetVar(interp, "client_name", client_name, 0);
	dp = 0; tcl_cmd("set dp 0");
    }
    else if (client_port != NULL) {
#ifdef TCL_DP
	Tcl_SetVar(interp, "client_port", client_port, 0);
	if (client_host != NULL)
	    Tcl_SetVar(interp, "client_host", client_host, 0);
	dp = 1; tcl_cmd("set dp 1");
#else
	abort_session("no Tcl-DP support in this version of plserver");
	exit(1);
#endif
    }

/* Add user-specified directory(s) to auto_path */

    if (auto_path != NULL) {
	Tcl_SetVar(interp, "dir", auto_path, 0);
	tcl_cmd("set auto_path \"$dir $auto_path\"");

#ifdef DEBUG
	fprintf(stderr, "adding %s to auto_path\n", auto_path);
	path = Tcl_GetVar(interp, "auto_path", 0);
	fprintf(stderr, "auto_path is %s\n", path);
#endif
    }
}

/*----------------------------------------------------------------------*\
* client_cmd
*
* Sends specified command to client, aborting on an error.
* Always either send command in background or continue to process events,
* because client may be busy and unable to respond.
\*----------------------------------------------------------------------*/

static void
client_cmd(char *cmd)
{
    int result;
    static char dpsend_cmd[] = "dp_RPC $client -events all ";
    static char tksend_cmd[] = "send $client after 1 ";

    dbug_enter("client_cmd");
#ifdef DEBUG_ENTER
    fprintf(stderr, "Sending command: %s\n", cmd);
#endif

    copybuf(cmd);
    if (dp)
	result = Tcl_VarEval(interp, dpsend_cmd, cmdbuf, (char **) NULL);
    else
	result = Tcl_VarEval(interp, tksend_cmd, cmdbuf, (char **) NULL);

    if (result) {
	fprintf(stderr, "Client command \"%s\" failed:\n\t %s\n",
		cmd, interp->result);
	abort_session("");
    }
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
	Tcl_Eval(interp, "exit");
    }
}

/*----------------------------------------------------------------------*\
* copybuf
*
* Puts command in a static string buffer, to ensure it's in writable
* memory (grrr...).
\*----------------------------------------------------------------------*/

static void
copybuf(char *cmd)
{
    if (cmdbuf == NULL) {
	cmdbuf_len = 100;
	cmdbuf = (char *) malloc(cmdbuf_len);
    }

    if (strlen(cmd) >= cmdbuf_len) {
	free((void *) cmdbuf);
	cmdbuf_len = strlen(cmd) + 20;
	cmdbuf = (char *) malloc(cmdbuf_len);
    }

    strcpy(cmdbuf, cmd);
}

/*----------------------------------------------------------------------*\
* tcl_eval
*
* Evals the specified string, returning the result.
* Use a static string buffer to hold the command, to ensure it's in
* writable memory (grrr...).
\*----------------------------------------------------------------------*/

static int
tcl_eval(char *cmd)
{
    copybuf(cmd);
    return(Tcl_VarEval(interp, cmdbuf, (char **) NULL));
}
