/* $Id$
 * $Log$
 * Revision 1.3  1994/06/25 20:37:12  mjl
 * Added API calls for pladv, plbop, plbox, pleop, plstyl, plvsta, plwind.
 *
 * Revision 1.2  1994/06/24  20:38:21  mjl
 * Changed name of struct to tclMatrix to avoid conflicts with C++ Matrix
 * classes.
 *
 * Revision 1.1  1994/06/23  22:45:50  mjl
 * Contains functions, a command table, a hash table, and other tools to
 * support calling PLplot functions via Tcl commands.  Support for calling
 * PLplot directly (from pltcl) or by widget commands (from plframe) is
 * provided.
 */

/*----------------------------------------------------------------------*\
 * 
 * pltclAPI.c --
 *
 *	This module implements a Tcl command set for interpretively
 *	calling PLplot functions.  Each Tcl command is responsible for
 *	calling the appropriate underlying function in the C API.  Can be
 *	used with any driver, in principle. 
 *
 * Maurice LeBrun
 * IFS, University of Texas at Austin
 * 19-Jun-1994
 *
\*----------------------------------------------------------------------*/

#include <pltcl.h>

/* PLplot/Tcl API handlers.  Prototypes must come before Cmds struct */

static int pladvCmd	(ClientData, Tcl_Interp *, int, char **);
static int plbopCmd	(ClientData, Tcl_Interp *, int, char **);
static int plboxCmd	(ClientData, Tcl_Interp *, int, char **);
static int plcol0Cmd	(ClientData, Tcl_Interp *, int, char **);
static int pleopCmd	(ClientData, Tcl_Interp *, int, char **);
static int plenvCmd	(ClientData, Tcl_Interp *, int, char **);
static int plgraCmd	(ClientData, Tcl_Interp *, int, char **);
static int pllabCmd	(ClientData, Tcl_Interp *, int, char **);
static int pllineCmd	(ClientData, Tcl_Interp *, int, char **);
static int plpoinCmd	(ClientData, Tcl_Interp *, int, char **);
static int plsetoptCmd	(ClientData, Tcl_Interp *, int, char **);
static int plstylCmd	(ClientData, Tcl_Interp *, int, char **);
static int plsxaxCmd	(ClientData, Tcl_Interp *, int, char **);
static int plsyaxCmd	(ClientData, Tcl_Interp *, int, char **);
static int pltextCmd	(ClientData, Tcl_Interp *, int, char **);
static int plvstaCmd	(ClientData, Tcl_Interp *, int, char **);
static int plwindCmd	(ClientData, Tcl_Interp *, int, char **);

/*
 * The following structure defines all of the commands in the PLplot/Tcl
 * core, and the C procedures that execute them.
 */

typedef struct Command {
    int (*proc)();		/* Procedure to process command. */
    ClientData clientData;	/* Arbitrary value to pass to proc. */
    int *deleteProc;		/* Procedure to invoke when deleting
				 * command. */
    ClientData deleteData;	/* Arbitrary value to pass to deleteProc
				 * (usually the same as clientData). */
} Command;

typedef struct {
    char *name;
    int (*proc)();
} CmdInfo;

/* Built-in commands, and the procedures associated with them */

static CmdInfo Cmds[] = {
    {"pladv",		pladvCmd},
    {"plbop",		plbopCmd},
    {"plbox",		plboxCmd},
    {"plcol",		plcol0Cmd},
    {"plcol0",		plcol0Cmd},
    {"pleop",		pleopCmd},
    {"plenv",		plenvCmd},
    {"plgra",		plgraCmd},
    {"pllab",		pllabCmd},
    {"plline",		pllineCmd},
    {"plpoin",		plpoinCmd},
    {"plstyl",		plstylCmd},
    {"plsxax",		plsxaxCmd},
    {"plsyax",		plsyaxCmd},
    {"plsetopt",	plsetoptCmd},
    {"pltext",		pltextCmd},
    {"plvsta",		plvstaCmd},
    {"plwind",		plwindCmd},
    {NULL,		NULL}
};

/* Hash table and associated flag for directing control */

static int cmdTable_initted;
static Tcl_HashTable cmdTable;

/*----------------------------------------------------------------------*\
 * Append_Cmdlist
 *
 * Generates command list from Cmds, storing into interp->result.
\*----------------------------------------------------------------------*/

static void
Append_Cmdlist(Tcl_Interp *interp)
{
    register CmdInfo *cmdInfoPtr;
    for (cmdInfoPtr = Cmds; cmdInfoPtr->name != NULL; cmdInfoPtr++) {
	Tcl_AppendResult(interp, cmdInfoPtr->name, (char *) NULL);
    }
}

/*----------------------------------------------------------------------*\
 * plTclCmd_Init
 *
 * Sets up command hash table for use with plframe to PLplot Tcl API.
 *
 * Right now all API calls are allowed, although some of these may not
 * make much sense when used with a widget.
\*----------------------------------------------------------------------*/

static void
plTclCmd_Init(Tcl_Interp *interp)
{
    register Command *cmdPtr;
    register CmdInfo *cmdInfoPtr;

    Tcl_InitHashTable(&cmdTable, TCL_STRING_KEYS);

/* Create the hash table entry for each command */

    for (cmdInfoPtr = Cmds; cmdInfoPtr->name != NULL; cmdInfoPtr++) {
	int new;
	Tcl_HashEntry *hPtr;

	hPtr = Tcl_CreateHashEntry(&cmdTable, cmdInfoPtr->name, &new);
	if (new) {
	    cmdPtr = (Command *) ckalloc(sizeof(Command));
	    cmdPtr->proc = cmdInfoPtr->proc;
	    cmdPtr->clientData = (ClientData) NULL;
	    cmdPtr->deleteProc = NULL;
	    cmdPtr->deleteData = (ClientData) NULL;
	    Tcl_SetHashValue(hPtr, cmdPtr);
	}
    }
}

/*----------------------------------------------------------------------*\
 * plTclCmd
 *
 * Front-end to PLplot/Tcl API for use from the plframe widget.
 *
 * In a widget-based application, a plplot "command" doesn't make much
 * sense by itself since it isn't connected to a specific widget.
 * Instead, you have widget commands.  This allows arbitrarily many
 * widgets and requires a slightly different syntax than if there were
 * only a single output device.  That is, the widget name (and in this
 * case, the "cmd" widget command, after that comes the subcommand)
 * must come first.  The plframe widget checks first for one of its
 * internal subcommands, those specifically designed for use with the
 * plframe widget.  If not found, control comes here. 
\*----------------------------------------------------------------------*/

int
plTclCmd(char *cmdlist, Tcl_Interp *interp, int argc, char **argv)
{
    register Tcl_HashEntry *hPtr;
    int result = TCL_OK;

/* Create hash table on first call */

    if ( ! cmdTable_initted) {
	cmdTable_initted = 1;
	plTclCmd_Init(interp);
    }

/* no option -- return list of available PLPlot commands */

    if (argc == 0) {
	Tcl_AppendResult(interp, cmdlist, (char *) NULL);
	Append_Cmdlist(interp);
	return TCL_OK;
    }

/* Pick out the desired command */

    hPtr = Tcl_FindHashEntry(&cmdTable, argv[0]);
    if (hPtr == NULL) {
	Tcl_AppendResult(interp, "bad option to \"cmd\": must be one of ",
			 cmdlist, (char *) NULL);
	Append_Cmdlist(interp);
	result = TCL_ERROR;
    }
    else {
	register Command *cmdPtr = (Command *) Tcl_GetHashValue(hPtr);
	result = (*cmdPtr->proc)(cmdPtr->clientData, interp, argc, argv);
    }

    return result;
}

/*----------------------------------------------------------------------*\
 * Pltcl_Init
 *
 * Initialization routine for extended wish'es.
 * Creates the matrix command, as well as numerous commands for
 * interfacing to PLplot.  Should not be used in a widget-based system.
\*----------------------------------------------------------------------*/

int
Pltcl_Init( Tcl_Interp *interp )
{
    register CmdInfo *cmdInfoPtr;

/* matrix -- matrix support command */

    Tcl_CreateCommand(interp, "matrix", Tcl_MatrixCmd,
                      (ClientData) NULL, (void (*)(ClientData)) NULL);

/* PLplot API commands */

    for (cmdInfoPtr = Cmds; cmdInfoPtr->name != NULL; cmdInfoPtr++) {

	Tcl_CreateCommand(interp, cmdInfoPtr->name, cmdInfoPtr->proc,
			  (ClientData) NULL, (void (*)(ClientData)) NULL);
    }
}

/*----------------------------------------------------------------------*\
 * PLplot API Calls
 *
 * Any call that results in something actually being plotted must be
 * followed by by a call to plflush(), to make sure all output from 
 * that command is finished.  Devices that have text/graphics screens
 * (e.g. Tek4xxx and emulators) implicitly switch to the graphics screen
 * before graphics commands, so a plgra() is not necessary in this case.
 * Although if you switch to the text screen via user control (instead of
 * using pltext()), the device will get confused.
\*----------------------------------------------------------------------*/

/* TEMPLATE FOR NEW CALLS */

/*----------------------------------------------------------------------*\
 * plxxxCmd
 *
 * Processes plxxx Tcl command.
\*----------------------------------------------------------------------*/

static int
plxxxCmd(ClientData clientData, Tcl_Interp *interp,
	 int argc, char **argv)
{
    if (argc != 0 ) {
	Tcl_AppendResult(interp, "wrong # args: should be \"",
			 argv[0], " \"",
			 (char *) NULL);
	return TCL_ERROR;
    }


    plflush();
    return TCL_OK;
}

/*----------------------------------------------------------------------*\
 * pladvCmd
 *
 * Processes pladv Tcl command.
\*----------------------------------------------------------------------*/

static int
pladvCmd(ClientData clientData, Tcl_Interp *interp,
	 int argc, char **argv)
{
    int sub = 0;

    if (argc > 2 ) {
	Tcl_AppendResult(interp, "wrong # args: should be \"",
			 argv[0], " ?subwindow?\"",
			 (char *) NULL);
	return TCL_ERROR;
    }

    if (argc == 2) 
	sub = atoi(argv[1]);

    pladv(sub);

    plflush();
    return TCL_OK;
}

/*----------------------------------------------------------------------*\
 * plbopCmd
 *
 * Processes plbop Tcl command.
\*----------------------------------------------------------------------*/

static int
plbopCmd(ClientData clientData, Tcl_Interp *interp,
	 int argc, char **argv)
{
    if (argc > 1 ) {
	Tcl_AppendResult(interp, "wrong # args: should be \"",
			 argv[0], "\"",
			 (char *) NULL);
	return TCL_ERROR;
    }

    plbop();

    plflush();
    return TCL_OK;
}

/*----------------------------------------------------------------------*\
 * plboxCmd
 *
 * Processes plbox Tcl command.
\*----------------------------------------------------------------------*/

static int
plboxCmd(ClientData clientData, Tcl_Interp *interp,
	 int argc, char **argv)
{
    char *xopt, *yopt;
    PLFLT xtick, ytick;
    PLINT nxsub, nysub;

    if (argc != 7 ) {
	Tcl_AppendResult(interp, "wrong # args: should be \"",
			 argv[0], " xopt xtick nxsub yopt ytick nysub\"",
			 (char *) NULL);
	return TCL_ERROR;
    }

    xopt = argv[1];
    xtick = atof(argv[2]);
    nxsub = atoi(argv[3]);

    yopt = argv[4];
    ytick = atof(argv[5]);
    nysub = atoi(argv[6]);

    plbox(xopt, xtick, nxsub, yopt, ytick, nysub);

    plflush();
    return TCL_OK;
}

/*----------------------------------------------------------------------*\
 * plcol0Cmd
 *
 * Processes plcol0 Tcl command.
\*----------------------------------------------------------------------*/

static int
plcol0Cmd(ClientData clientData, Tcl_Interp *interp,
	  int argc, char **argv)
{
    PLINT col;

    if (argc != 2) {
	Tcl_AppendResult(interp, "wrong # args: should be \"", argv[0],
			 " color-index\"",
			 (char *) NULL);
	return TCL_ERROR;
    }

    col = atoi( argv[1] );

    plcol0( col );

    plflush();
    return TCL_OK;
}

/*----------------------------------------------------------------------*\
 * plenvCmd
 *
 * Processes plenv Tcl command.
\*----------------------------------------------------------------------*/

static int
plenvCmd(ClientData clientData, Tcl_Interp *interp,
	 int argc, char **argv)
{
    PLFLT xmin, xmax, ymin, ymax;
    PLINT just, axis;

    if (argc != 7 ) {
	Tcl_AppendResult(interp, "wrong # args: should be \"", argv[0],
			 " xmin xmax ymin ymax just axis\"",
			 (char *) NULL);
	return TCL_ERROR;
    }

    xmin = atof( argv[1] );
    xmax = atof( argv[2] );
    ymin = atof( argv[3] );
    ymax = atof( argv[4] );
    just = atoi( argv[5] );
    axis = atoi( argv[6] );

    plenv( xmin, xmax, ymin, ymax, just, axis );

    plflush();
    return TCL_OK;
}

/*----------------------------------------------------------------------*\
 * pleopCmd
 *
 * Processes pleop Tcl command.
\*----------------------------------------------------------------------*/

static int
pleopCmd(ClientData clientData, Tcl_Interp *interp,
	 int argc, char **argv)
{
    if (argc > 1 ) {
	Tcl_AppendResult(interp, "wrong # args: should be \"",
			 argv[0], "\"",
			 (char *) NULL);
	return TCL_ERROR;
    }

    pleop();

    plflush();
    return TCL_OK;
}

/*----------------------------------------------------------------------*\
 * plgraCmd
 *
 * Processes plgra Tcl command.
\*----------------------------------------------------------------------*/

static int
plgraCmd(ClientData clientData, Tcl_Interp *interp,
	 int argc, char **argv)
{
    plgra();
    return TCL_OK;
}

/*----------------------------------------------------------------------*\
 * pllabCmd
 *
 * Processes pllab Tcl command.
\*----------------------------------------------------------------------*/

static int
pllabCmd(ClientData clientData, Tcl_Interp *interp,
	  int argc, char **argv)
{
    if (argc != 4 ) {
	Tcl_AppendResult(interp, "wrong # args: should be \"",
			 argv[0], "  xlabel ylabel tlabel\"",
			 (char *) NULL);
	return TCL_ERROR;
    }

    pllab( argv[1], argv[2], argv[3] );

    plflush();
    return TCL_OK;
}

/*----------------------------------------------------------------------*\
 * pllineCmd
 *
 * Processes plline Tcl command.
\*----------------------------------------------------------------------*/

static int
pllineCmd(ClientData clientData, Tcl_Interp *interp,
	   int argc, char **argv)
{
    PLFLT *x, *y;
    tclMatrix *matx, *maty;
    int npts;

    if (argc != 4 ) {
	Tcl_AppendResult(interp, "wrong # args: should be \"", argv[0],
			 " <npts> <x-array-name> <y-array-name>\"",
			 (char *) NULL);
	return TCL_ERROR;
    }

    matx = Tcl_GetMatrixPtr(interp, argv[2]);
    x = matx->fdata;
    maty = Tcl_GetMatrixPtr(interp, argv[3]);
    y = maty->fdata;

    if (strncmp(argv[1], "*", 1) == 0)
	npts = MIN(matx->len, maty->len);
    else
	npts = atoi(argv[1]);

    plline( npts, x, y );

    plflush();
    return TCL_OK;
}

/*----------------------------------------------------------------------*\
 * plpoinCmd
 *
 * Processes plpoin Tcl command.
\*----------------------------------------------------------------------*/

static int
plpoinCmd(ClientData clientData, Tcl_Interp *interp,
	   int argc, char **argv)
{
    PLFLT *x, *y;
    tclMatrix *matx, *maty;
    int npts, code;

    if (argc != 5 ) {
	Tcl_AppendResult(interp, "wrong # args: should be \"", argv[0],
			 " <npts> <x-array-name> <y-array-name> <code>\"",
			 (char *) NULL);
	return TCL_ERROR;
    }

    matx = Tcl_GetMatrixPtr(interp, argv[2]);
    x = matx->fdata;
    maty = Tcl_GetMatrixPtr(interp, argv[3]);
    y = maty->fdata;

    if (strncmp(argv[1], "*", 1) == 0)
	npts = MIN(matx->len, maty->len);
    else
	npts = atoi(argv[1]);

    code = atoi( argv[4] );

    plpoin( npts, x, y, code );

    plflush();
    return TCL_OK;
}

/*----------------------------------------------------------------------*\
 * plsetoptCmd
 *
 * Processes plsetopt Tcl command.
 * Just calls plSetInternalOpt() 
\*----------------------------------------------------------------------*/

static int
plsetoptCmd(ClientData clientData, Tcl_Interp *interp,
	     int argc, char **argv)
{
    if (argc < 2 || argc > 3) {
	Tcl_AppendResult(interp, "wrong # args: should be \"",
			 argv[0], " option ?argument?\"", (char *) NULL);
	return TCL_ERROR;
    }
    else {
#ifdef DEBUG
	fprintf(stderr, "Processing command: %s\n", argv[1]);
	if (argv[2] != NULL)
	    fprintf(stderr, "Processing option: %s\n", argv[2]);
#endif
	plSetInternalOpt(argv[1], argv[2]);
    }

    plflush();
    return TCL_OK;
}

/*----------------------------------------------------------------------*\
 * plstylCmd
 *
 * Processes plstyl Tcl command.
\*----------------------------------------------------------------------*/

static int
plstylCmd(ClientData clientData, Tcl_Interp *interp,
	 int argc, char **argv)
{
    PLINT *space, *mark, nels;
    tclMatrix *mat1, *mat2;

    if (argc != 4 ) {
	Tcl_AppendResult(interp, "wrong # args: should be \"",
			 argv[0], " nels mark-array space-array\"",
			 (char *) NULL);
	return TCL_ERROR;
    }

    nels = atoi(argv[1]);
    mat1 = Tcl_GetMatrixPtr(interp, argv[2]);
    space = mat1->idata;
    mat2 = Tcl_GetMatrixPtr(interp, argv[3]);
    mark = mat2->idata;

    plstyl(nels, mark, space);

    plflush();
    return TCL_OK;
}

/*----------------------------------------------------------------------*\
 * plsxaxCmd
 *
 * Processes plsxax Tcl command.
\*----------------------------------------------------------------------*/

static int
plsxaxCmd(ClientData clientData, Tcl_Interp *interp,
	  int argc, char **argv)
{
    PLINT digmax;

    if (argc < 1 ) {
	Tcl_AppendResult(interp, "wrong # args: should be \"",
			 argv[0], " digmax\"",
			 (char *) NULL);
	return TCL_ERROR;
    }

    digmax = atoi(argv[1]);
    plsxax(digmax, 0);
    return TCL_OK;
}

/*----------------------------------------------------------------------*\
 * plsyaxCmd
 *
 * Processes plsyax Tcl command.
\*----------------------------------------------------------------------*/

static int
plsyaxCmd(ClientData clientData, Tcl_Interp *interp,
	  int argc, char **argv)
{
    PLINT digmax;

    if (argc < 1 ) {
	Tcl_AppendResult(interp, "wrong # args: should be \"",
			 argv[0], " digmax\"",
			 (char *) NULL);
	return TCL_ERROR;
    }

    digmax = atoi(argv[1]);
    plsyax(digmax, 0);
    return TCL_OK;
}

/*----------------------------------------------------------------------*\
 * pltextCmd
 *
 * Processes pltext Tcl command.
\*----------------------------------------------------------------------*/

static int
pltextCmd(ClientData clientData, Tcl_Interp *interp,
	  int argc, char **argv)
{
    pltext();
    return TCL_OK;
}

/*----------------------------------------------------------------------*\
 * plvstaCmd
 *
 * Processes plvsta Tcl command.
\*----------------------------------------------------------------------*/

static int
plvstaCmd(ClientData clientData, Tcl_Interp *interp,
	  int argc, char **argv)
{
    if (argc > 1 ) {
	Tcl_AppendResult(interp, "wrong # args: should be \"",
			 argv[0], "\"",
			 (char *) NULL);
	return TCL_ERROR;
    }

    plvsta();

    plflush();
    return TCL_OK;
}

/*----------------------------------------------------------------------*\
 * plwindCmd
 *
 * Processes plwind Tcl command.
\*----------------------------------------------------------------------*/

static int
plwindCmd(ClientData clientData, Tcl_Interp *interp,
	  int argc, char **argv)
{
    PLFLT xmin, xmax, ymin, ymax;

    if (argc != 5 ) {
	Tcl_AppendResult(interp, "wrong # args: should be \"",
			 argv[0], " xmin xmax ymin ymax\"",
			 (char *) NULL);
	return TCL_ERROR;
    }

    xmin = atof(argv[1]);
    xmax = atof(argv[2]);
    ymin = atof(argv[3]);
    ymax = atof(argv[4]);

    plwind(xmin, xmax, ymin, ymax);

    plflush();
    return TCL_OK;
}

