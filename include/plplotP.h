/* $Id$
 * $Log$
 * Revision 1.24  1994/07/23 04:46:26  mjl
 * Decreased the amount of custom system handling due to use of plConfig.h.
 * Need to do some more yet...
 *
 * Revision 1.23  1994/07/22  15:54:09  furnish
 * Fix overaggressive behavior.
 *
 * Revision 1.22  1994/07/19  22:35:04  mjl
 * Added includes for pdf.h and plConfig.h.
 *
 * Revision 1.21  1994/06/30  18:36:06  mjl
 * Removed prototypes for deleted accessor functions.  Inserted an extern
 * PLStream *plsc declaration for easier access to PLplot state info (only
 * plcore.h, which declares the real plsc, doesn't see this declaration).
 * Now includes plstream.h.
 *
 * Revision 1.20  1994/05/20  22:25:14  mjl
 * Switched order of inclusion of plplot.h with system-specific defines to
 * ensure that all sick hacks are made before too much damage occurs
 * (specifically to fix a problem on the Cray, caddr_t again, sigh).
 *
 * Revision 1.19  1994/05/10  21:51:04  mjl
 * Added prototype for new function plcmap1_calc().
 *
 * Revision 1.18  1994/04/08  12:13:14  mjl
 * Removed PLESC tag defs (now in plplot.h) and unsigned var defs (now in
 * pdf.h).
 *
 * Revision 1.17  1994/03/23  07:05:51  mjl
 * Cruft elimination, including stuff that was for dealing with non-ANSI
 * or marginally ANSI compliant compilers (special treatment for malloc
 * includes, etc).  New function prototypes, and new defines for PLSTATE
 * settings governing changes to cmap 1 and/or the color palette.
*/

/*
    plplotP.h

    Copyright (C) 1993 by 
    Maurice J. LeBrun, Geoff Furnish, Tony Richardson.

    Internal (private) macros and prototypes for the PLPLOT package.  This
    header file must be included before all others, including system header
    files.

    This software may be freely copied, modified and redistributed without
    fee provided that this copyright notice is preserved intact on all
    copies and modified copies. 
 
    There is no warranty or other guarantee of fitness of this software.
    It is provided solely "as is". The author(s) disclaim(s) all
    responsibility and liability with respect to this software's usage or
    its effect upon hardware or computer systems. 
*/

#ifndef __PLPLOTP_H__
#define __PLPLOTP_H__

/*----------------------------------------------------------------------*\
 *        SYSTEM-SPECIFIC SETTINGS
 * 
 * Here we enable or disable based on system specific capabilities of
 * PLPLOT.  At present there are only two different "optional"
 * capabilities.  They are:
 *
 * POSIX_TTY	Defined if POSIX.1 tty control functions are available. 
 * STDC_HEADERS	Defined if libc is ANSI-compliant.
 *
 * POSIX.1 tty control functions are used by some of the drivers, e.g. to
 * switch to character-oriented (CBREAK) i/o (notably the tek driver and
 * all its variants).  It is usable without this but not as powerful.  The
 * reason for using this is that some supported systems are still not
 * POSIX.1 compliant (and some may never be).
 *
 * ANSI libc calls are used for: (a) setting up handlers to be called
 * before program exit (via the "atexit" call), and (b) for seek
 * operations.  Again, the code is usable without these.  An ANSI libc
 * should be available, given the requirement of an ANSI compiler.  Some
 * reasons why not: (a) the vendor didn't supply a complete ANSI
 * environment, or (b) the ANSI libc calls are buggy, or (c) you ported
 * gcc to your system but not glibc (for whatever reason).  Note: without
 * an ANSI C lib, if you ^C out of a program using one of the PLplot tek
 * drivers, your terminal may be left in a strange state.
 * 
\*----------------------------------------------------------------------*/

#include "plConfig.h"

#define POSIX_TTY

#ifdef CRAY			/* Cray */
#undef _POSIX_SOURCE		/* because of moronic broken X headers */
#endif

#ifdef SX			/* NEC SX-3 */
#undef POSIX_TTY		/* Not POSIX.1 compliant */
#endif

#ifdef MSDOS			/* MSDOS */
#undef POSIX_TTY		/* Not POSIX.1 compliant */
#endif

#ifdef AMIGA			/* Amiga */
#undef POSIX_TTY		/* Not POSIX.1 compliant */
#ifdef STDC_HEADERS		/* Required by SAS/C 5.X */
#define STDC_HEADERS 1		/* (still pretty prevalent, so..) */
#endif
#endif

/* Hacks to deal with non-ANSI libc */

#ifdef STDC_HEADERS
#define FPOS_T fpos_t
#define pl_fsetpos(a,b) fsetpos(a, b)
#define pl_fgetpos(a,b) fgetpos(a, b)

#else
#define FPOS_T long
#define pl_fsetpos(a,b) fseek(a, *b, 0)
#define pl_fgetpos(a,b) (-1L == (*b = ftell(a)))
#endif

/* Include all externally-visible definitions and prototypes */
/* plplot.h also includes some handy system header files */

#include "plplot.h"
#include "pdf.h"
#include "plstream.h"

/* If not including this file from inside of plcore.h, declare plsc */

#ifndef __PLCORE_H__
extern PLStream	*plsc;
#endif

/*----------------------------------------------------------------------*\
 *			Data types
\*----------------------------------------------------------------------*/

/* Signed char type, for the font tables */

typedef signed char SCHAR;

/*----------------------------------------------------------------------*\
 *                       Utility macros
\*----------------------------------------------------------------------*/

#ifndef TRUE
#define TRUE  1
#define FALSE 0
#endif

/* For the truly desperate debugging task */

#ifdef DEBUG_ENTER
#define dbug_enter(a) \
    fprintf(stderr, "%s: Entered %s\n", __FILE__, a);

#else
#define dbug_enter(a)
#endif

/* Used to help ensure everything malloc'ed gets freed */

#define free_mem(a) \
    if (a != NULL) { free((void *) a); a = NULL; }

/* Allows multi-argument setup calls to not affect selected arguments */

#define plsetvar(a, b) \
    if (b != PL_NOTSET) a = b;

/* Lots of cool math macros */

#ifndef MAX
#define MAX(a,b)    (((a) > (b)) ? (a) : (b))
#endif
#ifndef MIN
#define MIN(a,b)    (((a) < (b)) ? (a) : (b))
#endif
#ifndef ABS
#define ABS(a)      ((a)<0 ? -(a) : (a))
#endif
#ifndef ROUND
#define ROUND(a)    (PLINT)((a)<0. ? ((a)-.5) : ((a)+.5))
#endif
#ifndef BETW
#define BETW(ix,ia,ib)  (((ix)<=(ia)&&(ix)>=(ib)) || ((ix)>=(ia)&&(ix)<=(ib)))
#endif
#ifndef SSQR
#define SSQR(a,b)       sqrt((a)*(a)+(b)*(b))
#endif
#ifndef SIGN
#define SIGN(a)         ((a)<0 ? -1 : 1)
#endif

/* A coordinate value that should never occur */

#define UNDEFINED -9999999

/*----------------------------------------------------------------------*\
 *                       PLPLOT control macros
\*----------------------------------------------------------------------*/

/* Some constants */

#define PL_MAXPOLY	256	/* Max segments in polyline or polygon */
#define PL_NSTREAMS	100	/* Max number of concurrent streams. */
#define PL_RGB_COLOR	1<<7	/* A hack */

#define TEXT_MODE	0
#define GRAPHICS_MODE	1
#ifndef PI
#define PI		3.1415926535897932384
#endif

/* These define the metafile & X driver (virtual) coordinate systems */

#define PIXELS_X	8192		/* Number of virtual pixels in x */
#define PIXELS_Y	8192		/* Number of virtual pixels in x */
#define LPAGE_X		254.0		/* Page length in x in virtual mm */
#define LPAGE_Y		190.5		/* Page length in y in virtual mm */

/* Switches for state function call. */

#define PLSTATE_WIDTH		1	/* pen width */
#define PLSTATE_COLOR0		2	/* change to color in cmap 0 */
#define PLSTATE_COLOR1		3	/* change to color in cmap 1 */
#define PLSTATE_FILL		4	/* set area fill attribute */
#define PLSTATE_CMAP0		5	/* change to cmap 0 */
#define PLSTATE_CMAP1		6	/* change to cmap 1 */

/* Bit switches used in the driver interface */

#define PLDI_MAP	0x01
#define PLDI_ORI	0x02
#define PLDI_PLT	0x04
#define PLDI_DEV	0x08

/* Default size for family files, in KB.
 *  If you want it bigger, set it from the makefile or at runtime.
 */

#ifndef PL_FILESIZE_KB
#define PL_FILESIZE_KB 1000
#endif

/* Font file names. */

#define PLPLOT5_FONTS

#ifdef PLPLOT5_FONTS
#define PL_XFONT	"plxtnd5.fnt"
#define PL_SFONT	"plstnd5.fnt"
#else
#define PL_XFONT	"plxtnd4.fnt"
#define PL_SFONT	"plstnd4.fnt"
#endif

/*----------------------------------------------------------------------*\
 *		Function Prototypes
\*----------------------------------------------------------------------*/

#ifdef __cplusplus
extern "C" {
#endif
	/* These should not be called directly by the user */

/* Determines interval between numeric labels */

void
pldtik(PLFLT vmin, PLFLT vmax, PLFLT *tick, PLINT *nsubt,
       PLINT *mode, PLINT *prec, PLINT digmax, PLINT *scale);

/* Draws a polyline within the clip limits. */

void
plP_pllclp(PLINT *x, PLINT *y, PLINT npts,
	   PLINT xmin, PLINT xmax, PLINT ymin, PLINT ymax, 
	   void (*draw) (short *, short *, PLINT));

/* Fills a polygon within the clip limits. */

void
plP_plfclp(PLINT *x, PLINT *y, PLINT npts,
	   PLINT xmin, PLINT xmax, PLINT ymin, PLINT ymax, 
	   void (*draw) (short *, short *, PLINT));

/* Pattern fills in software the polygon bounded by the input points. */

void
plfill_soft(short *x, short *y, PLINT npts);

/* In case of an abort this routine is called.  It just prints out an */
/* error message and tries to clean up as much as possible. */

void
plexit(char *errormsg);

/* Just a front-end to exit().  */

void
pl_exit(void);

/* A handy way to issue warnings, if need be. */

void
plwarn(char *errormsg);

/* Same as plwarn(), but appends ", aborting plot" to the error message */

void
plabort(char *errormsg);

/* Loads either the standard or extended font. */

void
plfntld(PLINT fnt);

/* Release memory for fonts. */

void
plfontrel(void);

/* Initializes color map 0. */

void
plCmap0_init(void);

/* Initializes color map 1. */

void
plCmap1_init(void);

/* Bin up cmap 1 space and assign colors to make inverse mapping easy. */

void
plcmap1_calc(void);

/* Writes the Hershey symbol "ch" centred at the physical coordinate */
/* (x,y). */

void
plhrsh(PLINT ch, PLINT x, PLINT y);

/* Draws a slanting tick at position (mx,my) (measured in mm) of */
/* vector length (dx,dy). */

void 
plstik(PLFLT mx, PLFLT my, PLFLT dx, PLFLT dy);

/* Prints out a "string" at reference position with physical coordinates */
/* (refx,refy). */

void
plstr(PLINT base, PLFLT *xform, PLINT refx, PLINT refy, const char *string);

/* Draws a tick parallel to x. */

void
plxtik(PLINT x, PLINT y, PLINT below, PLINT above);

/* Draws a tick parallel to y. */

void
plytik(PLINT x, PLINT y, PLINT left, PLINT right);

/* Get x-y domain in world coordinates for 3d plots */

void
plP_gdom(PLFLT *p_xmin, PLFLT *p_xmax, PLFLT *p_ymin, PLFLT *p_ymax);

/* Get vertical (z) scale parameters for 3-d plot */

void
plP_grange(PLFLT *p_zscl, PLFLT *p_zmin, PLFLT *p_zmax);

/* Get parameters used in 3d plots */

void
plP_gw3wc(PLFLT *p_dxx, PLFLT *p_dxy, PLFLT *p_dyx, PLFLT *p_dyy,
	  PLFLT *p_dyz);

/* Get viewport boundaries in physical coordinates */

void
plP_gvpp(PLINT *p_ixmin, PLINT *p_ixmax, PLINT *p_iymin, PLINT *p_iymax);

/* Set viewport boundaries in physical coordinates */

void
plP_svpp(PLINT ixmin, PLINT ixmax, PLINT iymin, PLINT iymax);

/* Get subpage boundaries in physical coordinates */

void
plP_gspp(PLINT *p_ixmin, PLINT *p_ixmax, PLINT *p_iymin, PLINT *p_iymax);

/* Get clip boundaries in physical coordinates */

void
plP_gclp(PLINT *p_ixmin, PLINT *p_ixmax, PLINT *p_iymin, PLINT *p_iymax);

/* Set clip boundaries in physical coordinates */

void
plP_sclp(PLINT ixmin, PLINT ixmax, PLINT iymin, PLINT iymax);

/* Get physical device limits in physical coordinates */

void
plP_gphy(PLINT *p_ixmin, PLINT *p_ixmax, PLINT *p_iymin, PLINT *p_iymax);

/* Set physical device limits in physical coordinates */

void
plP_sphy(PLINT ixmin, PLINT ixmax, PLINT iymin, PLINT iymax);

/* Get number of subpages on physical device and current subpage */

void
plP_gsub(PLINT *p_nx, PLINT *p_ny, PLINT *p_cs);

/* Set number of subpages on physical device and current subpage */

void
plP_ssub(PLINT nx, PLINT ny, PLINT cs);

/* Set up plot parameters according to the number of subpages. */

void
plP_subpInit(void);

/* Get number of micrometers in a pixel */

void
plP_gumpix(PLINT *p_ix, PLINT *p_iy);

/* Set number of micrometers in a pixel */

void
plP_sumpix(PLINT ix, PLINT iy);

/* Get font and color attributes */

void
plP_gatt(PLINT *p_ifnt, PLINT *p_icol0);

/* Set font and color attributes */

void
plP_satt(PLINT ifnt, PLINT icol0);

/* Get subpage boundaries in normalized device coordinates */

void
plP_gspd(PLFLT *p_xmin, PLFLT *p_xmax, PLFLT *p_ymin, PLFLT *p_ymax);

/* Get viewport boundaries in normalized device coordinates */

void
plP_gvpd(PLFLT *p_xmin, PLFLT *p_xmax, PLFLT *p_ymin, PLFLT *p_ymax);

/* Get viewport boundaries in normalized device coordinates */

void
plP_svpd(PLFLT xmin, PLFLT xmax, PLFLT ymin, PLFLT ymax);

/* Get viewport boundaries in world coordinates */

void
plP_gvpw(PLFLT *p_xmin, PLFLT *p_xmax, PLFLT *p_ymin, PLFLT *p_ymax);

/* Set viewport boundaries in world coordinates */

void
plP_svpw(PLFLT xmin, PLFLT xmax, PLFLT ymin, PLFLT ymax);

/* Get number of pixels to a millimeter */

void
plP_gpixmm(PLFLT *p_x, PLFLT *p_y);

/* Set number of pixels to a millimeter */

void
plP_spixmm(PLFLT x, PLFLT y);

/* All the drivers call this to set physical pixels/mm. */

void
plP_setpxl(PLFLT xpmm0, PLFLT ypmm0);

/* Get transformation variables for world coordinates to mm */

void
plP_gwm(PLFLT *p_xscl, PLFLT *p_xoff, PLFLT *p_yscl, PLFLT *p_yoff);

/* Set transformation variables for world coordinates to mm */

void
plP_swm(PLFLT xscl, PLFLT xoff, PLFLT yscl, PLFLT yoff);

/* Get transformation variables for millimeters from bottom left */

void
plP_gmp(PLFLT *p_xscl, PLFLT *p_xoff, PLFLT *p_yscl, PLFLT *p_yoff);

/* Set transformation variables for millimeters from bottom left */

void
plP_smp(PLFLT xscl, PLFLT xoff, PLFLT yscl, PLFLT yoff);

/* Get background parameters for 3d plot. */

void
plP_gzback(PLINT **zbf, PLINT **zbc, PLFLT **zbt);

/* Move to physical coordinates (x,y). */

void
plP_movphy(PLINT x, PLINT y);

/* Draw to physical coordinates (x,y). */

void
plP_draphy(PLINT x, PLINT y);

/* Move to world coordinates (x,y). */

void
plP_movwor(PLFLT x, PLFLT y);

/* Draw to world coordinates (x,y). */

void
plP_drawor(PLFLT x, PLFLT y);

/* Draw polyline in physical coordinates. */

void
plP_draphy_poly(PLINT *x, PLINT *y, PLINT n);

/* Draw polyline in world coordinates. */

void
plP_drawor_poly(PLFLT *x, PLFLT *y, PLINT n);

/* Sets up physical limits of plotting device and the mapping between
   normalized device coordinates and physical coordinates. */

void
plP_setphy(PLINT xmin, PLINT xmax, PLINT ymin, PLINT ymax);

/* Set up the subpage boundaries according to the current subpage selected */

void
plP_setsub(void);

/* Get the floating point precision (in number of places) in numeric labels. */

void
plP_gprec(PLINT *p_setp, PLINT *p_prec);

	/* Functions that return floats */

/* Computes the length of a string in mm, including escape sequences. */

PLFLT
plstrl(const char *string);

	/* Conversion functions */

/* device coords to millimeters from bottom left-hand corner (x) */

PLFLT
plP_dcmmx(PLFLT x);

/* device coords to millimeters from bottom left-hand corner (y) */

PLFLT
plP_dcmmy(PLFLT y);

/* define transformations between device coords and subpage coords (x) */

PLFLT
plP_dcscx(PLFLT x);

/* define transformations between device coords and subpage coords (y) */

PLFLT
plP_dcscy(PLFLT y);

/* millimeters from bottom left corner into device coords (x) */

PLFLT
plP_mmdcx(PLFLT x);

/* millimeters from bottom left corner into device coords (y) */

PLFLT
plP_mmdcy(PLFLT y);

/* subpage coords to device coords (x) */

PLFLT
plP_scdcx(PLFLT x);

/* subpage coords to device coords (y) */

PLFLT
plP_scdcy(PLFLT y);

/* world coords into millimeters (x) */

PLFLT
plP_wcmmx(PLFLT x);

/* world coords into millimeters (y) */

PLFLT
plP_wcmmy(PLFLT y);

/* undocumented transformation for 3d plot routines (x) */

PLFLT
plP_w3wcx(PLFLT x, PLFLT y, PLFLT z);

/* undocumented transformation for 3d plot routines (y) */

PLFLT
plP_w3wcy(PLFLT x, PLFLT y, PLFLT z);

/* device coords to physical coords (x) */

PLINT
plP_dcpcx(PLFLT x);

/* device coords to physical coords (y) */

PLINT
plP_dcpcy(PLFLT y);

/* millimeters from bottom left-hand corner to physical coords (x) */

PLINT
plP_mmpcx(PLFLT x);

/* millimeters from bottom left-hand corner to physical coords (y) */

PLINT
plP_mmpcy(PLFLT y);

/* world coords to physical coords (x) */

PLINT
plP_wcpcx(PLFLT x);

/* world coords to physical coords (y) */

PLINT
plP_wcpcy(PLFLT y);

/* Gets the character digitisation of Hershey table entry "char". */

PLINT
plcvec(PLINT ch, SCHAR ** xygr);

/* Similar to strpos, but searches for occurence of string str2. */

PLINT
plP_stindex(const char *str1, const char *str2);

/* Searches string str for first occurence of character chr.  */

PLINT
plP_strpos(char *str, int chr);

/* Searches string str for character chr (case insensitive). */

PLINT
plP_stsearch(const char *str, int chr);

	/* Driver calls */

/* Initialize device. */

void
plP_init(void);

/* Draw line between two points */

void
plP_line(short *x, short *y);

/* Draw polyline */

void
plP_polyline(short *x, short *y, PLINT npts);

/* Fill polygon */

void
plP_fill(short *x, short *y, PLINT npts);

/* End of page */

void
plP_eop(void);

/* End of page */

void
plP_bop(void);

/* Tidy up device (flush buffers, close file, etc.) */

void
plP_tidy(void);

/* Change state. */

void
plP_state(PLINT op);

/* Escape function, for driver-specific commands. */

void
plP_esc(PLINT op, void *ptr);

#ifdef __cplusplus
}
#endif

#endif	/* __PLPLOTP_H__ */
