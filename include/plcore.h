/* $Id$
   $Log$
   Revision 1.7  1993/07/02 07:22:14  mjl
   Namespace changes.

 * Revision 1.6  1993/04/26  20:00:10  mjl
 * The beginnings of a TK driver added.
 *
 * Revision 1.5  1993/03/15  21:45:18  mjl
 * Changed _clear/_page driver functions to the names _eop/_bop, to be
 * more representative of what's actually going on.
 *
 * Revision 1.4  1993/03/08  21:47:48  mjl
 * Added a type field for each driver to the dispatch table.  Used for
 * determining whether a device is file-oriented or not.
 *
 * Revision 1.3  1993/03/03  19:42:16  mjl
 * Changed PLSHORT -> short everywhere; now all device coordinates are expected
 * to fit into a 16 bit address space (reasonable, and good for performance).
 *
 * Revision 1.2  1993/02/23  04:53:14  mjl
 * Eliminated xxx_adv (gradv) driver function prototypes.
 *
 * Revision 1.1  1993/01/23  05:34:51  mjl
 * Added this file for inclusion only by plcore.c.  It declares the plstream
 * data structure, and defines, declares, and initializes the dispatch
 * table data structure.
 *
*/

/*	plcore.h

	Contains declarations for core plplot data structures.  This file
	should be included only by plcore.c.
*/

#include "plplotP.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "drivers.h"

/*----------------------------------------------------------------------*\
* PLPLOT internal variables are listed below.
*
* ipls		Stream number
* font..	Current default font and control parameters
\*----------------------------------------------------------------------*/

static PLINT ipls;			/* stream number */
static PLINT font, initfont, fontset;	/* font control parameters */
static PLINT offset;			/* offset for dispatch calls */

/*
* PLStream data structure (defined in plstream.h).
* It contains a copy of every variable that is stream dependent.
* Only the first [index=0] stream is statically allocated; the rest
* are dynamically allocated when you switch streams.  Yes, it is legal
* to only initialize the first element of the array of pointers.
*/

static PLStream pls0;			/* preallocated stream */
static PLStream *plsc = &pls0;		/* current stream pointer */

static PLStream *pls[PL_NSTREAMS] = {&pls0};	/* Array of stream pointers */

/*----------------------------------------------------------------------*\
* Define structure containing pointers to device dependent functions.
*
* pl_MenuStr	Pointer to string that is printed in device menu. 
*
* pl_DevName	A short device "name" for device selection by name. 
*
* pl_type	0 for file-oriented device, 1 for interactive
*		(the null driver uses -1 here)
*
* pl_setup	Use this routine to set orientation, x and y  resolution
*		(dots/mm)  and x and y page widths. Some device drivers
*		may  choose to ignore any or all of these. A call to
*		this routine is optional. If a particular driver requires
*		any of these parameters and they are not set by a call to
*		pl_setup() then they should be prompted for in pl_init().
*		The user may call this routine only once and it is called
*		before plstar() or plbeg(). 
*
* pl_orient	Sets orientation of output to landscape or portrait.
*
* pl_init	Initialize device.  This routine may also prompt the user
*		for certain device parameters or open a graphics file (see
*		note). Called only once to set things up. 
*
* pl_line	Draws a line between two points. 
*
* pl_polyline	Draws a polyline (no broken segments).
*
* pl_eop	Finishes out current page (see note). 
*
* pl_bop	Set up for plotting on a new page. May also open a new
*		a new graphics file (see note). 
*
* pl_tidy	Tidy up. May close graphics file (see note). 
*
* pl_color	Change pen color. 
*
* pl_text	Switch device to text mode. 
*
* pl_graph	Switch device to graphics mode. 
*
* pl_width	Set graphics pen width. 
*
* pl_esc	Escape function for driver-specific commands.
*
* NOTE: Most devices allow multi-page plots to be stored in a single 
*	graphics file, in which case the graphics file should be opened 
*	in the pl_init() routine, closed in pl_tidy(), and page advances
*	done by pl_adv(). If multi-page plots need to be stored in
*	different files then pl_bop() should open the file and pl_eop()
*	should close it. Do NOT open files in both pl_init() and pl_bop()
*	or close files in both pl_eop() and pl_tidy().
\*----------------------------------------------------------------------*/

typedef struct {
   char *pl_MenuStr;
   char *pl_DevName;
   int  pl_type;
   void (*pl_init)	(PLStream *);
   void (*pl_line)	(PLStream *, short, short, short, short);
   void (*pl_polyline)	(PLStream *, short *, short *, PLINT);
   void (*pl_eop)	(PLStream *);
   void (*pl_bop)	(PLStream *);
   void (*pl_tidy)	(PLStream *);
   void (*pl_color)	(PLStream *);
   void (*pl_text)	(PLStream *);
   void (*pl_graph)	(PLStream *);
   void (*pl_width)	(PLStream *);
   void (*pl_esc)	(PLStream *, PLINT, void *);
} PLDispatchTable;

/*----------------------------------------------------------------------*\
* Initialize dispatch table.
*
* Each device is selected by the appropriate define, passed in from the
* makefile.  When installing plplot you may wish to exclude devices not 
* present on your system in order to reduce screen clutter.
*
* If you hit a <CR> in response to plstar() prompt, you get the FIRST one
* active below, so arrange them accordingly for your system (i.e. all the
* system-specific ones should go first, since they won't appear on most
* systems.) 
\*----------------------------------------------------------------------*/

static PLDispatchTable dispatch_table[] = {

    /* Terminal types */

#ifdef NEXT
    {
        "NeXT Display",
        "next",
	1,
        plD_init_nx,
        plD_line_nx,
        plD_polyline_nx,
        plD_eop_nx,
        plD_bop_nx,
        plD_tidy_nx,
        plD_color_nx,
        plD_text_nx,
        plD_graph_nx,
        plD_width_nx,
        plD_esc_nx
    },
#endif

#ifdef AMIGA
   {
	"Amiga Window",
	"amiwn",
	1,
	plD_init_amiwn,
	plD_line_amiwn,
	plD_polyline_amiwn,
	plD_eop_amiwn,
	plD_bop_amiwn,
	plD_tidy_amiwn,
	plD_color_amiwn,
	plD_text_amiwn,
	plD_graph_amiwn,
	plD_width_amiwn,
	plD_esc_amiwn
   },
#endif

#ifdef OS2PM
    {
	"OS/2 PM Screen",
	"os2",
	1,
	plD_init_os2,
	plD_line_os2,
	plD_polyline_os2,
	plD_eop_os2,
	plD_bop_os2,
	plD_tidy_os2,
	plD_color_os2,
	plD_text_os2,
	plD_graph_os2,
	plD_width_os2,
	plD_esc_os2
    },
#endif

#ifdef XWIN
    {
	"X-Window (Xlib)",
	"xwin",
	1,
	plD_init_xw,
	plD_line_xw,
	plD_polyline_xw,
	plD_eop_xw,
	plD_bop_xw,
	plD_tidy_xw,
	plD_color_xw,
	plD_text_xw,
	plD_graph_xw,
	plD_width_xw,
	plD_esc_xw
    },
#endif

#ifdef TK
    {
	"TCL/TK Window",
	"tk",
	1,
	plD_init_tk,
	plD_line_tk,
	plD_polyline_tk,
	plD_eop_tk,
	plD_bop_tk,
	plD_tidy_tk,
	plD_color_tk,
	plD_text_tk,
	plD_graph_tk,
	plD_width_tk,
	plD_esc_tk
    },
#endif

#ifdef VGA
    {
	"DOS VGA Screen",
	"vga",
	1,
	plD_init_vga,
	plD_line_vga,
	plD_polyline_vga,
	plD_eop_vga,
	plD_bop_vga,
	plD_tidy_vga,
	plD_color_vga,
	plD_text_vga,
	plD_graph_vga,
	plD_width_vga,
	plD_esc_vga
    },
#endif

#ifdef GNUSVGA
    {
	"DOS SVGA Screen",
	"svga",
	1,
	splD_init_vga,
	splD_line_vga,
	splD_polyline_vga,
	splD_eop_vga,
	splD_bop_vga,
	splD_tidy_vga,
	splD_color_vga,
	splD_text_vga,
	splD_graph_vga,
	splD_width_vga,
	splD_esc_vga
    },
#endif

#ifdef XTERM
    {
	"Xterm Window",
	"xterm",
	1,
	plD_init_xte,
	plD_line_xte,
	plD_polyline_xte,
	plD_eop_xte,
	plD_bop_xte,
	plD_tidy_xte,
	plD_color_xte,
	plD_text_xte,
	plD_graph_xte,
	plD_width_xte,
	plD_esc_xte
    },
#endif

#ifdef TEK
    {
	"Tektronix Terminal",
	"tekt",
	1,
	plD_init_tekt,
	plD_line_tek,
	plD_polyline_tek,
	plD_eop_tek,
	plD_bop_tek,
	plD_tidy_tek,
	plD_color_tek,
	plD_text_tek,
	plD_graph_tek,
	plD_width_tek,
	plD_esc_tek
    },
#endif

#ifdef DG300
    {
	"DG300 Terminal",
	"dg300",
	1,
	plD_init_dg,
	plD_line_dg,
	plD_polyline_dg,
	plD_eop_dg,
	plD_bop_dg,
	plD_tidy_dg,
	plD_color_dg,
	plD_text_dg,
	plD_graph_dg,
	plD_width_dg,
	plD_esc_dg
    },
#endif

    /* File types */

#ifdef PLMETA
    {
	"PLPLOT Native Meta-File",
	"plmeta",
	0,
	plD_init_plm,
	plD_line_plm,
	plD_polyline_plm,
	plD_eop_plm,
	plD_bop_plm,
	plD_tidy_plm,
	plD_color_plm,
	plD_text_plm,
	plD_graph_plm,
	plD_width_plm,
	plD_esc_plm
    },
#endif

#ifdef TEK
    {
	"Tektronix File",
	"tekf",
	0,
	plD_init_tekf,
	plD_line_tek,
	plD_polyline_tek,
	plD_eop_tek,
	plD_bop_tek,
	plD_tidy_tek,
	plD_color_tek,
	plD_text_tek,
	plD_graph_tek,
	plD_width_tek,
	plD_esc_tek
    },
#endif

#ifdef PS
    {
	"PostScript File",
	"ps",
	0,
	plD_init_ps,
	plD_line_ps,
	plD_polyline_ps,
	plD_eop_ps,
	plD_bop_ps,
	plD_tidy_ps,
	plD_color_ps,
	plD_text_ps,
	plD_graph_ps,
	plD_width_ps,
	plD_esc_ps
    },
#endif

#ifdef XFIG
    {
	"Xfig file",
	"xfig",
	0,
	plD_init_xfig,
	plD_line_xfig,
	plD_polyline_xfig,
	plD_eop_xfig,
	plD_bop_xfig,
	plD_tidy_xfig,
	plD_color_xfig,
	plD_text_xfig,
	plD_graph_xfig,
	plD_width_xfig,
	plD_esc_xfig
    },
#endif

#ifdef LJII
    {
	"LaserJet II Bitmap File (150 dpi)",
	"ljii",
	0,
	plD_init_jet,
	plD_line_jet,
	plD_polyline_jet,
	plD_eop_jet,
	plD_bop_jet,
	plD_tidy_jet,
	plD_color_jet,
	plD_text_jet,
	plD_graph_jet,
	plD_width_jet,
	plD_esc_jet
    },
#endif

#ifdef AMIGA
    {
	"Amiga Printer (prefs settings)",
	"amipr",
	0,
	plD_init_amipr,
	plD_line_amipr,
	plD_polyline_amipr,
	plD_eop_amipr,
	plD_bop_amipr,
	plD_tidy_amipr,
	plD_color_amipr,
	plD_text_amipr,
	plD_graph_amipr,
	plD_width_amipr,
	plD_esc_amipr
    },
#endif

#ifdef IFF	/* probably only of interest on the Amiga */
   {
	"IFF Graphics File",
	"iff",
	0,
	plD_init_iff,
	plD_line_iff,
	plD_polyline_iff,
	plD_eop_iff,
	plD_bop_iff,
	plD_tidy_iff,
	plD_color_iff,
	plD_text_iff,
	plD_graph_iff,
	plD_width_iff,
	plD_esc_iff
   },
#endif

#ifdef AEGIS	/* probably only of interest on the Amiga */
   {
	"Aegis Draw File",
	"aegis",
	0,
	plD_init_aegis,
	plD_line_aegis,
	plD_polyline_aegis,
	plD_eop_aegis,
	plD_bop_aegis,
	plD_tidy_aegis,
	plD_color_aegis,
	plD_text_aegis,
	plD_graph_aegis,
	plD_width_aegis,
	plD_esc_aegis
   },
#endif

#ifdef HP7470
    {
	"HP 7470 Plotter File (HPGL Cartridge, Small Plotter)",
	"hp7470",
	0,
	plD_init_hp7470,
	plD_line_hp7470,
	plD_polyline_hp7470,
	plD_eop_hp7470,
	plD_bop_hp7470,
	plD_tidy_hp7470,
	plD_color_hp7470,
	plD_text_hp7470,
	plD_graph_hp7470,
	plD_width_hp7470,
	plD_esc_hp7470
    },
#endif

#ifdef HP7580
    {
	"HP 7580 Plotter File (Large Plotter)",
	"hp7580",
	0,
	plD_init_hp7580,
	plD_line_hp7580,
	plD_polyline_hp7580,
	plD_eop_hp7580,
	plD_bop_hp7580,
	plD_tidy_hp7580,
	plD_color_hp7580,
	plD_text_hp7580,
	plD_graph_hp7580,
	plD_width_hp7580,
	plD_esc_hp7580
    },
#endif

#ifdef IMP
    {
	"Impress File",
	"imp",
	0,
	plD_init_imp,
	plD_line_imp,
	plD_polyline_imp,
	plD_eop_imp,
	plD_bop_imp,
	plD_tidy_imp,
	plD_color_imp,
	plD_text_imp,
	plD_graph_imp,
	plD_width_imp,
	plD_esc_imp
    },
#endif

#ifdef NULLDEV
    {
	"Null device",
	"null",
	-1,
	plD_init_null,
	plD_line_null,
	plD_polyline_null,
	plD_eop_null,
	plD_bop_null,
	plD_tidy_null,
	plD_color_null,
	plD_text_null,
	plD_graph_null,
	plD_width_null,
	plD_esc_null
    }
#endif
};

static PLINT npldrivers = (sizeof(dispatch_table)/sizeof(PLDispatchTable));
