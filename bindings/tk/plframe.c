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
#include "default.h"
#include "tkConfig.h"
#include "tk.h"

#define plframe_cmd(code) \
    if ((code) == TCL_ERROR) return (TCL_ERROR);

/* Externals */

int   plFrameCmd     	(ClientData, Tcl_Interp *, int, char **);

/*
 * A data structure of the following type is kept for each
 * plframe that currently exists for this process:
 */

typedef struct {
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

    /* The rest are all additions to frame */

    char *client;		/* Client main window.  Malloc'ed. */
    PLRDev *plr;		/* Renderer state information.  Malloc'ed */
    PLStream *pls;		/* Plplot internal state structure */

    int prevWidth;		/* Previous window width */
    int prevHeight;		/* Previous window height */
    GC xorGC;			/* GC used for rubber-band drawing */
    XPoint pts[5];		/* Points for rubber-band drawing */
    int continue_draw;		/* Set when doing rubber-band draws */
    Cursor draw_cursor;		/* cursor used for drawing */
} PlFrame;

/*
 * Flag bits for plframes:
 *
 * REDRAW_PENDING:		Non-zero means a DoWhenIdle handler
 *				has already been queued to refresh
 *				this window.
 * CLEAR_NEEDED;		Need to clear the window when redrawing.
 * RESIZE_PENDING;		Used to reschedule resize events
 */

#define REDRAW_PENDING		1
#define CLEAR_NEEDED		2
#define RESIZE_PENDING		4

/* Most of the configuration info is shared with frame widgets */

static Tk_ConfigSpec configSpecs[] = {
    {TK_CONFIG_BORDER, "-background", "background", "Background",
	DEF_FRAME_BG_COLOR, Tk_Offset(PlFrame, border), TK_CONFIG_COLOR_ONLY},
    {TK_CONFIG_BORDER, "-background", "background", "Background",
	DEF_FRAME_BG_MONO, Tk_Offset(PlFrame, border), TK_CONFIG_MONO_ONLY},
    {TK_CONFIG_SYNONYM, "-bd", "borderWidth", (char *) NULL,
	(char *) NULL, 0, 0},
    {TK_CONFIG_SYNONYM, "-bg", "background", (char *) NULL,
	(char *) NULL, 0, 0},
    {TK_CONFIG_PIXELS, "-borderwidth", "borderWidth", "BorderWidth",
	DEF_FRAME_BORDER_WIDTH, Tk_Offset(PlFrame, borderWidth), 0},
    {TK_CONFIG_ACTIVE_CURSOR, "-cursor", "cursor", "Cursor",
	DEF_FRAME_CURSOR, Tk_Offset(PlFrame, cursor), TK_CONFIG_NULL_OK},
    {TK_CONFIG_STRING, "-geometry", "geometry", "Geometry",
	DEF_FRAME_GEOMETRY, Tk_Offset(PlFrame, geometry), TK_CONFIG_NULL_OK},
    {TK_CONFIG_PIXELS, "-height", "height", "Height",
	DEF_FRAME_HEIGHT, Tk_Offset(PlFrame, height), 0},
    {TK_CONFIG_RELIEF, "-relief", "relief", "Relief",
	DEF_FRAME_RELIEF, Tk_Offset(PlFrame, relief), 0},
    {TK_CONFIG_PIXELS, "-width", "width", "Width",
	DEF_FRAME_WIDTH, Tk_Offset(PlFrame, width), 0},
    {TK_CONFIG_END, (char *) NULL, (char *) NULL, (char *) NULL,
	(char *) NULL, 0, 0}
};

/*
 * Forward declarations for procedures defined later in this file:
 */

static int   ConfigurePlFrame	(Tcl_Interp *, PlFrame *, int, char **, int);
static void  DestroyPlFrame	(ClientData);
static void  DisplayPlFrame	(ClientData);
static void  PlFrameInit	(ClientData);
static void  PlFrameEventProc	(ClientData, XEvent *);
static int   PlFrameWidgetCmd	(ClientData, Tcl_Interp *, int, char **);
static void  MapPlFrame 	(ClientData);
static int   tcl_eval		(PlFrame *, char *);
static int   client_cmd		(PlFrame *, char *);

static int   OpenFifo		(Tcl_Interp *, PlFrame *, int, char **);
static int   ReadData		(Tcl_Interp *, PlFrame *, int, char **);
static int   DumpPlot		(Tcl_Interp *, PlFrame *, int, char **);
static int   ViewPlot		(Tcl_Interp *, PlFrame *, int, char **);
static void  ScalePlot		(PlFrame *, int, int, int, int, int, int);
static void  ResetPlot		(PlFrame *);

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
	} else if ((argv[0][0] == 't') && (c == 's')
		&& (strncmp(argv[src], "-screen", strlen(argv[src])) == 0)) {
	    screen = argv[src+1];
	} else {
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
    } else {
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
    int i;
    char c;

    dbug_enter("PlFrameWidgetCmd");
#ifdef DEBUG_ENTER
    fprintf(stderr, "Arguments are:");
    for (i = 0; i < argc; i++) {
	fprintf(stderr, " %s", argv[i]);
    }
    fprintf(stderr, "\n");
#endif

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
	} else {
	    if (plFramePtr->client != NULL) {
		ckfree((char *) plFramePtr->client);
		plFramePtr->client = NULL;
	    }
	    plFramePtr->client = ckalloc(strlen(argv[2]+1) * sizeof(char));
	    strcpy(plFramePtr->client, argv[2]);
	    result = TCL_OK;
	}
    }

/* configure */

    else if ((c == 'c') && (strncmp(argv[1], "configure", length) == 0)) {
	if (argc == 2) {
	    result = Tk_ConfigureInfo(interp, plFramePtr->tkwin, configSpecs,
		    (char *) plFramePtr, (char *) NULL, 0);
	} else if (argc == 3) {
	    result = Tk_ConfigureInfo(interp, plFramePtr->tkwin, configSpecs,
		    (char *) plFramePtr, argv[2], 0);
	} else {
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
	} else {
	    if (plFramePtr->client != NULL) {
		ckfree((char *) plFramePtr->client);
		plFramePtr->client = NULL;
	    }
	    result = TCL_OK;
	}
    }

/* dump -- dumps plot to the specified plot file type */

    else if ((c == 'd') && (strncmp(argv[1], "dump", length) == 0)) {
	if (argc == 2) {
	    Tcl_AppendResult(interp, "wrong # args: should be \"",
		    argv[0], " dump ?options?\"", (char *) NULL);
	    result = TCL_ERROR;
	    goto done;
	} else {
	    result = DumpPlot(interp, plFramePtr, argc-2, argv+2);
	}
    }

/* openfifo -- Open the specified fifo */

    else if ((c == 'o') && (strncmp(argv[1], "openfifo", length) == 0)) {
	if (argc == 2 || argc > 3) {
	    Tcl_AppendResult(interp, "wrong # args: should be \"",
		    argv[0], " openfifo fifoname\"", (char *) NULL);
	    result = TCL_ERROR;
	    goto done;
	} else {
	    result = OpenFifo(interp, plFramePtr, argc-2, argv+2);
	}
    }

/* read -- read <nbytes> of data from fifo or socket */

    else if ((c == 'r') && (strncmp(argv[1], "read", length) == 0)) {
	if (argc == 2 || argc > 3) {
	    Tcl_AppendResult(interp, "wrong # args: should be \"",
		    argv[0], " read nbytes\"", (char *) NULL);
	    result = TCL_ERROR;
	    goto done;
	} else {
	    result = ReadData(interp, plFramePtr, argc-2, argv+2);
	}
    }

/* view -- starts area selection and displays plot when finished */
/* can be used with either zooms or pans */

    else if ((c == 'v') && (strncmp(argv[1], "view", length) == 0)) {
	if (argc == 2) {
	    Tcl_AppendResult(interp, "wrong # args: should be \"",
		    argv[0], " view ?options?\"", (char *) NULL);
	    result = TCL_ERROR;
	    goto done;
	} else {
	    result = ViewPlot(interp, plFramePtr, argc-2, argv+2);
	}
    }

/* unrecognized widget command */

    else {
	Tcl_AppendResult(interp, "bad option \"", argv[1],
	 "\":  must be attach, configure, detach, dump, openfifo, read, ",
	 "or view", (char *) NULL);
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
    int i=0, j=0;

    dbug_enter("ConfigurePlFrame");
#ifdef DEBUG_ENTER
    fprintf(stderr, "Arguments are:");
    for (i = 0; i < argc; i++) {
	fprintf(stderr, " %s", argv[i]);
    }
    fprintf(stderr, "\n");
#endif

    if (Tk_ConfigureWidget(interp, plFramePtr->tkwin, configSpecs,
	    argc, argv, (char *) plFramePtr, flags) != TCL_OK) {
	return TCL_ERROR;
    }

    Tk_SetBackgroundFromBorder(plFramePtr->tkwin, plFramePtr->border);
    Tk_SetInternalBorder(plFramePtr->tkwin, plFramePtr->borderWidth);
    if (plFramePtr->geometry != NULL) {
	int height, width;

	if (sscanf(plFramePtr->geometry, "%dx%d", &width, &height) != 2) {
	    Tcl_AppendResult(interp, "bad geometry \"", plFramePtr->geometry,
		    "\": expected widthxheight", (char *) NULL);
	    return TCL_ERROR;
	}
	Tk_GeometryRequest(plFramePtr->tkwin, width, height);
    } else if ((plFramePtr->width > 0) && (plFramePtr->height > 0)) {
	Tk_GeometryRequest(plFramePtr->tkwin, plFramePtr->width,
		plFramePtr->height);
    }

    if (Tk_IsMapped(plFramePtr->tkwin)
	    && !(plFramePtr->flags & REDRAW_PENDING)) {
	Tk_DoWhenIdle(DisplayPlFrame, (ClientData) plFramePtr);
	plFramePtr->flags |= REDRAW_PENDING|CLEAR_NEEDED;
    }
    return TCL_OK;
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
		!(plFramePtr->flags & REDRAW_PENDING)) {

		Tk_DoWhenIdle(DisplayPlFrame, (ClientData) plFramePtr);
		plFramePtr->flags |= REDRAW_PENDING;
	    }
	}
	break;

    case ConfigureNotify:
	plFramePtr->flags |= RESIZE_PENDING;
	if ((plFramePtr->tkwin != NULL) &&
	    !(plFramePtr->flags & REDRAW_PENDING)) {

	    Tk_DoWhenIdle(DisplayPlFrame, (ClientData) plFramePtr);
	    plFramePtr->flags |= REDRAW_PENDING;
	}
	break;

    case DestroyNotify:
	Tcl_DeleteCommand(plFramePtr->interp, Tk_PathName(plFramePtr->tkwin));
	plFramePtr->tkwin = NULL;
	if (plFramePtr->flags & REDRAW_PENDING) {
	    Tk_CancelIdleCall(DisplayPlFrame, (ClientData) plFramePtr);
	}
	Tk_CancelIdleCall(MapPlFrame, (ClientData) plFramePtr);
	Tk_EventuallyFree((ClientData) plFramePtr, DestroyPlFrame);
	break;

    case MapNotify:
	if (plFramePtr->flags & REDRAW_PENDING) {
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
 *	Client is also told to proceed here.
 *
 *---------------------------------------------------------------------------
 */

static void
PlFrameInit(ClientData clientData)
{
    register PlFrame *plFramePtr = (PlFrame *) clientData;
    register Tk_Window tkwin = plFramePtr->tkwin;

/* Plplot initialization */

    plsdev("xwin");
    plsxwin(Tk_WindowId(tkwin));
    plspause(0);
    plinit();
    plgpls(&(plFramePtr->pls));
    pladv(0);

/* Draw plframe */

    plFramePtr->prevWidth = Tk_Width(tkwin);
    plFramePtr->prevHeight = Tk_Height(tkwin);
    plFramePtr->flags &= REDRAW_PENDING;
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

/* If not mapped yet, return and cancel pending redraws */

    if ((plFramePtr->tkwin == NULL) || !Tk_IsMapped(tkwin)) {
	plFramePtr->flags &= ~REDRAW_PENDING;
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

/* Process exposures and resizes */

    if (plFramePtr->flags & REDRAW_PENDING) {

/* Reschedule resizes to avoid occasional ordering conflicts with */
/* the packer's resize of the window (this call must come last). */

	if (plFramePtr->flags & RESIZE_PENDING) {
	    plFramePtr->flags &= ~RESIZE_PENDING;
	    Tk_DoWhenIdle(DisplayPlFrame, clientData);
	    return;
	}

/* Do a resize if window bounds have changed, otherwise an expose */

	if ((plFramePtr->prevWidth != Tk_Width(tkwin)) ||
	    (plFramePtr->prevHeight != Tk_Height(tkwin))) {

	    window.width = Tk_Width(tkwin);
	    window.height = Tk_Height(tkwin);

	    pl_cmd(PL_RESIZE, (void *) &window);
	    plFramePtr->prevWidth = Tk_Width(tkwin);
	    plFramePtr->prevHeight = Tk_Height(tkwin);
	}
	else {
	    pl_cmd(PL_EXPOSE, NULL);
	}
	plFramePtr->flags &= ~REDRAW_PENDING;
    }
}

/*----------------------------------------------------------------------*\
* OpenFifo
*
* Open FIFO to transfer data from client to server.
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

    if ((fd = open (plr->filename, O_RDONLY | O_NONBLOCK)) == -1) {
	Tcl_AppendResult(interp, "cannot open fifo ", plr->filename,
			 " for read", (char *) NULL);
	return TCL_ERROR;
    }

    plr->file = fdopen(fd, "rb");
    plr->filetype = "fifo";

    return TCL_OK;
}

/*----------------------------------------------------------------------*\
* ReadData
*
* Reads data from fifo or socket and issues messages about it.
\*----------------------------------------------------------------------*/

static int
ReadData(Tcl_Interp *interp, register PlFrame *plFramePtr,
	 int argc, char **argv)
{
    register PLRDev *plr = plFramePtr->plr;
    int i, nbytes, count;

    plr->nbytes = atoi(argv[0]);

/* Read & process data */

#ifdef DEBUG
    fprintf(stderr, "%s: Reading %d characters\n", __FILE__, plr->nbytes);
    fflush(stderr);
#endif

    if (plr_process(plr) == -1) {
	Tcl_AppendResult(interp, "unable to read from ", plr->filetype,
			 (char *) NULL);
	return TCL_ERROR;
    }
}

/*----------------------------------------------------------------------*\
* DumpPlot
*
* Dumps plot to a file.
\*----------------------------------------------------------------------*/

static int
DumpPlot(Tcl_Interp *interp, register PlFrame *plFramePtr,
	 int argc, char **argv)
{
    register PLStream *pls = plFramePtr->pls;
    register Tk_Window tkwin = plFramePtr->tkwin;

    int result = TCL_OK;
    char c = argv[0][0];
    int length = strlen(argv[0]);
    int lx = Tk_Width(tkwin), ly = Tk_Height(tkwin);
    int xmin = 0, xmax = lx - 1, ymin = 0, ymax = ly - 1;
    int x0, y0, x1, y1;

/* list -- spits out list of supported device types */

    if ((c == 'l') && (strncmp(argv[0], "list", length) == 0)) {
    }

/* anything else -- assume it is a device name */
/* must be accompanied by a file name */

    else {
    }

    return result;
}

/*----------------------------------------------------------------------*\
* ViewPlot
*
* Handles translation & scaling of view into plot.
* Also supports rubber-band drawing of the outline.
\*----------------------------------------------------------------------*/

static int
ViewPlot(Tcl_Interp *interp, register PlFrame *plFramePtr,
	 int argc, char **argv)
{
    register PLStream *pls = plFramePtr->pls;
    register Tk_Window tkwin = plFramePtr->tkwin;

    int result = TCL_OK;
    char c = argv[0][0];
    int length = strlen(argv[0]);
    int width = Tk_Width(tkwin), height = Tk_Height(tkwin);
    int xmin = 0, xmax = width - 1, ymin = 0, ymax = height - 1;
    int x0, y0, x1, y1;

/* init -- sets up for rubber-band drawing */

    if ((c == 'i') && (strncmp(argv[0], "init", length) == 0)) {
	Tk_DefineCursor(tkwin, plFramePtr->draw_cursor);
    }

/* draw -- rubber-band draw around currently selected area */
/* first draw erases old outline */

    else if ((c == 'd') && (strncmp(argv[0], "draw", length) == 0)) {
	if (argc < 5) {
	    Tcl_AppendResult(interp, "wrong # args: should be \"",
			     " view draw x0 y0 x1 y1\"", (char *) NULL);
	    result = TCL_ERROR;
	} else {
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

/* reset -- Resets plot */

    else if ((c == 'r') && (strncmp(argv[0], "reset", length) == 0)) {
	ResetPlot(plFramePtr);
    }

/* select -- Redraws plot for selected view */

    else if ((c == 's') && (strncmp(argv[0], "select", length) == 0)) {
	Tk_DefineCursor(tkwin, plFramePtr->cursor);
	if (argc < 5) {
	    Tcl_AppendResult(interp, "wrong # args: should be \"",
			     " view select x0 y0 x1 y1\"", (char *) NULL);
	    result = TCL_ERROR;
	} else {
	    x0 = atoi(argv[1]);
	    y0 = atoi(argv[2]);
	    x1 = atoi(argv[3]);
	    y1 = atoi(argv[4]);

	    x0 = MAX(xmin, MIN(xmax, x0));
	    y0 = MAX(ymin, MIN(ymax, y0));
	    x1 = MAX(xmin, MIN(xmax, x1));
	    y1 = MAX(ymin, MIN(ymax, y1));

	    ScalePlot(plFramePtr, x0, y0, x1, y1, width, height);

	    plFramePtr->continue_draw = 0;
	}
    }

    return result;
}

/*----------------------------------------------------------------------*\
* ScalePlot
*
* Redraws plot in the specified view (X coordinates).
\*----------------------------------------------------------------------*/

static void
ScalePlot(register PlFrame *plFramePtr, int x0, int y0, int x1, int y1,
	  int width, int height)
{
    register PLStream *pls = plFramePtr->pls;
    register Tk_Window tkwin = plFramePtr->tkwin;
    register PLRDev *plr = plFramePtr->plr;

    PLINT xmin, xmax, ymin, ymax;
    double x0p, x1p, y0p, y1p, xl, xr, yl, yr, lx, ly;

/* Convert to relative coordinates and shift origin to lower left */

    x0p = (double) x0 / (double) width;
    x1p = (double) x1 / (double) width;
    y0p = 1. - (double) y0 / (double) height;
    y1p = 1. - (double) y1 / (double) height;

/* Only need two vertices, pick the lower left and upper right */

    xl = MIN(x0p, x1p);
    xr = MAX(x0p, x1p);
    yl = MIN(y0p, y1p);
    yr = MAX(y0p, y1p);

    plsdiplz(xl, yl, xr, yr);

/* Issue plot command */

    pl_cmd(PL_REDRAW, NULL);
}

/*----------------------------------------------------------------------*\
* ResetPlot
*
* Restores plot to original view.
\*----------------------------------------------------------------------*/

static void
ResetPlot(register PlFrame *plFramePtr)
{
    plsdiplt(0., 0., 1., 1.);

    pl_cmd(PL_REDRAW, NULL);
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
* A front-end to Tcl_Eval just to make it easier to use here.
\*----------------------------------------------------------------------*/

static int
tcl_eval(PlFrame *plFramePtr, char *cmd)
{
    return(Tcl_Eval(plFramePtr->interp, cmd, 0, (char **) NULL));
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

