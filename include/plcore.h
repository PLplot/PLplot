/* $Id$
   $Log$
   Revision 1.5  1993/03/15 21:45:18  mjl
   Changed _clear/_page driver functions to the names _eop/_bop, to be
   more representative of what's actually going on.

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

#include "plplot.h"
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
*/

static PLStream pls[PL_NSTREAMS];

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
   void (*pl_esc)	(PLStream *, PLINT, char *);
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
        nx_init,
        nx_line,
        nx_polyline,
        nx_eop,
        nx_bop,
        nx_tidy,
        nx_color,
        nx_text,
        nx_graph,
        nx_width,
        nx_esc
    },
#endif

#ifdef AMIGA
   {
	"Amiga Window",
	"amiwn",
	1,
	amiwn_init,
	amiwn_line,
	amiwn_polyline,
	amiwn_eop,
	amiwn_bop,
	amiwn_tidy,
	amiwn_color,
	amiwn_text,
	amiwn_graph,
	amiwn_width,
	amiwn_esc
   },
#endif

#ifdef OS2PM
    {
	"OS/2 PM Screen",
	"os2",
	1,
	os2_init,
	os2_line,
	os2_polyline,
	os2_eop,
	os2_bop,
	os2_tidy,
	os2_color,
	os2_text,
	os2_graph,
	os2_width,
	os2_esc
    },
#endif

#ifdef MOTIF
    {
	"X-Window Screen (Motif)",
	"xm",
	1,
	xm_init,
	xm_line,
	xm_polyline,
	xm_eop,
	xm_bop,
	xm_tidy,
	xm_color,
	xm_text,
	xm_graph,
	xm_width,
	xm_esc
    },
#endif

#ifdef XWIN
    {
	"X-Window Screen (Xlib)",
	"xwin",
	1,
	xw_init,
	xw_line,
	xw_polyline,
	xw_eop,
	xw_bop,
	xw_tidy,
	xw_color,
	xw_text,
	xw_graph,
	xw_width,
	xw_esc
    },
#endif

#ifdef VGA
    {
	"DOS VGA Screen",
	"vga",
	1,
	vga_init,
	vga_line,
	vga_polyline,
	vga_eop,
	vga_bop,
	vga_tidy,
	vga_color,
	vga_text,
	vga_graph,
	vga_width,
	vga_esc
    },
#endif

#ifdef GNUSVGA
    {
	"DOS SVGA Screen",
	"svga",
	1,
	svga_init,
	svga_line,
	svga_polyline,
	svga_eop,
	svga_bop,
	svga_tidy,
	svga_color,
	svga_text,
	svga_graph,
	svga_width,
	svga_esc
    },
#endif

#ifdef XTERM
    {
	"Xterm Window",
	"xterm",
	1,
	xte_init,
	xte_line,
	xte_polyline,
	xte_eop,
	xte_bop,
	xte_tidy,
	xte_color,
	xte_text,
	xte_graph,
	xte_width,
	xte_esc
    },
#endif

#ifdef TEK
    {
	"Tektronix Terminal",
	"tekt",
	1,
	tekt_init,
	tek_line,
	tek_polyline,
	tek_eop,
	tek_bop,
	tek_tidy,
	tek_color,
	tek_text,
	tek_graph,
	tek_width,
	tek_esc
    },
#endif

#ifdef DG300
    {
	"DG300 Terminal",
	"dg300",
	1,
	dg_init,
	dg_line,
	dg_polyline,
	dg_eop,
	dg_bop,
	dg_tidy,
	dg_color,
	dg_text,
	dg_graph,
	dg_width,
	dg_esc
    },
#endif

    /* File types */

#ifdef PLMETA
    {
	"PLPLOT Native Meta-File",
	"plmeta",
	0,
	plm_init,
	plm_line,
	plm_polyline,
	plm_eop,
	plm_bop,
	plm_tidy,
	plm_color,
	plm_text,
	plm_graph,
	plm_width,
	plm_esc
    },
#endif

#ifdef TEK
    {
	"Tektronix File",
	"tekf",
	0,
	tekf_init,
	tek_line,
	tek_polyline,
	tek_eop,
	tek_bop,
	tek_tidy,
	tek_color,
	tek_text,
	tek_graph,
	tek_width,
	tek_esc
    },
#endif

#ifdef PS
    {
	"PostScript File",
	"ps",
	0,
	ps_init,
	ps_line,
	ps_polyline,
	ps_eop,
	ps_bop,
	ps_tidy,
	ps_color,
	ps_text,
	ps_graph,
	ps_width,
	ps_esc
    },
#endif

#ifdef XFIG
    {
	"Xfig file",
	"xfig",
	0,
	xfig_init,
	xfig_line,
	xfig_polyline,
	xfig_eop,
	xfig_bop,
	xfig_tidy,
	xfig_color,
	xfig_text,
	xfig_graph,
	xfig_width,
	xfig_esc
    },
#endif

#ifdef LJII
    {
	"LaserJet II Bitmap File (150 dpi)",
	"ljii",
	0,
	jet_init,
	jet_line,
	jet_polyline,
	jet_eop,
	jet_bop,
	jet_tidy,
	jet_color,
	jet_text,
	jet_graph,
	jet_width,
	jet_esc
    },
#endif

#ifdef AMIGA
    {
	"Amiga Printer (prefs settings)",
	"amipr",
	0,
	amipr_init,
	amipr_line,
	amipr_polyline,
	amipr_eop,
	amipr_bop,
	amipr_tidy,
	amipr_color,
	amipr_text,
	amipr_graph,
	amipr_width,
	amipr_esc
    },
#endif

#ifdef IFF	/* probably only of interest on the Amiga */
   {
	"IFF Graphics File",
	"iff",
	0,
	iff_init,
	iff_line,
	iff_polyline,
	iff_eop,
	iff_bop,
	iff_tidy,
	iff_color,
	iff_text,
	iff_graph,
	iff_width,
	iff_esc
   },
#endif

#ifdef AEGIS	/* probably only of interest on the Amiga */
   {
	"Aegis Draw File",
	"aegis",
	0,
	aegis_init,
	aegis_line,
	aegis_polyline,
	aegis_eop,
	aegis_bop,
	aegis_tidy,
	aegis_color,
	aegis_text,
	aegis_graph,
	aegis_width,
	aegis_esc
   },
#endif

#ifdef HP7470
    {
	"HP 7470 Plotter File (HPGL Cartridge, Small Plotter)",
	"hp7470",
	0,
	hp7470_init,
	hp7470_line,
	hp7470_polyline,
	hp7470_eop,
	hp7470_bop,
	hp7470_tidy,
	hp7470_color,
	hp7470_text,
	hp7470_graph,
	hp7470_width,
	hp7470_esc
    },
#endif

#ifdef HP7580
    {
	"HP 7580 Plotter File (Large Plotter)",
	"hp7580",
	0,
	hp7580_init,
	hp7580_line,
	hp7580_polyline,
	hp7580_eop,
	hp7580_bop,
	hp7580_tidy,
	hp7580_color,
	hp7580_text,
	hp7580_graph,
	hp7580_width,
	hp7580_esc
    },
#endif

#ifdef IMP
    {
	"Impress File",
	"imp",
	0,
	imp_init,
	imp_line,
	imp_polyline,
	imp_eop,
	imp_bop,
	imp_tidy,
	imp_color,
	imp_text,
	imp_graph,
	imp_width,
	imp_esc
    },
#endif

#ifdef NULLDEV
    {
	"Null device",
	"null",
	-1,
	null_init,
	null_line,
	null_polyline,
	null_eop,
	null_bop,
	null_tidy,
	null_color,
	null_text,
	null_graph,
	null_width,
	null_esc
    }
#endif
};

static PLINT npldrivers = (sizeof(dispatch_table)/sizeof(PLDispatchTable));
