/* $Id$
 * $Log$
 * Revision 1.10  1993/08/28 06:34:00  mjl
 * Put several send commands in background; should help over a slow network.
 *
 * Revision 1.9  1993/08/20  19:40:44  mjl
 * Now causes proc to update scrollbars to be evaluated when the driver
 * interface values are changed.  So settings zoom from the command line
 * using -wplt option will bring up scrollbars.
 *
 * Revision 1.8  1993/08/18  19:05:41  mjl
 * Changed way device window parameters are passed -- now via the command
 * line flags -mar, -a, -jx, and -jy.
 *
 * Revision 1.7  1993/08/11  19:26:30  mjl
 * Improved code that determines when command is sent to server to read from
 * the FIFO.  Changed to non-blocking i/o and asynchronous TK sends.
 *
 * Revision 1.6  1993/08/03  20:30:59  mjl
 * Fixed more problems with writing into non-writable strings by gcc.
 *
 * Revision 1.5  1993/08/03  01:49:55  mjl
 * Fixed some code that caused problems with some compilers due to strings
 * that might be allocated in non-writable memory being passed to Tcl_Eval
 * (which may modify the string).
 *
 * Revision 1.4  1993/07/31  08:00:57  mjl
 * Several driver functions consolidated, for all drivers.  The width and
 * color commands are now part of a more general "state" command.  The
 * text and graph commands used for switching between modes is now handled
 * by the escape function (very few drivers require it).  An escape
 * function call was added to handle driver interface commands -- these
 * are much better passed on to the remote plframe widget for handling
 * since then they are modifiable by the user.
 *
 * Revision 1.3  1993/07/28  05:46:30  mjl
 * Now explicitly initializes remote plplot by using the "cmd" widget
 * command.  Added support for -nopixmap option, and all malloc'ed
 * memory is eventually freed.
 *
 * Revision 1.2  1993/07/16  22:09:12  mjl
 * Made keypress handler a bit easier to invoke.  Eliminated low-level
 * aspect and orientation handling.  Some name changes.  Now uses standard
 * meta coordinate system.
 *
 * Revision 1.1  1993/07/02  06:58:37  mjl
 * The new TCL/TK driver!  Yes it's finally here!  YAAAAAAAAYYYYYYY!!!
*/

/*	tk.c
*
*	Maurice LeBrun
*	30-Apr-93
*
*	PLPLOT TCL/TK device driver.
*
*	Passes graphics commands to renderer and certain X
*	events back to user if requested.
*/

#ifdef TK

#include "plserver.h"
#include "drivers.h"
#include "metadefs.h"
#include "pdf.h"
#include "plevent.h"
#include "xwin.h"

#define BUFMAX 3500

#define tk_wr(code) \
if (code) { abort_session(pls, "Unable to write to pipe"); }

/* Use vfork() if the system supports it */

#ifndef FORK
#define FORK fork
#endif

/* INDENT OFF */
/*----------------------------------------------------------------------*/
/* Struct to hold device-specific info. */

typedef struct {
    Tk_Window w;		/* Main window */
    Tcl_Interp *interp;		/* Interpreter */

    FILE  *file;		/* fifo or socket file descriptor */
    char  *filename;		/* Name of fifo or socket */
    char  *filetype;		/* Set to "fifo" or "socket" */

    char  *program;		/* Name of client main window */

    short xold, yold;		/* Coordinates of last point plotted */
    int   exit_eventloop;	/* Flag for breaking out of event loop */
    int   pass_thru;		/* Skips normal error termination when set */
    int   launched_server;	/* Keep track of who started server */

    char *cmdbuf;		/* Command buffer */
    int cmdbuf_len;
} TkDev;

/* Function prototypes */

static void  tk_start		(PLStream *);
static void  tk_stop		(PLStream *);
static void  tk_di		(PLStream *);
static void  WaitForPage	(PLStream *);
static void  HandleEvents	(PLStream *);
static void  tk_configure	(PLStream *);
static void  launch_server	(PLStream *);
static void  flush_output	(PLStream *);
static void  plwindow_init	(PLStream *);
static void  link_init		(PLStream *);
static void  bgcolor_init	(PLStream *);

/* Tcl/TK utility commands */

static void  tk_wait		(PLStream *, char *);
static void  abort_session	(PLStream *, char *);
static void  server_cmd		(PLStream *, char *);
static void  tcl_cmd		(PLStream *, char *);
static int   tcl_eval		(PLStream *, char *);
static void  copybuf		(PLStream *pls, char *cmd);

/* These are internal TCL commands */

static int   Abort		(ClientData, Tcl_Interp *, int, char **);
static int   KeyEH		(ClientData, Tcl_Interp *, int, char **);

/* INDENT ON */
/*----------------------------------------------------------------------*\
* plD_init_tk()
*
* Initialize device.
* TK-dependent stuff done in tk_start().  You can set the display by
* calling plsfnam() with the display name as the (string) argument.
\*----------------------------------------------------------------------*/

void
plD_init_tk(PLStream *pls)
{
    U_CHAR c = (U_CHAR) INITIALIZE;
    TkDev *dev;
    int xmin = 0;
    int xmax = PIXELS_X - 1;
    int ymin = 0;
    int ymax = PIXELS_Y - 1;

    float pxlx = (double) PIXELS_X / (double) LPAGE_X;
    float pxly = (double) PIXELS_Y / (double) LPAGE_Y;

    dbug_enter("plD_init_tk");

    pls->termin = 1;		/* is an interactive terminal */
    pls->icol0 = 1;
    pls->width = 1;
    pls->bytecnt = 0;
    pls->page = 0;
    pls->dev_di = 1;

    if ( ! pls->bgcolorset) 
	bgcolor_init(pls);

    if (pls->bufmax == 0)
	pls->bufmax = BUFMAX;

/* Allocate and initialize device-specific data */

    if (pls->dev != NULL)
	free((void *) pls->dev);

    pls->dev = calloc(1, (size_t) sizeof(TkDev));
    if (pls->dev == NULL)
	plexit("plD_init_tk: Out of memory.");

    dev = (TkDev *) pls->dev;

/* Start interpreter and spawn server process */

    tk_start(pls);

/* Get ready for plotting */

    dev->xold = UNDEFINED;
    dev->yold = UNDEFINED;

    plP_setpxl(pxlx, pxly);
    plP_setphy(xmin, xmax, ymin, ymax);

/* Send init info */

    tk_wr(pdf_wr_1byte(dev->file, c));
    pls->bytecnt++;

/* The header and version fields will be useful when the client & server */
/* reside on different machines */

    tk_wr(pdf_wr_header(dev->file, PLSERV_HEADER));
    tk_wr(pdf_wr_header(dev->file, PLSERV_VERSION));

    tk_wr(pdf_wr_header(dev->file, "xmin"));
    tk_wr(pdf_wr_2bytes(dev->file, (U_SHORT) xmin));

    tk_wr(pdf_wr_header(dev->file, "xmax"));
    tk_wr(pdf_wr_2bytes(dev->file, (U_SHORT) xmax));

    tk_wr(pdf_wr_header(dev->file, "ymin"));
    tk_wr(pdf_wr_2bytes(dev->file, (U_SHORT) ymin));

    tk_wr(pdf_wr_header(dev->file, "ymax"));
    tk_wr(pdf_wr_2bytes(dev->file, (U_SHORT) ymax));

    tk_wr(pdf_wr_header(dev->file, ""));

/* Flush pipe since number of bytes not accounted for */

    flush_output(pls);
}

/*----------------------------------------------------------------------*\
* plD_line_tk()
*
* Draw a line in the current color from (x1,y1) to (x2,y2).
\*----------------------------------------------------------------------*/

void
plD_line_tk(PLStream *pls, short x1, short y1, short x2, short y2)
{
    U_CHAR c;
    U_SHORT xy[4];
    static long count = 0, max_count = 100;
    TkDev *dev = (TkDev *) pls->dev;

    if ( ++count/max_count >= 1 ) {
	count = 0;
	HandleEvents(pls);	/* Check for events */
    }

    if (x1 == dev->xold && y1 == dev->yold) {
	c = (U_CHAR) LINETO;
	tk_wr(pdf_wr_1byte(dev->file, c));

	xy[0] = x2;
	xy[1] = y2;
	tk_wr(pdf_wr_2nbytes(dev->file, xy, 2));

	pls->bytecnt += 5;
    }
    else {
	c = (U_CHAR) LINE;
	tk_wr(pdf_wr_1byte(dev->file, c));

	xy[0] = x1;
	xy[1] = y1;
	xy[2] = x2;
	xy[3] = y2;
	tk_wr(pdf_wr_2nbytes(dev->file, xy, 4));

	pls->bytecnt += 9;
    }
    dev->xold = x2;
    dev->yold = y2;

    if (pls->bytecnt > pls->bufmax)
	flush_output(pls);
}

/*----------------------------------------------------------------------*\
* plD_polyline_tk()
*
* Draw a polyline in the current color from (x1,y1) to (x2,y2).
\*----------------------------------------------------------------------*/

void
plD_polyline_tk(PLStream *pls, short *xa, short *ya, PLINT npts)
{
    U_CHAR c = (U_CHAR) POLYLINE;
    static long count = 0, max_count = 100;
    TkDev *dev = (TkDev *) pls->dev;

    if ( ++count/max_count >= 1 ) {
	count = 0;
	HandleEvents(pls);	/* Check for events */
    }

    tk_wr(pdf_wr_1byte(dev->file, c));
    tk_wr(pdf_wr_2bytes(dev->file, (U_SHORT) npts));
    tk_wr(pdf_wr_2nbytes(dev->file, (U_SHORT *) xa, npts));
    tk_wr(pdf_wr_2nbytes(dev->file, (U_SHORT *) ya, npts));
    pls->bytecnt += (3 + 4 * npts);

    dev->xold = xa[npts - 1];
    dev->yold = ya[npts - 1];

    if (pls->bytecnt > pls->bufmax)
	flush_output(pls);
}

/*----------------------------------------------------------------------*\
* plD_eop_tk()
*
* End of page.  
* User must hit <RETURN> or click right mouse button to continue.
\*----------------------------------------------------------------------*/

void
plD_eop_tk(PLStream *pls)
{
    U_CHAR c = (U_CHAR) EOP;
    TkDev *dev = (TkDev *) pls->dev;

    dbug_enter("plD_eop_tk");

    if (pls->nopause)
	return;

    tk_wr(pdf_wr_1byte(dev->file, c));
    pls->bytecnt += 1;
    WaitForPage(pls);
}

/*----------------------------------------------------------------------*\
* plD_bop_tk()
*
* Set up for the next page.
\*----------------------------------------------------------------------*/

void
plD_bop_tk(PLStream *pls)
{
    U_CHAR c = (U_CHAR) BOP;
    TkDev *dev = (TkDev *) pls->dev;

    dbug_enter("plD_bop_tk");

    dev->xold = UNDEFINED;
    dev->yold = UNDEFINED;
    pls->page++;
    tk_wr(pdf_wr_1byte(dev->file, c));
    pls->bytecnt += 1;
}

/*----------------------------------------------------------------------*\
* plD_tidy_tk()
*
* Close graphics file
\*----------------------------------------------------------------------*/

void
plD_tidy_tk(PLStream *pls)
{
    TkDev *dev = (TkDev *) pls->dev;

    dbug_enter("plD_tidy_tk");

    tk_stop(pls);
    pls->fileset = 0;
    pls->page = 0;
    free_mem(dev->cmdbuf);
}

/*----------------------------------------------------------------------*\
* plD_state_tk()
*
* Handle change in PLStream state (color, pen width, fill attribute, etc).
\*----------------------------------------------------------------------*/

void 
plD_state_tk(PLStream *pls, PLINT op)
{
    TkDev *dev = (TkDev *) pls->dev;
    U_CHAR c = (U_CHAR) CHANGE_STATE;

    dbug_enter("plD_state_tk");

    tk_wr(pdf_wr_1byte(dev->file, c));

    switch (op) {

    case PLSTATE_WIDTH:
	tk_wr(pdf_wr_1byte(dev->file, op));
	tk_wr(pdf_wr_2bytes(dev->file, (U_SHORT) (pls->width)));
	pls->bytecnt += 3;
	break;

    case PLSTATE_COLOR0:
	tk_wr(pdf_wr_1byte(dev->file, op));
	tk_wr(pdf_wr_1byte(dev->file, (U_CHAR) pls->icol0));
	pls->bytecnt += 2;
	if (pls->icol0 == PL_RGB_COLOR) {
	    tk_wr(pdf_wr_1byte(dev->file, pls->curcolor.r));
	    tk_wr(pdf_wr_1byte(dev->file, pls->curcolor.g));
	    tk_wr(pdf_wr_1byte(dev->file, pls->curcolor.b));
	    pls->bytecnt += 3;
	}
	break;

    case PLSTATE_COLOR1:
	break;
    }

    if (pls->bytecnt > pls->bufmax)
	flush_output(pls);
}

/*----------------------------------------------------------------------*\
* plD_esc_tk()
*
* Escape function.
\*----------------------------------------------------------------------*/

void
plD_esc_tk(PLStream *pls, PLINT op, void *ptr)
{
    dbug_enter("plD_esc_tk");

    switch (op) {

      case PLESC_DI:
	tk_di(pls);
	break;
    }
}

/*----------------------------------------------------------------------*\
* tk_di
*
* Process driver interface command.
* Just send the command to the remote plplot library.
\*----------------------------------------------------------------------*/

static void
tk_di(PLStream *pls)
{
    TkDev *dev = (TkDev *) pls->dev;
    char str[10];

    dbug_enter("tk_di");

/* Safety feature, should never happen */

    if (dev == NULL) 
	plexit("tk_di: Illegal call to driver (not yet initialized)");

/* Change orientation */

    if (pls->difilt & PLDI_ORI) {
	sprintf(str, "%f", pls->diorot);
	Tcl_SetVar(dev->interp, "rot", str, 0);

	server_cmd( pls, "$plwidget cmd setopt -ori $rot" );
	pls->difilt &= ~PLDI_ORI;
    }

/* Change window into plot space */

    if (pls->difilt & PLDI_PLT) {
	sprintf(str, "%f", pls->dipxmin);
	Tcl_SetVar(dev->interp, "xl", str, 0);
	sprintf(str, "%f", pls->dipymin);
	Tcl_SetVar(dev->interp, "yl", str, 0);
	sprintf(str, "%f", pls->dipxmax);
	Tcl_SetVar(dev->interp, "xr", str, 0);
	sprintf(str, "%f", pls->dipymax);
	Tcl_SetVar(dev->interp, "yr", str, 0);

	server_cmd( pls, "$plwidget cmd setopt -wplt $xl,$yl,$xr,$yr" );
	pls->difilt &= ~PLDI_PLT;
    }

/* Change window into device space */

    if (pls->difilt & PLDI_DEV) {
	sprintf(str, "%f", pls->mar);
	Tcl_SetVar(dev->interp, "mar", str, 0);
	sprintf(str, "%f", pls->aspect);
	Tcl_SetVar(dev->interp, "aspect", str, 0);
	sprintf(str, "%f", pls->jx);
	Tcl_SetVar(dev->interp, "jx", str, 0);
	sprintf(str, "%f", pls->jy);
	Tcl_SetVar(dev->interp, "jy", str, 0);

	server_cmd( pls, "$plwidget cmd setopt -mar $mar" );
	server_cmd( pls, "$plwidget cmd setopt -a $aspect" );
	server_cmd( pls, "$plwidget cmd setopt -jx $jx" );
	server_cmd( pls, "$plwidget cmd setopt -jy $jy" );
	pls->difilt &= ~PLDI_DEV;
    }

/* Update view */

    server_cmd( pls, "update" );
    server_cmd( pls, "plw_update_view $plwindow" );
}

/*----------------------------------------------------------------------*\
* tk_start
*
* Create TCL interpreter and spawn off server process.
* Each stream that uses the tk driver gets its own interpreter.
\*----------------------------------------------------------------------*/

static void
tk_start(PLStream *pls)
{
    TkDev *dev = (TkDev *) pls->dev;

    dbug_enter("tk_start");

/* Instantiate a TCL interpreter. */

    dev->interp = Tcl_CreateInterp();

/* Initialize top level window */
/* Request pls->program (if set) for the main window name */

    if (pls->program == NULL)
	pls->program = "plclient";

    if (tk_toplevel(&dev->w, dev->interp, pls->FileName, pls->program, NOMAP))
	abort_session(pls, "Unable to create top-level window");

/* Initialize stuff known to interpreter */

    tk_configure(pls);

/* Launch server process if necessary */

    launch_server(pls);

/* Initialize widgets */

    plwindow_init(pls);

/* Initialize data link */

    link_init(pls);

    server_cmd( pls, "update" );

    return;
}

/*----------------------------------------------------------------------*\
* tk_stop
*
* Normal termination & cleanup.
\*----------------------------------------------------------------------*/

static void
tk_stop(PLStream *pls)
{
    TkDev *dev = (TkDev *) pls->dev;

    dbug_enter("tk_stop");

/* Safety check for out of control code */

    if (dev->pass_thru)
	return;

    dev->pass_thru = 1;

/* Close fifo if it exists */

    if (dev->file != NULL) {
	if (fclose(dev->file)) {
	    fprintf(stderr, "%s: Error closing fifo\n", dev->program);
	}
	dev->file = NULL;
    }

/* If we launched server, blow it away */
/* First unset its client variable so it won't try communicating */

    if (tcl_eval(pls, "winfo $plserver exists")) {
	server_cmd( pls, "$plw_end $plwindow" );
	if (dev->launched_server) {
	    server_cmd( pls, "after 1 destroy ." );
	}
    }

/* Blow away main window */

    tcl_cmd(pls, "destroy .");

/* Blow away interpreter if it exists */

    if (dev->interp != NULL) {
	Tcl_DeleteInterp(dev->interp);
	dev->interp = NULL;
    }
}

/*----------------------------------------------------------------------*\
* abort_session
*
* Terminates with an error.  
* Cleanup is done by plD_tidy_tk(), called by plexit().
\*----------------------------------------------------------------------*/

static void
abort_session(PLStream *pls, char *msg)
{
    TkDev *dev = (TkDev *) pls->dev;

    dbug_enter("abort_session");

/* Safety check for out of control code */

    if (dev->pass_thru)
	return;

    dev->pass_thru = 1;

    pls->nopause = TRUE;
    plexit(msg);
}

/*----------------------------------------------------------------------*\
* tk_configure
*
* Does global variable & command initialization, mostly for interpreter.
\*----------------------------------------------------------------------*/

static void
tk_configure(PLStream *pls)
{
    TkDev *dev = (TkDev *) pls->dev;

    dbug_enter("tk_configure");

/* Use main window name as program name, now that we have it */

    dev->program = Tk_Name(dev->w);
    Tcl_SetVar(dev->interp, "client", dev->program, 0);

/* Tell interpreter about commands. */

    Tcl_CreateCommand(dev->interp, "abort", Abort,
		      (ClientData) pls, (void (*)()) NULL);

    Tcl_CreateCommand(dev->interp, "keypress", KeyEH,
		      (ClientData) pls, (void (*)()) NULL);

/* Set default names for server widget procs */

    Tcl_SetVar(dev->interp, "plserver_init", "plserver_init", 0);
    Tcl_SetVar(dev->interp, "plw_create",    "plw_create", 0);
    Tcl_SetVar(dev->interp, "plw_init",      "plw_init", 0);
    Tcl_SetVar(dev->interp, "plw_flash",     "plw_flash", 0);
    Tcl_SetVar(dev->interp, "plw_end",       "plw_end", 0);

/* Eval user-specified TCL command -- can be used to modify defaults */

    if (pls->tcl_cmd != NULL)
	tcl_cmd(pls, pls->tcl_cmd);
}

/*----------------------------------------------------------------------*\
* launch_server
*
* Starts server process if necessary.
* There are three cases:
*
* pls->plserver == NULL		plserver is started
*
* pls->plserver != NULL
*   && plserver already exists	No action is taken
*
* pls->plserver != NULL
*   && plserver doesn't exist	The specified server is created
*
* In the second case there is no work to be done so we just return.
*
* In the third case, the client has specified which server to start.
* This additional flexibility may come in handy but usually the default
* server will suffice (since the user may customize its init file).
\*----------------------------------------------------------------------*/

static void
launch_server(PLStream *pls)
{
    TkDev *dev = (TkDev *) pls->dev;
    char *argv[20];
    int i;
    pid_t pid;

    dbug_enter("launch_server");

#ifdef DEBUG
    fprintf(stderr, "%s -- PID: %d, PGID: %d, PPID: %d\n",
	    dev->program, getpid(), getpgrp(), getppid());
#endif

/* Check for already existing server */

    if (pls->plserver != NULL) {
	Tcl_SetVar(dev->interp, "plserver", pls->plserver, 0);
	if (tcl_eval(pls, "winfo $plserver exists")) {
	    return;
	}
	Tcl_UnsetVar(dev->interp, "plserver", 0);
    }
    else {
	pls->plserver = "plserver";
    }

/* Build argument list for exec */

    i = 0;
    argv[i++] = pls->plserver;		/* Name of server */

    argv[i++] = "-client";		/* Send back notification */
    argv[i++] = dev->program;

    argv[i++] = "-child";		/* Tell plserver its ancestry */

    if (pls->auto_path != NULL) {
	argv[i++] = "-auto_path";	/* Additional directory(s) */
	argv[i++] = pls->auto_path;	/* to autoload */
    }

    if (pls->FileName != NULL) {
	argv[i++] = "-display";		/* X display */
	argv[i++] = pls->FileName;
    }

    if (pls->geometry != NULL) {
	argv[i++] = "-geometry";	/* Top level window geometry */
	argv[i++] = pls->geometry;
    }

/* Start server process */

    if ( (pid = FORK()) < 0) {
	abort_session(pls, "fork error");
    }
    else if (pid == 0) {
	argv[i++] = NULL;
	if (execvp("plserver", argv)) {
	    fprintf(stderr, "execvp error\n");
	    _exit(1);
	}
    }

/*
* Wait for server to send back notification.
*
* The true server main window name will then be given by $plserver.
* This is not always the same as the name of the application since you
* could have multiple copies running on the display (resulting in
* names of the form "plserver #2", etc).
*/

    tcl_cmd(pls, "tkwait variable plserver");

    dev->launched_server = 1;
}

/*----------------------------------------------------------------------*\
* plwindow_init
*
* Configures the widget hierarchy we are sending the data stream to.  
*
* If a widget name (identifying the actual widget or a container widget)
* hasn't been supplied already we assume it needs to be created.
*
* In order to achieve maximum flexibility, the plplot tk driver requires
* only that certain TCL procs must be defined in the server interpreter.
* These can be used to set up the desired widget configuration.  The procs
* invoked from this driver currently include:
*
*    $plw_create	Creates the widget environment
*    $plw_init		Initializes the widget(s)
*    $plw_end		Prepares for shutdown
*    $plw_flash		Invoked when waiting for page advance
*
* Since all of these are interpreter variables, they can be trivially
* changed by the user (use the -tcl_cmd option).
*
* Each of these utility procs is called with a widget name ($plwindow)
* as argument.  "plwindow" is set from the value of pls->plwindow, and
* if null is generated from the name of the client main window (to
* ensure uniqueness).  $plwindow usually indicates the container frame
* for the actual plplot widget, but can be arbitrary -- as long as the
* usage in all the TCL procs is consistent.
*
* In order that the TK driver be able to invoke the actual plplot
* widget, the proc "$plw_init" deposits the widget name in the local
* interpreter variable "plwidget".
*
* In addition, the name of the client main window is given as (2nd)
* argument to "$plw_init".  This establishes the client name used
* for communication for all child widgets that require it.
\*----------------------------------------------------------------------*/

static void
plwindow_init(PLStream *pls)
{
    TkDev *dev = (TkDev *) pls->dev;
    char str[10];
    long bg;
    int i;

    dbug_enter("plwindow_init");

/* If widget does not exist we must create it */

    if (pls->plwindow == NULL) {

/* Give it a name */
/* To make sure it's unique, use the main window id (replace blanks */
/* with underscores to avoid quoting problems) */

	pls->plwindow = (char *)
	    malloc(1+(strlen(dev->program)) * sizeof(char));

	sprintf(pls->plwindow, ".%s", dev->program);
	for (i = 0; i < strlen(pls->plwindow); i++) {
	    if (pls->plwindow[i] == ' ')
		pls->plwindow[i] = '_';
	}

	Tcl_SetVar(dev->interp, "plwindow", pls->plwindow, 0);

/* Create the plframe widget & anything else you want with it. */

	server_cmd( pls, "update" );
	server_cmd( pls, "$plw_create $plwindow" );
    }
    else {
	Tcl_SetVar(dev->interp, "plwindow", pls->plwindow, 0);
    }

/* Initialize the widget(s) */

    server_cmd( pls, "update" );
    server_cmd( pls, "[list $plw_init $plwindow $client]" );

/* Now we should have the actual plplot widget name in $plwidget */
/* Configure remote plplot stream. */

    server_cmd( pls, "update" );

/* Send background command */

    bg = (((pls->bgcolor.r << 8) | pls->bgcolor.g) << 8) | pls->bgcolor.b;
    sprintf(str, "#%06x", (bg & 0xFFFFFF));
    Tcl_SetVar(dev->interp, "bg", str, 0);
    server_cmd( pls, "$plwidget configure -bg $bg" );

/* nopixmap option */

    if (pls->nopixmap) {
	server_cmd( pls, "$plwidget cmd setopt -nopixmap" );
    }

/* Start up remote plplot */

    server_cmd( pls, "update" );
    server_cmd( pls, "$plwidget cmd init" );
}

/*----------------------------------------------------------------------*\
* bgcolor_init
*
* Sets up the background color in the remote window.
*
* Since background color is a very TK-ish thing, I support it in plframe
* widgets in the normal TK-ish way, then pass the relevant info down to
* plplot.  Setting the background color directly from plplot does not
* give the right results.
*
* Also: while TK has the facility to choose color defaults on the basis of
* whether the display device is color or monochrome, at present there is no
* method particular to grayscale devices.  On these, I a white background
* looks best, so I set the background (if not already set) from here to
* white.  The result is the following set of contortions.
\*----------------------------------------------------------------------*/

static void
bgcolor_init(PLStream *pls)
{
    int is_color = pls->color;

/* If the user hasn't forced "color", see if we are grayscale */

    if ( ! pls->colorset) {
	Display *display;

	display = XOpenDisplay(pls->FileName);
	if (display != NULL) {
	    is_color = ! pl_AreWeGrayscale(display);
	    XCloseDisplay(display);
	}
    }

/* Default is white if grayscale and no explicit color commands given */

    if ( ! is_color) {
	pls->bgcolorset = 1;
	pls->bgcolor.r = 0xFF;
	pls->bgcolor.g = 0xFF;
	pls->bgcolor.b = 0xFF;
    }
}

/*----------------------------------------------------------------------*\
* link_init
*
* Initializes the link between the client and the plplot widget for
* data transfer.  Right now only fifo's are supported.
\*----------------------------------------------------------------------*/

static void
link_init(PLStream *pls)
{
    TkDev *dev = (TkDev *) pls->dev;
    int fd;

    dbug_enter("link_init");

/* Create the fifo for data transfer to the plframe widget */

    dev->filetype = "fifo";
    dev->filename = (char *) tempnam((char *) NULL, "pltk");

    if (mkfifo(dev->filename, S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH) < 0) {
	abort_session(pls, "mkfifo error");
    }

/* Tell plframe widget to open fifo (for reading). */

    Tcl_SetVar(dev->interp, "fifoname", dev->filename, 0);
    server_cmd( pls, "update" );
    server_cmd( pls, "after 1 $plwidget openfifo $fifoname" );

/* Open the fifo for writing */
/* This will block until the server opens it for reading */

    if ((fd = open(dev->filename, O_WRONLY)) == -1) {
	abort_session(pls, "Error opening fifo for write");
    }
    dev->file = fdopen(fd, "wb");

/* Unlink fifo now so that it isn't left around if program crashes. */
/* This also ensures no other program can mess with it. */

    if (unlink(dev->filename) == -1) {
        abort_session(pls, "Error removing fifo");
    }
}

/*----------------------------------------------------------------------*\
* WaitForPage()
*
* Waits for a page advance.  A call to HandleEvents() is made before
* returning to ensure all pending events were dispatched.
\*----------------------------------------------------------------------*/

static void
WaitForPage(PLStream *pls)
{
    TkDev *dev = (TkDev *) pls->dev;

    dbug_enter("WaitForPage");

    if (pls->bytecnt > 0) 
	flush_output(pls);

    server_cmd( pls, "after 1 $plw_flash $plwindow" );
    tk_wait(pls, "[info exists advance] && ($advance == 1)" );
    server_cmd( pls, "after 1 $plw_flash $plwindow" );

    Tcl_SetVar(dev->interp, "advance", "0", 0);

    HandleEvents(pls);
}

/*----------------------------------------------------------------------*\
* HandleEvents()
*
* Just a front-end to the update command, for use when not actually
* waiting for an event but only checking the event queue.  
\*----------------------------------------------------------------------*/

static void
HandleEvents(PLStream *pls)
{
    dbug_enter("HandleEvents");

    tcl_eval(pls, "update");
}

/*----------------------------------------------------------------------*\
* flush_output()
*
* Flushes output and sends command to the server to read from the pipe.
* Best to let server process the commands asynchronously by putting in
* the "after 1" -- the send command then returns immediately and the
* process can continue.  
*
* Note: the flush actually takes place after the send command.  This
* way bigger buffers can be used since both processes can work on it
* asnychronously for a while.  If the renderer just hangs at some
* point, it may indicate the buffer is too large for your system and
* further writes aren't being permitted without first reading from the
* pipe.  And since the data writer is responsible for telling the
* server to do the reads, they both hang forever!  The only way to get
* around this type of problem is to use i/o events in the server's TK
* event loop.  (Actually there is another way: use non-blocking i/o
* with good error recovery, but that's a lot of work!)  Maybe once the
* stock TK has this capability I will switch over, but sticking the
* input events in the TK event loop may be bad for performance.
\*----------------------------------------------------------------------*/

static void
flush_output(PLStream *pls)
{
    TkDev *dev = (TkDev *) pls->dev;
    char tmp[20];

    dbug_enter("flush_output");

#ifdef DEBUG
    fprintf(stderr, "%s: Flushing buffer, bytecnt = %d\n",
	    dev->program, pls->bytecnt);
#endif

    sprintf(tmp, "%d", pls->bytecnt);
    Tcl_SetVar(dev->interp, "nbytes", tmp, 0);
    server_cmd( pls, "after 1 $plwidget read $nbytes" );

    tk_wr(fflush(dev->file));
    pls->bytecnt = 0;
}

/*----------------------------------------------------------------------*\
* Abort
*
* Just a TCL front-end to abort_session().
\*----------------------------------------------------------------------*/

static int
Abort(ClientData clientData, Tcl_Interp *interp, int argc, char **argv)
{
    PLStream *pls = (PLStream *) clientData;

    dbug_enter("Abort");

    abort_session(pls, "");
    return TCL_OK;
}

/*----------------------------------------------------------------------*\
* KeyEH()
*
* This TCL command handles keyboard events.
*
* Arguments:
*	command name
*	keysym name (textual string)
*	keysym value
*	ASCII equivalent (optional)
*
* The first argument is keysym name -- this is all that's really required 
* although it's better to send the numeric keysym value since then we
* can avoid a long lookup procedure.  Sometimes, when faking input, it
* is inconvenient to have to worry about what the numeric keysym value
* is, so in a few cases a missing keysym value is tolerated.
\*----------------------------------------------------------------------*/

static int
KeyEH(ClientData clientData, Tcl_Interp *interp, int argc, char **argv)
{
    PLStream *pls = (PLStream *) clientData;
    TkDev *dev = (TkDev *) pls->dev;

    PLKey key;
    char *keysym, c;
    int advance = 0;

    dbug_enter("KeyEH");

    if (argc < 2) {
	Tcl_AppendResult(interp, "wrong # args: should be \"",
		argv[0], " keysym-name ?keysym-value?\"", (char *) NULL);
	return TCL_ERROR;
    }
    key.code = 0;
    key.string[0] = '\0';

/* Keysym name */

    keysym = argv[1];

/* Keysym value */
/* If missing, explicitly check for a few common ones */

    if (argc > 2)
	key.code = atol(argv[2]);

    if (argc == 2 || key.code == 0) {
	c = *keysym;
	if ((c == 'B') && (strcmp(keysym, "BackSpace") == 0)) {
	    key.code = PLK_BackSpace;
	}
	else if ((c == 'D') && (strcmp(keysym, "Delete") == 0)) {
	    key.code = PLK_Delete;
	}
	else if ((c == 'L') && (strcmp(keysym, "Linefeed") == 0)) {
	    key.code = PLK_Linefeed;
	}
	else if ((c == 'R') && (strcmp(keysym, "Return") == 0)) {
	    key.code = PLK_Return;
	}
	else if ((c == 'P') && (strcmp(keysym, "Prior") == 0)) {
	    key.code = PLK_Prior;
	}
	else if ((c == 'N') && (strcmp(keysym, "Next") == 0)) {
	    key.code = PLK_Next;
	}
	else {
	    Tcl_AppendResult(interp, "Unrecognized keysym \"",
		    argv[1], "\"; must specify keycode", (char *) NULL);
	    return TCL_ERROR;
	}
    }

/* ASCII value */

    if (argc > 3) {
	key.string[0] = argv[3][0];
	key.string[1] = '\0';
    }

#ifdef DEBUG
    fprintf(stderr, "KeyEH: Keysym %s, hex %x, ASCII: %s\n",
	    keysym, key.code, key.string);
#endif

/* Call user event handler */
/* Since this is called first, the user can disable all plplot internal
   event handling by setting key.code to 0 and key.string to '\0' */

    if (pls->KeyEH != NULL)
	(*pls->KeyEH) (&key, pls->KeyEH_data, &advance);

/* Handle internal events */

/* Advance to next page (i.e. terminate event loop) on a <eol> */
/* Check for both <CR> and <LF> for portability, also a <Page Down> */

    if (key.code == PLK_Return ||
	key.code == PLK_Linefeed ||
	key.code == PLK_Next)
	advance = TRUE;

    if (advance)
	Tcl_SetVar(dev->interp, "advance", "1", 0);

/* Terminate on a 'Q' (not 'q', since it's too easy to hit by mistake) */

    if (key.string[0] == 'Q') {
	pls->nopause = TRUE;
	plexit("");
    }
    return TCL_OK;
}

/*----------------------------------------------------------------------*\
* tk_wait()
*
* Waits for the specified expression to evaluate to true before
* proceeding.  While we are waiting to proceed, all events (for this
* or other interpreters) are handled.  
*
* Use a static string buffer to hold the command, to ensure it's in
* writable memory (grrr...).
\*----------------------------------------------------------------------*/

static void
tk_wait(PLStream *pls, char *cmd)
{
    TkDev *dev = (TkDev *) pls->dev;
    int result = 0;

    dbug_enter("tk_wait");

    copybuf(pls, cmd);
    while ( ! result) {
	if (Tcl_ExprBoolean(dev->interp, dev->cmdbuf, &result)) {
	    fprintf(stderr, "tk_wait command \"%s\" failed:\n\t %s\n",
		    cmd, dev->interp->result);
	    break;
	}
	Tk_DoOneEvent(0);
    }
}

/*----------------------------------------------------------------------*\
* server_cmd
*
* Sends specified command to server, aborting on an error.
\*----------------------------------------------------------------------*/

static void
server_cmd(PLStream *pls, char *cmd)
{
    TkDev *dev = (TkDev *) pls->dev;

    dbug_enter("server_cmd");
#ifdef DEBUG_ENTER
    fprintf(stderr, "Sending command: %s\n", cmd);
#endif

    if (Tcl_VarEval(dev->interp, "send $plserver ", cmd, (char **) NULL)) {
	fprintf(stderr, "Server command \"%s\" failed:\n\t %s\n",
		cmd, dev->interp->result);
	abort_session(pls, "");
    }
}

/*----------------------------------------------------------------------*\
* tcl_cmd
*
* Evals the specified command, aborting on an error.
\*----------------------------------------------------------------------*/

static void
tcl_cmd(PLStream *pls, char *cmd)
{
    TkDev *dev = (TkDev *) pls->dev;

    dbug_enter("tcl_cmd");
#ifdef DEBUG_ENTER
    fprintf(stderr, "Evaluating command: %s\n", cmd);
#endif

    if (tcl_eval(pls, cmd)) {
	fprintf(stderr, "TCL command \"%s\" failed:\n\t %s\n",
		cmd, dev->interp->result);
	abort_session(pls, "");
    }
}

/*----------------------------------------------------------------------*\
* tcl_eval
*
* Evals the specified string, returning the result.
\*----------------------------------------------------------------------*/

static int
tcl_eval(PLStream *pls, char *cmd)
{
    TkDev *dev = (TkDev *) pls->dev;

    copybuf(pls, cmd);
    return(Tcl_Eval(dev->interp, dev->cmdbuf, 0, (char **) NULL));
}

/*----------------------------------------------------------------------*\
* copybuf
*
* Puts command in a static string buffer, to ensure it's in writable
* memory (grrr...).
\*----------------------------------------------------------------------*/

static void
copybuf(PLStream *pls, char *cmd)
{
    TkDev *dev = (TkDev *) pls->dev;

    if (dev->cmdbuf == NULL) {
	dev->cmdbuf_len = 100;
	dev->cmdbuf = (char *) malloc(dev->cmdbuf_len);
    }

    if (strlen(cmd) >= dev->cmdbuf_len) {
	free((void *) dev->cmdbuf);
	dev->cmdbuf_len = strlen(cmd) + 20;
	dev->cmdbuf = (char *) malloc(dev->cmdbuf_len);
    }

    strcpy(dev->cmdbuf, cmd);
}

/*----------------------------------------------------------------------*/
#else
int
pldummy_tk()
{
    return 0;
}

#endif				/* TK */
