/* $Id$
 * $Log$
 * Revision 1.12  1993/08/18 19:07:03  mjl
 * Fixed function prototype.
 *
 * Revision 1.11  1993/08/03  03:26:52  mjl
 * Additions to support new MGR and LinuxVGA drivers.
 *
 * Revision 1.10  1993/07/31  07:56:51  mjl
 * Several driver functions consolidated, for all drivers.  The width and color
 * commands are now part of a more general "state" command.  The text and
 * graph commands used for switching between modes is now handled by the
 * escape function (very few drivers require it).  The device-specific PLDev
 * structure is now malloc'ed for each driver that requires it, and freed when
 * the stream is terminated.
 *
 * Revision 1.9  1993/07/28  05:49:10  mjl
 * Modified table entries for xterm driver, added new ones for tek4107
 * terminal & file driver.
 *
 * Revision 1.8  1993/07/16  22:26:14  mjl
 * Added explicit support for color vs monochrome postscript output,
 * arrays and constant definitions used in driver interface.
 *
 * Revision 1.7  1993/07/02  07:22:14  mjl
 * Namespace changes.
 *
 * Revision 1.6  1993/04/26  20:00:10  mjl
 * The beginnings of a TK driver added.
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

/* Static function prototypes */

static void	grline		(short *, short *, PLINT);
static void	grpolyline	(short *, short *, PLINT);
static void	grfill		(short *, short *, PLINT);
static void	difilt		(PLINT *, PLINT *, PLINT,
				 PLINT *, PLINT *, PLINT *, PLINT *);
static void	plGetDev	(void);
static void	pldi_ini	(void);
static void	calc_diplt	(void);
static void	calc_didev	(void);
static void	calc_diori	(void);

/* Static variables */

static PLINT xscl[PL_MAXPOLYLINE], yscl[PL_MAXPOLYLINE];
static PLINT xscl1[PL_MAXPOLYLINE], yscl1[PL_MAXPOLYLINE];

static PLINT font, initfont, fontset;	/* font control parameters */
static PLINT offset;			/* offset for dispatch calls */

/*----------------------------------------------------------------------*\
* Allocate a PLStream data structure (defined in plstream.h).
*
* This struct contains a copy of every variable that is stream dependent.
* Only the first [index=0] stream is statically allocated; the rest
* are dynamically allocated when you switch streams (yes, it is legal
* to only initialize the first element of the array of pointers).
\*----------------------------------------------------------------------*/

static PLStream pls0;			/* preallocated stream */
static PLStream *plsc = &pls0;		/* current stream pointer */
static PLINT ipls;			/* current stream number */

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
* pl_init	Initialize device.  This routine may also prompt the user
*		for certain device parameters or open a graphics file
*		(see note).  Called only once to set things up.  Certain
*		options such as familying and resolution (dots/mm) should
*		be set up before calling this routine (note: some drivers
*		ignore these).
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
* pl_state	Handle change in PLStream state
*		(color, pen width, fill attribute, etc).
*
* pl_esc	Escape function for driver-specific commands.
*
*
* Notes:
*
* Most devices allow multi-page plots to be stored in a single graphics
* file, in which case the graphics file should be opened in the pl_init()
* routine, closed in pl_tidy(), and page advances done by calling pl_eop
* and pl_bop() in sequence. If multi-page plots need to be stored in
* different files then pl_bop() should open the file and pl_eop() should
* close it.  Do NOT open files in both pl_init() and pl_bop() or close
* files in both pl_eop() and pl_tidy().
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
   void (*pl_state)	(PLStream *, PLINT);
   void (*pl_esc)	(PLStream *, PLINT, void *);
} PLDispatchTable;

/*----------------------------------------------------------------------*\
* Initialize dispatch table.
*
* Each device is selected by the appropriate define, passed in from the
* makefile.  When installing plplot you may wish to exclude devices not 
* present on your system in order to reduce screen clutter.
*
* If you hit a <CR> in response to the plinit() prompt, you get the FIRST
* one active below, so arrange them accordingly for your system (i.e. all
* the system-specific ones should go first, since they won't appear on
* most systems.)
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
        plD_state_nx,
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
	plD_state_amiwn,
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
	plD_state_os2,
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
	plD_state_xw,
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
	plD_state_tk,
	plD_esc_tk
    },
#endif

#ifdef LINUXVGA
    {
	"Linux console VGA Screen",
	"lxvga",
	1,
	plD_init_lxvga,
	plD_line_lxvga,
	plD_polyline_lxvga,
	plD_eop_lxvga,
	plD_bop_lxvga,
	plD_tidy_lxvga,
	plD_state_lxvga,
	plD_esc_lxvga
    },
#endif

#ifdef MGR
    {
	"MGR Window",
	"mgr",
	1,
	plD_init_mgr,
	plD_line_mgr,
	plD_polyline_mgr,
	plD_eop_mgr,
	plD_bop_mgr,
	plD_tidy_mgr,
	plD_state_mgr,
	plD_esc_mgr
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
	plD_state_vga,
	plD_esc_vga
    },
#endif

#ifdef GNUSVGA
    {
	"DOS SVGA Screen",
	"svga",
	1,
	plD_init_svga,
	plD_line_svga,
	plD_polyline_svga,
	plD_eop_svga,
	plD_bop_svga,
	plD_tidy_svga,
	plD_state_svga,
	plD_esc_svga
    },
#endif

#ifdef XTERM
    {
	"Xterm Window",
	"xterm",
	1,
	plD_init_xte,
	plD_line_tek,
	plD_polyline_tek,
	plD_eop_tek,
	plD_bop_tek,
	plD_tidy_tek,
	plD_state_tek,
	plD_esc_tek
    },
#endif

#ifdef TEK4010
    {
	"Tektronix Terminal (4010)",
	"tekt",
	1,
	plD_init_tekt,
	plD_line_tek,
	plD_polyline_tek,
	plD_eop_tek,
	plD_bop_tek,
	plD_tidy_tek,
	plD_state_tek,
	plD_esc_tek
    },
#endif

#ifdef TEK4107
    {
	"Tektronix Terminal (4105/4107)",
	"t4107t",
	1,
	plD_init_t4107t,
	plD_line_tek,
	plD_polyline_tek,
	plD_eop_tek,
	plD_bop_tek,
	plD_tidy_tek,
	plD_state_tek,
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
	plD_state_dg,
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
	plD_state_plm,
	plD_esc_plm
    },
#endif

#ifdef TEK4010
    {
	"Tektronix File (4010)",
	"tekf",
	0,
	plD_init_tekf,
	plD_line_tek,
	plD_polyline_tek,
	plD_eop_tek,
	plD_bop_tek,
	plD_tidy_tek,
	plD_state_tek,
	plD_esc_tek
    },
#endif

#ifdef TEK4107
    {
	"Tektronix File (4105/4107)",
	"t4107f",
	0,
	plD_init_t4107f,
	plD_line_tek,
	plD_polyline_tek,
	plD_eop_tek,
	plD_bop_tek,
	plD_tidy_tek,
	plD_state_tek,
	plD_esc_tek
    },
#endif

#ifdef PS
    {
	"PostScript File (monochrome)",
	"ps",
	0,
	plD_init_psm,
	plD_line_ps,
	plD_polyline_ps,
	plD_eop_ps,
	plD_bop_ps,
	plD_tidy_ps,
	plD_state_ps,
	plD_esc_ps
    },
    {
	"PostScript File (color)",
	"psc",
	0,
	plD_init_psc,
	plD_line_ps,
	plD_polyline_ps,
	plD_eop_ps,
	plD_bop_ps,
	plD_tidy_ps,
	plD_state_ps,
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
	plD_state_xfig,
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
	plD_state_jet,
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
	plD_state_amipr,
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
	plD_state_iff,
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
	plD_state_aegis,
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
	plD_state_hp7470,
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
	plD_state_hp7580,
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
	plD_state_imp,
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
	plD_state_null,
	plD_esc_null
    }
#endif
};

static PLINT npldrivers = (sizeof(dispatch_table)/sizeof(PLDispatchTable));
