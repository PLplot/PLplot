/* $Id$
 * $Log$
 * Revision 1.62.4.1  2000/07/28 17:48:10  airwin
 * AWI: pltk patch changes to existing files
 *
 * Revision 1.60  1999/09/06 vince
 * 
 * Simplified things for Tcl/Tk 8.2, streamlined interface between plwidget
 * (Plplotwin) and this widget.  Added optional argument 'wait' to some
 * of the 'view' subcommands so the plot is only ever updated once after
 * zooming/resetting.  Previously it could be updated/redrawn/resized
 * as many as three times, when the user really only carried out one
 * operation.  Plplotwin code updated to use new wait argument.
 * 
 * Revision 1.59  1998/08/21 21:35:14 vince
 * 
 * Fixes to compile under Unix with Tcl8.
 *
 * '-background' config option works properly now (with redraw/dbl-buffer etc)
 * 
 *
 * Scrollbars were having some problems; not sure why, so I rewrote
 * them to use the new syntax for Tk4 and newer.  It's actually much
 * simpler than the old stuff.  Replaced functions xScroll and yScroll
 * by one simple 'Scroll'.  Renamed widget commands xscroll and yscroll
 * to xview and yview so (i) they're like the newer Tk widgets and
 * (ii) so they give an obvious error if you try to use the old mechanism.
 * 
 * Fixed the weird init process in the widget where there were all
 * sorts of problems due to widget not being mapped when desired
 * etc.  These seemed to break with each successive Tk release.
 * The solution?  Force the window into existence (not onto the
 * screen, just into existence inside Tk, so Tk_WindowId(tkwin)
 * works) using Tk_MakeWindowExist, then you can continue with
 * the initialisation immediately.
 * 
 * Rearranged a few things to be more like the widgets in 
 * the Tk 8 distribution.  Works on Windows.  I hope it
 * still works on Unix and MacOS!
 * 
 * Revision 1.58  1996/07/13 21:35:14 vince
 * Fixed for cross-platform support.  Works on MacOS now.
 * Removed backwards compatibility stuff (for old Tcl/Tk).  
 * This is designed to be a cross-platform widget now, and that 
 * old stuff was just getting in the way.
 * Some stuff like OpenLink, Print don't work on MacOS yet.
 * Uses the new 'tkwin' driver rather than 'xwin'.
 * Crosshair cursor always used in plot now, if the plot is 'active'
 * -- set with a new configuration option -activeplot
 * Added ButtonPressEH for rubber-banding.
 *
 * Revision 1.57  1996/06/26  21:35:14  furnish
 * Various hacks to support Tcl 7.5 and Tk 4.1.
 *
 * Revision 1.56  1996/02/24  05:06:30  shouman
 * Added #ifdef to allow compilation without itcl.
 * Set value of ndev on call to plgFileDevs (bounds check).
 *
 * Revision 1.55  1996/02/23  21:03:57  furnish
 * Introduce support for rubber banding in the plframe.  This entails
 * adding new functions to support drawing, updating, etc, the rubber
 * band, and also fixing some of the old event handlers for the xhairs so
 * that they work for both the xhairs and the rubber band, possibly even
 * both simultaneously.  Initial testing seems to indicate this is
 * working just fine.  However, we probably need to add some more
 * configuration support so Tcl apps can query, set, and otherwise
 * manipulate the rubber band ends.  Anyway, it is a good start.
 *
 * Revision 1.54  1995/09/22  16:04:11  mjl
 * Fixes to names of member variables of PLiodev structs.
 *
 * Revision 1.53  1995/09/18  20:15:46  furnish
 * Some changes to mirror what was done to Tk in the newest itcl 2.
 *
 * Revision 1.52  1995/08/22  16:17:22  mjl
 * Widget configure command now only calls for a refresh when required (i.e.
 * when geometry is changed).  Bug in ConfigureNotify event handling fixed
 * (first seen under Tk4 for some reason).  Locator report changed to be
 * of the form "<x> <y>", where <x> is the floating point representation
 * of the x coordinate, etc.
 *
 * Revision 1.51  1995/06/02  20:32:58  mjl
 * Fixed obscure bug that was causing the plframe to not be properly
 * redisplayed after a zoom in some cases.
 *
 * Revision 1.50  1995/06/01  21:23:10  mjl
 * Changed header file inclusions.
 *
 * Revision 1.49  1995/05/26  20:13:01  mjl
 * Changed "save as" widget command to accept device keyword, not number.
 * Eliminated "info devices" widget command in favor of "info devkeys", which
 * returns device keywords, and "info devnames", which returns device names.
 *
 * Revision 1.48  1995/05/19  22:24:18  mjl
 * Fixes for Tk 4.0.  Updated Tk_Draw3DRectangle syntax when compiling with
 * Tk versions 4.0 and later.  Eliminated -geometry option for much the same
 * reason as it was eliminated in the Tk 4.0 frame, to avoid problems with
 * some valid X *geometry specifications.  Use -width and/or -height instead.
 * Code is still compatible with Tk 3.6 and should remain so for a while.
 * Newer Tk 4.0 frame options like -class, -colormap, and -visual will be
 * added at a later date.
 *
 * Revision 1.47  1995/05/08  20:26:26  mjl
 * Split the crossing event handler into separate enter and leave event
 * handlers to better keep track of whether we should be drawing graphic
 * crosshairs or not.  Now works properly under all combinations of expose,
 * pointer in or out of window, and with or without a pixmap, as far as I can
 * tell.
 *
 * Revision 1.46  1995/05/06  17:10:51  mjl
 * Added EnterNotify and LeaveNotify event handling, to draw or undraw
 * crosshairs as needed.  Improved debugging output.
 *
 * Revision 1.45  1995/04/17  19:21:52  furnish
 * Implemented a new "report" widget command for performing on-demand
 * translations of window coordinates to world coordinates.  This is
 * especiallly useful when invoked from within a Tk binding.
 *
 * Revision 1.44  1995/04/12  08:08:09  mjl
 * A data link cleanup command added.
 *
 * Revision 1.43  1995/03/16  23:07:26  mjl
 * Implemented graphic crosshairs, which are turned on or off using the
 * "-xhairs <boolean>" widget configure command.  This inserts an event handler
 * to draw and update the graphic crosshairs on cursor motion, as well as
 * intercepting cursor key movements and moving the drawn cursor appropriately.
 * The expose event handling was broken off into a separate handler and changed
 * to redraw the graphic crosshairs if necessary.
 *
 * Revision 1.42  1995/01/13  23:20:44  mjl
 * Changed widget commands to set/get the cmap1 palette to handle the new
 * "rev" cmap1 control point attribute.
 *
 * Revision 1.41  1994/10/11  18:54:50  mjl
 * Now uses "early initialization" of PLplot X driver so that the fg/bg
 * colors can be set from the plframe widget.  This ensures that fg/bg colors
 * are consistent in both the widget and the driver, even if the bg color is
 * changed dynamically (through program or user interface control).  A bug
 * fixed in the xor GC also.  This fixes problems with the rubber-band
 * drawing (used in zooms), where the line color would sometimes change or be
 * hard to see against the background.
 *
 * Revision 1.40  1994/09/23  07:39:14  mjl
 * Incorrect order of calls to freeing malloc'ed memory at exit fixed, found
 * by Brent Townshend.
 */

/*
    plframe.c

    Copyright 1993, 1994, 1995
    Maurice LeBrun                      mjl@dino.ph.utexas.edu
    Institute for Fusion Studies        University of Texas at Austin

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

    Based upon tkFrame.c from the TK 3.2 distribution:
   
    Copyright 1990 Regents of the University of California.
    Permission to use, copy, modify, and distribute this
    software and its documentation for any purpose and without
    fee is hereby granted, provided that the above copyright
    notice appear in all copies.  The University of California
    makes no representations about the suitability of this
    software for any purpose.  It is provided "as is" without
    express or implied warranty.

* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
        
    This module implements "plframe" widgets for the Tk toolkit.  These are
    frames that have extra logic to allow them to be interfaced with the
    PLplot X driver.  These are then drawn into and respond to keyboard and
    mouse events.
*/
/*
#define DEBUG_ENTER
#define DEBUG
*/

#include "plserver.h"
#include "pltkwd.h"

#ifdef __WIN32__
#define XSynchronize(display, bool) {display->request++;}
#define XSync(display, bool) {display->request++;}
#define XFlush(display)
#endif

#if defined(__MWERKS__) && (defined(__WIN32__) || defined (MAC_TCL))
/* 
 * For some stupid reason, the Metrowerks Codewarrior compiler
 * wants these stupid parameters 
 */
extern void XWarpPointer(
#if NeedFunctionPrototypes
    Display*        d1/* display */,
    Window      d2/* src_w */,
    Window      d3/* dest_w */,
    int         d4/* src_x */,
    int         d5/* src_y */,
    unsigned int   d6 /* src_width */,
    unsigned int   d7 /* src_height */,
    int        d8 /* dest_x */,
    int        d9 /* dest_y */         
#endif
);
#endif

#define _TCLINT
#ifdef USE_TCL_STUBS
/* Unfortunately, tkInt.h ends up loading Malloc.h under Windows */
/* So we have to deal with this mess */
    #undef malloc 
    #undef free
    #undef realloc
    #undef calloc
#include <tkInt.h>
    #define malloc ckalloc
    #define free(m) ckfree((char*)m)
    #define realloc ckrealloc
    #define calloc ckcalloc
#else
#include <tkInt.h>
#endif

#include "tcpip.h"

#ifdef DEBUG_ENTER
/* This version of dbug_enter works cross-platform */
#undef dbug_enter
#define dbug_enter(a) if(plsc->debug) { \
    Tcl_Write(Tcl_GetStdChannel(TCL_STDERR),a,-1);}
#endif

#ifndef __WIN32__
#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif
#endif
#include <fcntl.h>

#ifdef ckalloc
#undef ckalloc
#define ckalloc malloc
#endif
#ifdef ckfree
#undef ckfree
#define ckfree free
#endif


extern int plplot_ccmap;

#define NDEV    20              /* Max number of output device types */

/* If set, BUFFER_FIFO causes FIFO i/o to be buffered */

#define BUFFER_FIFO 1

/* If set, causes a file handler to be used with FIFO */

#define FH_FIFO 0

/* A handy command wrapper */

#define plframe_cmd(code) \
    if ((code) == TCL_ERROR) return (TCL_ERROR);

/*
 * A data structure of the following type is kept for each
 * plframe that currently exists for this process:
 */

typedef struct PlFrame {

/* This is stuff taken from tkFrame.c */

    Tk_Window tkwin;            /* Window that embodies the frame.  NULL
                                 * means that the window has been destroyed
                                 * but the data structures haven't yet been
                                 * cleaned up.*/
    Display *display;           /* Display containing widget.  Used, among
                                 * other things, so that resources can be
                                 * freed even after tkwin has gone away. */
    Tcl_Interp *interp;         /* Interpreter associated with
                                 * widget.  Used to delete widget
                                 * command.  */
    Tcl_Command widgetCmd;      /* Token for frame's widget command. */
    Tk_3DBorder border;         /* Structure used to draw 3-D border and
                                 * background. */
    int borderWidth;            /* Width of 3-D border (if any). */
    int relief;                 /* 3-d effect: TK_RELIEF_RAISED etc. */
    int width;                  /* Width to request for window.  <= 0 means
                                 * don't request any size. */
    int height;                 /* Height to request for window.  <= 0 means
                                 * don't request any size. */
    Tk_Cursor cursor;           /* Current cursor for window, or None. */
    int flags;                  /* Various flags;  see below for
                                 * definitions. */

/* These are new to plframe widgets */

/* control stuff */

    int tkwin_initted;          /* Set first time widget is mapped */
    PLStream *pls;              /* PLplot stream pointer */
    PLINT ipls;                 /* PLplot stream number */
    PLINT ipls_save;            /* PLplot stream number, save files */

    PLRDev *plr;                /* Renderer state information.  Malloc'ed */
    char *plpr_cmd;             /* Holds print command name.  Malloc'ed */

/* Used to allow active stuff on the plot */
    int active_plot;            /* The plot responds to mouse movement etc. */
    int isActive;               /* Used to turn event handling on and off. */
    
/* Used to handle resize and expose events */

    PLDisplay pldis;            /* Info about the display window */
    int prevWidth;              /* Previous window width */
    int prevHeight;             /* Previous window height */

/* Support for save operations */

    char *SaveFnam;             /* File name we are currently saving to.
                                   Malloc'ed. */
    char **devDesc;             /* Descriptive names for file-oriented 
                                 * devices.  Malloc'ed. */
    char **devName;             /* Keyword names of file-oriented devices.
                                 * Malloc'ed. */

/* Used in selecting & modifying plot or device area */

    GC xorGC;                   /* GC used for rubber-band drawing */
    XPoint pts[5];              /* Points for rubber-band drawing */
    int continue_draw;          /* Set when doing rubber-band draws */
    Tk_Cursor xhair_cursor;     /* cursor used for drawing */
    PLFLT xl, xr, yl, yr;       /* Bounds on plot viewing area */
    char *xScrollCmd;           /* Command prefix for communicating with
                                 * horizontal scrollbar.  NULL means no
                                 * command to issue.  Malloc'ed. */
    char *yScrollCmd;           /* Command prefix for communicating with
                                 * vertical scrollbar.  NULL means no
                                 * command to issue.  Malloc'ed. */

/* Used for flashing bop or eop condition */

    char *bopCmd;               /* Proc to call at bop */
    char *eopCmd;               /* Proc to call at eop */

/* Used for drawing graphic crosshairs */

    int xhairs;                 /* Configuration option to turn on xhairs */
    int drawing_xhairs;         /* Set if we are currently drawing xhairs */
    XPoint xhair_x[2];          /* Points for horizontal xhair line */
    XPoint xhair_y[2];          /* Points for vertical xhair line */

/* Used for drawing a rubber band lilne segment */

    int rband;                  /* Configuration option to turn on rband */
    int drawing_rband;          /* See if we are currently drawing rband */
    XPoint rband_pt[2];         /* Ends of rubber band line */
    int double_buffer;		/* Double buffering configuration option */
} PlFrame;

/*
 * Flag bits for plframes:
 *
 * REFRESH_PENDING:             Non-zero means a DoWhenIdle handler
 *                              has already been queued to refresh
 *                              this window.
 * RESIZE_PENDING;              Used to reschedule resize events
 * REDRAW_PENDING;              Used to redraw contents of plot buffer
 * UPDATE_V_SCROLLBAR:          Non-zero means vertical scrollbar needs
 *                              to be updated.
 * UPDATE_H_SCROLLBAR:          Non-zero means horizontal scrollbar needs
 *                              to be updated.
 */

#define REFRESH_PENDING         1
#define RESIZE_PENDING          2
#define REDRAW_PENDING          4
#define UPDATE_V_SCROLLBAR      8
#define UPDATE_H_SCROLLBAR      16

/* Defaults for plframes: */

#define DEF_PLFRAME_BG_COLOR            "Black"
#define DEF_PLFRAME_BG_MONO             "White"
#define DEF_PLFRAME_BORDER_WIDTH        "0"
#define DEF_PLFRAME_CURSOR              ((char *) NULL)
#define DEF_PLFRAME_HEIGHT              "250"
#define DEF_PLFRAME_RELIEF              "flat"
#define DEF_PLFRAME_WIDTH               "250"

/* Configuration info */

static Tk_ConfigSpec configSpecs[] = {
    {TK_CONFIG_BOOLEAN, "-activeplot", (char *) NULL, (char *) NULL,
        "1", Tk_Offset(PlFrame, active_plot), TK_CONFIG_DONT_SET_DEFAULT},
    {TK_CONFIG_BORDER, "-background", "background", "Background",
	DEF_PLFRAME_BG_COLOR, Tk_Offset(PlFrame, border),
	TK_CONFIG_COLOR_ONLY},
    {TK_CONFIG_BORDER, "-background", "background", "Background",
	DEF_PLFRAME_BG_MONO, Tk_Offset(PlFrame, border),
	TK_CONFIG_MONO_ONLY},
    {TK_CONFIG_SYNONYM, "-bd", "borderWidth", (char *) NULL,
	(char *) NULL, 0, 0},
    {TK_CONFIG_SYNONYM, "-bg", "background", (char *) NULL,
	(char *) NULL, 0, 0},
    {TK_CONFIG_STRING, "-bopcmd", "bopcmd", "PgCommand",
        (char *) NULL, Tk_Offset(PlFrame, bopCmd), TK_CONFIG_NULL_OK},
    {TK_CONFIG_PIXELS, "-borderwidth", "borderWidth", "BorderWidth",
	DEF_PLFRAME_BORDER_WIDTH, Tk_Offset(PlFrame, borderWidth), 0},
    {TK_CONFIG_BOOLEAN, "-doublebuffer", (char *) NULL, (char *) NULL,
        "0", Tk_Offset(PlFrame, double_buffer), TK_CONFIG_DONT_SET_DEFAULT},
    {TK_CONFIG_ACTIVE_CURSOR, "-cursor", "cursor", "Cursor",
        DEF_PLFRAME_CURSOR, Tk_Offset(PlFrame, cursor), TK_CONFIG_NULL_OK},
    {TK_CONFIG_STRING, "-eopcmd", "eopcmd", "PgCommand",
        (char *) NULL, Tk_Offset(PlFrame, eopCmd), TK_CONFIG_NULL_OK},
    {TK_CONFIG_PIXELS, "-height", "height", "Height",
        DEF_PLFRAME_HEIGHT, Tk_Offset(PlFrame, height), 0},
    {TK_CONFIG_RELIEF, "-relief", "relief", "Relief",
        DEF_PLFRAME_RELIEF, Tk_Offset(PlFrame, relief), 0},
    {TK_CONFIG_PIXELS, "-width", "width", "Width",
        DEF_PLFRAME_WIDTH, Tk_Offset(PlFrame, width), 0},
    {TK_CONFIG_BOOLEAN, "-xhairs", (char *) NULL, (char *) NULL,
        "0", Tk_Offset(PlFrame, xhairs), TK_CONFIG_DONT_SET_DEFAULT},
    {TK_CONFIG_BOOLEAN, "-rubberband", (char *) NULL, (char *) NULL,
        "0", Tk_Offset(PlFrame, rband), TK_CONFIG_DONT_SET_DEFAULT},
	{TK_CONFIG_STRING, "-xscrollcommand", "xScrollCommand", "ScrollCommand",
        (char *) NULL, Tk_Offset(PlFrame, xScrollCmd), TK_CONFIG_NULL_OK},
    {TK_CONFIG_STRING, "-yscrollcommand", "yScrollCommand", "ScrollCommand",
        (char *) NULL, Tk_Offset(PlFrame, yScrollCmd), TK_CONFIG_NULL_OK},
    {TK_CONFIG_END, (char *) NULL, (char *) NULL, (char *) NULL,
        (char *) NULL, 0, 0}
};

/* Forward declarations for procedures defined later in this file: */

/* Externals */

int   plFrameCmd        (ClientData, Tcl_Interp *, int, char **);
void PlframeAtEop(Tcl_Interp *interp, register PlFrame *plFramePtr);
void PlframeAtBop(Tcl_Interp *interp, register PlFrame *plFramePtr);

/* These are invoked by the TK dispatcher */

static void  DestroyPlFrame     (char *);
static void  DisplayPlFrame     (ClientData);
static void  PlFrameInit        (ClientData);
static void  PlFrameFirstInit   (ClientData clientData);
static void  PlFrameConfigureEH (ClientData, XEvent *);
static void  PlFrameExposeEH    (ClientData, XEvent *);
static void  PlFrameMotionEH    (ClientData, register XEvent *);
static void  PlFrameEnterEH     (ClientData, register XEvent *);
static void  PlFrameLeaveEH     (ClientData, register XEvent *);
static void  PlFrameKeyEH       (ClientData, register XEvent *);
static void  PlFrameButtonPressEH    (ClientData    clientData, register XEvent *);
static int   PlFrameWidgetCmd   (ClientData, Tcl_Interp *, int, char **);
static int   ReadData           (ClientData, int);
static void  Install_cmap       (PlFrame *plFramePtr);

/* These are invoked by PlFrameWidgetCmd to process widget commands */

static int   Closelink          (Tcl_Interp *, PlFrame *, int, char **);
static int   Cmd                (Tcl_Interp *, PlFrame *, int, char **);
static int   ConfigurePlFrame   (Tcl_Interp *, PlFrame *, int, char **, int);
static int   Draw               (Tcl_Interp *, PlFrame *, int, char **);
static int   Info               (Tcl_Interp *, PlFrame *, int, char **);
static int   Openlink           (Tcl_Interp *, PlFrame *, int, char **);
static int   Orient             (Tcl_Interp *, PlFrame *, int, char **);
static int   Page               (Tcl_Interp *, PlFrame *, int, char **);
static int   NextPage           (Tcl_Interp *, PlFrame *, int, char **);
static int   Print              (Tcl_Interp *, PlFrame *, int, char **);
static int   Redraw             (Tcl_Interp *, PlFrame *, int, char **);
static int   Save               (Tcl_Interp *, PlFrame *, int, char **);
static int   View               (Tcl_Interp *, PlFrame *, int, char **);
static int   Scroll             (Tcl_Interp *, PlFrame *);
static int   report             (Tcl_Interp *, PlFrame *, int, char **);

/* Routines for manipulating graphic crosshairs */

static void  ActiveState        (register PlFrame *plFramePtr, int on);
static void  CreateXhairs       (PlFrame *);
static void  DestroyXhairs      (PlFrame *);
static void  DrawXhairs         (PlFrame *, int, int);
static void  UpdateXhairs       (PlFrame *);

/* Routines for manipulating the rubberband line */

static void  CreateRband        (PlFrame *);
static void  DestroyRband       (PlFrame *);
static void  DrawRband          (PlFrame *, int, int);
static void  UpdateRband        (PlFrame *);

/* Utility routines */

static void  gbox               (PLFLT *, PLFLT *, PLFLT *, PLFLT *, char **);
static void  UpdateVScrollbar   (register PlFrame *);
static void  UpdateHScrollbar   (register PlFrame *);

/*
 *---------------------------------------------------------------------------
 *
 * plFrameCmd --
 *
 *      This procedure is invoked to process the "plframe" Tcl
 *      command.  See the user documentation for details on what it
 *      does.
 *
 * Results:
 *      A standard Tcl result.
 *
 * Side effects:
 *      See the user documentation.
 *
 *---------------------------------------------------------------------------
 */

int
plFrameCmd(ClientData clientData, Tcl_Interp *interp,
           int argc, char **argv)
{
    /* Removed: we simply use 'Tk_MainWindow' below */
    /* Tk_Window main = (Tk_Window) clientData; */
    Tk_Window tkwin;
    register PlFrame *plFramePtr;
    register PLRDev *plr;
    int i, ndev;

    dbug_enter("plFrameCmd");

    if (argc < 2) {
        Tcl_AppendResult(interp, "wrong # args: should be \"",
                argv[0], " pathName ?options?\"", (char *) NULL);
        return TCL_ERROR;
    }

/* Create the window. */

    tkwin = Tk_CreateWindowFromPath(interp, Tk_MainWindow(interp), argv[1], (char *) NULL);
    if (tkwin == NULL) {
        return TCL_ERROR;
    }
    Tk_SetClass(tkwin, "plframe");

    plFramePtr = (PlFrame *) malloc(sizeof(PlFrame));
    plFramePtr->tkwin = tkwin;
    plFramePtr->display = Tk_Display(tkwin);
    plFramePtr->interp = interp;
    plFramePtr->widgetCmd = 
      Tcl_CreateCommand(interp, Tk_PathName(plFramePtr->tkwin),
			PlFrameWidgetCmd, (ClientData) plFramePtr, 
			(Tcl_CmdDeleteProc*) NULL);
    plFramePtr->xorGC = NULL;
    plFramePtr->border = NULL;
    plFramePtr->cursor = None;
    plFramePtr->xhair_cursor = None;
    plFramePtr->flags = 0;
    plFramePtr->width  = Tk_Width(plFramePtr->tkwin);
    plFramePtr->height = Tk_Height(plFramePtr->tkwin);
    plFramePtr->prevWidth = 0;
    plFramePtr->prevHeight = 0;
    plFramePtr->continue_draw = 0;
    plFramePtr->ipls = 0;
    plFramePtr->ipls_save = 0;
    plFramePtr->tkwin_initted = 0;
    plFramePtr->plpr_cmd = NULL;
    plFramePtr->bopCmd = NULL;
    plFramePtr->eopCmd = NULL;
    plFramePtr->xhairs = 0;
    plFramePtr->active_plot = 1;
    plFramePtr->isActive = 0;
    plFramePtr->drawing_xhairs = 0;
    plFramePtr->rband = 0;
    plFramePtr->drawing_rband = 0;
    plFramePtr->xScrollCmd = NULL;
    plFramePtr->yScrollCmd = NULL;
    plFramePtr->xl = 0.;
    plFramePtr->yl = 0.;
    plFramePtr->xr = 1.;
    plFramePtr->yr = 1.;
    plFramePtr->SaveFnam = NULL;

    plFramePtr->plr = (PLRDev *) malloc(sizeof(PLRDev));
    plr = plFramePtr->plr;
    plr->pdfs = NULL;
    plr->iodev = (PLiodev *) malloc(sizeof(PLiodev));
    plr_start(plr);

/* Associate new PLplot stream with this widget */

    plmkstrm(&plFramePtr->ipls);
    plgpls(&plFramePtr->pls);
    /* Tell the stream about myself */
    plFramePtr->pls->plFramePtr = plFramePtr;
        
/* Set up stuff for rubber-band drawing */

    plFramePtr->xhair_cursor =
        Tk_GetCursor(plFramePtr->interp, plFramePtr->tkwin, "crosshair");

/* Partially initialize Tk driver. */

    plD_open_tkw(plFramePtr->pls);

/* Create list of valid device names and keywords for page dumps */

    plFramePtr->devDesc = (char **) malloc(NDEV * sizeof(char **));
    plFramePtr->devName = (char **) malloc(NDEV * sizeof(char **));
    for (i = 0; i < NDEV; i++) {
        plFramePtr->devDesc[i] = NULL;
        plFramePtr->devName[i] = NULL;
    }
    ndev = NDEV;
    plgFileDevs(&plFramePtr->devDesc, &plFramePtr->devName, &ndev);

/* Start up event handlers and other good stuff */

    Tk_CreateEventHandler(plFramePtr->tkwin, StructureNotifyMask,
                          PlFrameConfigureEH, (ClientData) plFramePtr);

    Tk_CreateEventHandler(plFramePtr->tkwin, ExposureMask,
                          PlFrameExposeEH, (ClientData) plFramePtr);

/* for active plot */
    ActiveState(plFramePtr, 1);
    
    if (ConfigurePlFrame(interp, plFramePtr, argc-2, argv+2, 0) != TCL_OK) {
        Tk_DestroyWindow(plFramePtr->tkwin);
        return TCL_ERROR;
    }
    Tk_MakeWindowExist(plFramePtr->tkwin);
    PlFrameFirstInit((ClientData)plFramePtr);
    Tk_GeometryRequest(plFramePtr->tkwin,200,200);
    
    interp->result = Tk_PathName(plFramePtr->tkwin);

    return TCL_OK;
}

/*
 *---------------------------------------------------------------------------
 *
 * PlFrameWidgetCmd --
 *
 *      This procedure is invoked to process the Tcl command that
 *      corresponds to a plframe widget.  See the user
 *      documentation for details on what it does.
 *
 * Results:
 *      A standard Tcl result.
 *
 * Side effects:
 *      See the user documentation.
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

/* cmd -- issue a command to the PLplot library */

    if ((c == 'c') && (strncmp(argv[1], "cmd", length) == 0)) {
        result = Cmd(interp, plFramePtr, argc-2, argv+2);
    }

/* configure */

    else if ((c == 'c') && (strncmp(argv[1], "cget", length) == 0)
	    && (length >= 2)) {
	if (argc != 3) {
	    Tcl_AppendResult(interp, "wrong # args: should be \"",
		    argv[0], " cget option\"",
		    (char *) NULL);
	    result = TCL_ERROR;
	    goto done;
	}
	result = Tk_ConfigureValue(interp, plFramePtr->tkwin, configSpecs,
		(char *) plFramePtr, argv[2], 0);
    } else if ((c == 'c') && (strncmp(argv[1], "configure", length) == 0)) {
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

/* closelink -- Close a binary data link previously opened with openlink */

    else if ((c == 'c') && (strncmp(argv[1], "closelink", length) == 0)) {
        if (argc > 2) {
            Tcl_AppendResult(interp, "wrong # args: should be \"",
                    argv[0], (char *) NULL);
            result = TCL_ERROR;
            goto done;
        }
        else {
            result = Closelink(interp, plFramePtr, argc-2, argv+2);
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

/* next page.  called to cancel wait for page in tkwin driver */

    else if ((c == 'n') && (strncmp(argv[1], "nextpage", length) == 0)) {
        result = NextPage(interp, plFramePtr, argc-2, argv+2);
    }

/* orient -- Set plot orientation */

    else if ((c == 'o') && (strncmp(argv[1], "orient", length) == 0)) {
        result = Orient(interp, plFramePtr, argc-2, argv+2);
    }

/* openlink -- Open a binary data link (FIFO or socket) */

    else if ((c == 'o') && (strncmp(argv[1], "openlink", length) == 0)) {
        if (argc < 3) {
            Tcl_AppendResult(interp, "wrong # args: should be \"",
                    argv[0], " option ?arg arg ...?\"", (char *) NULL);
            result = TCL_ERROR;
            goto done;
        }
        else {
            result = Openlink(interp, plFramePtr, argc-2, argv+2);
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

/* report -- find out useful info about the plframe (GMF) */

    else if ((c == 'r') && (strncmp(argv[1], "report", length) == 0)) {
        result = report( interp, plFramePtr, argc-2, argv+2 );
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

    else if ((c == 'x') && (strncmp(argv[1], "xview", length) == 0)) {
	int count, type;
	double width = plFramePtr->xr - plFramePtr->xl;
	
	double fraction;

	type = Tk_GetScrollInfo(interp, argc, argv, &fraction, &count);
	switch (type) {
	  case TK_SCROLL_ERROR:
	    result = TCL_ERROR;
	    goto done;
	  case TK_SCROLL_MOVETO:
	    plFramePtr->xl = fraction;
	    plFramePtr->xr = fraction + width;
	    break;
	  case TK_SCROLL_PAGES:
	    plFramePtr->xl += count * width * .9;
	    plFramePtr->xr += count * width * .9;
	    break;
	  case TK_SCROLL_UNITS:
	    plFramePtr->xl += count*width/50;
	    plFramePtr->xr += count*width/50;
	    break;
	}
	if(plFramePtr->xr > 1.0) {
	    plFramePtr->xr = 1.0;
	    plFramePtr->xl = 1.0 - width;
	} else if(plFramePtr->xl < 0.0) {
	    plFramePtr->xl = 0.0;
	    plFramePtr->xr = width;
	}
	Scroll(interp,plFramePtr);
    }

/* yscroll -- vertically scroll window into plot */

    else if ((c == 'y') && (strncmp(argv[1], "yview", length) == 0)) {
	int count, type;
	double height = plFramePtr->yr - plFramePtr->yl;
	
	double fraction;

	type = Tk_GetScrollInfo(interp, argc, argv, &fraction, &count);
	switch (type) {
	  case TK_SCROLL_ERROR:
	    result = TCL_ERROR;
	    goto done;
	  case TK_SCROLL_MOVETO:
	    plFramePtr->yl = 1.0-fraction -height;
	    plFramePtr->yr = 1.0-fraction;
	    break;
	  case TK_SCROLL_PAGES:
	    plFramePtr->yl -= count * height * .9;
	    plFramePtr->yr -= count * height * .9;
	    break;
	  case TK_SCROLL_UNITS:
	    plFramePtr->yl -= count*height/50;
	    plFramePtr->yr -= count*height/50;
	    break;
	}
	if(plFramePtr->yr > 1.0) {
	    plFramePtr->yr = 1.0;
	    plFramePtr->yl = 1.0 - height;
	} else if(plFramePtr->yl < 0.0) {
	    plFramePtr->yl = 0.0;
	    plFramePtr->yr = height;
	}
	Scroll(interp,plFramePtr);
    }

/* unrecognized widget command */

    else {
        Tcl_AppendResult(interp, "bad option \"", argv[1],
         "\":  must be closelink, cmd, configure, draw, info, nextpage ",
         "openlink, orient, page, print, redraw, save, view, ",
         "xview, or yview", (char *) NULL);

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
 *      This procedure is invoked by Tk_EventuallyFree or Tk_Release to
 *      clean up the internal structure of a plframe at a safe time
 *      (when no-one is using it anymore).
 *
 * Results:
 *      None.
 *
 * Side effects:
 *      Everything associated with the plframe is freed up.
 *
 *---------------------------------------------------------------------------
 */

static void DestroyPlFrame(char* clientData)
{
    register PlFrame *plFramePtr = (PlFrame *) clientData;
    register PLRDev *plr = plFramePtr->plr;
    TkwDev *dev = (TkwDev *) plFramePtr->pls->dev;

    dbug_enter("DestroyPlFrame");

    dev->exit_eventloop = TRUE;
    if (plFramePtr->border != NULL) {
        Tk_Free3DBorder(plFramePtr->border);
    }
    if (plFramePtr->plpr_cmd != NULL) {
        free((char *) plFramePtr->plpr_cmd);
    }
    if (plFramePtr->cursor != None) {
        Tk_FreeCursor(plFramePtr->display, plFramePtr->cursor);
    }
    if (plFramePtr->xhair_cursor != None) {
        Tk_FreeCursor(plFramePtr->display, plFramePtr->xhair_cursor);
    }
    if (plFramePtr->xorGC != NULL) {
        Tk_FreeGC(plFramePtr->display, plFramePtr->xorGC);
    }
    if (plFramePtr->yScrollCmd != NULL) {
        free((char *) plFramePtr->yScrollCmd);
    }
    if (plFramePtr->xScrollCmd != NULL) {
        free((char *) plFramePtr->xScrollCmd);
    }
    if (plFramePtr->SaveFnam != NULL) {
        free((char *) plFramePtr->SaveFnam);
    }
    if (plFramePtr->devDesc != NULL) {
        free((char *) plFramePtr->devDesc);
    }
    if (plFramePtr->devName != NULL) {
        free((char *) plFramePtr->devName);
    }

/* Clean up data connection */

    pdf_close(plr->pdfs);
    free((char *) plFramePtr->plr->iodev);

/* Tell PLplot to clean up */

    plsstrm( plFramePtr->ipls );
    plend1();

/* Delete main data structures */

    free((char *) plFramePtr->plr);
    free((char *) plFramePtr);
}

/*
 *---------------------------------------------------------------------------
 *
 * PlFrameConfigureEH --
 *
 *      Invoked by the Tk dispatcher on structure changes to a plframe. 
 *
 * Results:
 *      None.
 *
 * Side effects:
 *      When the window gets deleted, internal structures get cleaned up.
 *      When it gets resized, it is redrawn.
 *
 *---------------------------------------------------------------------------
 */

static void
PlFrameConfigureEH(ClientData clientData, register XEvent *eventPtr)
{
    register PlFrame *plFramePtr = (PlFrame *) clientData;
    register Tk_Window tkwin = plFramePtr->tkwin;

    dbug_enter("PlFrameConfigureEH");

    switch (eventPtr->type) {

    case ConfigureNotify:
        pldebug("PLFrameConfigureEH", "ConfigureNotify\n");
        plFramePtr->flags |= RESIZE_PENDING;
        plFramePtr->width  = Tk_Width(tkwin);
        plFramePtr->height = Tk_Height(tkwin);
        if ((tkwin != NULL) && !(plFramePtr->flags & REFRESH_PENDING)) {
            Tcl_DoWhenIdle(DisplayPlFrame, (ClientData) plFramePtr);
            plFramePtr->flags |= REFRESH_PENDING;
            plFramePtr->flags |= UPDATE_V_SCROLLBAR|UPDATE_H_SCROLLBAR;
        }
        break;

    case DestroyNotify:
        pldebug("PLFrameConfigureEH", "DestroyNotify\n");
        Tcl_DeleteCommand(plFramePtr->interp, Tk_PathName(tkwin));
        plFramePtr->tkwin = NULL;
        if (plFramePtr->flags & REFRESH_PENDING) {
            Tcl_CancelIdleCall(DisplayPlFrame, (ClientData) plFramePtr);
        }
        Tk_EventuallyFree((ClientData) plFramePtr, DestroyPlFrame);
        break;

    case MapNotify:
        pldebug("PLFrameConfigureEH", "MapNotify\n");
        if (plFramePtr->flags & REFRESH_PENDING) {
            Tcl_CancelIdleCall(DisplayPlFrame, (ClientData) plFramePtr);
        }

/* Vince thinks we don't want these lines any more */
/* We forced the window into existence when we created it. */
#if 0
    /* For some reason, "." must be mapped or PlFrameInit will die (Note:
     * mapped & withdrawn or mapped in the withdrawn state is OK). Issuing
     * an update fixes this.  I'd love to know why this occurs.  
     */

        if (!plFramePtr->tkwin_initted) {
            Tcl_VarEval(plFramePtr->interp, "update", (char *) NULL);
        }
#endif
	/* Set up window parameters and arrange for window to be refreshed */
	
	Tcl_DoWhenIdle(PlFrameInit, (ClientData) plFramePtr);
	break;
    }
}

/*
 *---------------------------------------------------------------------------
 *
 * PlFrameExposeEH --
 *
 *      Invoked by the Tk dispatcher on exposes of a plframe. 
 *
 * Results:
 *      None.
 *
 * Side effects:
 *      Widget is redisplayed.
 *
 * Note: it's customary in Tk to collapse multiple exposes, so for best
 * performance without losing the window contents, I keep track of the
 * smallest single rectangle that can satisfy all expose events.  If there
 * are any overlaid graphics (like crosshairs), however, we need to refresh
 * the entire plot in order to have a predictable outcome.
 *
 *--------------------------------------------------------------------------- */

static void
PlFrameExposeEH(ClientData clientData, register XEvent *eventPtr)
{
    register PlFrame *plFramePtr = (PlFrame *) clientData;
    XExposeEvent *event = (XExposeEvent *) eventPtr;
    register Tk_Window tkwin = plFramePtr->tkwin;

    dbug_enter("PlFrameExposeEH");

    pldebug("PLFrameExposeEH", "Expose\n");

/* Set up the area to refresh */

    if ( ! (plFramePtr->drawing_xhairs || plFramePtr->drawing_rband) ) {
        int x0_old, x1_old, y0_old, y1_old, x0_new, x1_new, y0_new, y1_new;

        x0_old = plFramePtr->pldis.x;
        y0_old = plFramePtr->pldis.y;
        x1_old = x0_old + plFramePtr->pldis.width;
        y1_old = y0_old + plFramePtr->pldis.height;

        x0_new = event->x;
        y0_new = event->y;
        x1_new = x0_new + event->width;
        y1_new = y0_new + event->height;

        plFramePtr->pldis.x      = MIN(x0_old, x0_new);
        plFramePtr->pldis.y      = MIN(y0_old, y0_new);
        plFramePtr->pldis.width  = MAX(x1_old, x1_new) - plFramePtr->pldis.x;
        plFramePtr->pldis.height = MAX(y1_old, y1_new) - plFramePtr->pldis.y;
    }

/* Invoke DoWhenIdle handler to redisplay widget. */

    if (event->count == 0) {
        if ((tkwin != NULL) && !(plFramePtr->flags & REFRESH_PENDING)) {
            Tcl_DoWhenIdle(DisplayPlFrame, (ClientData) plFramePtr);
            plFramePtr->width  = Tk_Width(tkwin);
            plFramePtr->height = Tk_Height(tkwin);
            plFramePtr->flags |= REFRESH_PENDING;
        }
    }
}

/*
 *---------------------------------------------------------------------------
 *
 * PlFrameMotionEH --
 *
 *      Invoked by the Tk dispatcher on MotionNotify events in a plframe. 
 *      Not invoked unless we are drawing graphic crosshairs.
 *
 * Results:
 *      None.
 *
 * Side effects:
 *      Graphic crosshairs are drawn.
 *
 *---------------------------------------------------------------------------
 */

static void
PlFrameMotionEH(ClientData clientData, register XEvent *eventPtr)
{
    register PlFrame *plFramePtr = (PlFrame *) clientData;
    XMotionEvent *event = (XMotionEvent *) eventPtr;

    dbug_enter("PlFrameMotionEH");

    if (plFramePtr->drawing_xhairs) {
        DrawXhairs(plFramePtr, event->x, event->y);
    }
    if (plFramePtr->drawing_rband) {
        DrawRband(plFramePtr, event->x, event->y);
    }
}

/*
 *---------------------------------------------------------------------------
 *
 * PlFrameEnterEH --
 *
 *      Invoked by the Tk dispatcher on EnterNotify events in a plframe.
 *      Not invoked unless we are drawing graphic crosshairs.
 *
 * Results:
 *      None.
 *
 * Side effects:
 *      Graphic crosshairs are updated.
 *
 *--------------------------------------------------------------------------- */

static void
PlFrameEnterEH(ClientData clientData, register XEvent *eventPtr)
{
    register PlFrame *plFramePtr = (PlFrame *) clientData;
    XCrossingEvent *crossingEvent = (XCrossingEvent *) eventPtr;

    dbug_enter("PlFrameEnterEH");

    if (plFramePtr->xhairs) {
        DrawXhairs(plFramePtr, crossingEvent->x, crossingEvent->y);
        plFramePtr->drawing_xhairs = 1;
    }
    if (plFramePtr->rband) {
        plFramePtr->drawing_rband = 1;
        UpdateRband(plFramePtr);
        DrawRband(plFramePtr, crossingEvent->x, crossingEvent->y);
    }
}

/*
 *---------------------------------------------------------------------------
 *
 * PlFrameLeaveEH --
 *
 *      Invoked by the Tk dispatcher on LeaveNotify events in a plframe.
 *      Not invoked unless we are drawing graphic crosshairs.
 *
 * Results:
 *      None.
 *
 * Side effects:
 *      Graphic crosshairs are updated.
 *
 *--------------------------------------------------------------------------- */

static void
PlFrameLeaveEH(ClientData clientData, register XEvent *eventPtr)
{
    register PlFrame *plFramePtr = (PlFrame *) clientData;

    dbug_enter("PlFrameLeaveEH");

    if (plFramePtr->drawing_xhairs) {
        UpdateXhairs(plFramePtr);
        plFramePtr->drawing_xhairs = 0;
    }
    if (plFramePtr->drawing_rband) {
        UpdateRband(plFramePtr);
        plFramePtr->drawing_rband = 0;
    }
}

static void
PlFrameButtonPressEH(ClientData clientData, register XEvent *eventPtr)
{
    register PlFrame *plFramePtr = (PlFrame *) clientData;
    XButtonEvent *event = (XButtonEvent *) eventPtr;
    register Tk_Window tkwin = plFramePtr->tkwin;
    /* Get modifier keys */
    switch (event->state) {
      case 256: /* plain */
        if(plFramePtr->drawing_rband) {
            UpdateRband(plFramePtr);
        }
        if(plFramePtr->rband)
            CreateRband(plFramePtr);
        break;
    }
}
/*
 *---------------------------------------------------------------------------
 *
 * PlFrameKeyEH --
 *
 *      Invoked by the Tk dispatcher on Keypress events in a plframe. 
 *      Not invoked unless we are drawing graphic crosshairs.
 *
 * Results:
 *      None.
 *
 * Side effects:
 *      Keypress events get filtered.  If a cursor key is pushed, the
 *      graphic crosshairs are moved in the appropriate direction.  Using a
 *      modifier key multiplies the movement a factor of 5 for each key
 *      added.
 *
 *---------------------------------------------------------------------------
 */

static void
PlFrameKeyEH(ClientData clientData, register XEvent *eventPtr)
{
    register PlFrame *plFramePtr = (PlFrame *) clientData;
    XKeyEvent *event = (XKeyEvent *) eventPtr;
    Tk_Window tkwin = plFramePtr->tkwin;
    Tcl_DString ds;
    KeySym keysym;
    char* string;
    XComposeStatus cs;

    dbug_enter("PlFrameKeyEH");
    string = TkpGetString((TkWindow*)tkwin,eventPtr,&ds);
    pldebug("PlFrameKeyEH", "Translation: %s\n", string);
    keysym = TkStringToKeysym(string);
    Tcl_DStringFree(&ds);
	
    if (IsModifierKey(keysym)) {
        eventPtr->type = 0;
    } else if (IsCursorKey(keysym)) {
        int x1, y1, dx = 0, dy = 0;
        int x0 = event->x, y0 = event->y;
        int xmin = 0, xmax = Tk_Width(tkwin) - 1;
        int ymin = 0, ymax = Tk_Height(tkwin) - 1;

        switch (keysym) {
        case XK_Left:
            dx = -1;
            break;
        case XK_Right:
            dx = 1;
            break;
        case XK_Up:
            dy = -1;
            break;
        case XK_Down:
            dy = 1;
            break;
        }

    /* Each modifier key added increases the multiplication factor by 5 */

    /* Shift */

        if (event->state & 0x01) {
            dx *= 5;
            dy *= 5;
        }

    /* Caps Lock */

        if (event->state & 0x02) {
            dx *= 5;
            dy *= 5;
        }

    /* Control */

        if (event->state & 0x04) {
            dx *= 5;
            dy *= 5;
        }

    /* Alt */

        if (event->state & 0x08) {
            dx *= 5;
            dy *= 5;
        }

    /* Bounds checking so that we don't send cursor out of window */

        x1 = x0 + dx;
        y1 = y0 + dy;

        if (x1 < xmin) dx = xmin - x0;
        if (y1 < ymin) dy = ymin - y0;
        if (x1 > xmax) dx = xmax - x0;
        if (y1 > ymax) dy = ymax - y0;

    /* Engage... */

        XWarpPointer(plFramePtr->display, Tk_WindowId(tkwin),
                     None, 0, 0, 0, 0, dx, dy);
        eventPtr->type = 0;
    }
}

/*--------------------------------------------------------------------------*\
 * CreateXhairs()
 *
 * Creates graphic crosshairs at current pointer location.
\*--------------------------------------------------------------------------*/

static void
CreateXhairs(PlFrame *plFramePtr)
{
    register Tk_Window tkwin = plFramePtr->tkwin;
    Window root, child;
    int root_x, root_y, win_x, win_y;
    unsigned int mask;


/* Find current pointer location and draw graphic crosshairs if pointer is */
/* inside our window. */

    if (XQueryPointer(plFramePtr->display, Tk_WindowId(tkwin),
                      &root, &child, &root_x, &root_y, &win_x, &win_y,
                      &mask)) {

        #ifdef MAC_TCL
        /* Mac Tk only has a partial implementation of the above function
           so we must fix it */
        Tk_GetRootCoords(tkwin,&win_x,&win_y);
        win_x = root_x - win_x;
        win_y = root_y - win_y;
        #endif
        if (win_x >= 0 && win_x < Tk_Width(tkwin) &&
            win_y >= 0 && win_y < Tk_Height(tkwin)) {
            DrawXhairs(plFramePtr, win_x, win_y);
            plFramePtr->drawing_xhairs = 1;
        }
    }

}

/*--------------------------------------------------------------------------*\
 * DestroyXhairs()
 *
 * Destroys graphic crosshairs.
\*--------------------------------------------------------------------------*/

static void
DestroyXhairs(PlFrame *plFramePtr)
{

/* This draw removes the last set of graphic crosshairs */

    UpdateXhairs(plFramePtr);
    plFramePtr->drawing_xhairs = 0;
}

/*--------------------------------------------------------------------------*\
 * DrawXhairs()
 *
 * Draws graphic crosshairs at (x0, y0).  The first draw erases the old set.
\*--------------------------------------------------------------------------*/

static void
DrawXhairs(PlFrame *plFramePtr, int x0, int y0)
{
    register Tk_Window tkwin = plFramePtr->tkwin;
    int xmin = 0, xmax = Tk_Width(tkwin) - 1;
    int ymin = 0, ymax = Tk_Height(tkwin) - 1;

    if (plFramePtr->drawing_xhairs)
        UpdateXhairs(plFramePtr);

    plFramePtr->xhair_x[0].x = xmin; plFramePtr->xhair_x[0].y = y0;
    plFramePtr->xhair_x[1].x = xmax; plFramePtr->xhair_x[1].y = y0;

    plFramePtr->xhair_y[0].x = x0; plFramePtr->xhair_y[0].y = ymin;
    plFramePtr->xhair_y[1].x = x0; plFramePtr->xhair_y[1].y = ymax;

    UpdateXhairs(plFramePtr);
}

/*--------------------------------------------------------------------------*\
 * UpdateXhairs()
 *
 * Updates graphic crosshairs.  If already there, they are erased.
\*--------------------------------------------------------------------------*/

static void
UpdateXhairs(PlFrame *plFramePtr)
{
    register Tk_Window tkwin = plFramePtr->tkwin;

    XDrawLines(Tk_Display(tkwin), Tk_WindowId(tkwin),
               plFramePtr->xorGC, plFramePtr->xhair_x, 2,
               CoordModeOrigin);

    XDrawLines(Tk_Display(tkwin), Tk_WindowId(tkwin),
               plFramePtr->xorGC, plFramePtr->xhair_y, 2,
               CoordModeOrigin);
}

/*--------------------------------------------------------------------------*\
 * CreateRband()
 *
 * Initiate rubber banding.
\*--------------------------------------------------------------------------*/

static void
CreateRband(PlFrame *plFramePtr)
{
    register Tk_Window tkwin = plFramePtr->tkwin;
    Window root, child;
    int root_x, root_y, win_x, win_y;
    unsigned int mask;

/* Find current pointer location, and initiate rubber banding. */

    if (XQueryPointer(plFramePtr->display, Tk_WindowId(tkwin),
                      &root, &child, &root_x, &root_y, &win_x, &win_y,
                      &mask)) {
        #ifdef MAC_TCL
        /* Mac Tk only has a partial implementation of the above function
           so we must fix it */
        Tk_GetRootCoords(tkwin,&win_x,&win_y);
        win_x = root_x - win_x;
        win_y = root_y - win_y;
        #endif
        if (win_x >= 0 && win_x < Tk_Width(tkwin) &&
            win_y >= 0 && win_y < Tk_Height(tkwin)) {

        /* Okay, pointer is in our window. */
            plFramePtr->rband_pt[0].x = win_x;
            plFramePtr->rband_pt[0].y = win_y;

            DrawRband(plFramePtr, win_x, win_y);
            plFramePtr->drawing_rband = 1;
        } else {
        /* Hmm, somehow they turned it on without even being in the window.
           Just put the anchor in top left, they'll soon realize this is a
           mistake... */

            plFramePtr->rband_pt[0].x = 0;
            plFramePtr->rband_pt[0].y = 0;

            DrawRband(plFramePtr, win_x, win_y);
            plFramePtr->drawing_rband = 1;
        }
    }

}

/*--------------------------------------------------------------------------*\
 * DestroyRband()
 *
 * Turn off rubber banding.
\*--------------------------------------------------------------------------*/

static void
DestroyRband(PlFrame *plFramePtr)
{

/* This draw removes the residual rubber band. */

    UpdateRband(plFramePtr);
    plFramePtr->drawing_rband = 0;
}

/*--------------------------------------------------------------------------*\
 * DrawRband()
 *
 * Draws a rubber band from the anchor to the current cursor location.
\*--------------------------------------------------------------------------*/

static void
DrawRband(PlFrame *plFramePtr, int x0, int y0)
{
    register Tk_Window tkwin = plFramePtr->tkwin;
    int xmin = 0, xmax = Tk_Width(tkwin) - 1;
    int ymin = 0, ymax = Tk_Height(tkwin) - 1;

/* If the line is already up, clear it. */

    if (plFramePtr->drawing_rband)
        UpdateRband(plFramePtr);

    plFramePtr->rband_pt[1].x = x0; plFramePtr->rband_pt[1].y = y0;

    UpdateRband(plFramePtr);
}

/*--------------------------------------------------------------------------*\
 * UpdateRband()
 *
 * Updates rubber band.  If already there, it is erased.
\*--------------------------------------------------------------------------*/

static void
UpdateRband(PlFrame *plFramePtr)
{
    register Tk_Window tkwin = plFramePtr->tkwin;

    XDrawLines(Tk_Display(tkwin), Tk_WindowId(tkwin),
               plFramePtr->xorGC, plFramePtr->rband_pt, 2,
               CoordModeOrigin);
}

/* First-time initialization */
static void PlFrameFirstInit(ClientData clientData) {
    register PlFrame *plFramePtr = (PlFrame *) clientData;
    register Tk_Window tkwin = plFramePtr->tkwin;

    plsstrm(plFramePtr->ipls);
    plsdev("tkwin");
/* We should probably rename plsxwin to plstkwin */
    plsxwin(Tk_WindowId(tkwin));
    plspause(0);
    plinit();
    if (plplot_ccmap) {
	Install_cmap(plFramePtr);
    }
    plbop();
    
    plFramePtr->tkwin_initted = 1;
    plFramePtr->width  = Tk_Width(tkwin);
    plFramePtr->height = Tk_Height(tkwin);
    plFramePtr->prevWidth = plFramePtr->width;
    plFramePtr->prevHeight = plFramePtr->height;
}

/*
 *---------------------------------------------------------------------------
 *
 * PlFrameInit --
 *
 *      Invoked to handle miscellaneous initialization after window gets
 *      mapped.  
 *
 * Results:
 *      None.
 *
 * Side effects:
 *      PLplot internal parameters and device driver are initialized.
 *
 *--------------------------------------------------------------------------- */

static void
PlFrameInit(ClientData clientData)
{
    register PlFrame *plFramePtr = (PlFrame *) clientData;
    register Tk_Window tkwin = plFramePtr->tkwin;

/* Set up window parameters and arrange for window to be refreshed */

    plFramePtr->flags |= REFRESH_PENDING;
    plFramePtr->flags |= UPDATE_V_SCROLLBAR | UPDATE_H_SCROLLBAR;

/* Draw plframe */

    DisplayPlFrame(clientData);

    if (plFramePtr->xhairs)
        CreateXhairs(plFramePtr);

    if (plFramePtr->rband)
        CreateRband(plFramePtr);
}

/*
 *---------------------------------------------------------------------------
 *
 * Install_cmap --
 *
 *      Installs X driver color map as necessary when custom color maps
 *      are used. 
 *
 * Results:
 *      None.
 *
 * Side effects:
 *      Parent color maps may get changed.
 *
 *---------------------------------------------------------------------------
 */

static void
Install_cmap(PlFrame *plFramePtr)
{
    TkwDev *dev;

#define INSTALL_COLORMAP_IN_TK
#ifdef  INSTALL_COLORMAP_IN_TK
    dev = (TkwDev *) plFramePtr->pls->dev;
    Tk_SetWindowColormap(Tk_MainWindow(plFramePtr->interp), dev->tkwd->map);

/*
 * If the colormap is local to this widget, the WM must be informed that
 * it should be installed when the widget gets the focus.  The top level
 * window must be added to the end of its own list, because otherwise the
 * window manager adds it to the front (as required by the ICCCM).  Thanks
 * to Paul Mackerras for providing this info in his TK photo widget.
 */

#else
    int count = 0;
    Window top, colormap_windows[5];

    top = Tk_WindowId(Tk_MainWindow(plFramePtr->interp));

    colormap_windows[count++] = Tk_WindowId(plFramePtr->tkwin);
    colormap_windows[count++] = top;

    if ( ! XSetWMColormapWindows(plFramePtr->display,
                                 top, colormap_windows, count))
      fprintf(stderr, "Unable to set color map property!\n");
#endif
}

/*
 *---------------------------------------------------------------------------
 *
 * DisplayPlFrame --
 *
 *      This procedure is invoked to display a plframe widget.
 *
 * Results:
 *      None.
 *
 * Side effects:
 *      Commands are output to X to display the plframe in its
 *      current mode.
 *
 *---------------------------------------------------------------------------
 */

static void
DisplayPlFrame(ClientData clientData)
{
    register PlFrame *plFramePtr = (PlFrame *) clientData;
    register Tk_Window tkwin = plFramePtr->tkwin;

    dbug_enter("DisplayPlFrame");

/* Update scrollbars if needed */

    if (plFramePtr->flags & UPDATE_V_SCROLLBAR) {
        UpdateVScrollbar(plFramePtr);
    }
    if (plFramePtr->flags & UPDATE_H_SCROLLBAR) {
        UpdateHScrollbar(plFramePtr);
    }
    plFramePtr->flags &= ~(UPDATE_V_SCROLLBAR|UPDATE_H_SCROLLBAR);

/* If not mapped yet, just return and cancel pending refresh */

    if ((plFramePtr->tkwin == NULL) || ! Tk_IsMapped(tkwin)) {
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
            Tcl_DoWhenIdle(DisplayPlFrame, clientData);
            return;
        }

    /* Redraw border if necessary */

	if ((plFramePtr->border != NULL) &&
	    (plFramePtr->relief != TK_RELIEF_FLAT)) {
	    Tk_Draw3DRectangle(plFramePtr->tkwin, Tk_WindowId(tkwin),
			       plFramePtr->border, 0, 0, Tk_Width(tkwin), Tk_Height(tkwin),
			       plFramePtr->borderWidth, plFramePtr->relief);
	}

    /* Redraw -- replay contents of plot buffer */

        if (plFramePtr->flags & REDRAW_PENDING) {
            plFramePtr->flags &= ~REDRAW_PENDING;
            plsstrm(plFramePtr->ipls);
            pl_cmd(PLESC_REDRAW, (void *) NULL);
        }

    /* Resize -- if window bounds have changed */

        else if ((plFramePtr->width != plFramePtr->prevWidth) ||
                 (plFramePtr->height != plFramePtr->prevHeight)) {

            plFramePtr->pldis.width = plFramePtr->width;
            plFramePtr->pldis.height = plFramePtr->height;

            plsstrm(plFramePtr->ipls);
            pl_cmd(PLESC_RESIZE, (void *) &(plFramePtr->pldis));
            plFramePtr->prevWidth = plFramePtr->width;
            plFramePtr->prevHeight = plFramePtr->height;
        }

    /* Expose -- if window bounds are unchanged */

        else {
            if (plFramePtr->drawing_xhairs) {
                #ifdef MAC_TCL
                /* For MacTk we just zap the whole window like this */
                Tk_Draw3DRectangle(plFramePtr->tkwin, Tk_WindowId(tkwin),
                        plFramePtr->border, 0, 0, Tk_Width(tkwin), Tk_Height(tkwin),
                        plFramePtr->borderWidth, plFramePtr->relief);
                #else
                XClearWindow(plFramePtr->display, Tk_WindowId(tkwin));
                #endif
                XFlush(plFramePtr->display);
		plsstrm(plFramePtr->ipls);
                pl_cmd(PLESC_EXPOSE, NULL);
            }
            else {
		plsstrm(plFramePtr->ipls);
                pl_cmd(PLESC_EXPOSE, (void *) &(plFramePtr->pldis));
            }

        /* Reset window bounds so that next time they are set fresh */

            plFramePtr->pldis.x      = Tk_X(tkwin) + Tk_Width(tkwin);
            plFramePtr->pldis.y      = Tk_Y(tkwin) + Tk_Height(tkwin);
            plFramePtr->pldis.width  = -Tk_Width(tkwin);
            plFramePtr->pldis.height = -Tk_Height(tkwin);
        }

    /* Update graphic crosshairs if necessary */

        if (plFramePtr->drawing_xhairs) {
            UpdateXhairs(plFramePtr);
        }

    /* Update rubber band if necessary. */

        if (plFramePtr->drawing_rband) {
            UpdateRband(plFramePtr);
        }
    }
}

/*--------------------------------------------------------------------------*\
 * Routines to process widget commands.
\*--------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------*\
 * scol0
 *
 * Sets a color in cmap0.
\*--------------------------------------------------------------------------*/

static int
scol0(Tcl_Interp *interp, register PlFrame *plFramePtr,
      int i, char *col, int *p_changed)
{
    PLStream *pls = plFramePtr->pls;
    XColor xcol;
    PLINT r, g, b;

    if ( col == NULL ) {
        Tcl_AppendResult(interp, "color value not specified",
                         (char *) NULL);
        return TCL_ERROR;
    }

    if ( ! XParseColor(plFramePtr->display,
                       Tk_Colormap(plFramePtr->tkwin), col, &xcol)) {
        Tcl_AppendResult(interp, "Couldn't parse color ", col,
                         (char *) NULL);
        return TCL_ERROR;
    }

    r = (unsigned) (xcol.red   & 0xFF00) >> 8;
    g = (unsigned) (xcol.green & 0xFF00) >> 8;
    b = (unsigned) (xcol.blue  & 0xFF00) >> 8;

    if ( (pls->cmap0[i].r != r) ||
         (pls->cmap0[i].g != g) ||
         (pls->cmap0[i].b != b) ) {

        pls->cmap0[i].r = r;
        pls->cmap0[i].g = g;
        pls->cmap0[i].b = b;
        *p_changed = 1;
    }

    return TCL_OK;
}

/*--------------------------------------------------------------------------*\
 * scol1
 *
 * Sets a color in cmap1.
\*--------------------------------------------------------------------------*/

static int
scol1(Tcl_Interp *interp, register PlFrame *plFramePtr,
      int i, char *col, char *pos, char *rev, int *p_changed)
{
    PLStream *pls = plFramePtr->pls;
    XColor xcol;
    PLFLT h, l, s, r, g, b, p;
    int reverse;

    if ( col == NULL ) {
        Tcl_AppendResult(interp, "color value not specified",
                         (char *) NULL);
        return TCL_ERROR;
    }

    if ( pos == NULL ) {
        Tcl_AppendResult(interp, "control point position not specified",
                         (char *) NULL);
        return TCL_ERROR;
    }

    if ( rev == NULL ) {
        Tcl_AppendResult(interp, "interpolation sense not specified",
                         (char *) NULL);
        return TCL_ERROR;
    }

    if ( ! XParseColor(plFramePtr->display,
                       Tk_Colormap(plFramePtr->tkwin), col, &xcol)) {
        Tcl_AppendResult(interp, "Couldn't parse color ", col,
                         (char *) NULL);
        return TCL_ERROR;
    }

    r = ((unsigned) (xcol.red   & 0xFF00) >> 8) / 255.0;
    g = ((unsigned) (xcol.green & 0xFF00) >> 8) / 255.0;
    b = ((unsigned) (xcol.blue  & 0xFF00) >> 8) / 255.0;

    plRGB_HLS(r, g, b, &h, &l, &s);

    p = atof(pos) / 100.0;
    reverse = atoi(rev);

    if ( (pls->cmap1cp[i].h != h) ||
         (pls->cmap1cp[i].l != l) ||
         (pls->cmap1cp[i].s != s) ||
         (pls->cmap1cp[i].p != p) ||
         (pls->cmap1cp[i].rev != reverse) ) {

        pls->cmap1cp[i].h = h;
        pls->cmap1cp[i].l = l;
        pls->cmap1cp[i].s = s;
        pls->cmap1cp[i].p = p;
        pls->cmap1cp[i].rev = reverse;
        *p_changed = 1;
    }
    return TCL_OK;
}

/*--------------------------------------------------------------------------*\
 * Cmd
 *
 * Processes "cmd" widget command.
 * Handles commands that go more or less directly to the PLplot library.
 * Most of these come out of the PLplot Tcl API support file.
\*--------------------------------------------------------------------------*/

static int
Cmd(Tcl_Interp *interp, register PlFrame *plFramePtr,
    int argc, char **argv)
{
    PLStream *pls = plFramePtr->pls;
    int length;
    char c3;
    int result = TCL_OK;
    char cmdlist[] = "plgcmap0 plgcmap1 plscmap0 plscmap1 plscol0 plscol1";

#ifdef DEBUG
    if (pls->debug) {
        int i;
        fprintf(stderr, "There are %d arguments to Cmd:", argc);
        for (i = 0; i < argc; i++) {
            fprintf(stderr, " %s", argv[i]);
        }
        fprintf(stderr, "\n");
    }
#endif

/* no option -- return list of available PLplot commands */

    if (argc == 0) 
        return plTclCmd(cmdlist, interp, argc, argv);

/* Make sure widget has been initialized before going any further */

    if ( ! plFramePtr->tkwin_initted) {
        Tcl_VarEval(plFramePtr->interp, "update", (char *) NULL);
    }

/* Set stream number and get ready to process the command */

    plsstrm(plFramePtr->ipls);

    c3 = argv[0][2];
    length = strlen(argv[0]);

/* plgcmap0 -- get color map 0 */
/* first arg is number of colors, the rest are hex number specifications */

    if ((c3 == 'g') && (strncmp(argv[0], "plgcmap0", length) == 0)) {
        int i;
        unsigned long plcolor;
        char str[10];

        sprintf(str, "%d", (int) pls->ncol0);
        Tcl_AppendElement(interp, str);
        for (i = 0; i < pls->ncol0; i++) {
            plcolor = ((pls->cmap0[i].r << 16) | 
                       (pls->cmap0[i].g << 8) |
                       (pls->cmap0[i].b));

            sprintf(str, "#%06lx", (plcolor & 0xFFFFFF));
            Tcl_AppendElement(interp, str);
        }
        result = TCL_OK;
    }

/* plgcmap1 -- get color map 1 */
/* first arg is number of control points */
/* the rest are hex number specifications followed by positions (0-100) */

    else if ((c3 == 'g') && (strncmp(argv[0], "plgcmap1", length) == 0)) {
        int i;
        unsigned long plcolor;
        char str[10];
        PLFLT h, l, s, r, g, b;
        int r1, g1, b1;

        sprintf(str, "%d", (int) pls->ncp1);
        Tcl_AppendElement(interp, str);
        for (i = 0; i < pls->ncp1; i++) {
            h = pls->cmap1cp[i].h;
            l = pls->cmap1cp[i].l;
            s = pls->cmap1cp[i].s;

            plHLS_RGB(h, l, s, &r, &g, &b);

            r1 = MAX(0, MIN(255, (int) (256. * r)));
            g1 = MAX(0, MIN(255, (int) (256. * g)));
            b1 = MAX(0, MIN(255, (int) (256. * b)));

            plcolor = ((r1 << 16) | (g1 << 8) | (b1));

            sprintf(str, "#%06lx", (plcolor & 0xFFFFFF));
            Tcl_AppendElement(interp, str);

            sprintf(str, "%02d", (int) (100*pls->cmap1cp[i].p));
            Tcl_AppendElement(interp, str);

            sprintf(str, "%01d", (int) (pls->cmap1cp[i].rev));
            Tcl_AppendElement(interp, str);
        }
        result = TCL_OK;
    }

/* plscmap0 -- set color map 0 */
/* first arg is number of colors, the rest are hex number specifications */

    else if ((c3 == 's') && (strncmp(argv[0], "plscmap0", length) == 0)) {
        int i, changed = 1, ncol0 = atoi(argv[1]);
        char *col;

        if (ncol0 > 16 || ncol0 < 1) {
            Tcl_AppendResult(interp, "illegal number of colors in cmap0: ",
                             argv[1], (char *) NULL);
            return TCL_ERROR;
        }

        pls->ncol0 = ncol0;
        for (i = 0; i < pls->ncol0; i++) {
            col = strtok(argv[2+i], " ");
            if ( col == NULL )
                break;

            if (scol0(interp, plFramePtr, i, col, &changed) != TCL_OK)
                return TCL_ERROR;

        }

        if (changed)
            plP_state(PLSTATE_CMAP0);
    }

/* plscmap1 -- set color map 1 */
/* first arg is number of colors, the rest are hex number specifications */

    else if ((c3 == 's') && (strncmp(argv[0], "plscmap1", length) == 0)) {
        int i, changed = 1, ncp1 = atoi(argv[1]);
        char *col, *pos, *rev;

        if (ncp1 > 32 || ncp1 < 1) {
            Tcl_AppendResult(interp,
                             "illegal number of control points in cmap1: ",
                             argv[1], (char *) NULL);
            return TCL_ERROR;
        }

        col = strtok(argv[2], " ");
        pos = strtok(NULL, " ");
        rev = strtok(NULL, " ");
        for (i = 0; i < ncp1; i++) {
            if ( col == NULL )
                break;

            if (scol1(interp, plFramePtr,
                      i, col, pos, rev, &changed) != TCL_OK)
                return TCL_ERROR;

            col = strtok(NULL, " ");
            pos = strtok(NULL, " ");
            rev = strtok(NULL, " ");
        }

        if (changed) {
            plsc->ncp1 = ncp1;
            plcmap1_calc();
        }
    }

/* plscol0 -- set single color in cmap0 */
/* first arg is the color number, the next is the color in hex */

    else if ((c3 == 's') && (strncmp(argv[0], "plscol0", length) == 0)) {
        int i = atoi(argv[1]), changed = 1;

        if (i > pls->ncol0 || i < 0) {
            Tcl_AppendResult(interp, "illegal color number in cmap0: ",
                             argv[1], (char *) NULL);
            return TCL_ERROR;
        }

        if (scol0(interp, plFramePtr, i, argv[2], &changed) != TCL_OK)
            return TCL_ERROR;

        if (changed)
            plP_state(PLSTATE_CMAP0);
    }

/* plscol1 -- set color of control point in cmap1 */
/* first arg is the control point, the next two are the color in hex and pos */

    else if ((c3 == 's') && (strncmp(argv[0], "plscol1", length) == 0)) {
        int i = atoi(argv[1]), changed = 1;

        if (i > pls->ncp1 || i < 0) {
            Tcl_AppendResult(interp, "illegal control point number in cmap1: ",
                             argv[1], (char *) NULL);
            return TCL_ERROR;
        }

        if (scol1(interp, plFramePtr,
                  i, argv[2], argv[3], argv[4], &changed) != TCL_OK)
            return TCL_ERROR;

        if (changed) 
            plcmap1_calc();
    }

/* Added by Vince, disabled by default since we want a minimal patch */
#ifdef USING_PLESC_COPY
/* plcopy -- copy a region of the plot; useful for scrolling plots */
/* first 4 args are the source rectangle, next 2 args are the destination */

    else if ((c3 == 'c') && (strncmp(argv[0], "plcopy", length) == 0)) {
	PLFLT xx[3], yy[3];
	if (argc != 7) {
	    Tcl_AppendResult(interp, "Need exactly 6 arguments to copy.",
			     (char *) NULL);
	    return TCL_ERROR;
	}
	xx[0] = atof(argv[1]);
	yy[0] = atof(argv[2]);
	xx[1] = atof(argv[3]);
	yy[1] = atof(argv[4]);
	xx[2] = atof(argv[5]);
	yy[2] = atof(argv[6]);
	plcopy(xx,yy);
    }
#endif

/* unrecognized, so give it to plTclCmd to take care of */

    else 
        result = plTclCmd(cmdlist, interp, argc, argv);

    plflush();
    return result;
}

static void ActiveState(register PlFrame *plFramePtr, int on) {
    if(on ) {
        if(!plFramePtr->isActive) {
            Tk_CreateEventHandler(plFramePtr->tkwin, ButtonPressMask,
                                  PlFrameButtonPressEH, (ClientData) plFramePtr);

            Tk_CreateEventHandler(plFramePtr->tkwin, PointerMotionMask,
                                  PlFrameMotionEH, (ClientData) plFramePtr);
        
            Tk_CreateEventHandler(plFramePtr->tkwin, EnterWindowMask,
                                  PlFrameEnterEH, (ClientData) plFramePtr);
        
            Tk_CreateEventHandler(plFramePtr->tkwin, LeaveWindowMask,
                                  PlFrameLeaveEH, (ClientData) plFramePtr);
        
            Tk_CreateEventHandler(plFramePtr->tkwin, KeyPressMask,
                                  PlFrameKeyEH, (ClientData) plFramePtr);
        /* Switch to crosshair cursor. */
        
            Tk_DefineCursor(plFramePtr->tkwin, plFramePtr->xhair_cursor);
        }
    } else {
        if(plFramePtr->isActive) {
            Tk_DeleteEventHandler(plFramePtr->tkwin, ButtonPressMask,
                                  PlFrameButtonPressEH, (ClientData) plFramePtr);
            Tk_DeleteEventHandler(plFramePtr->tkwin, PointerMotionMask,
                                  PlFrameMotionEH, (ClientData) plFramePtr);
    
            Tk_DeleteEventHandler(plFramePtr->tkwin, EnterWindowMask,
                                  PlFrameEnterEH, (ClientData) plFramePtr);
    
            Tk_DeleteEventHandler(plFramePtr->tkwin, LeaveWindowMask,
                                  PlFrameLeaveEH, (ClientData) plFramePtr);
            Tk_DeleteEventHandler(plFramePtr->tkwin, KeyPressMask,
                                  PlFrameKeyEH, (ClientData) plFramePtr);
        /* Switch back to boring old pointer */
        
            Tk_DefineCursor(plFramePtr->tkwin, plFramePtr->cursor);
        }
    }
}


/*
 *---------------------------------------------------------------------------
 *
 * ConfigurePlFrame --
 *
 *      This procedure is called to process an argv/argc list, plus the Tk
 *      option database, in order to configure (or reconfigure) a 
 *      plframe widget.
 *
 * Results:
 *      The return value is a standard Tcl result.  If TCL_ERROR is
 *      returned, then interp->result contains an error message.
 *
 * Side effects:
 *      Configuration information, such as text string, colors, font, etc.
 *      get set for plFramePtr; old resources get freed, if there were
 *      any.
 *
 *---------------------------------------------------------------------------
 */

static int
ConfigurePlFrame(Tcl_Interp *interp, register PlFrame *plFramePtr,
                 int argc, char **argv, int flags)
{
    register Tk_Window tkwin = plFramePtr->tkwin;
    PLStream *pls = plFramePtr->pls;
    TkwDev *dev = (TkwDev *) pls->dev;
    TkwDisplay *tkwd = (TkwDisplay *) dev->tkwd;
    XGCValues gcValues;
    unsigned long mask;
    int need_redisplay = 0;

#ifdef DEBUG
    if (pls->debug) {
        int i;
        fprintf(stderr, "Arguments to configure are:");
        for (i = 0; i < argc; i++) {
            fprintf(stderr, " %s", argv[i]);
        }
        fprintf(stderr, "\n");
    }
#endif

    dbug_enter("ConfigurePlFrame");

    if (Tk_ConfigureWidget(interp, tkwin, configSpecs,
            argc, argv, (char *) plFramePtr, flags) != TCL_OK) {
        return TCL_ERROR;
    }

/*
 * Set background color using tkwin driver's pixel value.  Done this way so
 * that (a) we can use r/w color cells, and (b) the BG pixel values as set
 * here and in the tkwin driver are consistent.
 */

    plsstrm(plFramePtr->ipls);
    if(PLColor_from_XColor_Changed(&pls->cmap0[0], 
      Tk_3DBorderColor(plFramePtr->border))) {
    	need_redisplay = 1;
    	/* need to redraw as well as simply refresh the window */
        plFramePtr->flags |= REDRAW_PENDING;
    }
    plX_setBGFG(pls);

    Tk_SetWindowBackground(tkwin, tkwd->cmap0[0].pixel);
    Tk_SetWindowBorder(tkwin, tkwd->cmap0[0].pixel);

    /* Set up GC for rubber-band draws */

    gcValues.background = tkwd->cmap0[0].pixel;
    gcValues.foreground = 0xFF;
    gcValues.function = GXxor;
    mask = GCForeground | GCBackground | GCFunction;

    if (plFramePtr->xorGC != NULL)
        Tk_FreeGC(plFramePtr->display, plFramePtr->xorGC);

    plFramePtr->xorGC = Tk_GetGC(plFramePtr->tkwin, mask, &gcValues);

/* Geometry settings */

    Tk_SetInternalBorder(tkwin, plFramePtr->borderWidth);
    if ((plFramePtr->width > 0) || (plFramePtr->height > 0)) {
        Tk_GeometryRequest(tkwin, plFramePtr->width, plFramePtr->height);
        if ((plFramePtr->width != plFramePtr->prevWidth) ||
            (plFramePtr->height != plFramePtr->prevHeight))
            need_redisplay = 1;
    }

/* Create or destroy graphic crosshairs as specified */

    if (Tk_IsMapped(tkwin)) {
        if (plFramePtr->xhairs) {
            if (! plFramePtr->drawing_xhairs)
                CreateXhairs(plFramePtr);
        }
        else {
            if (plFramePtr->drawing_xhairs)
                DestroyXhairs(plFramePtr);
        }
    }

/* Create or destroy rubber band as specified */

    if (Tk_IsMapped(tkwin)) {
        if (plFramePtr->rband) {
            if (! plFramePtr->drawing_rband)
                CreateRband(plFramePtr);
        }
        else {
            if (plFramePtr->drawing_rband)
                DestroyRband(plFramePtr);
        }
    }
/* Sets or clears events for the plot */
    ActiveState(plFramePtr,plFramePtr->active_plot);
        
    if (!pls->nopixmap) {
	/* can only adjust if this flag not set */
	if(plFramePtr->double_buffer != pls->db) {
	    pls->db = plFramePtr->double_buffer;
	    /* turn on/off dbl-buffering in the driver */
	    dev->write_to_window = !pls->db;
	}
    } else {
	plFramePtr->double_buffer = 0;
    }
	
/* Arrange for window to be refreshed if necessary */
    if (need_redisplay && Tk_IsMapped(tkwin)
            && !(plFramePtr->flags & REFRESH_PENDING)) {
        Tcl_DoWhenIdle(DisplayPlFrame, (ClientData) plFramePtr);
        plFramePtr->flags |= REFRESH_PENDING;
        plFramePtr->flags |= UPDATE_V_SCROLLBAR|UPDATE_H_SCROLLBAR;
    }

    return TCL_OK;
}

/*--------------------------------------------------------------------------*\
 * Draw
 *
 * Processes "draw" widget command.
 * Handles rubber-band drawing.
\*--------------------------------------------------------------------------*/

static int
Draw(Tcl_Interp *interp, register PlFrame *plFramePtr,
     int argc, char **argv)
{
    register Tk_Window tkwin = plFramePtr->tkwin;
    int result = TCL_OK;
    char c = argv[0][0];
    int length = strlen(argv[0]);

/* Make sure widget has been initialized before going any further */

    if ( ! plFramePtr->tkwin_initted) {
        Tcl_VarEval(plFramePtr->interp, "update", (char *) NULL);
    }

/* init -- sets up for rubber-band drawing */

    if ((c == 'i') && (strncmp(argv[0], "init", length) == 0)) {
        Tk_DefineCursor(tkwin, plFramePtr->xhair_cursor);
    }

/* end -- ends rubber-band drawing */

    else if ((c == 'e') && (strncmp(argv[0], "end", length) == 0)) {

        Tk_DefineCursor(tkwin, plFramePtr->cursor);
        if (plFramePtr->continue_draw) {
            XDrawLines(Tk_Display(tkwin), Tk_WindowId(tkwin),
                       plFramePtr->xorGC, plFramePtr->pts, 5,
                       CoordModeOrigin);
            XSync(Tk_Display(tkwin), 0);
        }

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

/*--------------------------------------------------------------------------*\
 * Info
 *
 * Processes "info" widget command.
 * Returns requested info.
\*--------------------------------------------------------------------------*/

static int
Info(Tcl_Interp *interp, register PlFrame *plFramePtr,
     int argc, char **argv)
{
    int length;
    char c;
    int result = TCL_OK;

/* no option -- return list of available info commands */

    if (argc == 0) {
        Tcl_SetResult(interp, "devkeys devnames", TCL_STATIC);
        return TCL_OK;
    }

    c = argv[0][0];
    length = strlen(argv[0]);

/* devkeys -- return list of supported device keywords */

    if ((c == 'd') && (strncmp(argv[0], "devkeys", length) == 0)) {
        int i = 0;
        while (plFramePtr->devName[i] != NULL) 
            Tcl_AppendElement(interp, plFramePtr->devName[i++]);

        result = TCL_OK;
    }

/* devkeys -- return list of supported device types */

    else if ((c == 'd') && (strncmp(argv[0], "devnames", length) == 0)) {
        int i = 0;
        while (plFramePtr->devDesc[i] != NULL) 
            Tcl_AppendElement(interp, plFramePtr->devDesc[i++]);

        result = TCL_OK;
    }

/* unrecognized */

    else {
        Tcl_AppendResult(interp, "bad option to \"info\": must be ", 
         "devkeys, devnames", (char *) NULL);

        result = TCL_ERROR;
    }

    return result;
}

/*--------------------------------------------------------------------------*\
 * Openlink
 *
 * Processes "openlink" widget command.
 * Opens channel (FIFO or socket) for binary data transfer between client
 * and server.
\*--------------------------------------------------------------------------*/

static int
Openlink(Tcl_Interp *interp, register PlFrame *plFramePtr,
         int argc, char **argv)
{
#if !defined(MAC_TCL) && !defined(__WIN32__)
    register PLRDev *plr = plFramePtr->plr;
    register PLiodev *iodev = plr->iodev;

    char c = argv[0][0];
    int length = strlen(argv[0]);

    dbug_enter("Openlink");

/* Open fifo */

    if ((c == 'f') && (strncmp(argv[0], "fifo", length) == 0)) {

        if (argc < 1) {
            Tcl_AppendResult(interp, "bad command -- must be: ",
                             "openlink fifo <pathname>",
                             (char *) NULL);
            return TCL_ERROR;
        }
        if ((iodev->fd = open (argv[1], O_RDONLY)) == -1) {
            Tcl_AppendResult(interp, "cannot open fifo ", argv[1],
                             " for read", (char *) NULL);
            return TCL_ERROR;
        }
        iodev->type = 0;
        iodev->typeName = "fifo";
        iodev->file = fdopen(iodev->fd, "rb");
    }

/* Open socket */

    else if ((c == 's') && (strncmp(argv[0], "socket", length) == 0)) {

        if (argc < 1) {
            Tcl_AppendResult(interp, "bad command -- must be: ",
                             "openlink socket <sock-id>",
                             (char *) NULL);
            return TCL_ERROR;
        }
        iodev->type = 1;
        iodev->typeName = "socket";
        iodev->fileHandle = argv[1];

        if (Tcl_GetOpenFile(interp, iodev->fileHandle,
                            0, 1, (ClientData) &iodev->file) != TCL_OK) {
            return TCL_ERROR;
        }
        iodev->fd = fileno(iodev->file);
    }

/* unrecognized */

    else {
        Tcl_AppendResult(interp, "bad option to \"openlink\": must be ", 
         "fifo or socket", (char *) NULL);

        return TCL_ERROR;
    }

    plr->pdfs = pdf_bopen( NULL, 4200 );
    Tcl_CreateFileHandler( iodev->fd,
                           TK_READABLE, (Tk_FileProc *) ReadData,
                           (ClientData) plFramePtr );

#endif

    return TCL_OK;
}

/*--------------------------------------------------------------------------*\
 * Closelink
 *
 * Processes "closelink" widget command.
 * CLoses channel previously opened with the "openlink" widget command.
\*--------------------------------------------------------------------------*/

static int
Closelink(Tcl_Interp *interp, register PlFrame *plFramePtr,
         int argc, char **argv)
{
#if !defined(MAC_TCL) && !defined(__WIN32__)
    register PLRDev *plr = plFramePtr->plr;
    register PLiodev *iodev = plr->iodev;

    dbug_enter("Closelink");

    if (iodev->fd == 0) {
        Tcl_AppendResult(interp, "no link currently open", (char *) NULL);
        return TCL_ERROR;
    }

    Tcl_DeleteFileHandler(iodev->fd);
    pdf_close(plr->pdfs);
    iodev->fd = 0;
#endif
    return TCL_OK;
}

/*--------------------------------------------------------------------------*\
 * process_data
 *
 * Utility function for processing data and other housekeeping.
\*--------------------------------------------------------------------------*/

static int
process_data(Tcl_Interp *interp, register PlFrame *plFramePtr)
{
    register PLRDev *plr = plFramePtr->plr;
    register PLiodev *iodev = plr->iodev;
    int result = TCL_OK;

/* Process data */

    if (plr_process(plr) == -1) {
        Tcl_AppendResult(interp, "unable to read from ", iodev->typeName,
                         (char *) NULL);
        result = TCL_ERROR;
    }

/* Signal bop if necessary */

    if (plr->at_bop && plFramePtr->bopCmd != NULL) {
        plr->at_bop = 0;
        if (Tcl_Eval(interp, plFramePtr->bopCmd) != TCL_OK)
            fprintf(stderr, "Command \"%s\" failed:\n\t %s\n",
                    plFramePtr->bopCmd, interp->result);
    }

/* Signal eop if necessary */

    if (plr->at_eop && plFramePtr->eopCmd != NULL) {
        plr->at_eop = 0;
        if (Tcl_Eval(interp, plFramePtr->eopCmd) != TCL_OK)
            fprintf(stderr, "Command \"%s\" failed:\n\t %s\n",
                    plFramePtr->eopCmd, interp->result);
    }

    return result;
}

void PlframeAtEop(Tcl_Interp *interp, register PlFrame *plFramePtr) {
    if (plFramePtr->eopCmd != NULL) {
        if (Tcl_Eval(interp, plFramePtr->eopCmd) != TCL_OK)
            fprintf(stderr, "Command \"%s\" failed:\n\t %s\n",
                    plFramePtr->eopCmd, interp->result);
    }
}

void PlframeAtBop(Tcl_Interp *interp, register PlFrame *plFramePtr) {
    if (plFramePtr->bopCmd != NULL) {
        if (Tcl_Eval(interp, plFramePtr->bopCmd) != TCL_OK)
            fprintf(stderr, "Command \"%s\" failed:\n\t %s\n",
                    plFramePtr->bopCmd, interp->result);
    }
}

/*--------------------------------------------------------------------------*\
 * ReadData
 *
 * Reads & processes data.
 * Intended to be installed as a filehandler command.
\*--------------------------------------------------------------------------*/

static int
ReadData(ClientData clientData, int mask)
{
    register PlFrame *plFramePtr = (PlFrame *) clientData;
    register Tcl_Interp *interp = plFramePtr->interp;

    register PLRDev *plr = plFramePtr->plr;
    register PLiodev *iodev = plr->iodev;
    register PDFstrm *pdfs = plr->pdfs;
    int result = TCL_OK;

    if (mask & TK_READABLE) {

    /* Read from FIFO or socket */

        plsstrm(plFramePtr->ipls);
        #ifndef MAC_TCL
        if (pl_PacketReceive(interp, iodev, pdfs)) {
        #else
        if (1) {
        #endif
            Tcl_AppendResult(interp, "Packet receive failed:\n\t %s\n",
                             interp->result, (char *) NULL);
            return TCL_ERROR;
        }

    /* If the packet isn't complete it will be put back and we just return.
     * Otherwise, the buffer pointer is saved and then cleared so that reads
     * from the buffer start at the beginning.
     */
        if (pdfs->bp == 0)
            return TCL_OK;

        plr->nbytes = pdfs->bp;
        pdfs->bp = 0;
        result = process_data(interp, plFramePtr);
    }

    return result;
}

/*--------------------------------------------------------------------------*\
 * Orient
 *
 * Processes "orient" widget command.
 * Handles orientation of plot.
\*--------------------------------------------------------------------------*/

static int
Orient(Tcl_Interp *interp, register PlFrame *plFramePtr,
       int argc, char **argv)
{
    int result = TCL_OK;

/* orient -- return orientation of current plot window */

    plsstrm(plFramePtr->ipls);

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

/*--------------------------------------------------------------------------*\
 * Print
 *
 * Processes "print" widget command.
 * Handles printing of plot, duh.
 *
 * Creates a temporary file, dumps the current plot to it in metafile
 * form, and then execs the "plpr" script to actually print it.  Since we
 * output it in metafile form here, plpr must invoke plrender to drive the
 * output to the appropriate file type.  The script is responsible for the
 * deletion of the plot metafile.
\*--------------------------------------------------------------------------*/

static int
Print(Tcl_Interp *interp, register PlFrame *plFramePtr,
       int argc, char **argv)
{
    PLINT ipls;
    int result = TCL_OK;
    char *sfnam;
    FILE *sfile;
    pid_t pid;

/* Make sure widget has been initialized before going any further */

    if ( ! plFramePtr->tkwin_initted) {
        Tcl_AppendResult(interp, "Error -- widget not plotted to yet", 
                         (char *) NULL);
        return TCL_ERROR;
    }

/* Create stream for save */

    plmkstrm(&ipls);
    if (ipls < 0) {
        Tcl_AppendResult(interp, "Error -- cannot create stream", 
                         (char *) NULL);
        return TCL_ERROR;
    }

/* Open file for writes */

    sfnam = (char *) tmpnam(NULL);

    if ((sfile = fopen(sfnam, "wb+")) == NULL) {
        Tcl_AppendResult(interp, 
                         "Error -- cannot open plot file for writing",
                         (char *) NULL);
        plend1();
        return TCL_ERROR;
    }

/* Initialize stream */

    plsdev("plmeta");
    plsfile(sfile);
    plcpstrm(plFramePtr->ipls, 0);
    pladv(0);

/* Remake current plot, close file, and switch back to original stream */

    plreplot();
    plend1();
    plsstrm(plFramePtr->ipls);

/* So far so good.  Time to exec the print script. */

    if (plFramePtr->plpr_cmd == NULL)
        plFramePtr->plpr_cmd = plFindCommand("plpr");
#if !defined(MAC_TCL) && !defined(__WIN32__)
    if ((plFramePtr->plpr_cmd == NULL) || (pid = fork()) < 0) {
        Tcl_AppendResult(interp, 
                         "Error -- cannot fork print process",
                         (char *) NULL);
        result = TCL_ERROR;
    }
    else if (pid == 0) {
        if (execl(plFramePtr->plpr_cmd, plFramePtr->plpr_cmd, sfnam, 
                  (char *) 0)) {
            fprintf(stderr, "Unable to exec print command.\n");
            _exit(1);
        }
    }
#endif
    return result;
}

/*--------------------------------------------------------------------------*\
 * NextPage
 *
 * Tells the tkwin driver to move along to the next page.
\*--------------------------------------------------------------------------*/

static int 
NextPage(Tcl_Interp *interp, register PlFrame *plFramePtr,
     int argc, char **argv)
{
    TkwDev *dev = (TkwDev *) plFramePtr->pls->dev;
    int len = 0;
    if(argc == 0) {
    	dev->exit_eventloop = TRUE;
    } else {
        Tcl_AppendResult(interp, "wrong # args: should be \"",
                         " nextpage\"", (char *) NULL);
        return TCL_ERROR;
    }
    return TCL_OK;
}

/*--------------------------------------------------------------------------*\
 * Page
 *
 * Processes "page" widget command.
 * Handles parameters such as margin, aspect ratio, and justification
 * of final plot.
\*--------------------------------------------------------------------------*/

static int
Page(Tcl_Interp *interp, register PlFrame *plFramePtr,
     int argc, char **argv)
{

/* page -- return current device window parameters */

    plsstrm(plFramePtr->ipls);

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
    return (Redraw(interp, plFramePtr, argc-1, argv+1));
}

/*--------------------------------------------------------------------------*\
 * Redraw
 *
 * Processes "redraw" widget command.
 * Turns loose a DoWhenIdle command to redraw plot by replaying contents
 * of plot buffer.
\*--------------------------------------------------------------------------*/

static int
Redraw(Tcl_Interp *interp, register PlFrame *plFramePtr,
       int argc, char **argv)
{
    dbug_enter("Redraw");

    plFramePtr->flags |= REDRAW_PENDING;
    if ((plFramePtr->tkwin != NULL) &&
        !(plFramePtr->flags & REFRESH_PENDING)) {

        Tcl_DoWhenIdle(DisplayPlFrame, (ClientData) plFramePtr);
        plFramePtr->flags |= REFRESH_PENDING;
    }

    return TCL_OK;
}

/*--------------------------------------------------------------------------*\
 * Save
 *
 * Processes "save" widget command.
 * Saves plot to a file.
\*--------------------------------------------------------------------------*/

static int
Save(Tcl_Interp *interp, register PlFrame *plFramePtr,
     int argc, char **argv)
{
    int length;
    char c;
    FILE *sfile;

/* Make sure widget has been initialized before going any further */

    if ( ! plFramePtr->tkwin_initted) {
        Tcl_AppendResult(interp, "Error -- widget not plotted to yet", 
                         (char *) NULL);
        return TCL_ERROR;
    }

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

        plsdev(argv[1]);
        plsfile(sfile);
        plcpstrm(plFramePtr->ipls, 0);
        pladv(0);

    /* Remake current plot and then switch back to original stream */

        plreplot();
        plflush();
        plsstrm(plFramePtr->ipls);
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

/*--------------------------------------------------------------------------*\
 * View
 *
 * Processes "view" widget command.
 * Handles translation & scaling of view into plot.
\*--------------------------------------------------------------------------*/

static int
View(Tcl_Interp *interp, register PlFrame *plFramePtr,
     int argc, char **argv)
{
    int length;
    int dontRedraw = 0;
    char c;
    PLFLT xl, xr, yl, yr;

/* view -- return current relative plot window coordinates */

    plsstrm(plFramePtr->ipls);

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
	if (argc > 1 && (strcmp(argv[1], "wait") == 0)) {
	    /* We're going to update in a while, so don't do it now */
	    dontRedraw = 1;
	}
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
        } else {
            gbox(&xl, &yl, &xr, &yr, argv+1);
            pldid2pc(&xl, &yl, &xr, &yr);
            plsdiplz(xl, yl, xr, yr);
	    if (argc > 5 && (strcmp(argv[5], "wait") == 0)) {
		/* We're going to update in a while, so don't do it now */
		dontRedraw = 1;
	    }
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

    if (dontRedraw) {
        return TCL_OK;
    } else {
	return Redraw(interp, plFramePtr, argc, argv);
    }
}

/*--------------------------------------------------------------------------*\
 * Scroll
 *
 * Processes "xview or yview" widget command.
 * Handles horizontal/vert scroll-bar invoked translation of view into plot.
\*--------------------------------------------------------------------------*/

static int
Scroll(Tcl_Interp *interp, register PlFrame *plFramePtr)
{
    plsstrm(plFramePtr->ipls);
    plsdiplt(plFramePtr->xl, plFramePtr->yl, plFramePtr->xr, plFramePtr->yr);

    plFramePtr->flags |= UPDATE_V_SCROLLBAR | UPDATE_H_SCROLLBAR;
    return (Redraw(interp, plFramePtr, 0, NULL));
}


/*--------------------------------------------------------------------------*\
 * report
 *
 * 4/17/95 GMF
 * Processes "report" widget command.
\*--------------------------------------------------------------------------*/

static int
report( Tcl_Interp *interp, register PlFrame *plFramePtr,
        int argc, char **argv )
{
    float x, y;
/*    fprintf( stdout, "Made it into report, argc=%d\n", argc ); */

    if (argc == 0) {
        interp->result = "report what?";
        return TCL_ERROR;
    }

    if (!strcmp( argv[0], "wc" )) {

        TkwDev *dev = (TkwDev *) plFramePtr->pls->dev;
        PLGraphicsIn *gin = &(dev->gin);

        if (argc != 3) {
            interp->result = "Wrong # of args: report wc x y";
            return TCL_ERROR;
        }

        x = atof( argv[1] );
        y = atof( argv[2] );

        gin->dX = (PLFLT) x / (dev->width - 1);
        gin->dY = 1.0 - (PLFLT) y / (dev->height - 1);

    /* Try to locate cursor */

        if (plTranslateCursor(gin)) {
            sprintf( interp->result, "%f %f", gin->wX, gin->wY );
            return TCL_OK;
        }

        interp->result = "Cannot locate";
        return TCL_OK;
    }

    interp->result = "nonsensical request.";
    return TCL_ERROR;
}

/*--------------------------------------------------------------------------*\
 * Utility routines
\*--------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------*\
 * UpdateVScrollbar
 *
 * Updates vertical scrollbar if needed.
\*--------------------------------------------------------------------------*/

static void
UpdateVScrollbar(register PlFrame *plFramePtr)
{
    char string[60];
    int result;

    if (plFramePtr->yScrollCmd == NULL)
        return;

    sprintf(string, " %f %f",1.-plFramePtr->yr,1.-plFramePtr->yl);

    result = Tcl_VarEval(plFramePtr->interp, plFramePtr->yScrollCmd, string,
                         (char *) NULL);

    if (result != TCL_OK) {
        Tcl_BackgroundError(plFramePtr->interp);
    }
}

/*--------------------------------------------------------------------------*\
 * UpdateHScrollbar
 *
 * Updates horizontal scrollbar if needed.
\*--------------------------------------------------------------------------*/

static void
UpdateHScrollbar(register PlFrame *plFramePtr)
{
    char string[60];
	int result;
	
    if (plFramePtr->xScrollCmd == NULL)
        return;

    sprintf(string, " %f %f",plFramePtr->xl,plFramePtr->xr);

    result = Tcl_VarEval(plFramePtr->interp, plFramePtr->xScrollCmd, string,
                         (char *) NULL);

    if (result != TCL_OK) {
        Tcl_BackgroundError(plFramePtr->interp);
    }
}

/*--------------------------------------------------------------------------*\
 * gbox
 *
 * Returns selection box coordinates.  It's best if the TCL script does
 * bounds checking on the input but I do it here as well just to be safe.
\*--------------------------------------------------------------------------*/

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

#if defined(__MWERKS__) && (defined(__WIN32__) || defined (MAC_TCL))
/* 
 * For some stupid reason, the Metrowerks Codewarrior compiler
 * wants these stupid parameters 
 */
extern void XWarpPointer(
#if NeedFunctionPrototypes
    Display*        d1/* display */,
    Window      d2/* src_w */,
    Window      d3/* dest_w */,
    int         d4/* src_x */,
    int         d5/* src_y */,
    unsigned int   d6 /* src_width */,
    unsigned int   d7 /* src_height */,
    int        d8 /* dest_x */,
    int        d9 /* dest_y */         
#endif
){
}
#endif















