/* $Id$

	Contains declarations for core plplot data structures.  This file
	should be included only by plcore.c.
*/

#ifndef __PLCORE_H__
#define __PLCORE_H__

#include "plplot/plplotP.h"
#include "plplot/drivers.h"
#include "plplot/plDevs.h"
#include "plplot/disptab.h"


/* Static function prototypes */

static void	grline		(short *, short *, PLINT);
static void	grpolyline	(short *, short *, PLINT);
static void	grfill		(short *, short *, PLINT);
static void	plGetDev	(void);
static void	plSelectDev	(void);
static void	pldi_ini	(void);
static void	calc_diplt	(void);
static void	calc_didev	(void);
static void	calc_diori	(void);
static void	calc_dimap	(void);
static void	plgdevlst	(char **, char **, int *, int);

static void	plInitDispatchTable	(void);
static PLINT dispatch_table_inited = 0;

static void	plLoadDriver	(void);

/* Static variables */

static PLINT xscl[PL_MAXPOLY], yscl[PL_MAXPOLY];

static PLINT initfont = 1;	/* initial font: extended by default */

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

static PLDispatchTable **dispatch_table = 0;
static int npldrivers = 0;

static PLDispatchInit static_device_initializers[] = {
#ifdef PLD_mac
    plD_dispatch_init_mac8,
    plD_dispatch_init_mac1,
#endif
#ifdef PLD_next
    plD_dispatch_init_nx,
#endif
#ifdef PLD_os2pm
    plD_dispatch_init_os2,
#endif
#if defined(PLD_xwin) /* && !defined(ENABLE_DYNAMIC_DRIVERS)*/
    plD_dispatch_init_xw,
#endif
#if defined(PLD_gnome) && !defined(ENABLE_DYNAMIC_DRIVERS)
    plD_dispatch_init_gnome,
#endif
#if defined(PLD_tk) /* && !defined(ENABLE_DYNAMIC_DRIVERS)*/
    plD_dispatch_init_tk,
#endif
#ifdef PLD_linuxvga
    plD_dispatch_init_vga,
#endif
#ifdef PLD_mgr
    plD_dispatch_init_mgr,
#endif
#ifdef PLD_win3
    plD_dispatch_init_win3,
#endif
#if defined (_MSC_VER) && defined (VGA)         /* graphics for msc */
    plD_dispatch_init_vga,
#endif
#ifdef PLD_bgi
    plD_dispatch_init_vga,
#endif
#ifdef PLD_gnusvga
    plD_dispatch_init_vga,
#endif
#ifdef PLD_tiff
    plD_dispatch_init_tiff,
#endif
#if defined(PLD_jpg)
    plD_dispatch_init_jpg,
#endif
#if defined(PLD_bmp)  && !defined(ENABLE_DYNAMIC_DRIVERS)
    plD_dispatch_init_bmp,
#endif
#ifdef PLD_emxvga		       /* graphics for emx+gcc */
    plD_dispatch_init_vga,
#endif
#if defined(PLD_xterm) && !defined(ENABLE_DYNAMIC_DRIVERS) 
    plD_dispatch_init_xterm,
#endif
#if defined(PLD_tek4010) && !defined(ENABLE_DYNAMIC_DRIVERS) 
    plD_dispatch_init_tekt,
#endif
#if defined(PLD_tek4107) && !defined(ENABLE_DYNAMIC_DRIVERS) 
    plD_dispatch_init_tek4107t,
#endif
#if defined(PLD_mskermit) && !defined(ENABLE_DYNAMIC_DRIVERS) 
    plD_dispatch_init_mskermit,
#endif
#if defined(PLD_versaterm) && !defined(ENABLE_DYNAMIC_DRIVERS) 
    plD_dispatch_init_versaterm,
#endif
#if defined(PLD_vlt) && !defined(ENABLE_DYNAMIC_DRIVERS) 
    plD_dispatch_init_vlt,
#endif
#if defined(PLD_conex) && !defined(ENABLE_DYNAMIC_DRIVERS) 
    plD_dispatch_init_conex,
#endif
#if defined(PLD_dg300) && !defined(ENABLE_DYNAMIC_DRIVERS)
    plD_dispatch_init_dg,
#endif
#if defined(PLD_plmeta) && !defined(ENABLE_DYNAMIC_DRIVERS)
    plD_dispatch_init_plm,
#endif
#if defined(PLD_tek4010) && !defined(ENABLE_DYNAMIC_DRIVERS) 
    plD_dispatch_init_tekf,
#endif
#if defined(PLD_tek4107) && !defined(ENABLE_DYNAMIC_DRIVERS) 
    plD_dispatch_init_tek4107f,
#endif
#if defined(PLD_ps) && !defined(ENABLE_DYNAMIC_DRIVERS)
    plD_dispatch_init_psm,
    plD_dispatch_init_psc,
#endif
#if defined(PLD_xfig) && !defined(ENABLE_DYNAMIC_DRIVERS)
    plD_dispatch_init_xfig,
#endif
#if defined(PLD_ljiip) && !defined(ENABLE_DYNAMIC_DRIVERS)
    plD_dispatch_init_ljiip,
#endif
#if defined(PLD_ljii) && !defined(ENABLE_DYNAMIC_DRIVERS)
    plD_dispatch_init_ljii,
#endif
#if defined( PLD_hp7470) && !defined(ENABLE_DYNAMIC_DRIVERS)
    plD_dispatch_init_hp7470,
#endif
#if defined( PLD_hp7580) && !defined(ENABLE_DYNAMIC_DRIVERS)
    plD_dispatch_init_hp7580,
#endif
#if defined( PLD_lj_hpgl) && !defined(ENABLE_DYNAMIC_DRIVERS)
    plD_dispatch_init_hpgl,
#endif
#if defined( PLD_imp) && !defined(ENABLE_DYNAMIC_DRIVERS)
    plD_dispatch_init_imp,
#endif
#if defined( PLD_pbm) && !defined(ENABLE_DYNAMIC_DRIVERS)
    plD_dispatch_init_pbm,
#endif
#if defined(PLD_png) && !defined(ENABLE_DYNAMIC_DRIVERS)
    plD_dispatch_init_png,
#endif
#if defined(PLD_jpeg) && !defined(ENABLE_DYNAMIC_DRIVERS)
    plD_dispatch_init_jpeg,
#endif
#if defined(PLD_pstex) && !defined(ENABLE_DYNAMIC_DRIVERS)
    plD_dispatch_init_pstex,
#endif
#if defined(PLD_ntk) && !defined(ENABLE_DYNAMIC_DRIVERS)
    plD_dispatch_init_ntk,
#endif
#ifdef PLD_cgm
    plD_dispatch_init_cgm,
#endif
#if defined(PLD_null) && !defined(ENABLE_DYNAMIC_DRIVERS)
    plD_dispatch_init_null,
#endif
};

#if 0
static PLDispatchTable static_devices[] = {

    /* Terminal types */

#ifdef PLD_mac
    {
        "Macintosh Display; 8 windows",
        "mac8",
        plDevType_Interactive,
        1,
        (plD_init_fp) plD_init_mac8,
        (plD_line_fp) plD_line_mac,
        (plD_polyline_fp) plD_polyline_mac,
        (plD_eop_fp) plD_eop_mac,
        (plD_bop_fp) plD_bop_mac,
        (plD_tidy_fp) plD_tidy_mac,
        (plD_state_fp) plD_state_mac,
        (plD_esc_fp) plD_esc_mac
    },
    {
        "Macintosh Display; 1 window, no pausing",
        "mac1",
        plDevType_Interactive,
        2,
        (plD_init_fp) plD_init_mac1,
        (plD_line_fp) plD_line_mac,
        (plD_polyline_fp) plD_polyline_mac,
        (plD_eop_fp) plD_eop_mac,
        (plD_bop_fp) plD_bop_mac,
        (plD_tidy_fp) plD_tidy_mac,
        (plD_state_fp) plD_state_mac,
        (plD_esc_fp) plD_esc_mac
    },
#endif

#ifdef PLD_next
    {
        "NeXT Display",
        "next",
	plDevType_Interactive,
        3,
        (plD_init_fp) plD_init_nx,
        (plD_line_fp) plD_line_nx,
        (plD_polyline_fp) plD_polyline_nx,
        (plD_eop_fp) plD_eop_nx,
        (plD_bop_fp) plD_bop_nx,
        (plD_tidy_fp) plD_tidy_nx,
        (plD_state_fp) plD_state_nx,
        (plD_esc_fp) plD_esc_nx
    },
#endif

#ifdef PLD_os2pm
    {
	"OS/2 PM Screen",
	"os2",
	plDevType_Interactive,
        4,
	(plD_init_fp) plD_init_os2,
	(plD_line_fp) plD_line_os2,
	(plD_polyline_fp) plD_polyline_os2,
	(plD_eop_fp) plD_eop_os2,
	(plD_bop_fp) plD_bop_os2,
	(plD_tidy_fp) plD_tidy_os2,
	(plD_state_fp) plD_state_os2,
	(plD_esc_fp) plD_esc_os2
    },
#endif

#ifdef PLD_xwin
    {
	"X-Window (Xlib)",
	"xwin",
        plDevType_Interactive,
        5,
	(plD_init_fp) plD_init_xw,
	(plD_line_fp) plD_line_xw,
	(plD_polyline_fp) plD_polyline_xw,
	(plD_eop_fp) plD_eop_xw,
	(plD_bop_fp) plD_bop_xw,
	(plD_tidy_fp) plD_tidy_xw,
	(plD_state_fp) plD_state_xw,
	(plD_esc_fp) plD_esc_xw
    },
#endif

#ifdef PLD_gnome  
    {
	"Gnome Canvas",
	"gnome",
	plDevType_Interactive,
        6,
	(plD_init_fp) plD_init_gnome,
	(plD_line_fp) plD_line_gnome,
	(plD_polyline_fp) plD_polyline_gnome,
	(plD_eop_fp) plD_eop_gnome,
	(plD_bop_fp) plD_bop_gnome,
	(plD_tidy_fp) plD_tidy_gnome,
	(plD_state_fp) plD_state_gnome,
	(plD_esc_fp) plD_esc_gnome
    },
#endif
  
#ifdef PLD_tk
    {
	"Tcl/TK Window",
	"tk",
	plDevType_Interactive,
        7,
	(plD_init_fp) plD_init_tk,
	(plD_line_fp) plD_line_tk,
	(plD_polyline_fp) plD_polyline_tk,
	(plD_eop_fp) plD_eop_tk,
	(plD_bop_fp) plD_bop_tk,
	(plD_tidy_fp) plD_tidy_tk,
	(plD_state_fp) plD_state_tk,
	(plD_esc_fp) plD_esc_tk
    },
#endif

#ifdef PLD_linuxvga
    {
	"Linux console VGA Screen",
	"vga",
	plDevType_Interactive,
        8,
	(plD_init_fp) plD_init_vga,
	(plD_line_fp) plD_line_vga,
	(plD_polyline_fp) plD_polyline_vga,
	(plD_eop_fp) plD_eop_vga,
	(plD_bop_fp) plD_bop_vga,
	(plD_tidy_fp) plD_tidy_vga,
	(plD_state_fp) plD_state_vga,
	(plD_esc_fp) plD_esc_vga
    },
#endif

#ifdef PLD_win3
    {
	"Windows 3.x Driver",
	"win3",
	plDevType_Interactive,
        10,
	(plD_init_fp) plD_init_win3,
	(plD_line_fp) plD_line_win3,
	(plD_polyline_fp) plD_polyline_win3,
	(plD_eop_fp) plD_eop_win3,
	(plD_bop_fp) plD_bop_win3,
	(plD_tidy_fp) plD_tidy_win3,
	(plD_state_fp) plD_state_win3,
	(plD_esc_fp) plD_esc_win3
    },
#endif

#if defined (_MSC_VER) && defined (VGA)         /* graphics for msc */
    {
	"DOS VGA Screen (msc)",
	"vga",
	plDevType_Interactive,
        11,
	(plD_init_fp) plD_init_vga,
	(plD_line_fp) plD_line_vga,
	(plD_polyline_fp) plD_polyline_vga,
	(plD_eop_fp) plD_eop_vga,
	(plD_bop_fp) plD_bop_vga,
	(plD_tidy_fp) plD_tidy_vga,
	(plD_state_fp) plD_state_vga,
	(plD_esc_fp) plD_esc_vga
    },
#endif

#ifdef PLD_bgi
    {
	"VGA Screen (BGI)",
	"vga",
	plDevType_Interactive,
        12,
	(plD_init_fp) plD_init_vga,
	(plD_line_fp) plD_line_vga,
	(plD_polyline_fp) plD_polyline_vga,
	(plD_eop_fp) plD_eop_vga,
	(plD_bop_fp) plD_bop_vga,
	(plD_tidy_fp) plD_tidy_vga,
	(plD_state_fp) plD_state_vga,
	(plD_esc_fp) plD_esc_vga
    },
#endif

#ifdef PLD_gnusvga
    {
	"SVGA Screen (GRX20)",
	"vga",
	plDevType_Interactive,
        13,
	(plD_init_fp) plD_init_vga,
	(plD_line_fp) plD_line_vga,
	(plD_polyline_fp) plD_polyline_vga,
	(plD_eop_fp) plD_eop_vga,
	(plD_bop_fp) plD_bop_vga,
	(plD_tidy_fp) plD_tidy_vga,
	(plD_state_fp) plD_state_vga,
	(plD_esc_fp) plD_esc_vga
    },
#endif

#ifdef PLD_tiff
    {
	"TIFF File (TIFFLIB / GRX20)",
	"tiff",
	plDevType_FileOriented,
        14,
	(plD_init_fp) plD_init_tiff,
	(plD_line_fp) plD_line_vga,
	(plD_polyline_fp) plD_polyline_vga,
	(plD_eop_fp) plD_eop_tiff,
	(plD_bop_fp) plD_bop_tiff,
	(plD_tidy_fp) plD_tidy_tiff,
	(plD_state_fp) plD_state_vga,
	(plD_esc_fp) plD_esc_tiff
    },
#endif

#ifdef PLD_jpg
    {
	"JPEG File (Independent JPEG Group based on GRX20)",
	"jpg",
	plDevType_FileOriented,
        15,
	(plD_init_fp) plD_init_jpg,
	(plD_line_fp) plD_line_vga,
	(plD_polyline_fp) plD_polyline_vga,
	(plD_eop_fp) plD_eop_jpg,
	(plD_bop_fp) plD_bop_jpg,
	(plD_tidy_fp) plD_tidy_jpg,
	(plD_state_fp) plD_state_vga,
	(plD_esc_fp) plD_esc_jpg
    },
#endif

#ifdef PLD_bmp
    {
	"Windows Bitmap File (GRX20)",
	"bmp",
	plDevType_FileOriented,
        16,
	(plD_init_fp) plD_init_bmp,
	(plD_line_fp) plD_line_vga,
	(plD_polyline_fp) plD_polyline_vga,
	(plD_eop_fp) plD_eop_bmp,
	(plD_bop_fp) plD_bop_bmp,
	(plD_tidy_fp) plD_tidy_bmp,
	(plD_state_fp) plD_state_vga,
	(plD_esc_fp) plD_esc_bmp
    },
#endif

#ifdef PLD_emxvga		       /* graphics for emx+gcc */
    {
	"VGA Screen (emx)",
	"vga",
	plDevType_Interactive,
        17,
	(plD_init_fp) plD_init_vga,
	(plD_line_fp) plD_line_vga,
	(plD_polyline_fp) plD_polyline_vga,
	(plD_eop_fp) plD_eop_vga,
	(plD_bop_fp) plD_bop_vga,
	(plD_tidy_fp) plD_tidy_vga,
	(plD_state_fp) plD_state_vga,
	(plD_esc_fp) plD_esc_vga
    },
#endif

#ifdef PLD_xterm
    {
	"Xterm Window",
	"xterm",
	plDevType_Interactive,
        18,
	(plD_init_fp) plD_init_xterm,
	(plD_line_fp) plD_line_tek,
	(plD_polyline_fp) plD_polyline_tek,
	(plD_eop_fp) plD_eop_tek,
	(plD_bop_fp) plD_bop_tek,
	(plD_tidy_fp) plD_tidy_tek,
	(plD_state_fp) plD_state_tek,
	(plD_esc_fp) plD_esc_tek
    },
#endif

#ifdef PLD_tek4010
    {
	"Tektronix Terminal (4010)",
	"tekt",
	plDevType_Interactive,
        19,
	(plD_init_fp) plD_init_tekt,
	(plD_line_fp) plD_line_tek,
	(plD_polyline_fp) plD_polyline_tek,
	(plD_eop_fp) plD_eop_tek,
	(plD_bop_fp) plD_bop_tek,
	(plD_tidy_fp) plD_tidy_tek,
	(plD_state_fp) plD_state_tek,
	(plD_esc_fp) plD_esc_tek
    },
#endif

#ifdef PLD_tek4107
    {
	"Tektronix Terminal (4105/4107)",
	"tek4107t",
	plDevType_Interactive,
        20,
	(plD_init_fp) plD_init_tek4107t,
	(plD_line_fp) plD_line_tek,
	(plD_polyline_fp) plD_polyline_tek,
	(plD_eop_fp) plD_eop_tek,
	(plD_bop_fp) plD_bop_tek,
	(plD_tidy_fp) plD_tidy_tek,
	(plD_state_fp) plD_state_tek,
	(plD_esc_fp) plD_esc_tek
    },
#endif

#ifdef PLD_mskermit
    {
	"MS-Kermit emulator",
	"mskermit",
	plDevType_Interactive,
        21,
	(plD_init_fp) plD_init_mskermit,
	(plD_line_fp) plD_line_tek,
	(plD_polyline_fp) plD_polyline_tek,
	(plD_eop_fp) plD_eop_tek,
	(plD_bop_fp) plD_bop_tek,
	(plD_tidy_fp) plD_tidy_tek,
	(plD_state_fp) plD_state_tek,
	(plD_esc_fp) plD_esc_tek
    },
#endif

#ifdef PLD_versaterm
    {
	"Versaterm vt100/tek emulator",
	"versaterm",
	plDevType_Interactive,
        22,
	(plD_init_fp) plD_init_versaterm,
	(plD_line_fp) plD_line_tek,
	(plD_polyline_fp) plD_polyline_tek,
	(plD_eop_fp) plD_eop_tek,
	(plD_bop_fp) plD_bop_tek,
	(plD_tidy_fp) plD_tidy_tek,
	(plD_state_fp) plD_state_tek,
	(plD_esc_fp) plD_esc_tek
    },
#endif

#ifdef PLD_vlt
    {
	"VLT vt100/tek emulator",
	"vlt",
	plDevType_Interactive,
        23,
	(plD_init_fp) plD_init_vlt,
	(plD_line_fp) plD_line_tek,
	(plD_polyline_fp) plD_polyline_tek,
	(plD_eop_fp) plD_eop_tek,
	(plD_bop_fp) plD_bop_tek,
	(plD_tidy_fp) plD_tidy_tek,
	(plD_state_fp) plD_state_tek,
	(plD_esc_fp) plD_esc_tek
    },
#endif

#ifdef PLD_conex
    {
	"Conex vt320/tek emulator",
	"conex",
	plDevType_Interactive,
        24,
	(plD_init_fp) plD_init_conex,
	(plD_line_fp) plD_line_tek,
	(plD_polyline_fp) plD_polyline_tek,
	(plD_eop_fp) plD_eop_tek,
	(plD_bop_fp) plD_bop_tek,
	(plD_tidy_fp) plD_tidy_tek,
	(plD_state_fp) plD_state_tek,
	(plD_esc_fp) plD_esc_tek
    },
#endif

#ifdef PLD_dg300
    {
	"DG300 Terminal",
	"dg300",
	plDevType_Interactive,
        25,
	(plD_init_fp) plD_init_dg,
	(plD_line_fp) plD_line_dg,
	(plD_polyline_fp) plD_polyline_dg,
	(plD_eop_fp) plD_eop_dg,
	(plD_bop_fp) plD_bop_dg,
	(plD_tidy_fp) plD_tidy_dg,
	(plD_state_fp) plD_state_dg,
	(plD_esc_fp) plD_esc_dg
    },
#endif

    /* File types */

#ifdef PLD_plmeta
    {
	"PLplot Native Meta-File",
	"plmeta",
	plDevType_FileOriented,
        26,
	(plD_init_fp) plD_init_plm,
	(plD_line_fp) plD_line_plm,
	(plD_polyline_fp) plD_polyline_plm,
	(plD_eop_fp) plD_eop_plm,
	(plD_bop_fp) plD_bop_plm,
	(plD_tidy_fp) plD_tidy_plm,
	(plD_state_fp) plD_state_plm,
	(plD_esc_fp) plD_esc_plm
    },
#endif

#ifdef PLD_tek4010
    {
	"Tektronix File (4010)",
	"tekf",
	plDevType_FileOriented,
        27,
	(plD_init_fp) plD_init_tekf,
	(plD_line_fp) plD_line_tek,
	(plD_polyline_fp) plD_polyline_tek,
	(plD_eop_fp) plD_eop_tek,
	(plD_bop_fp) plD_bop_tek,
	(plD_tidy_fp) plD_tidy_tek,
	(plD_state_fp) plD_state_tek,
	(plD_esc_fp) plD_esc_tek
    },
#endif

#ifdef PLD_tek4107
    {
	"Tektronix File (4105/4107)",
	"tek4107f",
	plDevType_FileOriented,
        28,
	(plD_init_fp) plD_init_tek4107f,
	(plD_line_fp) plD_line_tek,
	(plD_polyline_fp) plD_polyline_tek,
	(plD_eop_fp) plD_eop_tek,
	(plD_bop_fp) plD_bop_tek,
	(plD_tidy_fp) plD_tidy_tek,
	(plD_state_fp) plD_state_tek,
	(plD_esc_fp) plD_esc_tek
    },
#endif

#ifdef PLD_ps
    {
	"PostScript File (monochrome)",
	"ps",
	plDevType_FileOriented,
        29,
	(plD_init_fp) plD_init_psm,
	(plD_line_fp) plD_line_ps,
	(plD_polyline_fp) plD_polyline_ps,
	(plD_eop_fp) plD_eop_ps,
	(plD_bop_fp) plD_bop_ps,
	(plD_tidy_fp) plD_tidy_ps,
	(plD_state_fp) plD_state_ps,
	(plD_esc_fp) plD_esc_ps
    },
    {
	"PostScript File (color)",
	"psc",
	plDevType_FileOriented,
        30,
	(plD_init_fp) plD_init_psc,
	(plD_line_fp) plD_line_ps,
	(plD_polyline_fp) plD_polyline_ps,
	(plD_eop_fp) plD_eop_ps,
	(plD_bop_fp) plD_bop_ps,
	(plD_tidy_fp) plD_tidy_ps,
	(plD_state_fp) plD_state_ps,
	(plD_esc_fp) plD_esc_ps
    },
#endif

#ifdef PLD_xfig
    {
	"Xfig file",
	"xfig",
	plDevType_FileOriented,
        31,
	(plD_init_fp) plD_init_xfig,
	(plD_line_fp) plD_line_xfig,
	(plD_polyline_fp) plD_polyline_xfig,
	(plD_eop_fp) plD_eop_xfig,
	(plD_bop_fp) plD_bop_xfig,
	(plD_tidy_fp) plD_tidy_xfig,
	(plD_state_fp) plD_state_xfig,
	(plD_esc_fp) plD_esc_xfig
    },
#endif

#ifdef PLD_ljiip
    {
	"LaserJet IIp/deskjet compressed graphics",
	"ljiip",
	plDevType_FileOriented,
        32,
	(plD_init_fp) plD_init_ljiip,
	(plD_line_fp) plD_line_ljiip,
	(plD_polyline_fp) plD_polyline_ljiip,
	(plD_eop_fp) plD_eop_ljiip,
	(plD_bop_fp) plD_bop_ljiip,
	(plD_tidy_fp) plD_tidy_ljiip,
	(plD_state_fp) plD_state_ljiip,
	(plD_esc_fp) plD_esc_ljiip
    },
#endif
    
#ifdef PLD_ljii
    {
	"LaserJet II Bitmap File (150 dpi)",
	"ljii",
	plDevType_FileOriented,
        33,
	(plD_init_fp) plD_init_ljii,
	(plD_line_fp) plD_line_ljii,
	(plD_polyline_fp) plD_polyline_ljii,
	(plD_eop_fp) plD_eop_ljii,
	(plD_bop_fp) plD_bop_ljii,
	(plD_tidy_fp) plD_tidy_ljii,
	(plD_state_fp) plD_state_ljii,
	(plD_esc_fp) plD_esc_ljii
    },
#endif

#ifdef PLD_hp7470
    {
	"HP 7470 Plotter File (HPGL Cartridge, Small Plotter)",
	"hp7470",
	plDevType_FileOriented,
        34,
	(plD_init_fp) plD_init_hp7470,
	(plD_line_fp) plD_line_hpgl,
	(plD_polyline_fp) plD_polyline_hpgl,
	(plD_eop_fp) plD_eop_hpgl,
	(plD_bop_fp) plD_bop_hpgl,
	(plD_tidy_fp) plD_tidy_hpgl,
	(plD_state_fp) plD_state_hpgl,
	(plD_esc_fp) plD_esc_hpgl
    },
#endif

#ifdef PLD_hp7580
    {
	"HP 7580 Plotter File (Large Plotter)",
	"hp7580",
	plDevType_FileOriented,
        35,
	(plD_init_fp) plD_init_hp7580,
	(plD_line_fp) plD_line_hpgl,
	(plD_polyline_fp) plD_polyline_hpgl,
	(plD_eop_fp) plD_eop_hpgl,
	(plD_bop_fp) plD_bop_hpgl,
	(plD_tidy_fp) plD_tidy_hpgl,
	(plD_state_fp) plD_state_hpgl,
	(plD_esc_fp) plD_esc_hpgl
    },
#endif

#ifdef PLD_lj_hpgl
    {
	"HP Laserjet III, HPGL emulation mode",
	"lj_hpgl",
	plDevType_FileOriented,
        36,
	(plD_init_fp) plD_init_lj_hpgl,
	(plD_line_fp) plD_line_hpgl,
	(plD_polyline_fp) plD_polyline_hpgl,
	(plD_eop_fp) plD_eop_hpgl,
	(plD_bop_fp) plD_bop_hpgl,
	(plD_tidy_fp) plD_tidy_hpgl,
	(plD_state_fp) plD_state_hpgl,
	(plD_esc_fp) plD_esc_hpgl
    },
#endif

#ifdef PLD_imp
    {
	"Impress File",
	"imp",
	plDevType_FileOriented,
        37,
	(plD_init_fp) plD_init_imp,
	(plD_line_fp) plD_line_imp,
	(plD_polyline_fp) plD_polyline_imp,
	(plD_eop_fp) plD_eop_imp,
	(plD_bop_fp) plD_bop_imp,
	(plD_tidy_fp) plD_tidy_imp,
	(plD_state_fp) plD_state_imp,
	(plD_esc_fp) plD_esc_imp
    },
#endif

#ifdef PLD_pbm
    {
	"PDB (PPM) Driver",
	"pbm",
	plDevType_FileOriented,
        38,
	(plD_init_fp) plD_init_pbm,
	(plD_line_fp) plD_line_pbm,
	(plD_polyline_fp) plD_polyline_pbm,
	(plD_eop_fp) plD_eop_pbm,
	(plD_bop_fp) plD_bop_pbm,
	(plD_tidy_fp) plD_tidy_pbm,
	(plD_state_fp) plD_state_pbm,
	(plD_esc_fp) plD_esc_pbm
    },
#endif

#ifdef PLD_png
    {
        "PNG Driver",
	"png",
	plDevType_FileOriented,
        39,
	(plD_init_fp) plD_init_png,
	(plD_line_fp) plD_line_png,
	(plD_polyline_fp) plD_polyline_png,
	(plD_eop_fp) plD_eop_png,
	(plD_bop_fp) plD_bop_png,
	(plD_tidy_fp) plD_tidy_png,
	(plD_state_fp) plD_state_png,
	(plD_esc_fp) plD_esc_png
    },
#endif

#ifdef PLD_jpeg
    {
        "JPEG File (Independent JPEG Group based on libgd)",
	"jpeg",
	plDevType_FileOriented,
        40,
	(plD_init_fp) plD_init_png,
	(plD_line_fp) plD_line_png,
	(plD_polyline_fp) plD_polyline_png,
	(plD_eop_fp) plD_eop_jpeg,
	(plD_bop_fp) plD_bop_png,
	(plD_tidy_fp) plD_tidy_png,
	(plD_state_fp) plD_state_png,
	(plD_esc_fp) plD_esc_png
    },
#endif

#ifdef PLD_cgm
    {
        "CGM File (Computer Graphics Metafile libcd)",
	"cgm",
	plDevType_FileOriented,
        40,
	(plD_init_fp) plD_init_cgm,
	(plD_line_fp) plD_line_cgm,
	(plD_polyline_fp) plD_polyline_cgm,
	(plD_eop_fp) plD_eop_cgm,
	(plD_bop_fp) plD_bop_cgm,
	(plD_tidy_fp) plD_tidy_cgm,
	(plD_state_fp) plD_state_cgm,
	(plD_esc_fp) plD_esc_cgm
    },
#endif


#ifdef PLD_pstex
    {
	"Postscript/LaTeX device",
	"pstex",
	PlDevType_Null,
	41,
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
	plDevType_Null,
        42,
	(plD_init_fp) plD_init_null,
	(plD_line_fp) plD_line_null,
	(plD_polyline_fp) plD_polyline_null,
	(plD_eop_fp) plD_eop_null,
	(plD_bop_fp) plD_bop_null,
	(plD_tidy_fp) plD_tidy_null,
	(plD_state_fp) plD_state_null,
	(plD_esc_fp) plD_esc_null
    }
#endif
};

static int nplstaticdevices = (sizeof(static_devices)/sizeof(PLDispatchTable));
#endif

static int nplstaticdevices = ( sizeof(static_device_initializers) /
                                sizeof(PLDispatchInit) );
static int npldynamicdevices = 0;

/*--------------------------------------------------------------------------*\
 * Stuff to support the loadable device drivers.
\*--------------------------------------------------------------------------*/

typedef struct {
    char *devnam;
    char *description;
    char *drvnam;
    char *tag;
    int drvidx;
} PLLoadableDevice;

typedef struct {
    char *drvnam;
    void *dlhand;
    
} PLLoadableDriver;


static PLLoadableDevice *loadable_device_list;
static PLLoadableDriver *loadable_driver_list;

static int nloadabledrivers = 0;

#endif	/* __PLCORE_H__ */
