/* $Id$
 * $Log$
 * Revision 1.9  1995/01/27 03:56:24  mjl
 * Added "loopback" command: gobbles the following word, which must be "cmd",
 * and processes the remaining arguments as any other plplot/Tcl command.
 * This allows scripts involving widget commands -- of the form <widget> cmd
 * <plplot command> -- to be processed by pltcl, with <widget> replaced
 * by "loopback".
 *
 * Revision 1.8  1994/10/10  19:45:07  furnish
 * Imlemented plshade from Tcl.
 *
 * Revision 1.7  1994/10/10  17:25:30  furnish
 * Add knowledge of contouring.
 *
 * Revision 1.6  1994/09/23  07:53:14  mjl
 * New Tcl API functions added: plend, plend1, plsstrm, and plinit.
 *
 * Revision 1.5  1994/07/19  22:33:05  mjl
 * Internal header file inclusion changed to /not/ use a search path so that
 * it will work better with makedepend.
 *
 * Revision 1.4  1994/06/30  18:52:09  mjl
 * Added API calls for: plfont, plfontld, plhist, pljoin, plmtex, plptex,
 * plschr, plssub, plsym, plvpor, plwid.
 *
 * Revision 1.3  1994/06/25  20:37:12  mjl
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

#include "plplotP.h"
#include "pltcl.h"

/* PLplot/Tcl API handlers.  Prototypes must come before Cmds struct */

static int loopbackCmd	(ClientData, Tcl_Interp *, int, char **);

static int pladvCmd	(ClientData, Tcl_Interp *, int, char **);
static int plbopCmd	(ClientData, Tcl_Interp *, int, char **);
static int plboxCmd	(ClientData, Tcl_Interp *, int, char **);
static int plcol0Cmd	(ClientData, Tcl_Interp *, int, char **);
static int plcontCmd	(ClientData, Tcl_Interp *, int, char **);
static int pleopCmd	(ClientData, Tcl_Interp *, int, char **);
static int plendCmd	(ClientData, Tcl_Interp *, int, char **);
static int plend1Cmd	(ClientData, Tcl_Interp *, int, char **);
static int plenvCmd	(ClientData, Tcl_Interp *, int, char **);
static int plfontCmd	(ClientData, Tcl_Interp *, int, char **);
static int plfontldCmd	(ClientData, Tcl_Interp *, int, char **);
static int plgraCmd	(ClientData, Tcl_Interp *, int, char **);
static int plhistCmd	(ClientData, Tcl_Interp *, int, char **);
static int plinitCmd	(ClientData, Tcl_Interp *, int, char **);
static int pljoinCmd	(ClientData, Tcl_Interp *, int, char **);
static int pllabCmd	(ClientData, Tcl_Interp *, int, char **);
static int pllineCmd	(ClientData, Tcl_Interp *, int, char **);
static int plmtexCmd	(ClientData, Tcl_Interp *, int, char **);
static int plpoinCmd	(ClientData, Tcl_Interp *, int, char **);
static int plptexCmd	(ClientData, Tcl_Interp *, int, char **);
static int plschrCmd	(ClientData, Tcl_Interp *, int, char **);
static int plsetoptCmd	(ClientData, Tcl_Interp *, int, char **);
static int plshadeCmd	(ClientData, Tcl_Interp *, int, char **);
static int plssubCmd	(ClientData, Tcl_Interp *, int, char **);
static int plsstrmCmd	(ClientData, Tcl_Interp *, int, char **);
static int plsymCmd	(ClientData, Tcl_Interp *, int, char **);
static int plstylCmd	(ClientData, Tcl_Interp *, int, char **);
static int plsxaxCmd	(ClientData, Tcl_Interp *, int, char **);
static int plsyaxCmd	(ClientData, Tcl_Interp *, int, char **);
static int pltextCmd	(ClientData, Tcl_Interp *, int, char **);
static int plvporCmd	(ClientData, Tcl_Interp *, int, char **);
static int plvstaCmd	(ClientData, Tcl_Interp *, int, char **);
static int plwidCmd	(ClientData, Tcl_Interp *, int, char **);
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
    {"loopback",	loopbackCmd},
    {"pladv",		pladvCmd},
    {"plbop",		plbopCmd},
    {"plbox",		plboxCmd},
    {"plcol",		plcol0Cmd},
    {"plcol0",		plcol0Cmd},
    {"plcont",		plcontCmd},
    {"pleop",		pleopCmd},
    {"plend",		plendCmd},
    {"plend1",		plend1Cmd},
    {"plenv",		plenvCmd},
    {"plfont",		plfontCmd},
    {"plfontld",	plfontldCmd},
    {"plgra",		plgraCmd},
    {"plhist",		plhistCmd},
    {"plinit",		plinitCmd},
    {"pljoin",		pljoinCmd},
    {"pllab",		pllabCmd},
    {"plline",		pllineCmd},
    {"plmtex",		plmtexCmd},
    {"plpoin",		plpoinCmd},
    {"plptex",		plptexCmd},
    {"plschr",		plschrCmd},
    {"plshade",		plshadeCmd},
    {"plssub",		plssubCmd},
    {"plsstrm",		plsstrmCmd},
    {"plstyl",		plstylCmd},
    {"plsxax",		plsxaxCmd},
    {"plsyax",		plsyaxCmd},
    {"plsetopt",	plsetoptCmd},
    {"plsym",		plsymCmd},
    {"pltext",		pltextCmd},
    {"plvpor",		plvporCmd},
    {"plvsta",		plvstaCmd},
    {"plwid",		plwidCmd},
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
 * Front-end to PLplot/Tcl API for use from Tcl commands (e.g. plframe).
 *
 * This command is called by the plframe widget to process subcommands
 * of the "cmd" plframe widget command.  This is the plframe's direct
 * plotting interface to the PLplot library.  This routine can be called
 * from other commands that want a similar capability.
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
 * loopbackCmd
 *
 * Loop-back command for Tcl interpreter.  Main purpose is to enable a
 * compatible command syntax whether you are executing directly through a
 * Tcl interpreter or a plframe widget.  I.e. the syntax is:
 *
 *	<widget> cmd <plplot command>		(widget command)
 *	loopback cmd <plplot command>		(pltcl command)
 *
 * This routine is essentially the same as plTclCmd but without some of
 * the window dressing required by the plframe widget.
\*----------------------------------------------------------------------*/

static int
loopbackCmd(ClientData clientData, Tcl_Interp *interp,
	    int argc, char **argv)
{
    register Tcl_HashEntry *hPtr;
    int result = TCL_OK;

    argc--; argv++;
    if (argc == 0 || (strcmp(argv[0], "cmd") != 0)) {
	Tcl_AppendResult(interp, "bad option to \"loopback\": must be ",
			 "\"cmd ?options?\" ", (char *) NULL);
	return TCL_ERROR;
    }

/* Create hash table on first call */

    if ( ! cmdTable_initted) {
	cmdTable_initted = 1;
	plTclCmd_Init(interp);
    }

/* no option -- return list of available PLPlot commands */

    argc--; argv++;
    if (argc == 0) {
	Append_Cmdlist(interp);
	return TCL_OK;
    }

/* Pick out the desired command */

    hPtr = Tcl_FindHashEntry(&cmdTable, argv[0]);
    if (hPtr == NULL) {
	Tcl_AppendResult(interp,
			 "bad option to \"loopback cmd\": must be one of ",
			 (char *) NULL);
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

    return TCL_OK;
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

/* TEMPLATE */

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
			 argv[0], " \"",
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

    return TCL_OK;
}

/*----------------------------------------------------------------------*\
 * plcontCmd
 *
 * Processes plcont Tcl command.
\*----------------------------------------------------------------------*/

PLFLT tclMatrix_feval (PLINT i, PLINT j, PLPointer p)
{
    tclMatrix *matPtr = (tclMatrix *) p;

    return matPtr->fdata[I2D(i,j)];
}

static int
plcontCmd(ClientData clientData, Tcl_Interp *interp,
	 int argc, char **argv)
{
    char *mat;
    tclMatrix *matPtr, *pclev;

    if (argc != 3 ) {
	Tcl_AppendResult(interp, "wrong # args: should be \"",
			 argv[0], " data clev\"",
			 (char *) NULL);
	return TCL_ERROR;
    }

    mat = Tcl_GetVar( interp, argv[1], 0 );
    matPtr = Tcl_GetMatrixPtr( interp, mat );

    printf( "matPtr = %x\n", matPtr );

    mat = Tcl_GetVar( interp, argv[2], 0 );
    pclev = Tcl_GetMatrixPtr( interp, mat );

    if (matPtr->dim != 2) {
	interp->result = "Must use 2-d data.";
	return TCL_ERROR;
    }

    if (pclev->dim != 1) {
	interp->result = "clev must be 1-d matrix.";
	return TCL_ERROR;
    }

    /* contour the data.*/

    plcontf( tclMatrix_feval, matPtr,
	     matPtr->n[0], matPtr->n[1],
	     1, matPtr->n[0], 1, matPtr->n[1],
	     pclev->fdata, pclev->n[0], pltr0, NULL );

    plflush();
    return TCL_OK;
}

/*----------------------------------------------------------------------*\
 * plendCmd
 *
 * Processes plend Tcl command.
\*----------------------------------------------------------------------*/

static int
plendCmd(ClientData clientData, Tcl_Interp *interp,
	 int argc, char **argv)
{
    if (argc != 1 ) {
	Tcl_AppendResult(interp, "wrong # args: should be \"", argv[0],
			 "\"", (char *) NULL);
	return TCL_ERROR;
    }

    plend();

    return TCL_OK;
}

/*----------------------------------------------------------------------*\
 * plend1Cmd
 *
 * Processes plend1 Tcl command.
\*----------------------------------------------------------------------*/

static int
plend1Cmd(ClientData clientData, Tcl_Interp *interp,
	  int argc, char **argv)
{
    if (argc != 1 ) {
	Tcl_AppendResult(interp, "wrong # args: should be \"", argv[0],
			 "\"", (char *) NULL);
	return TCL_ERROR;
    }

    plend1();

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
 * plfontCmd
 *
 * Processes plfont Tcl command.
\*----------------------------------------------------------------------*/

static int
plfontCmd(ClientData clientData, Tcl_Interp *interp,
	  int argc, char **argv)
{
    PLINT font;

    if (argc != 2 ) {
	Tcl_AppendResult(interp, "wrong # args: should be \"",
			 argv[0], " font\"",
			 (char *) NULL);
	return TCL_ERROR;
    }

    font = atoi(argv[1]);

    plfont(font);

    return TCL_OK;
}

/*----------------------------------------------------------------------*\
 * plfontldCmd
 *
 * Processes plfontld Tcl command.
\*----------------------------------------------------------------------*/

static int
plfontldCmd(ClientData clientData, Tcl_Interp *interp,
	    int argc, char **argv)
{
    PLINT font;

    if (argc != 2 ) {
	Tcl_AppendResult(interp, "wrong # args: should be \"",
			 argv[0], " font\"",
			 (char *) NULL);
	return TCL_ERROR;
    }

    font = atoi(argv[1]);

    plfontld(font);

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
    if (argc > 1 ) {
	Tcl_AppendResult(interp, "wrong # args: should be \"",
			 argv[0], " \"",
			 (char *) NULL);
	return TCL_ERROR;
    }

    plgra();

    return TCL_OK;
}

/*----------------------------------------------------------------------*\
 * plhistCmd
 *
 * Processes plhist Tcl command.
\*----------------------------------------------------------------------*/

static int
plhistCmd(ClientData clientData, Tcl_Interp *interp,
	  int argc, char **argv)
{
    PLINT n, nbin, oldwin;
    PLFLT *data, datmin, datmax;
    tclMatrix *mat;

    if (argc != 7 ) {
	Tcl_AppendResult(interp, "wrong # args: should be \"",
			 argv[0], " n data datmin datmax nbin oldwin\"",
			 (char *) NULL);
	return TCL_ERROR;
    }

    n = atoi(argv[1]);
    mat = Tcl_GetMatrixPtr(interp, argv[2]);
    data = mat->fdata;
    datmin = atof( argv[3] );
    datmax = atof( argv[4] );
    nbin = atoi( argv[5] );
    oldwin = atoi( argv[6] );

    plhist(n, data, datmin, datmax, nbin, oldwin);

    plflush();
    return TCL_OK;
}

/*----------------------------------------------------------------------*\
 * plinitCmd
 *
 * Processes plinit Tcl command.
\*----------------------------------------------------------------------*/

static int
plinitCmd(ClientData clientData, Tcl_Interp *interp,
	  int argc, char **argv)
{
    PLINT n, nbin, oldwin;
    PLFLT *data, datmin, datmax;
    tclMatrix *mat;

    if (argc != 1 ) {
	Tcl_AppendResult(interp, "wrong # args: should be \"",
			 argv[0], "\"", (char *) NULL);
	return TCL_ERROR;
    }

    plinit();

    return TCL_OK;
}

/*----------------------------------------------------------------------*\
 * pljoinCmd
 *
 * Processes pljoin Tcl command.
\*----------------------------------------------------------------------*/

static int
pljoinCmd(ClientData clientData, Tcl_Interp *interp,
	  int argc, char **argv)
{
    PLFLT x1, y1, x2, y2;

    if (argc != 5 ) {
	Tcl_AppendResult(interp, "wrong # args: should be \"",
			 argv[0], " x1 y1 x2 y2\"",
			 (char *) NULL);
	return TCL_ERROR;
    }

    x1 = atof( argv[1] );
    y1 = atof( argv[2] );
    x2 = atof( argv[3] );
    y2 = atof( argv[4] );

    pljoin(x1, y1, x2, y2);

    plflush();
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
			 argv[0], " xlabel ylabel tlabel\"",
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
    int n;

    if (argc != 4 ) {
	Tcl_AppendResult(interp, "wrong # args: should be \"", argv[0],
			 " n x-array y-array\"",
			 (char *) NULL);
	return TCL_ERROR;
    }

    matx = Tcl_GetMatrixPtr(interp, argv[2]);
    x = matx->fdata;
    maty = Tcl_GetMatrixPtr(interp, argv[3]);
    y = maty->fdata;

    if (strncmp(argv[1], "*", 1) == 0)
	n = MIN(matx->len, maty->len);
    else
	n = atoi(argv[1]);

    plline( n, x, y );

    plflush();
    return TCL_OK;
}

/*----------------------------------------------------------------------*\
 * plmtexCmd
 *
 * Processes plmtex Tcl command.
\*----------------------------------------------------------------------*/

static int
plmtexCmd(ClientData clientData, Tcl_Interp *interp,
	  int argc, char **argv)
{
    char *side, *text;
    PLFLT disp, pos, just;

    if (argc != 6 ) {
	Tcl_AppendResult(interp, "wrong # args: should be \"",
			 argv[0], " side disp pos just text\"",
			 (char *) NULL);
	return TCL_ERROR;
    }

    side = argv[1];
    disp = atof(argv[2]);
    pos = atof(argv[3]);
    just = atof(argv[4]);
    text = argv[5];

    plmtex(side, disp, pos, just, text);

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
    int n, code;

    if (argc != 5 ) {
	Tcl_AppendResult(interp, "wrong # args: should be \"", argv[0],
			 " n x-array y-array code\"",
			 (char *) NULL);
	return TCL_ERROR;
    }

    matx = Tcl_GetMatrixPtr(interp, argv[2]);
    x = matx->fdata;
    maty = Tcl_GetMatrixPtr(interp, argv[3]);
    y = maty->fdata;
    code = atoi( argv[4] );

    if (strncmp(argv[1], "*", 1) == 0)
	n = MIN(matx->len, maty->len);
    else
	n = atoi(argv[1]);

    plpoin( n, x, y, code );

    plflush();
    return TCL_OK;
}

/*----------------------------------------------------------------------*\
 * plptexCmd
 *
 * Processes plptex Tcl command.
\*----------------------------------------------------------------------*/

static int
plptexCmd(ClientData clientData, Tcl_Interp *interp,
	  int argc, char **argv)
{
    PLFLT x, y, dx, dy, just;
    char *text;

    if (argc != 7 ) {
	Tcl_AppendResult(interp, "wrong # args: should be \"",
			 argv[0], " x y dx dy just text\"",
			 (char *) NULL);
	return TCL_ERROR;
    }

    x = atof(argv[1]);
    y = atof(argv[2]);
    dx = atof(argv[3]);
    dy = atof(argv[4]);
    just = atof(argv[5]);
    text = argv[6];

    plptex(x, y, dx, dy, just, text);

    plflush();
    return TCL_OK;
}

/*----------------------------------------------------------------------*\
 * plschrCmd
 *
 * Processes plschr Tcl command.
\*----------------------------------------------------------------------*/

static int
plschrCmd(ClientData clientData, Tcl_Interp *interp,
	  int argc, char **argv)
{
    PLFLT def, scale;

    if (argc != 3 ) {
	Tcl_AppendResult(interp, "wrong # args: should be \"",
			 argv[0], " def scale\"",
			 (char *) NULL);
	return TCL_ERROR;
    }

    def = atof(argv[1]);
    scale = atof(argv[2]);

    plschr(def, scale);

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

    plSetInternalOpt(argv[1], argv[2]);

    plflush();
    return TCL_OK;
}

/*----------------------------------------------------------------------*\
 * plshadeCmd
 *
 * Processes plshade Tcl command.
 * C version takes:
 *    data, nx, ny, defined,
 *    xmin, xmax, ymin, ymax,
 *    sh_min, sh_max, sh_cmap, sh_color, sh_width,
 *    min_col, min_wid, max_col, max_wid,
 *    plfill, rect, pltr, pltr_data
 *
 * We will be getting data through a 2-d Matrix, which carries along
 * nx and ny, so no need for those.  Toss defined.  Toss plfill since
 * it is the only valid choice anyway, and assume rect==1 and no
 * transformation, since I don't know how to do anything else anyway.
\*----------------------------------------------------------------------*/

static int
plshadeCmd(ClientData clientData, Tcl_Interp *interp,
	 int argc, char **argv)
{
    int result = TCL_OK;
    char *mat;
    tclMatrix *matPtr;

    PLFLT xmin, xmax, ymin, ymax, sh_min, sh_max, sh_col;

    PLFLT **z;
    PLINT sh_cmap =1, sh_wid =2;
    PLINT min_col =1, min_wid =0, max_col =0, max_wid =0;
    PLINT rect =1;
    int i, j;

    if (argc < 9 ) {
	Tcl_AppendResult(interp, "wrong # args: should be \"",
			 argv[0], " \"",
			 (char *) NULL);
	return TCL_ERROR;
    }

    mat = Tcl_GetVar( interp, argv[1], 0 );
    matPtr = Tcl_GetMatrixPtr( interp, mat );

    if (matPtr->dim != 2) {
	interp->result = "Must plot a 2-d matrix.";
	return TCL_ERROR;
    }

    plAlloc2dGrid( &z, matPtr->n[0], matPtr->n[1] );
    
    for( i=0; i < matPtr->n[0]; i++ )
	for( j=0; j < matPtr->n[1]; j++ )
	    z[i][j] = matPtr->fdata[I2D(i,j)];

    xmin = atof( argv[2] );
    xmax = atof( argv[3] );
    ymin = atof( argv[4] );
    ymax = atof( argv[5] );
    sh_min = atof( argv[6] );
    sh_max = atof( argv[7] );
    sh_col = atof( argv[8] );

    for( i=9; i < argc; i++ ) {
	/* process argv[i] for options like -min_col 4, etc. */
	/* Jump to end if error, so z is freed. */
    }

    plshade( z, matPtr->n[0], matPtr->n[1], NULL,
	     xmin, xmax, ymin, ymax,
	     sh_min, sh_max, sh_cmap, sh_col, sh_wid,
	     min_col, min_wid, max_col, max_wid,
	     plfill, rect, NULL, NULL );

    plflush();

  end:
    free( (void *) z );
    return result;
}

/*----------------------------------------------------------------------*\
 * plssubCmd
 *
 * Processes plssub Tcl command.
\*----------------------------------------------------------------------*/

static int
plssubCmd(ClientData clientData, Tcl_Interp *interp,
	  int argc, char **argv)
{
    PLINT nsubx, nsuby;

    if (argc != 3 ) {
	Tcl_AppendResult(interp, "wrong # args: should be \"",
			 argv[0], " nsubx nsuby\"",
			 (char *) NULL);
	return TCL_ERROR;
    }

    nsubx = atoi(argv[1]);
    nsuby = atoi(argv[2]);

    plssub(nsubx, nsuby);

    plflush();
    return TCL_OK;
}

/*----------------------------------------------------------------------*\
 * plsstrmCmd
 *
 * Processes plsstrm Tcl command.
\*----------------------------------------------------------------------*/

static int
plsstrmCmd(ClientData clientData, Tcl_Interp *interp,
	   int argc, char **argv)
{
    PLINT strm;

    if (argc != 2 ) {
	Tcl_AppendResult(interp, "wrong # args: should be \"",
			 argv[0], " stream-number\"",
			 (char *) NULL);
	return TCL_ERROR;
    }

    strm = atoi(argv[1]);

    plsstrm(strm);

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
 * plsymCmd
 *
 * Processes plsym Tcl command.
\*----------------------------------------------------------------------*/

static int
plsymCmd(ClientData clientData, Tcl_Interp *interp,
	 int argc, char **argv)
{
    PLFLT *x, *y;
    tclMatrix *matx, *maty;
    int n, code;

    if (argc != 5 ) {
	Tcl_AppendResult(interp, "wrong # args: should be \"", argv[0],
			 " n x-array y-array code\"",
			 (char *) NULL);
	return TCL_ERROR;
    }

    matx = Tcl_GetMatrixPtr(interp, argv[2]);
    x = matx->fdata;
    maty = Tcl_GetMatrixPtr(interp, argv[3]);
    y = maty->fdata;
    code = atoi( argv[4] );

    if (strncmp(argv[1], "*", 1) == 0)
	n = MIN(matx->len, maty->len);
    else
	n = atoi(argv[1]);

    plsym( n, x, y, code );

    plflush();
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
    if (argc > 1 ) {
	Tcl_AppendResult(interp, "wrong # args: should be \"",
			 argv[0], " \"",
			 (char *) NULL);
	return TCL_ERROR;
    }

    pltext();

    return TCL_OK;
}

/*----------------------------------------------------------------------*\
 * plvporCmd
 *
 * Processes plvpor Tcl command.
\*----------------------------------------------------------------------*/

static int
plvporCmd(ClientData clientData, Tcl_Interp *interp,
	  int argc, char **argv)
{
    PLFLT xmin, xmax, ymin, ymax;

    if (argc != 5 ) {
	Tcl_AppendResult(interp, "wrong # args: should be \"",
			 argv[0], " xmin xmax ymin ymax\"",
			 (char *) NULL);
	return TCL_ERROR;
    }

    xmin = atof( argv[1] );
    xmax = atof( argv[2] );
    ymin = atof( argv[3] );
    ymax = atof( argv[4] );

    plvpor(xmin, xmax, ymin, ymax);

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
			 argv[0], " \"",
			 (char *) NULL);
	return TCL_ERROR;
    }

    plvsta();

    return TCL_OK;
}

/*----------------------------------------------------------------------*\
 * plwidCmd
 *
 * Processes plwid Tcl command.
\*----------------------------------------------------------------------*/

static int
plwidCmd(ClientData clientData, Tcl_Interp *interp,
	 int argc, char **argv)
{
    PLINT width;

    if (argc != 2 ) {
	Tcl_AppendResult(interp, "wrong # args: should be \"",
			 argv[0], " width\"",
			 (char *) NULL);
	return TCL_ERROR;
    }

    width = atoi(argv[1]);

    plwid(width);

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

