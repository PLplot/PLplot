/* $Id$

	Contains declarations for core plplot data structures.  This file
	should be included only by plcore.c.
*/

#ifndef __PLCORE_H__
#define __PLCORE_H__

#include "plplot/plplotP.h"
#include "plplot/drivers.h"
#include "plplot/plDevs.h"

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
static void	calc_dimap	(void);
static void	plgdevlst	(char **, char **, int *, int);

/* Static variables */

static PLINT xscl[PL_MAXPOLY], yscl[PL_MAXPOLY];

static PLINT initfont = 1;	/* initial font: extended by default */
static PLINT offset;		/* offset for dispatch calls */

/*--------------------------------------------------------------------------*\
 * Allocate a PLStream data structure (defined in plstrm.h).
 *
 * This struct contains a copy of every variable that is stream dependent.
 * Only the first [index=0] stream is statically allocated; the rest
 * are dynamically allocated when you switch streams (yes, it is legal
 * to only initialize the first element of the array of pointers).
\*--------------------------------------------------------------------------*/

static PLStream pls0;			/* preallocated stream */
static PLINT ipls;			/* current stream number */

static PLStream *pls[PL_NSTREAMS] = {&pls0};	/* Array of stream pointers */

/* Current stream pointer.  Global, for easier access to state info */

PLStream *plsc = &pls0;

/* Only now can we include this */

#include "plplot/pldebug.h"

/*--------------------------------------------------------------------------*\
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
\*--------------------------------------------------------------------------*/

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

/*--------------------------------------------------------------------------*\
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
\*--------------------------------------------------------------------------*/

static PLDispatchTable dispatch_table[] = {

    /* Terminal types */

#ifdef PLD_mac
    {
        "Macintosh Display; 8 windows",
        "mac8",
        1,
        plD_init_mac8,
        plD_line_mac,
        plD_polyline_mac,
        plD_eop_mac,
        plD_bop_mac,
        plD_tidy_mac,
        plD_state_mac,
        plD_esc_mac
    },
    {
        "Macintosh Display; 1 window, no pausing",
        "mac1",
        1,
        plD_init_mac1,
        plD_line_mac,
        plD_polyline_mac,
        plD_eop_mac,
        plD_bop_mac,
        plD_tidy_mac,
        plD_state_mac,
        plD_esc_mac
    },
#endif

#ifdef PLD_next
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

#ifdef PLD_os2pm
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

#ifdef PLD_xwin
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

#ifdef PLD_gnome  
    {
	"Gnome Canvas",
	"gnome",
	1,
	plD_init_gnome,
	plD_line_gnome,
	plD_polyline_gnome,
	plD_eop_gnome,
	plD_bop_gnome,
	plD_tidy_gnome,
	plD_state_gnome,
	plD_esc_gnome
    },
#endif
  
#ifdef PLD_tk
    {
	"Tcl/TK Window",
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

#ifdef PLD_linuxvga
    {
	"Linux console VGA Screen",
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

#ifdef PLD_win3
    {
	"Windows 3.x Driver",
	"win3",
	1,
	plD_init_win3,
	plD_line_win3,
	plD_polyline_win3,
	plD_eop_win3,
	plD_bop_win3,
	plD_tidy_win3,
	plD_state_win3,
	plD_esc_win3
    },
#endif

#if defined (_MSC_VER) && defined (VGA)         /* graphics for msc */
    {
	"DOS VGA Screen (msc)",
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

#ifdef PLD_bgi
    {
	"VGA Screen (BGI)",
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

#ifdef PLD_gnusvga
    {
	"SVGA Screen (GRX20)",
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

#ifdef PLD_tiff
    {
	"TIFF File (TIFFLIB / GRX20)",
	"tiff",
	0,
	plD_init_tiff,
	plD_line_vga,
	plD_polyline_vga,
	plD_eop_tiff,
	plD_bop_tiff,
	plD_tidy_tiff,
	plD_state_vga,
	plD_esc_tiff
    },
#endif

#ifdef PLD_jpg
    {
	"JPEG File (Independent JPEG Group based on GRX20)",
	"jpg",
	0,
	plD_init_jpg,
	plD_line_vga,
	plD_polyline_vga,
	plD_eop_jpg,
	plD_bop_jpg,
	plD_tidy_jpg,
	plD_state_vga,
	plD_esc_jpg
    },
#endif

#ifdef PLD_bmp
    {
	"Windows Bitmap File (GRX20)",
	"bmp",
	0,
	plD_init_bmp,
	plD_line_vga,
	plD_polyline_vga,
	plD_eop_bmp,
	plD_bop_bmp,
	plD_tidy_bmp,
	plD_state_vga,
	plD_esc_bmp
    },
#endif

#ifdef PLD_emxvga		       /* graphics for emx+gcc */
    {
	"VGA Screen (emx)",
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

#ifdef PLD_xterm
    {
	"Xterm Window",
	"xterm",
	1,
	plD_init_xterm,
	plD_line_tek,
	plD_polyline_tek,
	plD_eop_tek,
	plD_bop_tek,
	plD_tidy_tek,
	plD_state_tek,
	plD_esc_tek
    },
#endif

#ifdef PLD_tek4010
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

#ifdef PLD_tek4107
    {
	"Tektronix Terminal (4105/4107)",
	"tek4107t",
	1,
	plD_init_tek4107t,
	plD_line_tek,
	plD_polyline_tek,
	plD_eop_tek,
	plD_bop_tek,
	plD_tidy_tek,
	plD_state_tek,
	plD_esc_tek
    },
#endif

#ifdef PLD_mskermit
    {
	"MS-Kermit emulator",
	"mskermit",
	1,
	plD_init_mskermit,
	plD_line_tek,
	plD_polyline_tek,
	plD_eop_tek,
	plD_bop_tek,
	plD_tidy_tek,
	plD_state_tek,
	plD_esc_tek
    },
#endif

#ifdef PLD_versaterm
    {
	"Versaterm vt100/tek emulator",
	"versaterm",
	1,
	plD_init_versaterm,
	plD_line_tek,
	plD_polyline_tek,
	plD_eop_tek,
	plD_bop_tek,
	plD_tidy_tek,
	plD_state_tek,
	plD_esc_tek
    },
#endif

#ifdef PLD_vlt
    {
	"VLT vt100/tek emulator",
	"vlt",
	1,
	plD_init_vlt,
	plD_line_tek,
	plD_polyline_tek,
	plD_eop_tek,
	plD_bop_tek,
	plD_tidy_tek,
	plD_state_tek,
	plD_esc_tek
    },
#endif

#ifdef PLD_conex
    {
	"Conex vt320/tek emulator",
	"conex",
	1,
	plD_init_conex,
	plD_line_tek,
	plD_polyline_tek,
	plD_eop_tek,
	plD_bop_tek,
	plD_tidy_tek,
	plD_state_tek,
	plD_esc_tek
    },
#endif

#ifdef PLD_dg300
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

#ifdef PLD_plmeta
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

#ifdef PLD_tek4010
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

#ifdef PLD_tek4107
    {
	"Tektronix File (4105/4107)",
	"tek4107f",
	0,
	plD_init_tek4107f,
	plD_line_tek,
	plD_polyline_tek,
	plD_eop_tek,
	plD_bop_tek,
	plD_tidy_tek,
	plD_state_tek,
	plD_esc_tek
    },
#endif

#ifdef PLD_ps
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

#ifdef PLD_xfig
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

#ifdef PLD_ljii
    {
	"LaserJet IIp/deskjet compressed graphics",
	"ljiip",
	0,
	plD_init_ljiip,
	plD_line_ljiip,
	plD_polyline_ljiip,
	plD_eop_ljiip,
	plD_bop_ljiip,
	plD_tidy_ljiip,
	plD_state_ljiip,
	plD_esc_ljiip
    },
    {
	"LaserJet II Bitmap File (150 dpi)",
	"ljii",
	0,
	plD_init_ljii,
	plD_line_ljii,
	plD_polyline_ljii,
	plD_eop_ljii,
	plD_bop_ljii,
	plD_tidy_ljii,
	plD_state_ljii,
	plD_esc_ljii
    },
#endif

#ifdef PLD_hp7470
    {
	"HP 7470 Plotter File (HPGL Cartridge, Small Plotter)",
	"hp7470",
	0,
	plD_init_hp7470,
	plD_line_hpgl,
	plD_polyline_hpgl,
	plD_eop_hpgl,
	plD_bop_hpgl,
	plD_tidy_hpgl,
	plD_state_hpgl,
	plD_esc_hpgl
    },
#endif

#ifdef PLD_hp7580
    {
	"HP 7580 Plotter File (Large Plotter)",
	"hp7580",
	0,
	plD_init_hp7580,
	plD_line_hpgl,
	plD_polyline_hpgl,
	plD_eop_hpgl,
	plD_bop_hpgl,
	plD_tidy_hpgl,
	plD_state_hpgl,
	plD_esc_hpgl
    },
#endif

#ifdef PLD_lj_hpgl
    {
	"HP Laserjet III, HPGL emulation mode",
	"lj_hpgl",
	0,
	plD_init_lj_hpgl,
	plD_line_hpgl,
	plD_polyline_hpgl,
	plD_eop_hpgl,
	plD_bop_hpgl,
	plD_tidy_hpgl,
	plD_state_hpgl,
	plD_esc_hpgl
    },
#endif

#ifdef PLD_imp
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

#ifdef PLD_pbm
    {
	"PDB (PPM) Driver",
	"pbm",
	0,
	plD_init_pbm,
	plD_line_pbm,
	plD_polyline_pbm,
	plD_eop_pbm,
	plD_bop_pbm,
	plD_tidy_pbm,
	plD_state_pbm,
	plD_esc_pbm
    },
#endif

#ifdef PLD_png
    {
        "PNG Driver",
	"png",
	0,
	plD_init_png,
	plD_line_png,
	plD_polyline_png,
	plD_eop_png,
	plD_bop_png,
	plD_tidy_png,
	plD_state_png,
	plD_esc_png
    },
#endif

#ifdef PLD_jpeg
    {
        "JPEG File (Independent JPEG Group based on libgd)",
	"jpeg",
	0,
	plD_init_png,
	plD_line_png,
	plD_polyline_png,
	plD_eop_jpeg,
	plD_bop_png,
	plD_tidy_png,
	plD_state_png,
	plD_esc_png
    },
#endif

#ifdef PLD_pstex
    {
	"Postscript/LaTeX device",
	"pstex",
	-1,
	plD_init_pstex,
	plD_line_ps,
	plD_polyline_ps,
	plD_eop_ps,
	plD_bop_pstex,
	plD_tidy_pstex,
	plD_state_ps,
	plD_esc_pstex
    },
#endif

#ifdef PLD_null
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

static int npldrivers = (sizeof(dispatch_table)/sizeof(PLDispatchTable));

#endif	/* __PLCORE_H__ */
