/* $Id$
   $Log$
   Revision 1.2  1993/02/23 04:53:14  mjl
   Eliminated xxx_adv (gradv) driver function prototypes.

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
* pl_clear	Clears screen or ejects page or closes file (see note). 
*
* pl_page	Set up for plotting on a new page. May also open a new
*		a new graphics file (see note). 
*
* pl_adv	Advances to new page.  Same as a pl_clear followed by a
*		pl_page on most devices.
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
*	different files then pl_page() should open the file and pl_clear()
*	should close it. Do NOT open files in both pl_init() and pl_page()
*	or close files in both pl_clear() and pl_tidy().
\*----------------------------------------------------------------------*/

typedef struct {
   char *pl_MenuStr;
   char *pl_DevName;
   void (*pl_init)	(PLStream *);
   void (*pl_line)	(PLStream *, PLSHORT, PLSHORT, PLSHORT, PLSHORT);
   void (*pl_polyline)	(PLStream *, PLSHORT *, PLSHORT *, PLINT);
   void (*pl_clear)	(PLStream *);
   void (*pl_page)	(PLStream *);
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
        nx_init,
        nx_line,
        nx_polyline,
        nx_clear,
        nx_page,
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
	amiwn_init,
	amiwn_line,
	amiwn_polyline,
	amiwn_clear,
	amiwn_page,
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
	os2_init,
	os2_line,
	os2_polyline,
	os2_clear,
	os2_page,
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
	xm_init,
	xm_line,
	xm_polyline,
	xm_clear,
	xm_page,
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
	xw_init,
	xw_line,
	xw_polyline,
	xw_clear,
	xw_page,
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
	vga_init,
	vga_line,
	vga_polyline,
	vga_clear,
	vga_page,
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
	svga_init,
	svga_line,
	svga_polyline,
	svga_clear,
	svga_page,
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
	xte_init,
	xte_line,
	xte_polyline,
	xte_clear,
	xte_page,
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
	tekt_init,
	tek_line,
	tek_polyline,
	tek_clear,
	tek_page,
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
	dg_init,
	dg_line,
	dg_polyline,
	dg_clear,
	dg_page,
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
	plm_init,
	plm_line,
	plm_polyline,
	plm_clear,
	plm_page,
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
	tekf_init,
	tek_line,
	tek_polyline,
	tek_clear,
	tek_page,
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
	ps_init,
	ps_line,
	ps_polyline,
	ps_clear,
	ps_page,
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
	xfig_init,
	xfig_line,
	xfig_polyline,
	xfig_clear,
	xfig_page,
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
	jet_init,
	jet_line,
	jet_polyline,
	jet_clear,
	jet_page,
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
	amipr_init,
	amipr_line,
	amipr_polyline,
	amipr_clear,
	amipr_page,
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
	iff_init,
	iff_line,
	iff_polyline,
	iff_clear,
	iff_page,
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
	aegis_init,
	aegis_line,
	aegis_polyline,
	aegis_clear,
	aegis_page,
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
	hp7470_init,
	hp7470_line,
	hp7470_polyline,
	hp7470_clear,
	hp7470_page,
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
	hp7580_init,
	hp7580_line,
	hp7580_polyline,
	hp7580_clear,
	hp7580_page,
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
	imp_init,
	imp_line,
	imp_polyline,
	imp_clear,
	imp_page,
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
	null_init,
	null_line,
	null_polyline,
	null_clear,
	null_page,
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
