/* $Id$
   $Log$
   Revision 1.1  1993/07/02 07:27:16  mjl
   Created to hold private PLPLOT macros, declarations, and prototypes.
   Included by all PLPLOT source files.

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

/* Wishful thinking.. */

#ifndef _POSIX_SOURCE
#define _POSIX_SOURCE
#endif

#include "plplot.h"

/*----------------------------------------------------------------------*\
*        SYSTEM-SPECIFIC SETTINGS
* 
* We define some internal macros which are used to make the plplot sources
* substantially system independent.  They are:
*
* #define PL_NEED_MALLOC
*	From "C -- The Pocket Reference" by Herbert Schildt (1988):
*
*	"The proposed ANSI standard specifies that the header information
*	necessary to the dynamic allocation system will be in stdlib.h.
*	However, at the time of this writing, a great many C compilers
*	require you to include the header malloc.h instead."
*
* #define PL_NEED_SIZE_T
*	On some systems, size_t is not defined in the usual place (stdlib.h),
*	and you must include stddef.h to get it.
*
\*----------------------------------------------------------------------*/

#ifdef __aix
#ifdef PL_NEED_MALLOC
#include <malloc.h>
#endif
#endif

#ifdef __hpux
#ifdef PL_NEED_MALLOC
#include <malloc.h>
#endif
#endif

#ifdef sun
#ifndef NO_ANSI_LIBC		/* Handles Suns without an ansi libc */
#define NO_ANSI_LIBC
#endif
#ifndef NULL
#define NULL	0
#endif
#ifdef PL_NEED_MALLOC
#include <malloc.h>
#endif
#endif

#ifdef CRAY
#ifdef CRAY1
#undef _POSIX_SOURCE
#endif
#ifdef PL_NEED_MALLOC
#include <malloc.h>
#endif
#endif

#ifdef AMIGA
#ifndef NO_ANSI_LIBC		/* NO_ANSI_LIBC is required by SAS/C 5.X */
#define NO_ANSI_LIBC		/* (still pretty prevalent, so..) */
#endif
#ifdef PL_NEED_SIZE_T
#include <stddef.h>
#endif
#endif

/* NO_ANSI_LIBC hack */

#ifdef NO_ANSI_LIBC
#define FPOS_T long
#define pl_fsetpos(a,b) fseek(a, *b, 0)
#define pl_fgetpos(a,b) (-1L == (*b = ftell(a)))

#else
#define FPOS_T fpos_t
#define pl_fsetpos(a,b) fsetpos(a, b)
#define pl_fgetpos(a,b) fgetpos(a, b)
#endif

/*----------------------------------------------------------------------*\
*			Data types
\*----------------------------------------------------------------------*/

/* Signed char type, in case we ever need it. */

#ifdef VAXC
#define NO_SIGNED_CHAR
#endif

#ifdef sun
#define NO_SIGNED_CHAR
#endif

#ifdef NO_SIGNED_CHAR
   typedef char        SCHAR;
#else
   typedef signed char SCHAR;
#endif


/* Some unsigned types */

#ifndef U_CHAR
#define U_CHAR unsigned char
#endif

#ifndef U_SHORT
#define U_SHORT unsigned short
#endif

#ifndef U_INT
#define U_INT unsigned int
#endif

#ifndef U_LONG
#define U_LONG unsigned long
#endif

/*----------------------------------------------------------------------*\
*                       Utility macros
\*----------------------------------------------------------------------*/

/*
* Some systems need the "b" flag when opening binary files.
* Other systems will choke on it, hence the need for this silliness.
*/

#ifdef MSDOS
#define BINARY_FLAG
#endif

#ifdef GNU386
#define BINARY_FLAG
#endif

#ifdef BINARY_FLAG
#define BINARY_WRITE "wb+"
#define BINARY_READ "rb"
#else
#define BINARY_WRITE "w+"
#define BINARY_READ "r"
#endif

#ifndef TRUE
#define TRUE  1
#define FALSE 0
#endif

#ifdef DEBUG_ENTER
#define dbug_enter(a) \
    fprintf(stderr, "%s: Entered %s\n", __FILE__, a); fflush(stderr);

#else
#define dbug_enter(a)
#endif

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

#define UNDEFINED -9999999

/*----------------------------------------------------------------------*\
*                       PLPLOT control macros
\*----------------------------------------------------------------------*/

/* Some constants */

#define TEXT_MODE	0
#define GRAPHICS_MODE	1
#define PL_MAXCOLORS	16
#define PL_RGB_COLOR	1<<7
#define PL_MAXPOLYLINE	64
#define PL_NSTREAMS	100	/* Max number of concurrent streams. */

/* Switches for escape function call. */
/* Some of these are obsolete but are retained in order to process
   old metafiles */

#define PL_SET_RGB	1	/* obsolete */
#define PL_ALLOC_NCOL	2	/* obsolete */
#define PL_SET_LPB	3	/* not used yet */
#define PL_EXPOSE	4	/* handle window expose */
#define PL_RESIZE	5	/* handle window resize */
#define PL_REDRAW	6	/* handle window redraw (opt. scaled) */

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

void  pldtik		(PLFLT, PLFLT, PLFLT *, PLINT *, 
			 PLINT *, PLINT *, PLINT, PLINT *);

void  plP_pllclp	(PLINT *, PLINT *, PLINT, PLINT, PLINT, PLINT, PLINT, 
			 void (*draw) (short *, short *, PLINT));

void  plexit		(char *);

void  pl_exit		(void);

void  plwarn		(char *errormsg);

void  plfntld		(PLINT fnt);

void  plfontrel		(void);

void  plHLS_RGB		(PLFLT, PLFLT, PLFLT, PLFLT *, PLFLT *, PLFLT *);

void  plhrsh		(PLINT, PLINT, PLINT);

void  plstik		(PLFLT, PLFLT, PLFLT, PLFLT);

void  plstr		(PLINT, PLFLT *, PLINT, PLINT, char *);

void  plxtik		(PLINT, PLINT, PLINT, PLINT);

void  plytik		(PLINT, PLINT, PLINT, PLINT);

void  plP_glev		(PLINT *);

void  plP_slev		(PLINT);

void  plP_gbase		(PLFLT *, PLFLT *, PLFLT *, PLFLT *);

void  plP_sbase		(PLFLT, PLFLT, PLFLT, PLFLT);

void  plP_gnms		(PLINT *);

void  plP_snms		(PLINT);

void  plP_gcurr		(PLINT *, PLINT *);

void  plP_scurr		(PLINT, PLINT);

void  plP_gdom		(PLFLT *, PLFLT *, PLFLT *, PLFLT *);

void  plP_sdom		(PLFLT, PLFLT, PLFLT, PLFLT);

void  plP_grange	(PLFLT *, PLFLT *, PLFLT *);

void  plP_srange	(PLFLT, PLFLT, PLFLT);

void  plP_gw3wc		(PLFLT *, PLFLT *, PLFLT *, PLFLT *, PLFLT *);

void  plP_sw3wc		(PLFLT, PLFLT, PLFLT, PLFLT, PLFLT);

void  plP_gvpp		(PLINT *, PLINT *, PLINT *, PLINT *);

void  plP_svpp		(PLINT, PLINT, PLINT, PLINT);

void  plP_gspp		(PLINT *, PLINT *, PLINT *, PLINT *);

void  plP_sspp		(PLINT, PLINT, PLINT, PLINT);

void  plP_gclp		(PLINT *, PLINT *, PLINT *, PLINT *);

void  plP_sclp		(PLINT, PLINT, PLINT, PLINT);

void  plP_gphy		(PLINT *, PLINT *, PLINT *, PLINT *);

void  plP_sphy		(PLINT, PLINT, PLINT, PLINT);

void  plP_gsub		(PLINT *, PLINT *, PLINT *);

void  plP_ssub		(PLINT, PLINT, PLINT);

void  plP_gumpix	(PLINT *, PLINT *);

void  plP_sumpix	(PLINT, PLINT);

void  plP_gatt		(PLINT *, PLINT *);

void  plP_satt		(PLINT, PLINT);

void  plP_gcol		(PLINT *);

void  plP_scol		(PLINT);

void  plP_gwid		(PLINT *);

void  plP_swid		(PLINT);

void  plP_gspd		(PLFLT *, PLFLT *, PLFLT *, PLFLT *);

void  plP_sspd		(PLFLT, PLFLT, PLFLT, PLFLT);

void  plP_gvpd		(PLFLT *, PLFLT *, PLFLT *, PLFLT *);

void  plP_svpd		(PLFLT, PLFLT, PLFLT, PLFLT);

void  plP_gvpw		(PLFLT *, PLFLT *, PLFLT *, PLFLT *);

void  plP_svpw		(PLFLT, PLFLT, PLFLT, PLFLT);

void  plP_gpixmm	(PLFLT *, PLFLT *);

void  plP_spixmm	(PLFLT, PLFLT);

void  plP_setpxl	(PLFLT, PLFLT);

void  plP_gwp		(PLFLT *, PLFLT *, PLFLT *, PLFLT *);

void  plP_swp		(PLFLT, PLFLT, PLFLT, PLFLT);

void  plP_gwm		(PLFLT *, PLFLT *, PLFLT *, PLFLT *);

void  plP_swm		(PLFLT, PLFLT, PLFLT, PLFLT);

void  plP_gdp		(PLFLT *, PLFLT *, PLFLT *, PLFLT *);

void  plP_sdp		(PLFLT, PLFLT, PLFLT, PLFLT);

void  plP_gmp		(PLFLT *, PLFLT *, PLFLT *,PLFLT *);

void  plP_smp		(PLFLT, PLFLT, PLFLT, PLFLT);

void  plP_schr		(PLFLT, PLFLT);

void  plP_gscale	(PLFLT *);

void  plP_sscale	(PLFLT);

void  plP_gsym		(PLFLT *, PLFLT *);

void  plP_ssym		(PLFLT, PLFLT);

void  plP_gmaj		(PLFLT *, PLFLT *);

void  plP_smaj		(PLFLT, PLFLT);

void  plP_gmin		(PLFLT *, PLFLT *);

void  plP_smin		(PLFLT, PLFLT);

void  plP_gzback	(PLINT **, PLINT **, PLFLT **);

void  plP_movphy	(PLINT, PLINT);

void  plP_draphy	(PLINT, PLINT);

void  plP_movwor	(PLFLT, PLFLT);

void  plP_drawor	(PLFLT, PLFLT);

void  plP_draphy_poly 	(PLINT *x, PLINT *y, PLINT n);

void  plP_drawor_poly 	(PLFLT *x, PLFLT *y, PLINT n);

void  plP_setphy	(PLINT, PLINT, PLINT, PLINT);

void  plP_setsub	(void);

void  plP_gmark		(PLINT **, PLINT **, PLINT *);

void  plP_gcure		(PLINT **, PLINT **, PLINT **, PLINT **);

void  plP_gpat		(PLINT **, PLINT **, PLINT *);

void  plP_spat		(PLINT *, PLINT *, PLINT);

void  plP_smark		(PLINT *, PLINT *, PLINT);

void  plP_scure		(PLINT, PLINT, PLINT, PLINT);

void  plP_gprec		(PLINT *, PLINT *);

	/* Functions that return floats */

PLFLT plstrl		(char *);

	/* Stuff in convrt.c */

PLFLT plP_dcmmx		(PLFLT);

PLFLT plP_dcmmy		(PLFLT);

PLFLT plP_dcscx		(PLFLT);

PLFLT plP_dcscy		(PLFLT);

PLFLT plP_mmdcx		(PLFLT);

PLFLT plP_mmdcy		(PLFLT);

PLFLT plP_scdcx		(PLFLT);

PLFLT plP_scdcy		(PLFLT);

PLFLT plP_wcmmx		(PLFLT);

PLFLT plP_wcmmy		(PLFLT);

PLFLT plP_w3wcx		(PLFLT, PLFLT, PLFLT);

PLFLT plP_w3wcy		(PLFLT, PLFLT, PLFLT);

	/* Functions returning PLINTs */

PLINT plcvec		(PLINT, SCHAR **);

PLINT plP_stindex	(char *, char *);

PLINT plP_strpos	(char *, int);

PLINT plP_stsearch	(char *, int);

	/* More stuff from convrt.c */

PLINT plP_dcpcx		(PLFLT);

PLINT plP_dcpcy		(PLFLT);

PLINT plP_mmpcx		(PLFLT);

PLINT plP_mmpcy		(PLFLT);

PLINT plP_wcpcx		(PLFLT);

PLINT plP_wcpcy		(PLFLT);

	/* Pseudo-driver calls */

void  plP_adv		(void);

void  plP_clr		(void);

void  plP_col		(void);

void  plP_gra		(void);

void  plP_wid		(void);

	/* Driver calls */

void plP_init		(void);

void plP_line		(short *, short *);

void plP_polyline	(short *, short *, PLINT);

void plP_clear		(void);

void plP_page		(void);

void plP_tidy		(void);

void plP_color		(void);

void plP_text		(void);

void plP_graph		(void);

void plP_width		(void);

void plP_esc		(PLINT, void *);

#ifdef __cplusplus
}
#endif

#endif	/* __PLPLOTP_H__ */
