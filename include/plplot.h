/* $Id$
   $Log$
   Revision 1.6  1992/10/20 20:14:11  mjl
   Added prototypes, definitions for new routine plfamadv(), for advancing
   to next family member file.

 * Revision 1.5  1992/10/12  17:10:32  mjl
 * Added support for PL_NEED_SIZE_T switch to plplot.h and reworked comments.
 * Moved plamiga.h to sys/amiga/src.
 *
 * Revision 1.4  1992/09/30  18:25:34  furnish
 * Massive cleanup to irradicate garbage code.  Almost everything is now
 * prototyped correctly.  Builds on HPUX, SUNOS (gcc), AIX, and UNICOS.
 *
 * Revision 1.3  1992/09/29  04:45:32  furnish
 * Massive clean up effort to remove support for garbage compilers (K&R).
 *
 * Revision 1.2  1992/07/31  06:05:16  mjl
 * Added prototype for new function used in contour plots (pltrf0).
 *
 * Revision 1.1  1992/05/20  21:33:43  furnish
 * Initial checkin of the whole PLPLOT project.
 *
*/

/*
    plplot.h

    Copyright (C) 1992 by 
    Maurice J. LeBrun, Geoff Furnish, Tony Richardson.

    Macros and prototypes for the PLPLOT package.

    This software may be freely copied, modified and redistributed without
    fee provided that this copyright notice is preserved intact on all
    copies and modified copies. 
 
    There is no warranty or other guarantee of fitness of this software.
    It is provided solely "as is". The author(s) disclaim(s) all
    responsibility and liability with respect to this software's usage or
    its effect upon hardware or computer systems. 

    Note: some systems allow the Fortran & C namespaces to clobber each
    other.  So for PLPLOT to work from Fortran, we do some rather nasty
    things to the externally callable C function names.  This shouldn't
    affect any user programs in C as long as this file is included.
*/

#ifndef __PLPLOT_H__
#define __PLPLOT_H__

#define _POSIX_SOURCE

/*----------------------------------------------------------------------*\
*    USING PLPLOT
* 
* To use PLPLOT from C or C++, it is only necessary to 
* 
*      #include "plplot.h"
* 
* This file does all the necessary setup to make PLPLOT accessible to
* your program as documented in the manual.  Additionally, this file
* allows you to request certain behavior by defining certain symbols
* before inclusion.  These are:
*
* #define PL_DOUBLE
*	This causes PLPLOT to use doubles instead of floats.  Use the type
*	PLFLT everywhere in your code, and it will always be the right thing.
* 
* Additionally, there are some internal macros which are used in the
* plplot sources (which of course also #include "plplot.h") to make
* the sources substantially system independent.  If you want to use
* them in your own code, you may.  They are:
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
*	and you must include <stddef.h> to get it.
*
* #define PL_NO_VOID
*	In case there's still a compiler out there somewhere in the void,
*	with no void.  
*
\*----------------------------------------------------------------------*/

/*----------------------------------------------------------------------*\
*        SYSTEM IDENTIFICATION
*
* Several systems are supported directly by PLPLOT.  In order to avoid
* confusion, one id macro per system is used.  Since different compilers
* may predefine different system id macros, we need to check all the
* possibilities, and then set the one we will be referencing.  These are:
*
* __cplusplus                Any C++ compiler
* unix                       Any unix system
* __hpux                     Any HP/UX system
* __aix                      Any AIX system
* __linux                    Linux for i386
* (others...)
*
\*----------------------------------------------------------------------*/

/* Check for AIX */

#if defined(_IBMR2) && defined(_AIX)
#define __aix
#define unix
#define STUB_L
#ifdef PL_NEED_MALLOC
#include <malloc.h>
#endif
#endif

/* Check for HP/UX */

#ifdef __hpux
#ifndef unix
#define unix
#endif
#define STUB_L
#define NO_SIGNED_CHAR
#ifndef _HPUX_SOURCE
#define _HPUX_SOURCE
#endif
#ifdef PL_NEED_MALLOC
#include <malloc.h>
#endif
#endif

/* Check for SUN systems */

#ifdef sun
#define STUB_LAU
#ifdef PL_NEED_MALLOC
#include <malloc.h>
#endif
#endif

/* Check for CRAY's */

#ifdef CRAY
#ifdef CRAY1
#undef _POSIX_SOURCE
#endif
#ifdef PL_NEED_MALLOC
#include <malloc.h>
#endif
#endif

/* Check for Amiga's */

#ifdef AMIGA
#ifdef PL_NEED_SIZE_T
#include <stddef.h>
#endif
#endif

/*----------------------------------------------------------------------*\
*                       Default types for PLPLOT
\*----------------------------------------------------------------------*/

#ifdef PL_DOUBLE
typedef double PLFLT;
#else
typedef float PLFLT;
#endif

typedef long PLINT;

/*----------------------------------------------------------------------*\
*                       Stuff from chdr.h
\*----------------------------------------------------------------------*/

/* Float and int types */

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

/* Some systems need the "b" flag when opening binary files.
   Other systems will choke on it, hence the need for this silliness.
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

/* Utility macros */

#ifndef TRUE
#define TRUE  1
#define FALSE 0
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
*                       End of stuff from chdr.h
\*----------------------------------------------------------------------*/

#ifdef PL_NO_VOID
#define VOID char
#else
#define VOID void
#endif

/* Some constants */

#define TEXT_MODE	0
#define GRAPHICS_MODE	1

/* Switches for escape function call. */

#define PL_SET_RGB	1
#define PL_ALLOC_NCOL	2
#define PL_SET_LPB	3

/* And data structures, for escape functions which require more
   than one argument (a pointer to the structure is passed). */

typedef struct {
    int icolor;
    char *name;
} pleNcol;

typedef struct {
    float red, green, blue;
} pleRGB;

/* Default size for family files, in KB.
*  If you want it bigger, set it from the makefile or at runtime.
*/

#ifndef PL_FILESIZE_KB
#define PL_FILESIZE_KB 1000
#endif

/* Font file names. */

#define PL_XFONT	"plxtnd4.fnt"
#define PL_SFONT	"plstnd4.fnt"

/* Max number of concurrent streams. */

#define PL_NSTREAMS 10

/*----------------------------------------------------------------------*\
*		BRAINDEAD-ness
*
*  Some systems allow the Fortran & C namespaces to clobber each other.
*  For plplot to work from Fortran on these systems, we must name the
*  the externally callable C functions something other than their Fortran
*  entry names.  In order to make this as easy as possible for the casual
*  user, yet reversible to those who abhor my solution, I have done the
*  following:
*
*	The C-language bindings are actually different from those 
*	described in the manual.  Macros are used to convert the 
*	documented names to the names used in this package.  The 
*	user MUST include plplot.h in order to get the name 
*	redefinition correct.
*
*  Sorry to have to resort to such an ugly kludge, but it is really the best
*  way to handle the situation at present.  If all available compilers on
*  eventually offer a way to correct this stupidity, then perhaps we can
*  reverse it (there is a way now, by throwing the -DNOBRAINDEAD switch,
*  but I really, really discourage you from doing this unless you know
*  what you are doing).  If you feel like screaming at someone (I sure
*  do), please direct it at your nearest system vendor who has a braindead
*  shared C/Fortran namespace scheme.
*
*  Note that this definition should not cause any noticable effects, with
*  the exception of when doing PLPLOT debugging, in which case you will
*  need to remember the real function names (same as before but with
*  a 'c_' prepended).
*
*  Also, to avoid macro conflicts, the BRAINDEAD part must not be
*  expanded in the stub routines.
\*----------------------------------------------------------------------*/

#ifndef BRAINDEAD
#define BRAINDEAD
#endif

#ifdef NOBRAINDEAD
#undef BRAINDEAD
#endif

#ifdef BRAINDEAD

#ifndef INCLUDED_PLSTUBS	/* i.e. do not expand this in the stubs */

#define    pladv	c_pladv
#define    plancol	c_plancol
#define    plaxes	c_plaxes
#define    plbeg	c_plbeg
#define    plbin	c_plbin
#define    plbox	c_plbox
#define    plbox3	c_plbox3
#define    plclr	c_plclr
#define    plcol	c_plcol
#define    plconf	c_plconf
#define    plcont	c_plcont
#define    plend	c_plend
#define    plend1	c_plend1
#define    plenv	c_plenv
#define    plerrx	c_plerrx
#define    plerry	c_plerry
#define    plfamadv	c_plfamadv
#define    plfill	c_plfill
#define    plfont	c_plfont
#define    plfontld	c_plfontld
#define    plgfam	c_plgfam
#define    plgfile	c_plgfile
#define    plgfnam	c_plgfnam
#define    plgpage	c_plgpage
#define    plgphy	c_plgphy
#define    plgra	c_plgra
#define    plgspa	c_plgspa
#define    plgstrm	c_plgstrm
#define    plgxax	c_plgxax
#define    plgyax	c_plgyax
#define    plgzax	c_plgzax
#define    plhist	c_plhist
#define    plhls        c_plhls
#define    pljoin	c_pljoin
#define    pllab	c_pllab
#define    plline	c_plline
#define    pllsty	c_pllsty
#define    plmesh	c_plmesh
#define    plmtex	c_plmtex
#define    plot3d	c_plot3d
#define    plpage	c_plpage
#define    plpat	c_plpat
#define    plpoin	c_plpoin
#define    plprec	c_plprec
#define    plpsty	c_plpsty
#define    plptex	c_plptex
#define    plrgb        c_plrgb
#define    plschr	c_plschr
#define    plsfam	c_plsfam
#define    plsfnam	c_plsfnam
#define    plsasp	c_plsasp
#define    plslpb	c_plslpb
#define    plsmaj	c_plsmaj
#define    plsmin	c_plsmin
#define    plsori	c_plsori
#define    plspage	c_plspage
#define    plspause	c_plspause
#define    plsstrm	c_plsstrm
#define    plssym	c_plssym
#define    plstar	c_plstar
#define    plstart	c_plstart
#define    plstyl	c_plstyl
#define    plsvpa	c_plsvpa
#define    plsxax	c_plsxax
#define    plsyax	c_plsyax
#define    plsym	c_plsym
#define    plszax	c_plszax
#define    pltext	c_pltext
#define    plvasp	c_plvasp
#define    plvpas	c_plvpas
#define    plvpor	c_plvpor
#define    plvsta	c_plvsta
#define    plw3d	c_plw3d
#define    plwid	c_plwid
#define    plwind	c_plwind

#endif /* INCLUDED_PLSTUBS */

#else

#define    c_pladv	pladv
#define    c_plancol	plancol
#define    c_plaxes	plaxes
#define    c_plbeg	plbeg
#define    c_plbin	plbin
#define    c_plbox	plbox
#define    c_plbox3	plbox3
#define    c_plclr	plclr
#define    c_plcol	plcol
#define    c_plconf	plconf
#define    c_plcont	plcont
#define    c_plend	plend
#define    c_plend1	plend1
#define    c_plenv	plenv
#define    c_plerrx	plerrx
#define    c_plerry	plerry
#define    c_plfamadv	plfamadv
#define    c_plfill	plfill
#define    c_plfont	plfont
#define    c_plfontld	plfontld
#define    c_plgfam	plgfam
#define    c_plgfile	plgfile
#define    c_plgfnam	plgfnam
#define    c_plgpage	plgpage
#define    c_plgra	plgra
#define    c_plgspa	plgspa
#define    c_plgstrm	plgstrm
#define    c_plgxax	plgxax
#define    c_plgyax	plgyax
#define    c_plgzax	plgzax
#define    c_plhist	plhist
#define    c_plhls	plhls       
#define    c_pljoin	pljoin
#define    c_pllab	pllab
#define    c_plline	plline
#define    c_pllsty	pllsty
#define    c_plmesh	plmesh
#define    c_plmtex	plmtex
#define    c_plot3d	plot3d
#define    c_plpage	plpage
#define    c_plpat	plpat
#define    c_plpoin	plpoin
#define    c_plprec	plprec
#define    c_plpsty	plpsty
#define    c_plptex	plptex
#define    c_plrgb	plrgb       
#define    c_plschr	plschr
#define    c_plsfam	plsfam
#define    c_plsfnam	plsfnam
#define    c_plsasp	plsasp
#define    c_plslpb	plslpb
#define    c_plsmaj	plsmaj
#define    c_plsmin	plsmin
#define    c_plsori	plsori
#define    c_plspage	plspage
#define    c_plspause	plspause
#define    c_plsstrm	plsstrm
#define    c_plssym	plssym
#define    c_plstar	plstar
#define    c_plstart	plstart
#define    c_plstyl	plstyl
#define    c_plsvpa	plsvpa
#define    c_plsxax	plsxax
#define    c_plsyax	plsyax
#define    c_plsym	plsym
#define    c_plszax	plszax
#define    c_pltext	pltext
#define    c_plvasp	plvasp
#define    c_plvpas	plvpas
#define    c_plvpor	plvpor
#define    c_plvsta	plvsta
#define    c_plw3d	plw3d
#define    c_plwid	plwid
#define    c_plwind	plwind

#endif	/* BRAINDEAD */

/*----------------------------------------------------------------------*\
*		Function Prototypes
\*----------------------------------------------------------------------*/

#ifdef __cplusplus
extern "C" {
#endif

	/* All void types */

	/* C routines callable from stub routines come first */

void c_pladv	(PLINT);

void c_plancol	(PLINT, char *);

void c_plaxes	(PLFLT, PLFLT, char *, PLFLT, PLINT, char *,
			PLFLT, PLINT);

void c_plbeg	(PLINT, PLINT, PLINT);

void c_plbin	(PLINT, PLFLT *, PLFLT *, PLINT);

void c_plbox	(char *, PLFLT, PLINT, char *, PLFLT, PLINT);

void c_plbox3	(char *, char *, PLFLT, PLINT, 
			char *, char *, PLFLT, PLINT, 
			char *, char *, PLFLT, PLINT);

void c_plclr	(void);

void c_plcol	(PLINT);

void c_plconf	(PLFLT **, PLINT, PLINT, PLINT, PLINT, 
			PLINT, PLINT, PLFLT *, PLINT, 
			void (*)(PLFLT, PLFLT, PLFLT *, PLFLT *, 
				 PLFLT *, PLFLT *, PLINT, PLINT), 
			PLFLT *, PLFLT *);

void c_plcont	(PLFLT **, PLINT, PLINT, PLINT, PLINT, 
			PLINT, PLINT, PLFLT *, PLINT, 
			void (*)(PLFLT, PLFLT, PLFLT *, PLFLT *));

void c_plend	(void);

void c_plend1	(void);

void c_plenv	(PLFLT, PLFLT, PLFLT, PLFLT, PLINT, PLINT);

void c_plerrx	(PLINT, PLFLT *, PLFLT *, PLFLT *);

void c_plerry	(PLINT, PLFLT *, PLFLT *, PLFLT *);

void c_plfamadv (void);

void c_plfill	(PLINT, PLFLT *, PLFLT *);

void c_plfont	(PLINT);

void c_plfontld	(PLINT);

void c_plgfam	( PLINT *, PLINT *, PLINT *);

void c_plgfnam	(char *);

void c_plgpage 	(PLFLT *, PLFLT *, PLINT *, PLINT *, PLINT *, PLINT *);

void c_plgra	(void);

void c_plgspa	(PLFLT *, PLFLT *, PLFLT *, PLFLT *);

void c_plgstrm	(PLINT *);

void c_plgxax	(PLINT *, PLINT *);

void c_plgyax	(PLINT *, PLINT *);

void c_plgzax	(PLINT *, PLINT *);

void c_plhist	(PLINT, PLFLT *, PLFLT, PLFLT, PLINT, PLINT);

void c_plhls	(PLFLT, PLFLT, PLFLT);

void c_pljoin	(PLFLT, PLFLT, PLFLT, PLFLT);

void c_pllab	(char *, char *, char *);

void c_plline	(PLINT, PLFLT *, PLFLT *);

void c_pllsty	(PLINT);

void c_plmesh	(PLFLT *, PLFLT *, PLFLT **, 
			PLINT, PLINT, PLINT);

void c_plmtex	(char *, PLFLT, PLFLT, PLFLT, char *);

void c_plot3d	(PLFLT *, PLFLT *, PLFLT **, 
			PLINT, PLINT, PLINT, PLINT);

void c_plpage	(void);

void c_plpat	(PLINT, PLINT *, PLINT *);

void c_plpoin	(PLINT, PLFLT *, PLFLT *, PLINT);

void c_plprec	(PLINT, PLINT);

void c_plpsty	(PLINT);

void c_plptex	(PLFLT, PLFLT, PLFLT, PLFLT, PLFLT, char *);

void c_plrgb	(PLFLT, PLFLT, PLFLT);

void c_plschr	(PLFLT, PLFLT);

void c_plsfam	( PLINT, PLINT, PLINT);

void c_plsfnam	(char *);

void c_plsasp	(PLFLT);

void c_plslpb	(PLFLT, PLFLT, PLFLT, PLFLT);

void c_plsmaj	(PLFLT, PLFLT);

void c_plsmin	(PLFLT, PLFLT);

void c_plsori	(PLINT);

void c_plspage 	(PLFLT, PLFLT, PLINT, PLINT, PLINT, PLINT);

void c_plspause	(PLINT);

void c_plsstrm	(PLINT);

void c_plssym	(PLFLT, PLFLT);

void c_plstar	(PLINT, PLINT);

void c_plstart	(char *, PLINT, PLINT);

void c_plstyl	(PLINT, PLINT *, PLINT *);

void c_plsvpa	(PLFLT, PLFLT, PLFLT, PLFLT);

void c_plsxax	(PLINT, PLINT);

void c_plsyax	(PLINT, PLINT);

void c_plsym	(PLINT, PLFLT *, PLFLT *, PLINT);

void c_plszax	(PLINT, PLINT);

void c_pltext	(void);

void c_plvasp	(PLFLT);

void c_plvpas	(PLFLT, PLFLT, PLFLT, PLFLT, PLFLT);

void c_plvpor	(PLFLT, PLFLT, PLFLT, PLFLT);

void c_plvsta	(void);

void c_plw3d	(PLFLT, PLFLT, PLFLT, PLFLT, 
			PLFLT, PLFLT, PLFLT, PLFLT, 
			PLFLT, PLFLT, PLFLT);

void c_plwend	(void);

void c_plwid	(PLINT);

void c_plwind	(PLFLT, PLFLT, PLFLT, PLFLT);

	/* The rest for use from C only */

	/* Contour plotters */

void  pltr0	(PLFLT, PLFLT, PLFLT *, PLFLT *);

void  pltr1	(PLFLT, PLFLT, PLFLT *, PLFLT *, 
			PLFLT *, PLFLT *, PLINT, PLINT);

void  pltr2	(PLFLT, PLFLT, PLFLT *, PLFLT *, 
			PLFLT *, PLFLT *, PLINT, PLINT);

void  pltr0f	(PLFLT x, PLFLT y, PLFLT * tx, PLFLT * ty);

void  pltr2f	(PLFLT, PLFLT, PLFLT *, PLFLT *, 
			PLFLT *, PLFLT *, PLINT, PLINT);

void  plcntr	(PLFLT **, PLINT, PLINT, PLINT, PLINT, 
			PLINT, PLINT, PLFLT, PLINT *, 
			PLINT *, PLINT *, PLINT, 
			void (*)(PLFLT, PLFLT, PLFLT *, PLFLT *));

void  plcntf	(PLFLT **, PLINT, PLINT, PLINT, PLINT,
			PLINT, PLINT, PLFLT, PLINT *,
			PLINT *, PLINT *, PLINT,
			void (*)(PLFLT, PLFLT, PLFLT *, PLFLT *, 
				 PLFLT *, PLFLT *, PLINT, PLINT), 
			PLFLT *, PLFLT *);

	/* These should not be called directly by the user */

void  plccal	(PLFLT **, PLINT, PLINT, PLFLT, 
			PLINT, PLINT, PLINT, PLINT, PLFLT *);

void  pldeco	(short **, PLINT *, char *);

void  pldtik	(PLFLT, PLFLT, PLFLT *, PLINT *, 
			PLINT *, PLINT *, PLINT, PLINT *);

void  plerx1	(PLFLT, PLFLT, PLFLT);

void  plery1	(PLFLT, PLFLT, PLFLT);

void  plexit	(char *);

void  pl_exit (void);

void  plwarn (char *errormsg);

void  plfntld (PLINT fnt);

void  plfontrel	(void);

void  plform	(PLFLT, PLINT, PLINT, char *);

void  plhrsh	(PLINT, PLINT, PLINT);

void  pllclp	(PLINT, PLINT, PLINT, PLINT);

void  plpoi1	(PLFLT, PLFLT, PLINT);

void  plr135	(PLINT *, PLINT *, PLINT);

void  plr45	(PLINT *, PLINT *, PLINT);

void  plsetup	(PLFLT, PLFLT, PLINT, PLINT);

void  plstik	(PLFLT, PLFLT, PLFLT, PLFLT);

void  plstr	(PLINT, PLFLT *, PLINT, PLINT, char *);

void  plsym1	(PLFLT, PLFLT, PLINT);

void  plxtik	(PLINT, PLINT, PLINT, PLINT);

void  plxybx	(char *, char *, PLFLT, PLFLT, PLFLT, PLFLT, 
			PLFLT, PLFLT, PLFLT, PLINT, PLINT, PLINT *);

void  plytik	(PLINT, PLINT, PLINT, PLINT);

void  plzbx	(char *, char *, PLINT, PLFLT, PLFLT, PLFLT, 
			PLFLT, PLFLT, PLFLT, PLFLT, PLFLT, PLINT, PLINT *);

void  plgrid3a	(PLFLT);

void  plnxtv	(PLINT *, PLINT *, PLINT, PLINT);

void  plside3a	(PLFLT *, PLFLT *, PLFLT **, PLINT, PLINT, PLINT);

void  plt3zz	(PLINT, PLINT, PLINT, PLINT, 
			PLINT, PLINT, PLFLT *, PLFLT *, PLFLT **, 
			PLINT, PLINT, PLINT *, PLINT *);

void  glev	(PLINT *);

void  slev	(PLINT);

void  gbase	(PLFLT *, PLFLT *, PLFLT *, PLFLT *);

void  sbase	(PLFLT, PLFLT, PLFLT, PLFLT);

void  gnms	(PLINT *);

void  snms	(PLINT);

void  gdev	(PLINT *, PLINT *, PLINT *);

void  sdev	(PLINT, PLINT, PLINT);

void  smod	(PLINT);

void  gcurr	(PLINT *, PLINT *);

void  scurr	(PLINT, PLINT);

void  gdom	(PLFLT *, PLFLT *, PLFLT *, PLFLT *);

void  sdom	(PLFLT, PLFLT, PLFLT, PLFLT);

void  grange	(PLFLT *, PLFLT *, PLFLT *);

void  srange	(PLFLT, PLFLT, PLFLT);

void  gw3wc	(PLFLT *, PLFLT *, PLFLT *, PLFLT *, PLFLT *);

void  sw3wc	(PLFLT, PLFLT, PLFLT, PLFLT, PLFLT);

void  gvpp	(PLINT *, PLINT *, PLINT *, PLINT *);

void  svpp	(PLINT, PLINT, PLINT, PLINT);

void  gspp	(PLINT *, PLINT *, PLINT *, PLINT *);

void  sspp	(PLINT, PLINT, PLINT, PLINT);

void  gclp	(PLINT *, PLINT *, PLINT *, PLINT *);

void  sclp	(PLINT, PLINT, PLINT, PLINT);

void  gphy	(PLINT *, PLINT *, PLINT *, PLINT *);

void  sphy	(PLINT, PLINT, PLINT, PLINT);

void  gsub	(PLINT *, PLINT *, PLINT *);

void  ssub	(PLINT, PLINT, PLINT);

void  gumpix	(PLINT *, PLINT *);

void  sumpix	(PLINT, PLINT);

void  gatt	(PLINT *, PLINT *);

void  satt	(PLINT, PLINT);

void  gcol	(PLINT *);

void  scol	(PLINT);

void  gwid	(PLINT *);

void  swid	(PLINT);

void  gspd	(PLFLT *, PLFLT *, PLFLT *, PLFLT *);

void  sspd	(PLFLT, PLFLT, PLFLT, PLFLT);

void  gvpd	(PLFLT *, PLFLT *, PLFLT *, PLFLT *);

void  svpd	(PLFLT, PLFLT, PLFLT, PLFLT);

void  gvpw	(PLFLT *, PLFLT *, PLFLT *, PLFLT *);

void  svpw	(PLFLT, PLFLT, PLFLT, PLFLT);

void  gpixmm	(PLFLT *, PLFLT *);

void  spixmm	(PLFLT, PLFLT);

void  setpxl	(PLFLT, PLFLT);

void  gwp	(PLFLT *, PLFLT *, PLFLT *, PLFLT *);

void  swp	(PLFLT, PLFLT, PLFLT, PLFLT);

void  gwm	(PLFLT *, PLFLT *, PLFLT *, PLFLT *);

void  swm	(PLFLT, PLFLT, PLFLT, PLFLT);

void  gdp	(PLFLT *, PLFLT *, PLFLT *, PLFLT *);

void  sdp	(PLFLT, PLFLT, PLFLT, PLFLT);

void  gmp	(PLFLT *, PLFLT *, PLFLT *,PLFLT *);

void  smp	(PLFLT, PLFLT, PLFLT, PLFLT);

void  gchr	(PLFLT *, PLFLT *);

void  schr	(PLFLT, PLFLT);

void  gsym	(PLFLT *, PLFLT *);

void  ssym	(PLFLT, PLFLT);

void  gmaj	(PLFLT *, PLFLT *);

void  smaj	(PLFLT, PLFLT);

void  gmin	(PLFLT *, PLFLT *);

void  smin	(PLFLT, PLFLT);

void  gzback	(PLINT **, PLINT **, PLFLT **);

void  spl3mode	(PLINT);

void  spl3upv	(PLINT);

void  goldhivw	(PLINT **);

void  goldlovw	(PLINT **);

void  cleanup	(void);

void  genlin	(PLINT, PLINT, PLINT, PLINT);

void  movphy	(PLINT, PLINT);

void  draphy	(PLINT, PLINT);

void  movwor	(PLFLT, PLFLT);

void  drawor	(PLFLT, PLFLT);

void  setphy	(PLINT, PLINT, PLINT, PLINT);

void  xform	(PLFLT, PLFLT, PLFLT *, PLFLT *);

void  gmark (PLINT *pmar[], PLINT *pspa[], PLINT **pnms);

void  gcure (PLINT **pcur, PLINT **ppen, PLINT **ptim, PLINT **pala);

void  gradv (void);

void  grclr (void);

void  grbeg (PLINT dev);

void  grcol (void);

void  gpat (PLINT *pinc[], PLINT *pdel[], PLINT *pnlin);

void  grgra (void);

void  spat (PLINT inc[], PLINT del[], PLINT nlin);

void  smark (PLINT mar[], PLINT spa[], PLINT nms);

void  scure (PLINT cur, PLINT pen, PLINT tim, PLINT ala);

void  grwid (void);

void   long2str (long *istring, char *cstring);

void   str2long (char *cstring, long *istring);

	/* Functions that return floats */

PLFLT plstrl	(char *);

	/* Stuff in convrt.c */

PLFLT dcmmx	(PLFLT);

PLFLT dcmmy	(PLFLT);

PLFLT dcscx	(PLFLT);

PLFLT dcscy	(PLFLT);

PLFLT mmdcx	(PLFLT);

PLFLT mmdcy	(PLFLT);

PLFLT scdcx	(PLFLT);

PLFLT scdcy	(PLFLT);

PLFLT wcmmx	(PLFLT);

PLFLT wcmmy	(PLFLT);

PLFLT w3wcx	(PLFLT, PLFLT, PLFLT);

PLFLT w3wcy	(PLFLT, PLFLT, PLFLT);

	/* Functions returning PLINTs */

PLINT plcvec	(PLINT, SCHAR **);

PLINT stindex	(char *, char *);

PLINT strpos	(char *, int);

PLINT stsearch	(char *, int);

	/* More stuff from convrt.c */

PLINT dcpcx	(PLFLT);

PLINT dcpcy	(PLFLT);

PLINT mmpcx	(PLFLT);

PLINT mmpcy	(PLFLT);

PLINT wcpcx	(PLFLT);

PLINT wcpcy	(PLFLT);

	/* Driver calls */

void grsetup	(void);

void grorient	(void);

void grinit	(void);

void grline	(PLINT, PLINT, PLINT, PLINT);

void grclear	(void);

void grpage	(void);

void grtidy	(void);

void grcolor	(void);

void grtext	(void);

void grgraph	(void);

void grwidth	(void);

void gresc	(PLINT, char *);

#ifdef __cplusplus
}
#endif

#endif	/* __PLPLOT_H__ */
