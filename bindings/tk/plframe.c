/*----------------------------------------------------------------------*\
 * 
 * plframe.c --
 *
 *	This module implements "plframe" widgets for the Tk toolkit.
 *	These are frames that have extra logic to allow them to be
 *	interfaced with the plplot X driver.  These are then drawn
 *	into and respond to keyboard and mouse events.
 *
 * Maurice LeBrun
 * IFS, University of Texas at Austin
 * 30-Apr-1993
 *
 * Based upon tkFrame.c from the TK 3.2 distribution:
 *
 * Copyright 1990 Regents of the University of California.
 * Permission to use, copy, modify, and distribute this
 * software and its documentation for any purpose and without
 * fee is hereby granted, provided that the above copyright
 * notice appear in all copies.  The University of California
 * makes no representations about the suitability of this
 * software for any purpose.  It is provided "as is" without
 * express or implied warranty.
\*----------------------------------------------------------------------*/

#include "plserver.h"
#include "xwin.h"

#define NDEV	20		/* Max number of output device types */
#define plframe_cmd(code) \
    if ((code) == TCL_ERROR) return (TCL_ERROR);

/*
 * A data structure of the following type is kept for each
 * plframe that currently exists for this process:
 */

typedef struct {

    /* This is stuff taken from tkFrame.c */

    Tk_Window tkwin;		/* Window that embodies the frame.  NULL
				 * means that the window has been destroyed
				 * but the data structures haven't yet been
				 * cleaned up.*/
    Display *display;		/* Display containing widget.  Used, among
				 * other things, so that resources can be
				 * freed even after tkwin has gone away. */
    Tcl_Interp *interp;		/* Interpreter associated with
				 * widget.  Used to delete widget
				 * command.  */
    Tk_Uid screenName;		/* If this window isn't a toplevel window
				 * then this is NULL;  otherwise it gives
				 * the name of the screen on which window
				 * is displayed. */
    Tk_3DBorder border;		/* Structure used to draw 3-D border and
				 * background. */
    int borderWidth;		/* Width of 3-D border (if any). */
    int relief;			/* 3-d effect: TK_RELIEF_RAISED etc. */
    int width;			/* Width to request for window.  <= 0 means
				 * don't request any size. */
    int height;			/* Height to request for window.  <= 0 means
				 * don't request any size. */
    char *geometry;		/* Geometry that user requested.  NULL
				 * means use width and height instead. 
				 * Malloc'ed. */
    Cursor cursor;		/* Current cursor for window, or None. */
    int flags;			/* Various flags;  see below for
				 * definitions. */

    /* These are new to plframe widgets */

    /* control stuff */

    int tkwin_initted;		/* Set first time widget is mapped */
    int plplot_initted;		/* Set first time plplot is initialized */
    PLINT ipls;			/* Plplot stream number */
    PLINT ipls_save;		/* Plplot stream number, save files */

    char *client;		/* Client main window.  Malloc'ed. */
    PLRDev *plr;		/* Renderer state information.  Malloc'ed */
    XColor *bgColor;		/* Background color */

    /* Used to distinguish resize from expose events */

    int prevWidth;		/* Previous window width */
    int prevHeight;		/* Previous window height */

    /* Support for save operations */

    char *SaveFnam;		/* File name we are currently saving to.
				   Malloc'ed. */
    char **devDesc;		/* Descriptive names for file-oriented 
				 * devices.  Malloc'ed. */
    char **devName;		/* Keyword names of file-oriented devices.
				 * Malloc'ed. */

    /* Used in selecting & modifying plot or device area */

    GC xorGC;			/* GC used for rubber-band drawing */
    XPoint pts[5];		/* Points for rubber-band drawing */
    int continue_draw;		/* Set when doing rubber-band draws */
    Cursor draw_cursor;		/* cursor used for drawing */
    PLFLT xl, xr, yl, yr;	/* Bounds on plot viewing area */
    char *yScrollCmd;		/* Command prefix for communicating with
				 * vertical scrollbar.  NULL means no
				 * command to issue.  Malloc'ed. */
    char *xScrollCmd;		/* Command prefix for communicating with
				 * horizontal scrollbar.  NULL means no
				 * command to issue.  Malloc'ed. */
} PlFrame;

/*
 * Flag bits for plframes:
 *
 * REFRESH_PENDING:		Non-zero means a DoWhenIdle handler
 *				has already been queued to refresh
 *				this window.
 * CLEAR_NEEDED;		Need to clear the window when redrawing.
 * RESIZE_PENDING;		Used to reschedule resize events
 * REDRAW_PENDING;		Used to redraw contents of plot buffer
 * UPDATE_V_SCROLLBAR:		Non-zero means vertical scrollbar needs
 *				to be updated.
 * UPDATE_H_SCROLLBAR:		Non-zero means horizontal scrollbar needs
 *				to be updated.
 */

#define REFRESH_PENDING		1
#define CLEAR_NEEDED		2
#define RESIZE_PENDING		4
#define REDRAW_PENDING		8
#define UPDATE_V_SCROLLBAR	16
#define UPDATE_H_SCROLLBAR	32

/*
 * Defaults for plframes:
 */

#define DEF_PLFRAME_BG_COLOR		"Black"
#define DEF_PLFRAME_BG_MONO		"White"
#define DEF_PLFRAME_BORDER_WIDTH	"0"
#define DEF_PLFRAME_CURSOR		((char *) NULL)
#define DEF_PLFRAME_GEOMETRY		((char *) NULL)
#define DEF_PLFRAME_HEIGHT		"0"
#define DEF_PLFRAME_RELIEF		"flat"
#define DEF_PLFRAME_WIDTH		"0"

/* Configuration info */

static Tk_ConfigSpec configSpecs[] = {
    {TK_CONFIG_BORDER, "-background", "background", "Background",
	DEF_PLFRAME_BG_COLOR, Tk_Offset(PlFrame, border),
	TK_CONFIG_COLOR_ONLY},
    {TK_CONFIG_COLOR, (char *) NULL, (char *) NULL, (char *) NULL,
	(char *) NULL, Tk_Offset(PlFrame, bgColor),
	TK_CONFIG_COLOR_ONLY},
    {TK_CONFIG_BORDER, "-background", "background", "Background",
	DEF_PLFRAME_BG_MONO, Tk_Offset(PlFrame, border),
	TK_CONFIG_MONO_ONLY},
    {TK_CONFIG_COLOR, (char *) NULL, (char *) NULL, (char *) NULL,
	(char *) NULL, Tk_Offset(PlFrame, bgColor),
	TK_CONFIG_MONO_ONLY},
    {TK_CONFIG_SYNONYM, "-bd", "borderWidth", (char *) NULL,
	(char *) NULL, 0, 0},
    {TK_CONFIG_SYNONYM, "-bg", "background", (char *) NULL,
	(char *) NULL, 0, 0},
    {TK_CONFIG_PIXELS, "-borderwidth", "borderWidth", "BorderWidth",
	DEF_PLFRAME_BORDER_WIDTH, Tk_Offset(PlFrame, borderWidth), 0},
    {TK_CONFIG_ACTIVE_CURSOR, "-cursor", "cursor", "Cursor",
	DEF_PLFRAME_CURSOR, Tk_Offset(PlFrame, cursor), TK_CONFIG_NULL_OK},
    {TK_CONFIG_STRING, "-geometry", "geometry", "Geometry",
	DEF_PLFRAME_GEOMETRY, Tk_Offset(PlFrame, geometry), TK_CONFIG_NULL_OK},
    {TK_CONFIG_PIXELS, "-height", "height", "Height",
	DEF_PLFRAME_HEIGHT, Tk_Offset(PlFrame, height), 0},
    {TK_CONFIG_RELIEF, "-relief", "relief", "Relief",
	DEF_PLFRAME_RELIEF, Tk_Offset(PlFrame, relief), 0},
    {TK_CONFIG_PIXELS, "-width", "width", "Width",
	DEF_PLFRAME_WIDTH, Tk_Offset(PlFrame, width), 0},
    {TK_CONFIG_STRING, "-xscrollcommand", "xScrollCommand", "ScrollCommand",
	(char *) NULL, Tk_Offset(PlFrame, xScrollCmd), TK_CONFIG_NULL_OK},
    {TK_CONFIG_STRING, "-yscrollcommand", "yScrollCommand", "ScrollCommand",
	(char *) NULL, Tk_Offset(PlFrame, yScrollCmd), TK_CONFIG_NULL_OK},
    {TK_CONFIG_END, (char *) NULL, (char *) NULL, (char *) NULL,
	(char *) NULL, 0, 0}
};

/*
 * Forward declarations for procedures defined later in this file:
 */

/* Externals */

int   plFrameCmd     	(ClientData, Tcl_Interp *, int, char **);

/* These are invoked by the TK dispatcher */

static void  DestroyPlFrame	(ClientData);
static void  DisplayPlFrame	(ClientData);
static void  PlFrameInit	(ClientData);
static void  PlFrameEventProc	(ClientData, XEvent *);
static int   PlFrameWidgetCmd	(ClientData, Tcl_Interp *, int, char **);
static void  MapPlFrame 	(ClientData);

/* These are invoked by PlFrameWidgetCmd to process widget commands */

static int   Cmd		(Tcl_Interp *, PlFrame *, int, char **);
static int   ConfigurePlFrame	(Tcl_Interp *, PlFrame *, int, char **, int);
static int   Draw		(Tcl_Interp *, PlFrame *, int, char **);
static int   Info		(Tcl_Interp *, PlFrame *, int, char **);
static int   OpenFifo		(Tcl_Interp *, PlFrame *, int, char **);
static int   Orient		(Tcl_Interp *, PlFrame *, int, char **);
static int   Page		(Tcl_Interp *, PlFrame *, int, char **);
static int   Print		(Tcl_Interp *, PlFrame *, int, char **);
static int   ReadData		(Tcl_Interp *, PlFrame *, int, char **);
static int   Redraw		(Tcl_Interp *, PlFrame *, int, char **);
static int   Save		(Tcl_Interp *, PlFrame *, int, char **);
static int   View		(Tcl_Interp *, PlFrame *, int, char **);
static int   xScroll		(Tcl_Interp *, PlFrame *, int, char **);
static int   yScroll		(Tcl_Interp *, PlFrame *, int, char **);

/* Utility routines */

static int   tcl_eval		(PlFrame *, char *);
static int   client_cmd		(PlFrame *, char *);
static void  gbox		(PLFLT *, PLFLT *, PLFLT *, PLFLT *, char **);
static void  UpdateVScrollbar	(register PlFrame *);
static void  UpdateHScrollbar	(register PlFrame *);

/*
 *---------------------------------------------------------------------------
 *
 * plFrameCmd --
 *
 *	This procedure is invoked to process the "plframe" Tcl
 *	command.  See the user documentation for details on what it
 *	does.
 *
 * Results:
 *	A standard Tcl result.
 *
 * Side effects:
 *	See the user documentation.
 *
 *---------------------------------------------------------------------------
 */

int
plFrameCmd(ClientData clientData, Tcl_Interp *interp,
	   int argc, char **argv)
{
    Tk_Window tkwin = (Tk_Window) clientData;
    Tk_Window new;
    register PlFrame *plFramePtr;
    register PLRDev *plr;

    Tk_Uid screenUid;
    char *className, *screen;
    int src, dst;

    XGCValues gcValues;
    unsigned long mask;

    dbug_enter("plFrameCmd");

    if (argc < 2) {
	Tcl_AppendResult(interp, "wrong # args: should be \"",
		argv[0], " pathName ?options?\"", (char *) NULL);
	return TCL_ERROR;
    }

    /*
     * The code below is a special hack that extracts a few key
     * options from the argument list now, rather than letting
     * ConfigurePlFrame do it.  This is necessary because we have
     * to know the window's screen (if it's top-level) and its
     * class before creating the window.
     */

    screen = NULL;
    className = (argv[0][0] == 't') ? "Toplevel" : "PlFrame";
    for (src = 2, dst = 2; src < argc;  src += 2) {
	char c;

	c = argv[src][1];
	if ((c == 'c')
		&& (strncmp(argv[src], "-class", strlen(argv[src])) == 0)) {
	    className = argv[src+1];
	}
	else if ((argv[0][0] == 't') && (c == 's')
		&& (strncmp(argv[src], "-screen", strlen(argv[src])) == 0)) {
	    screen = argv[src+1];
	}
	else {
	    argv[dst] = argv[src];
	    argv[dst+1] = argv[src+1];
	    dst += 2;
	}
    }
    argc -= src-dst;

    /*
     * Provide a default screen for top-level windows (same as screen
     * of parent window).
     */

    if ((argv[0][0] == 't') && (screen == NULL)) {
	screen = "";
    }
    if (screen != NULL) {
	screenUid = Tk_GetUid(screen);
    }
    else {
	screenUid = NULL;
    }

    /*
     * Create the window.
     */

    new = Tk_CreateWindowFromPath(interp, tkwin, argv[1], screenUid);
    if (new == NULL) {
	return TCL_ERROR;
    }

    Tk_SetClass(new, className);
    plFramePtr = (PlFrame *) ckalloc(sizeof(PlFrame));
    plFramePtr->tkwin = new;
    plFramePtr->display = Tk_Display(new);
    plFramePtr->interp = interp;
    plFramePtr->screenName = screenUid;
    plFramePtr->border = NULL;
    plFramePtr->geometry = NULL;
    plFramePtr->cursor = None;
    plFramePtr->flags = 0;
    plFramePtr->client = NULL;
    plFramePtr->prevWidth = Tk_Width(plFramePtr->tkwin);
    plFramePtr->prevHeight = Tk_Height(plFramePtr->tkwin);
    plFramePtr->continue_draw = 0;
    plFramePtr->ipls = 0;
    plFramePtr->ipls_save = 0;
    plFramePtr->tkwin_initted = 0;
    plFramePtr->plplot_initted = 0;
    plFramePtr->bgColor = NULL;
    plFramePtr->yScrollCmd = NULL;
    plFramePtr->xScrollCmd = NULL;
    plFramePtr->xl = 0.;
    plFramePtr->yl = 0.;
    plFramePtr->xr = 1.;
    plFramePtr->yr = 1.;
    plFramePtr->SaveFnam = NULL;
    plFramePtr->devDesc = NULL;
    plFramePtr->devName = NULL;

    plFramePtr->plr = (PLRDev *) ckalloc(sizeof(PLRDev));
    plr = plFramePtr->plr;

    plr->file = NULL;
    plr->filename = NULL;
    plr->filetype = NULL;
    plr_start(plr);

/* Set up stuff for rubber-band drawing */

    gcValues.background = 0;
    gcValues.foreground = 1;
    gcValues.function = GXxor;
    mask = GCForeground | GCBackground | GCFunction;
    plFramePtr->xorGC = Tk_GetGC(plFramePtr->tkwin, mask, &gcValues);
    plFramePtr->draw_cursor =
	Tk_GetCursor(plFramePtr->interp, plFramePtr->tkwin, "crosshair");

/* Start up event handlers */

    Tk_CreateEventHandler(plFramePtr->tkwin, ExposureMask|StructureNotifyMask,
			  PlFrameEventProc, (ClientData) plFramePtr);

    Tcl_CreateCommand(interp, Tk_PathName(plFramePtr->tkwin),
	    PlFrameWidgetCmd, (ClientData) plFramePtr, (void (*)()) NULL);

    if (ConfigurePlFrame(interp, plFramePtr, argc-2, argv+2, 0) != TCL_OK) {
	Tk_DestroyWindow(plFramePtr->tkwin);
	return TCL_ERROR;
    }
    if (screenUid != NULL) {
	Tk_DoWhenIdle(MapPlFrame, (ClientData) plFramePtr);
    }
    interp->result = Tk_PathName(plFramePtr->tkwin);

    return TCL_OK;
}

/*
 *---------------------------------------------------------------------------
 *
 * PlFrameWidgetCmd --
 *
 *	This procedure is invoked to process the Tcl command that
 *	corresponds to a plframe widget.  See the user
 *	documentation for details on what it does.
 *
 * Results:
 *	A standard Tcl result.
 *
 * Side effects:
 *	See the user documentation.
 *
 *---------------------------------------------------------------------------
 */

static int
PlFrameWidgetCmd(ClientData clientData, Tcl_Interp *interp,
		 int argc, char **argv)
{
    register PlFrame *plFramePtr = (PlFrame *) clientData;
    int result = TCL_OK;
    int length;
    char c;

    dbug_enter("PlFrameWidgetCmd");

    if (argc < 2) {
	Tcl_AppendResult(interp, "wrong # args: should be \"",
		argv[0], " option ?arg arg ...?\"", (char *) NULL);
	return TCL_ERROR;
    }
    Tk_Preserve((ClientData) plFramePtr);
    c = argv[1][0];
    length = strlen(argv[1]);

/* attach -- specifies the client main window */

    if ((c == 'a') && (strncmp(argv[1], "attach", length) == 0)) {
	if (argc == 2 || argc > 3) {
	    Tcl_AppendResult(interp, "wrong # args: should be \"",
		    argv[0], " attach clientname\"", (char *) NULL);
	    result = TCL_ERROR;
	    goto done;
	}
	else {
	    if (plFramePtr->client != NULL) {
		ckfree((char *) plFramePtr->client);
		plFramePtr->client = NULL;
	    }
	    plFramePtr->client = ckalloc(strlen(argv[2]+1) * sizeof(char));
	    strcpy(plFramePtr->client, argv[2]);
	    result = TCL_OK;
	}
    }

/* cmd -- issue a command to the plplot library */

    else if ((c == 'c') && (strncmp(argv[1], "cmd", length) == 0)) {
	result = Cmd(interp, plFramePtr, argc-2, argv+2);
    }

/* configure */

    else if ((c == 'c') && (strncmp(argv[1], "configure", length) == 0)) {
	if (argc == 2) {
	    result = Tk_ConfigureInfo(interp, plFramePtr->tkwin, configSpecs,
		    (char *) plFramePtr, (char *) NULL, 0);
	}
	else if (argc == 3) {
	    result = Tk_ConfigureInfo(interp, plFramePtr->tkwin, configSpecs,
		    (char *) plFramePtr, argv[2], 0);
	}
	else {
	    result = ConfigurePlFrame(interp, plFramePtr, argc-2, argv+2,
		    TK_CONFIG_ARGV_ONLY);
	}
    }

/* Detach -- eliminates references to "client" */

    else if ((c == 'd') && (strncmp(argv[1], "detach", length) == 0)) {
	if (argc != 2) {
	    Tcl_AppendResult(interp, "wrong # args: should be \"",
		    argv[0], " detach\"", (char *) NULL);
	    result = TCL_ERROR;
	    goto done;
	}
	else {
	    if (plFramePtr->client != NULL) {
		ckfree((char *) plFramePtr->client);
		plFramePtr->client = NULL;
	    }
	    result = TCL_OK;
	}
    }

/* draw -- rubber-band draw used in region selection */

    else if ((c == 'd') && (strncmp(argv[1], "draw", length) == 0)) {
	if (argc == 2) {
	    Tcl_AppendResult(interp, "wrong # args: should be \"",
		    argv[0], " draw op ?options?\"", (char *) NULL);
	    result = TCL_ERROR;
	    goto done;
	}
	else {
	    result = Draw(interp, plFramePtr, argc-2, argv+2);
	}
    }

/* info -- returns requested info */

    else if ((c == 'i') && (strncmp(argv[1], "info", length) == 0)) {
	result = Info(interp, plFramePtr, argc-2, argv+2);
    }

/* orient -- Set plot orientation */

    else if ((c == 'o') && (strncmp(argv[1], "orient", length) == 0)) {
	result = Orient(interp, plFramePtr, argc-2, argv+2);
    }

/* openfifo -- Open the specified fifo */

    else if ((c == 'o') && (strncmp(argv[1], "openfifo", length) == 0)) {
	if (argc == 2 || argc > 3) {
	    Tcl_AppendResult(interp, "wrong # args: should be \"",
		    argv[0], " openfifo fifoname\"", (char *) NULL);
	    result = TCL_ERROR;
	    goto done;
	}
	else {
	    result = OpenFifo(interp, plFramePtr, argc-2, argv+2);
	}
    }

/* page -- change or return output page setup */

    else if ((c == 'p') && (strncmp(argv[1], "page", length) == 0)) {
	result = Page(interp, plFramePtr, argc-2, argv+2);
    }

/* print -- prints plot */

    else if ((c == 'p') && (strncmp(argv[1], "print", length) == 0)) {
	result = Print(interp, plFramePtr, argc-2, argv+2);
    }

/* read -- read <nbytes> of data from fifo or socket */

    else if ((c == 'r') && (strncmp(argv[1], "read", length) == 0)) {
	if (argc == 2 || argc > 3) {
	    Tcl_AppendResult(interp, "wrong # args: should be \"",
		    argv[0], " read nbytes\"", (char *) NULL);
	    result = TCL_ERROR;
	    goto done;
	}
	else {
	    result = ReadData(interp, plFramePtr, argc-2, argv+2);
	}
    }

/* redraw -- redraw plot */

    else if ((c == 'r') && (strncmp(argv[1], "redraw", length) == 0)) {
	if (argc > 2) {
	    Tcl_AppendResult(interp, "wrong # args: should be \"",
		    argv[0], " redraw\"", (char *) NULL);
	    result = TCL_ERROR;
	    goto done;
	}
	else {
	    result = Redraw(interp, plFramePtr, argc-2, argv+2);
	}
    }

/* save -- saves plot to the specified plot file type */

    else if ((c == 's') && (strncmp(argv[1], "save", length) == 0)) {
	result = Save(interp, plFramePtr, argc-2, argv+2);
    }

/* view -- change or return window into plot */

    else if ((c == 'v') && (strncmp(argv[1], "view", length) == 0)) {
	result = View(interp, plFramePtr, argc-2, argv+2);
    }

/* xscroll -- horizontally scroll window into plot */

    else if ((c == 'x') && (strncmp(argv[1], "xscroll", length) == 0)) {
	if (argc == 2 || argc > 3) {
	    Tcl_AppendResult(interp, "wrong # args: should be \"",
		    argv[0], " xscroll pixel\"", (char *) NULL);
	    result = TCL_ERROR;
	    goto done;
	}
	else {
	    result = xScroll(interp, plFramePtr, argc-2, argv+2);
	}
    }

/* yscroll -- vertically scroll window into plot */

    else if ((c == 'y') && (strncmp(argv[1], "yscroll", length) == 0)) {
	if (argc == 2 || argc > 3) {
	    Tcl_AppendResult(interp, "wrong # args: should be \"",
		    argv[0], " yscroll pixel\"", (char *) NULL);
	    result = TCL_ERROR;
	    goto done;
	}
	else {
	    result = yScroll(interp, plFramePtr, argc-2, argv+2);
	}
    }

/* unrecognized widget command */

    else {
	Tcl_AppendResult(interp, "bad option \"", argv[1],
	 "\":  must be attach, cmd, configure, draw, detach, info, ",
	 "openfifo, orient, page, print, read, redraw, save, view, ",
	 "xscroll, or yscroll", (char *) NULL);

	result = TCL_ERROR;
    }

 done:
    Tk_Release((ClientData) plFramePtr);
    return result;
}

/*
 *---------------------------------------------------------------------------
 *
 * DestroyPlFrame --
 *
 *	This procedure is invoked by Tk_EventuallyFree or Tk_Release to
 *	clean up the internal structure of a plframe at a safe time
 *	(when no-one is using it anymore).
 *
 * Results:
 *	None.
 *
 * Side effects:
 *	Everything associated with the plframe is freed up.
 *
 *---------------------------------------------------------------------------
 */

static void
DestroyPlFrame(ClientData clientData)
{
    register PlFrame *plFramePtr = (PlFrame *) clientData;
    register PLRDev *plr = plFramePtr->plr;

    dbug_enter("DestroyPlFrame");

    if (plFramePtr->border != NULL) {
	Tk_Free3DBorder(plFramePtr->border);
    }
    if (plFramePtr->bgColor != NULL) {
	Tk_FreeColor(plFramePtr->bgColor);
    }
    if (plFramePtr->geometry != NULL) {
	ckfree(plFramePtr->geometry);
    }
    if (plFramePtr->cursor != None) {
	Tk_FreeCursor(plFramePtr->display, plFramePtr->cursor);
    }
    if (plFramePtr->draw_cursor != None) {
	Tk_FreeCursor(plFramePtr->display, plFramePtr->draw_cursor);
    }
    if (plFramePtr->xorGC != None) {
	Tk_FreeGC(plFramePtr->display, plFramePtr->xorGC);
    }
    if (plFramePtr->yScrollCmd != NULL) {
	ckfree(plFramePtr->yScrollCmd);
    }
    if (plFramePtr->xScrollCmd != NULL) {
	ckfree(plFramePtr->xScrollCmd);
    }
    if (plFramePtr->SaveFnam != NULL) {
	ckfree(plFramePtr->devDesc);
    }
    if (plFramePtr->devDesc != NULL) {
	ckfree(plFramePtr->devDesc);
    }
    if (plFramePtr->devName != NULL) {
	ckfree(plFramePtr->devName);
    }

/* Tell client to quit and close fifo */

    client_cmd(plFramePtr, "after 1 abort");

    if (plr->file != NULL) {
	fclose(plr->file);
    }
    if (plFramePtr->client != NULL) {
	ckfree((char *) plFramePtr->client);
    }

    ckfree((char *) plFramePtr->plr);
    ckfree((char *) plFramePtr);

/* Tell plplot to clean up */

    plend();
    plFramePtr->tkwin_initted = 0;
    plFramePtr->plplot_initted = 0;
}

/*
 *---------------------------------------------------------------------------
 *
 * PlFrameEventProc --
 *
 *	Invoked by the Tk dispatcher on exposes and structure
 *	changes to a plframe. 
 *
 * Results:
 *	None.
 *
 * Side effects:
 *	When the window gets deleted, internal structures get cleaned up.
 *	When it gets exposed, it is redisplayed.
 *
 *---------------------------------------------------------------------------
 */

static void
PlFrameEventProc(ClientData clientData, register XEvent *eventPtr)
{
    register PlFrame *plFramePtr = (PlFrame *) clientData;

    dbug_enter("PlFrameEventProc");

    switch (eventPtr->type) {
    case Expose:
	if (eventPtr->xexpose.count == 0) {
	    if ((plFramePtr->tkwin != NULL) &&
		!(plFramePtr->flags & REFRESH_PENDING)) {

		Tk_DoWhenIdle(DisplayPlFrame, (ClientData) plFramePtr);
		plFramePtr->flags |= REFRESH_PENDING;
	    }
	}
	break;

    case ConfigureNotify:
	plFramePtr->flags |= RESIZE_PENDING;
	if ((plFramePtr->tkwin != NULL) &&
	    !(plFramePtr->flags & REFRESH_PENDING)) {

	    Tk_DoWhenIdle(DisplayPlFrame, (ClientData) plFramePtr);
	    plFramePtr->flags |= REFRESH_PENDING;
	    plFramePtr->flags |= UPDATE_V_SCROLLBAR|UPDATE_H_SCROLLBAR;
	}
	break;

    case DestroyNotify:
	Tcl_DeleteCommand(plFramePtr->interp, Tk_PathName(plFramePtr->tkwin));
	plFramePtr->tkwin = NULL;
	if (plFramePtr->flags & REFRESH_PENDING) {
	    Tk_CancelIdleCall(DisplayPlFrame, (ClientData) plFramePtr);
	}
	Tk_CancelIdleCall(MapPlFrame, (ClientData) plFramePtr);
	Tk_EventuallyFree((ClientData) plFramePtr, DestroyPlFrame);
	break;

    case MapNotify:
	if (plFramePtr->flags & REFRESH_PENDING) {
	    Tk_CancelIdleCall(DisplayPlFrame, (ClientData) plFramePtr);
	}
	Tk_DoWhenIdle(PlFrameInit, (ClientData) plFramePtr);
	break;
    }
}

/*
 *---------------------------------------------------------------------------
 *
 * PlFrameInit --
 *
 *	Invoked to handle miscellaneous initialization after window gets
 *	mapped.  
 *
 * Results:
 *	None.
 *
 * Side effects:
 *	Plplot internal parameters and device driver are initialized.
 *
 *---------------------------------------------------------------------------
 */

static void
PlFrameInit(ClientData clientData)
{
    register PlFrame *plFramePtr = (PlFrame *) clientData;
    register Tk_Window tkwin = plFramePtr->tkwin;
    int i, ndev;

/* Set up window parameters and arrange for window to be refreshed */

    plFramePtr->flags &= REFRESH_PENDING;
    plFramePtr->flags |= UPDATE_V_SCROLLBAR|UPDATE_H_SCROLLBAR;

/* First-time initialization */
/* Plplot calls to set driver and related variables are made. */
/* The call to plinit() must come AFTER this section of code */
/* This part will require modification to support >1 embedded widgets */

    if ( ! plFramePtr->tkwin_initted) {
	plsdev("xwin");
	plsxwin(Tk_WindowId(tkwin));
	plspause(0);

	plFramePtr->tkwin_initted = 1;
	plFramePtr->prevWidth = Tk_Width(tkwin);
	plFramePtr->prevHeight = Tk_Height(tkwin);

	plFramePtr->devDesc = (char **) ckalloc(NDEV * sizeof(char **));
	plFramePtr->devName = (char **) ckalloc(NDEV * sizeof(char **));
	for (i = 0; i < NDEV; i++) {
	    plFramePtr->devDesc[i] = NULL;
	    plFramePtr->devName[i] = NULL;
	}
	plgFileDevs(&plFramePtr->devDesc, &plFramePtr->devName, &ndev);
    }
    else {
	plFramePtr->flags |= RESIZE_PENDING;
    }

/* Draw plframe */

    DisplayPlFrame(clientData);
}

/*
 *---------------------------------------------------------------------------
 *
 * DisplayPlFrame --
 *
 *	This procedure is invoked to display a plframe widget.
 *
 * Results:
 *	None.
 *
 * Side effects:
 *	Commands are output to X to display the plframe in its
 *	current mode.
 *
 *---------------------------------------------------------------------------
 */

static void
DisplayPlFrame(ClientData clientData)
{
    register PlFrame *plFramePtr = (PlFrame *) clientData;
    register Tk_Window tkwin = plFramePtr->tkwin;
    PLWindow window;

    dbug_enter("DisplayPlFrame");

/* Update scrollbars if needed */

    if (plFramePtr->flags & UPDATE_V_SCROLLBAR) {
	UpdateVScrollbar(plFramePtr);
    }
    if (plFramePtr->flags & UPDATE_H_SCROLLBAR) {
	UpdateHScrollbar(plFramePtr);
    }
    plFramePtr->flags &= ~(UPDATE_V_SCROLLBAR|UPDATE_H_SCROLLBAR);

/* If not mapped yet, return and cancel pending refresh */

    if ((plFramePtr->tkwin == NULL) || !Tk_IsMapped(tkwin)) {
	plFramePtr->flags &= ~REFRESH_PENDING;
	return;
    }

/* Clear window if needed */

    if (plFramePtr->flags & CLEAR_NEEDED) {
	XClearWindow(plFramePtr->display, Tk_WindowId(tkwin));
	plFramePtr->flags &= ~CLEAR_NEEDED;
    }

/* Redraw border if necessary */

    if ((plFramePtr->border != NULL)
	    && (plFramePtr->relief != TK_RELIEF_FLAT)) {
	Tk_Draw3DRectangle(plFramePtr->display, Tk_WindowId(tkwin),
		plFramePtr->border, 0, 0, Tk_Width(tkwin), Tk_Height(tkwin),
		plFramePtr->borderWidth, plFramePtr->relief);
    }

/* If plplot not initialized yet, return and cancel pending refresh */

    if ( ! plFramePtr->plplot_initted) {
	plFramePtr->flags &= ~REFRESH_PENDING;
	return;
    }

/* All refresh events */

    if (plFramePtr->flags & REFRESH_PENDING) {
	plFramePtr->flags &= ~REFRESH_PENDING;

/* Reschedule resizes to avoid occasional ordering conflicts with */
/* the packer's resize of the window (this call must come last). */

	if (plFramePtr->flags & RESIZE_PENDING) {
	    plFramePtr->flags |= REFRESH_PENDING;
	    plFramePtr->flags &= ~RESIZE_PENDING;
	    Tk_DoWhenIdle(DisplayPlFrame, clientData);
	    return;
	}

/* Redraw -- replay contents of plot buffer */

	if (plFramePtr->flags & REDRAW_PENDING) {
	    plFramePtr->flags &= ~REDRAW_PENDING;
	    pl_cmd(PLESC_REDRAW, (void *) NULL);
	    return;
	}

/* Resize -- if window bounds have changed */

	if ((plFramePtr->prevWidth != Tk_Width(tkwin)) ||
	    (plFramePtr->prevHeight != Tk_Height(tkwin))) {

	    window.width = Tk_Width(tkwin);
	    window.height = Tk_Height(tkwin);

	    pl_cmd(PLESC_RESIZE, (void *) &window);
	    plFramePtr->prevWidth = Tk_Width(tkwin);
	    plFramePtr->prevHeight = Tk_Height(tkwin);
	}

/* Expose -- if window bounds are unchanged */

	else {
	    pl_cmd(PLESC_EXPOSE, NULL);
	}
    }
}

/*----------------------------------------------------------------------*\
* Routines to process widget commands.
\*----------------------------------------------------------------------*/

/*----------------------------------------------------------------------*\
* Cmd
*
* Processes "cmd" widget command.
* Handles commands that go more or less directly to the plplot library.
* This function will probably become much larger, as additional direct
* plplot commands are supported.
\*----------------------------------------------------------------------*/

static int
Cmd(Tcl_Interp *interp, register PlFrame *plFramePtr,
    int argc, char **argv)
{
    int length;
    char c;
    int result = TCL_OK;

/* no option -- return list of available plplot commands */

    if (argc == 0) {
	Tcl_SetResult(interp, "init setopt", TCL_VOLATILE);
	return TCL_OK;
    }

    c = argv[0][0];
    length = strlen(argv[0]);

/* init -- starts the library up and advances to the first page */

    if ((c == 'i') && (strncmp(argv[0], "init", length) == 0)) {
	if (argc > 1) {
	    Tcl_AppendResult(interp, "wrong # args: should be \"",
		    argv[0], (char *) NULL);
	    result = TCL_ERROR;
	}
	else if ( ! plFramePtr->tkwin_initted) {
	    Tcl_AppendResult(interp, "widget creation must precede plplot",
		    "init command", (char *) NULL);
	    result = TCL_ERROR;
	}
	else {
	    plFramePtr->plplot_initted = 1;
	    plinit();
	    pladv(0);
	}
    }

/* setopt -- set a plplot option (command-line syntax) */
/* Just calls plSetInternalOpt() */

    else if ((c == 's') && (strncmp(argv[0], "setopt", length) == 0)) {
	if (argc < 2 || argc > 3) {
	    Tcl_AppendResult(interp, "wrong # args: should be \"",
		    argv[0], " option ?argument?\"", (char *) NULL);
	    result = TCL_ERROR;
	}
	else {
#ifdef DEBUG
	    fprintf(stderr, "Processing command: %s\n", argv[1]);
	    if (argv[2] != NULL)
		fprintf(stderr, "Processing option: %s\n", argv[2]);
#endif
	    plSetInternalOpt(argv[1], argv[2]);
	}
    }

/* unrecognized */

    else {
	Tcl_AppendResult(interp, "bad option to \"cmd\": must be ", 
	 "init or setopt", (char *) NULL);

	result = TCL_ERROR;
    }

    return result;
}

/*
 *---------------------------------------------------------------------------
 *
 * ConfigurePlFrame --
 *
 *	This procedure is called to process an argv/argc list, plus the Tk
 *	option database, in order to configure (or reconfigure) a 
 *	plframe widget.
 *
 * Results:
 *	The return value is a standard Tcl result.  If TCL_ERROR is
 *	returned, then interp->result contains an error message.
 *
 * Side effects:
 *	Configuration information, such as text string, colors, font, etc.
 *	get set for plFramePtr; old resources get freed, if there were
 *	any.
 *
 *---------------------------------------------------------------------------
 */

static int
ConfigurePlFrame(Tcl_Interp *interp, register PlFrame *plFramePtr,
		 int argc, char **argv, int flags)
{
    PLColor plbg;

#ifdef DEBUG
    int i;
    fprintf(stderr, "Arguments to configure are:");
    for (i = 0; i < argc; i++) {
	fprintf(stderr, " %s", argv[i]);
    }
    fprintf(stderr, "\n");
#endif

    dbug_enter("ConfigurePlFrame");

    if (Tk_ConfigureWidget(interp, plFramePtr->tkwin, configSpecs,
	    argc, argv, (char *) plFramePtr, flags) != TCL_OK) {
	return TCL_ERROR;
    }

/* Set background color.  Need to store in the plplot stream structure */
/* since redraws are handled from the plplot X driver. */

    Tk_SetBackgroundFromBorder(plFramePtr->tkwin, plFramePtr->border);

    PLColor_from_XColor(&plbg, plFramePtr->bgColor);
    plscolbg(plbg.r, plbg.g, plbg.b);

    Tk_SetInternalBorder(plFramePtr->tkwin, plFramePtr->borderWidth);
    if (plFramePtr->geometry != NULL) {
	int height, width;

	if (sscanf(plFramePtr->geometry, "%dx%d", &width, &height) != 2) {
	    Tcl_AppendResult(interp, "bad geometry \"", plFramePtr->geometry,
		    "\": expected widthxheight", (char *) NULL);
	    return TCL_ERROR;
	}
	Tk_GeometryRequest(plFramePtr->tkwin, width, height);
    }
    else if ((plFramePtr->width > 0) && (plFramePtr->height > 0)) {
	Tk_GeometryRequest(plFramePtr->tkwin, plFramePtr->width,
		plFramePtr->height);
    }

    if (Tk_IsMapped(plFramePtr->tkwin)
	    && !(plFramePtr->flags & REFRESH_PENDING)) {
	Tk_DoWhenIdle(DisplayPlFrame, (ClientData) plFramePtr);
	plFramePtr->flags |= REFRESH_PENDING|CLEAR_NEEDED;
	plFramePtr->flags |= UPDATE_V_SCROLLBAR|UPDATE_H_SCROLLBAR;
    }
    return TCL_OK;
}

/*----------------------------------------------------------------------*\
* Draw
*
* Processes "draw" widget command.
* Handles rubber-band drawing.
\*----------------------------------------------------------------------*/

static int
Draw(Tcl_Interp *interp, register PlFrame *plFramePtr,
     int argc, char **argv)
{
    register Tk_Window tkwin = plFramePtr->tkwin;
    int result = TCL_OK;
    char c = argv[0][0];
    int length = strlen(argv[0]);

/* init -- sets up for rubber-band drawing */

    if ((c == 'i') && (strncmp(argv[0], "init", length) == 0)) {
	Tk_DefineCursor(tkwin, plFramePtr->draw_cursor);
    }

/* end -- ends rubber-band drawing */

    else if ((c == 'e') && (strncmp(argv[0], "end", length) == 0)) {

	Tk_DefineCursor(tkwin, plFramePtr->cursor);
	plFramePtr->continue_draw = 0;
    }

/* rect -- draw a rectangle, used to select rectangular areas */
/* first draw erases old outline */

    else if ((c == 'r') && (strncmp(argv[0], "rect", length) == 0)) {
	if (argc < 5) {
	    Tcl_AppendResult(interp, "wrong # args: should be \"",
			     " draw rect x0 y0 x1 y1\"", (char *) NULL);
	    result = TCL_ERROR;
	}
	else {
	    int x0, y0, x1, y1;
	    int xmin = 0, xmax = Tk_Width(tkwin) - 1;
	    int ymin = 0, ymax = Tk_Height(tkwin) - 1;

	    x0 = atoi(argv[1]);
	    y0 = atoi(argv[2]);
	    x1 = atoi(argv[3]);
	    y1 = atoi(argv[4]);

	    x0 = MAX(xmin, MIN(xmax, x0));
	    y0 = MAX(ymin, MIN(ymax, y0));
	    x1 = MAX(xmin, MIN(xmax, x1));
	    y1 = MAX(ymin, MIN(ymax, y1));

	    if (plFramePtr->continue_draw) {
		XDrawLines(Tk_Display(tkwin), Tk_WindowId(tkwin),
			   plFramePtr->xorGC, plFramePtr->pts, 5,
			   CoordModeOrigin);
		XSync(Tk_Display(tkwin), 0);
	    }

	    plFramePtr->pts[0].x = x0; plFramePtr->pts[0].y = y0;
	    plFramePtr->pts[1].x = x1; plFramePtr->pts[1].y = y0;
	    plFramePtr->pts[2].x = x1; plFramePtr->pts[2].y = y1;
	    plFramePtr->pts[3].x = x0; plFramePtr->pts[3].y = y1;
	    plFramePtr->pts[4].x = x0; plFramePtr->pts[4].y = y0;

	    XDrawLines(Tk_Display(tkwin), Tk_WindowId(tkwin),
		       plFramePtr->xorGC, plFramePtr->pts, 5,
		       CoordModeOrigin);
	    XSync(Tk_Display(tkwin), 0);

	    plFramePtr->continue_draw = 1;
	}
    }

    return result;
}

/*----------------------------------------------------------------------*\
* Info
*
* Processes "info" widget command.
* Returns requested info.
\*----------------------------------------------------------------------*/

static int
Info(Tcl_Interp *interp, register PlFrame *plFramePtr,
     int argc, char **argv)
{
    int length;
    char c;
    int result = TCL_OK;

/* no option -- return list of available info commands */

    if (argc == 0) {
	Tcl_SetResult(interp, "devices", TCL_VOLATILE);
	return TCL_OK;
    }

    c = argv[0][0];
    length = strlen(argv[0]);

/* devices -- return list of supported device types */

    if ((c == 'd') && (strncmp(argv[0], "devices", length) == 0)) {
	int i = 0;
	while (plFramePtr->devDesc[i] != NULL) {
	    Tcl_AppendElement(interp, plFramePtr->devDesc[i++], 0);
	}
	result = TCL_OK;
    }

/* unrecognized */

    else {
	Tcl_AppendResult(interp, "bad option to \"info\": must be ", 
	 "devices", (char *) NULL);

	result = TCL_ERROR;
    }

    return result;
}

/*----------------------------------------------------------------------*\
* OpenFifo
*
* Processes "openfifo" widget command.
* Opens FIFO for data transfer between client and server.
\*----------------------------------------------------------------------*/

static int
OpenFifo(Tcl_Interp *interp, register PlFrame *plFramePtr,
	 int argc, char **argv)
{
    int fd;
    register PLRDev *plr = plFramePtr->plr;

    dbug_enter("OpenFifo");

    plr->filename = argv[0];

/* Open fifo */

    if ((fd = open (plr->filename, O_RDONLY)) == -1) {
	Tcl_AppendResult(interp, "cannot open fifo ", plr->filename,
			 " for read", (char *) NULL);
	return TCL_ERROR;
    }

    plr->file = fdopen(fd, "rb");
    plr->filetype = "fifo";

    return TCL_OK;
}

/*----------------------------------------------------------------------*\
* Orient
*
* Processes "orient" widget command.
* Handles orientation of plot.
\*----------------------------------------------------------------------*/

static int
Orient(Tcl_Interp *interp, register PlFrame *plFramePtr,
       int argc, char **argv)
{
    int result = TCL_OK;

/* orient -- return orientation of current plot window */

    if (argc == 0) {
	PLFLT rot;
	char result_str[128];
	plgdiori(&rot);
	sprintf(result_str, "%f", rot);
	Tcl_SetResult(interp, result_str, TCL_VOLATILE);
    }

/* orient <rot> -- Set orientation to <rot> */

    else {
	plsdiori(atof(argv[0]));
	result = Redraw(interp, plFramePtr, argc-1, argv+1);
    }

    return result;
}

/*----------------------------------------------------------------------*\
* Print
*
* Processes "print" widget command.
* Handles printing of plot, duh.
*
* Creates a temporary file, dumps the current plot to it in metafile form,
* and then execs the "plpr" script to actually print it.  Since we output
* it in metafile form here, plpr must invoke plrender to drive the output
* to the appropriate file type.  The script is responsible for the deletion
* of the plot metafile.
*
* This way of printing is reasonably flexible while retaining a measure
* of security.  Methods involving setting up the command to print from
* the Tcl side are not recommended at present since the print command
* could be changed to something dangerous (like an rm *).
\*----------------------------------------------------------------------*/

static int
Print(Tcl_Interp *interp, register PlFrame *plFramePtr,
       int argc, char **argv)
{
    PLINT ipls;
    int result = TCL_OK;
    char *sfnam;
    FILE *sfile;
    pid_t pid;

/* Create stream for save */

    plmkstrm(&ipls);
    if (ipls < 0) {
	Tcl_AppendResult(interp, "Error -- cannot create stream", 
			 (char *) NULL);
	return TCL_ERROR;
    }

/* Open file for writes */

    sfnam = (char *) tempnam((char *) NULL, "pltk");

    if ((sfile = fopen(sfnam, "wb+")) == NULL) {
	Tcl_AppendResult(interp, 
			 "Error -- cannot plot file for writing",
			 (char *) NULL);
	plend1();
	return TCL_ERROR;
    }

/* Initialize stream */

    plsdev("plmeta");
    plsfile(sfile);
    plinit();
    pladv(0);
    plcpstrm(plFramePtr->ipls, 0);

/* Remake current plot, close file, and switch back to original stream */

    plreplot();
    plend1();
    plsstrm(plFramePtr->ipls);

/* So far so good.  Time to exec the print script. */

    if ( (pid = fork()) < 0) {
	Tcl_AppendResult(interp, 
			 "Error -- cannot fork print process",
			 (char *) NULL);
	result = TCL_ERROR;
    }
    else if (pid == 0) {
	if (execlp("plpr", "plpr", sfnam, (char *) 0)) {
	    fprintf(stderr, "execlp error.\n");
	    _exit(1);
	}
    }

    return result;
}

/*----------------------------------------------------------------------*\
* Page
*
* Processes "page" widget command.
* Handles parameters such as margin, aspect ratio, and justification
* of final plot.
\*----------------------------------------------------------------------*/

static int
Page(Tcl_Interp *interp, register PlFrame *plFramePtr,
     int argc, char **argv)
{

/* page -- return current device window parameters */

    if (argc == 0) {
	PLFLT mar, aspect, jx, jy;
	char result_str[128];

	plgdidev(&mar, &aspect, &jx, &jy);
	sprintf(result_str, "%g %g %g %g", mar, aspect, jx, jy);
	Tcl_SetResult(interp, result_str, TCL_VOLATILE);
	return TCL_OK;
    }

/* page <mar> <aspect> <jx> <jy> -- set up page */

    if (argc < 4) {
	Tcl_AppendResult(interp, "wrong # args: should be \"",
			 " page mar aspect jx jy\"", (char *) NULL);
	return TCL_ERROR;
    } 

    plsdidev(atof(argv[0]), atof(argv[1]), atof(argv[2]), atof(argv[3]));
    return(Redraw(interp, plFramePtr, argc-1, argv+1));
}

/*----------------------------------------------------------------------*\
* ReadData
*
* Processes "read" widget command.
* Reads data from fifo or socket and processes.
\*----------------------------------------------------------------------*/

static int
ReadData(Tcl_Interp *interp, register PlFrame *plFramePtr,
	 int argc, char **argv)
{
    register PLRDev *plr = plFramePtr->plr;
    int result = TCL_OK;

    plr->nbytes = atoi(argv[0]);

/* Read & process data */

#ifdef DEBUG
    fprintf(stderr, "%s: Reading %d characters\n", __FILE__, plr->nbytes);
    fflush(stderr);
#endif

    if (plr_process(plr) == -1) {
	Tcl_AppendResult(interp, "unable to read from ", plr->filetype,
			 (char *) NULL);
	result = TCL_ERROR;
    }
    return result;
}

/*----------------------------------------------------------------------*\
* Redraw
*
* Processes "redraw" widget command.
* Turns loose a DoWhenIdle command to redraw plot by replaying contents
* of plot buffer.
\*----------------------------------------------------------------------*/

static int
Redraw(Tcl_Interp *interp, register PlFrame *plFramePtr,
       int argc, char **argv)
{
    dbug_enter("Redraw");

    plFramePtr->flags |= REDRAW_PENDING;
    if ((plFramePtr->tkwin != NULL) &&
	!(plFramePtr->flags & REFRESH_PENDING)) {

	Tk_DoWhenIdle(DisplayPlFrame, (ClientData) plFramePtr);
	plFramePtr->flags |= REFRESH_PENDING;
    }

    return TCL_OK;
}

/*----------------------------------------------------------------------*\
* Save
*
* Processes "save" widget command.
* Saves plot to a file.
\*----------------------------------------------------------------------*/

static int
Save(Tcl_Interp *interp, register PlFrame *plFramePtr,
     int argc, char **argv)
{
    int length, idev;
    char c;
    FILE *sfile;

/* save -- save to already open file */

    if (argc == 0) {
	if ( ! plFramePtr->ipls_save) {
	    Tcl_AppendResult(interp, "Error -- no current save file", 
			     (char *) NULL);
	    return TCL_ERROR;
	}
	plsstrm(plFramePtr->ipls_save);
	plcpstrm(plFramePtr->ipls, 0);
	pladv(0);
	plreplot();
	plflush();
	plsstrm(plFramePtr->ipls);
#ifdef DEBUG
	fprintf(stderr, "Saved plot to file %s\n", plFramePtr->SaveFnam);
#endif
	return TCL_OK;
    }

    c = argv[0][0];
    length = strlen(argv[0]);

/* save to specified device & file */

    if ((c == 'a') && (strncmp(argv[0], "as", length) == 0)) {
	if (argc < 3) {
	    Tcl_AppendResult(interp, "wrong # args: should be \"",
			     " save as device file\"", (char *) NULL);
	    return TCL_ERROR;
	} 
	idev = atoi(argv[1]);
#ifdef DEBUG
	fprintf(stderr, "Trying to save to device %s file %s\n",
		plFramePtr->devName[idev], argv[2]);
#endif

/* If save previously in effect, delete old stream */

	if (plFramePtr->ipls_save) {
	    plsstrm(plFramePtr->ipls_save);
	    plend1();
	}

/* Create stream for saves to selected device & file */

	plmkstrm(&plFramePtr->ipls_save);
	if (plFramePtr->ipls_save < 0) {
	    Tcl_AppendResult(interp, "Error -- cannot create stream", 
			     (char *) NULL);
	    plFramePtr->ipls_save = 0;
	    return TCL_ERROR;
	}

/* Open file for writes */

	if ((sfile = fopen(argv[2], "wb+")) == NULL) {
	    Tcl_AppendResult(interp, "Error -- cannot open file ", argv[2],
			     " for writing", (char *) NULL);
	    plFramePtr->ipls_save = 0;
	    plend1();
	    return TCL_ERROR;
	}

/* Initialize stream */

	plsdev(plFramePtr->devName[idev]);
	plsfile(sfile);
	plinit();
	pladv(0);
	plcpstrm(plFramePtr->ipls, 0);

/* Remake current plot and then switch back to original stream */

	plreplot();
	plflush();
	plsstrm(plFramePtr->ipls);

/* Save file name */

#ifdef DEBUG
	if (plFramePtr->SaveFnam != NULL)
	    free((void *) plFramePtr->SaveFnam);

	plFramePtr->SaveFnam = (char *)
	    ckalloc(1 + strlen(argv[2]));

	strcpy(plFramePtr->SaveFnam, argv[2]);

	fprintf(stderr, "Saved plot to file %s\n", plFramePtr->SaveFnam);
#endif
    }

/* close save file */

    else if ((c == 'c') && (strncmp(argv[0], "close", length) == 0)) {

	if ( ! plFramePtr->ipls_save) {
	    Tcl_AppendResult(interp, "Error -- no current save file", 
			     (char *) NULL);
	    return TCL_ERROR;
	}
	else {
	    plsstrm(plFramePtr->ipls_save);
	    plend1();
	    plFramePtr->ipls_save = 0;
	}
    }

/* unrecognized */

    else {
	Tcl_AppendResult(interp, "bad option to \"save\": must be ", 
	 "as or close", (char *) NULL);

	return TCL_ERROR;
    }

    return TCL_OK;
}

/*----------------------------------------------------------------------*\
* View
*
* Processes "view" widget command.
* Handles translation & scaling of view into plot.
\*----------------------------------------------------------------------*/

static int
View(Tcl_Interp *interp, register PlFrame *plFramePtr,
     int argc, char **argv)
{
    int length;
    char c;
    PLFLT xl, xr, yl, yr;

/* view -- return current relative plot window coordinates */

    if (argc == 0) {
	char result_str[128];
	plgdiplt(&xl, &yl, &xr, &yr);
	sprintf(result_str, "%g %g %g %g", xl, yl, xr, yr);
	Tcl_SetResult(interp, result_str, TCL_VOLATILE);
	return TCL_OK;
    }

    c = argv[0][0];
    length = strlen(argv[0]);

/* view bounds -- return relative device coordinates of bounds on current */
/* plot window */

    if ((c == 'b') && (strncmp(argv[0], "bounds", length) == 0)) {
	char result_str[128];
	xl = 0.; yl = 0.;
	xr = 1.; yr = 1.;
	pldip2dc(&xl, &yl, &xr, &yr);
	sprintf(result_str, "%g %g %g %g", xl, yl, xr, yr);
	Tcl_SetResult(interp, result_str, TCL_VOLATILE);
	return TCL_OK;
    }

/* view reset -- Resets plot */

    if ((c == 'r') && (strncmp(argv[0], "reset", length) == 0)) {
	xl = 0.; yl = 0.;
	xr = 1.; yr = 1.;
	plsdiplt(xl, yl, xr, yr);
    }

/* view select -- set window into plot space */
/* Specifies in terms of plot window coordinates, not device coordinates */

    else if ((c == 's') && (strncmp(argv[0], "select", length) == 0)) {
	if (argc < 5) {
	    Tcl_AppendResult(interp, "wrong # args: should be \"",
			     " view select xmin ymin xmax ymax\"",
			     (char *) NULL);
	    return TCL_ERROR;
	}
	else {
	    gbox(&xl, &yl, &xr, &yr, argv+1);
	    plsdiplt(xl, yl, xr, yr);
	}
    }

/* view zoom -- set window into plot space incrementally (zoom) */
/* Here we need to take the page (device) offsets into account */

    else if ((c == 'z') && (strncmp(argv[0], "zoom", length) == 0)) {
	if (argc < 5) {
	    Tcl_AppendResult(interp, "wrong # args: should be \"",
			     " view zoom xmin ymin xmax ymax\"",
			     (char *) NULL);
	    return TCL_ERROR;
	}
	else {
	    gbox(&xl, &yl, &xr, &yr, argv+1);
	    pldid2pc(&xl, &yl, &xr, &yr);
	    plsdiplz(xl, yl, xr, yr);
	}
    }

/* unrecognized */

    else {
	Tcl_AppendResult(interp, "bad option \"", argv[1],
	 "\":  options to \"view\" are: bounds, reset, select, or zoom",
	 (char *) NULL);

	return TCL_ERROR;
    }

/* Update plot window bounds and arrange for plot to be updated */

    plgdiplt(&xl, &yl, &xr, &yr);
    plFramePtr->xl = xl;
    plFramePtr->yl = yl;
    plFramePtr->xr = xr;
    plFramePtr->yr = yr;
    plFramePtr->flags |= UPDATE_V_SCROLLBAR|UPDATE_H_SCROLLBAR;

    return(Redraw(interp, plFramePtr, argc, argv));
}

/*----------------------------------------------------------------------*\
* xScroll
*
* Processes "xscroll" widget command.
* Handles horizontal scroll-bar invoked translation of view into plot.
\*----------------------------------------------------------------------*/

static int
xScroll(Tcl_Interp *interp, register PlFrame *plFramePtr,
	int argc, char **argv)
{
    int x0, width = Tk_Width(plFramePtr->tkwin);
    PLFLT xl, xr, yl, yr, xlen;

    xlen = plFramePtr->xr - plFramePtr->xl;
    x0 = atoi(argv[0]);
    xl = x0 / (double) width;
    xl = MAX( 0., MIN((1. - xlen), xl));
    xr = xl + xlen;

    yl = plFramePtr->yl;
    yr = plFramePtr->yr;

    plFramePtr->xl = xl;
    plFramePtr->xr = xr;

    plsdiplt(xl, yl, xr, yr);

    plFramePtr->flags |= UPDATE_V_SCROLLBAR|UPDATE_H_SCROLLBAR;
    return(Redraw(interp, plFramePtr, argc, argv));
}

/*----------------------------------------------------------------------*\
* yScroll
*
* Processes "yscroll" widget command.
* Handles vertical scroll-bar invoked translation of view into plot.
\*----------------------------------------------------------------------*/

static int
yScroll(Tcl_Interp *interp, register PlFrame *plFramePtr,
	int argc, char **argv)
{
    int y0, height = Tk_Height(plFramePtr->tkwin);
    PLFLT xl, xr, yl, yr, ylen;

    ylen = plFramePtr->yr - plFramePtr->yl;
    y0 = atoi(argv[0]);
    yr = 1. - y0 / (double) height;
    yr = MAX( 0.+ylen, MIN(1., yr));
    yl = yr - ylen;

    xl = plFramePtr->xl;
    xr = plFramePtr->xr;

    plFramePtr->yl = yl;
    plFramePtr->yr = yr;

    plsdiplt(xl, yl, xr, yr);

    plFramePtr->flags |= UPDATE_V_SCROLLBAR|UPDATE_H_SCROLLBAR;
    return(Redraw(interp, plFramePtr, argc, argv));
}

/*----------------------------------------------------------------------*\
* Utility routines
\*----------------------------------------------------------------------*/

/*----------------------------------------------------------------------*\
* UpdateVScrollbar
*
* Updates vertical scrollbar if needed.
\*----------------------------------------------------------------------*/

static void
UpdateVScrollbar(register PlFrame *plFramePtr)
{
    int height = Tk_Height(plFramePtr->tkwin);
    char string[60];
    int totalUnits, windowUnits, firstUnit, lastUnit, result;

    if (plFramePtr->yScrollCmd == NULL)
	return;

    totalUnits  = height;
    firstUnit   = 0.5 + (float) height * (1. - plFramePtr->yr);
    lastUnit    = 0.5 + (float) height * (1. - plFramePtr->yl);
    windowUnits = lastUnit - firstUnit;
    sprintf(string, " %d %d %d %d",
	    totalUnits, windowUnits, firstUnit, lastUnit);

    result = Tcl_VarEval(plFramePtr->interp, plFramePtr->yScrollCmd, string,
			 (char *) NULL);

    if (result != TCL_OK) {
	Tk_BackgroundError(plFramePtr->interp);
    }
}

/*----------------------------------------------------------------------*\
* UpdateHScrollbar
*
* Updates horizontal scrollbar if needed.
\*----------------------------------------------------------------------*/

static void
UpdateHScrollbar(register PlFrame *plFramePtr)
{
    int width = Tk_Width(plFramePtr->tkwin);
    char string[60];
    int totalUnits, windowUnits, firstUnit, lastUnit, result;

    if (plFramePtr->xScrollCmd == NULL)
	return;

    totalUnits  = width;
    firstUnit   = 0.5 + (float) width * plFramePtr->xl;
    lastUnit    = 0.5 + (float) width * plFramePtr->xr;
    windowUnits = lastUnit - firstUnit;
    sprintf(string, " %d %d %d %d",
	    totalUnits, windowUnits, firstUnit, lastUnit);

    result = Tcl_VarEval(plFramePtr->interp, plFramePtr->xScrollCmd, string,
			 (char *) NULL);

    if (result != TCL_OK) {
	Tk_BackgroundError(plFramePtr->interp);
    }
}

/*----------------------------------------------------------------------*\
* gbox
*
* Returns selection box coordinates.
* It's best if the TCL script does bounds checking on the input but I do
* it here as well just to be safe.
\*----------------------------------------------------------------------*/

static void
gbox(PLFLT *xl, PLFLT *yl, PLFLT *xr, PLFLT *yr, char **argv)
{
    float x0, y0, x1, y1;

    x0 = atof(argv[0]);
    y0 = atof(argv[1]);
    x1 = atof(argv[2]);
    y1 = atof(argv[3]);

    x0 = MAX(0., MIN(1., x0));
    y0 = MAX(0., MIN(1., y0));
    x1 = MAX(0., MIN(1., x1));
    y1 = MAX(0., MIN(1., y1));

/* Only need two vertices, pick the lower left and upper right */

    *xl = MIN(x0, x1);
    *yl = MIN(y0, y1);
    *xr = MAX(x0, x1);
    *yr = MAX(y0, y1);
}

/*----------------------------------------------------------------------*\
* client_cmd
*
* Sends specified command to client, aborting on an error.
\*----------------------------------------------------------------------*/

static int
client_cmd(register PlFrame *plFramePtr, char *cmd)
{
    dbug_enter("client_cmd");

    if (plFramePtr->client != NULL) {

	Tcl_SetVar(plFramePtr->interp, "client", plFramePtr->client, 0);
	if (tcl_eval(plFramePtr, "winfo $client exists")) {
#ifdef DEBUG_ENTER
	    fprintf(stderr, "Sending command: %s to %s\n", cmd,
		    plFramePtr->client);
#endif
	    return(Tcl_VarEval(plFramePtr->interp,
			       "send $client ", cmd, (char **) NULL));
	}
    }
    Tcl_AppendResult(plFramePtr->interp, "client", plFramePtr->client,
		     " does not exist", (char *) NULL);
    return TCL_ERROR;
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
tcl_eval(PlFrame *plFramePtr, char *cmd)
{
    if (cmdbuf == NULL) 
	cmdbuf = (char *) malloc(cmdbuf_len);

    if (strlen(cmd) >= cmdbuf_len) {
	free((void *) cmdbuf);
	cmdbuf_len = strlen(cmd) + 20;
	cmdbuf = (char *) malloc(cmdbuf_len);
    }

    strcpy(cmdbuf, cmd);
    return(Tcl_Eval(plFramePtr->interp, cmdbuf, 0, (char **) NULL));
}

/*
 *---------------------------------------------------------------------------
 *
 * MapPlFrame --
 *
 *	This procedure is invoked as a when-idle handler to map a
 *	newly-created top-level frame.  Since plplot widgets in
 *	practice never get created at top-level, this routine is
 *	never called.
 *
 * Results:
 *	None.
 *
 * Side effects:
 *	The plframe given by the clientData argument is mapped.
 *
 *---------------------------------------------------------------------------
 */

static void
MapPlFrame(ClientData clientData)
{
    PlFrame *plFramePtr = (PlFrame *) clientData;

    dbug_enter("MapPlFrame");

    /*
     * Wait for all other background events to be processed before
     * mapping window.  This ensures that the window's correct geometry
     * will have been determined before it is first mapped, so that the
     * window manager doesn't get a false idea of its desired geometry.
     */

    Tk_Preserve((ClientData) plFramePtr);
    while (1) {
	if (Tk_DoOneEvent(TK_IDLE_EVENTS) == 0) {
	    break;
	}

	/*
	 * After each event, make sure that the window still exists
	 * and quit if the window has been destroyed.
	 */

	if (plFramePtr->tkwin == NULL) {
	    Tk_Release((ClientData) plFramePtr);
	    return;
	}
    }
    Tk_MapWindow(plFramePtr->tkwin);
    Tk_Release((ClientData) plFramePtr);
}

