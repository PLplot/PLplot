/* $Id$
   $Log$
   Revision 1.29  1993/07/31 08:09:18  mjl
   Cleaned up a bit.

 * Revision 1.28  1993/07/16  22:28:09  mjl
 * Added prototypes for driver interface functions, new plshade function,
 * plgFileDevs (for retrieving file device list) function.  Simplified PLKey
 * definition by removing attribute info (not really necessary).
 *
 * Revision 1.27  1993/07/02  07:23:35  mjl
 * Now holds only those symbols absolutely needed by applications to use
 * PLPLOT.  Includes typedefs and function prototypes, maybe an occaisional
 * macro only.  _POSIX_SOURCE no longer defined here!
 *
 * Revision 1.26  1993/05/08  16:08:30  mjl
 * Bumped version number to 4.99d, but the 4.99d release won't happen for
 * some time yet.
 *
 * Revision 1.25  1993/04/26  20:00:53  mjl
 * Configuration info added for a DEC Alpha-based machine running OSF/1.
 *
 * Revision 1.24  1993/03/28  08:45:05  mjl
 * Added support for NEC SX-3.  Also moved determination of stub name handling
 * to plstubs.h (more appropriate).
*/

/*
    plplot.h

    Copyright (C) 1992 by 
    Maurice J. LeBrun, Geoff Furnish, Tony Richardson.

    Macros and prototypes for the PLPLOT package.  This header file must
    be included by all user codes.

    This software may be freely copied, modified and redistributed
    without fee provided that this copyright notice is preserved intact
    on all copies and modified copies.

    There is no warranty or other guarantee of fitness of this software.
    It is provided solely "as is". The author(s) disclaim(s) all
    responsibility and liability with respect to this software's usage
    or its effect upon hardware or computer systems.

    Note: some systems allow the Fortran & C namespaces to clobber each
    other.  So for PLPLOT to work from Fortran, we do some rather nasty
    things to the externally callable C function names.  This shouldn't
    affect any user programs in C as long as this file is included. 
*/

#ifndef __PLPLOT_H__
#define __PLPLOT_H__

#define PLPLOT_VERSION "4.99d"

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
* before inclusion.  At the moment the only one is:
*
* #define PL_DOUBLE
*
* This causes PLPLOT to use doubles instead of floats.  Use the type
* PLFLT everywhere in your code, and it will always be the right thing.
*
* Note: most of the functions visible here begin with "pl", while all
* of the data types and switches begin with "PL".  Eventually everything
* will conform to this rule in order to keep namespace pollution of the
* user code to a minimum.  All the PLPLOT source files actually include
* "plplotP.h", which includes this file as well as all the internally-
* visible declarations, etc.  
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
* __unix                     Any Unix-like system
* __hpux                     Any HP/UX system
* __aix                      Any AIX system
* __linux                    Linux for i386
* (others...)
*
\*----------------------------------------------------------------------*/

#ifdef unix			/* the old way */
#ifndef __unix
#define __unix
#endif
#endif

/* Make sure Unix systems define "__unix" */

#if defined(SX)	||				/* NEC Super-UX */      \
    (defined(_IBMR2) && defined(_AIX)) ||	/* AIX */               \
    defined(__hpux) ||				/* HP/UX */             \
    defined(sun) ||				/* SUN */               \
    defined(CRAY) ||				/* Cray */              \
    (defined(__alpha) && defined(__osf__))	/* DEC Alpha AXP/OSF */

#ifndef __unix
#define __unix
#endif
#endif

/*----------------------------------------------------------------------*\
* Base types for PLPLOT
*
* Only those that are necessary for function prototypes are defined here.
* Notes:
*
* PLINT is typedef'd to a long by default.  This choice is necessary on
* 16 bit int systems (most PC's) since 16 bits is too inaccurate for
* some PLPLOT functions.  Most current workstations have int==long for
* which the choice is irrelevant.  Many new 64-bit architectures (Alpha,
* R4000, Pentium) will have 64 bit longs and 32 bit ints which may
* require PLINT to be typedef'd to an int if a fortran INTEGER remains
* 32 bits (probable).
*
* short is currently used for device page coordinates, so they are
* bounded by (-32767, 32767).  This gives a max resolution of about 3000
* dpi, and improves performance in some areas over using a PLINT.
\*----------------------------------------------------------------------*/

#ifdef PL_DOUBLE
typedef double PLFLT;
#else
typedef float PLFLT;
#endif

#if defined(__alpha) && defined(__osf__)
typedef int PLINT;
#else
typedef long PLINT;
#endif

/* For passing user data, as with X's XtPointer */

typedef void* PLPointer;

/*----------------------------------------------------------------------*\
* Complex data types and other good stuff
\*----------------------------------------------------------------------*/

/* Plplot Option table & support constants */

/* Option-specific settings */

#define PL_OPT_ENABLED		0x0001	/* Processing is enabled */
#define PL_OPT_ARG		0x0002	/* Option has an argment */
#define PL_OPT_NODELETE		0x0004	/* Don't delete after processing */
#define PL_OPT_INVISIBLE	0x0008	/* Make invisible */

/* Option-processing settings -- mutually exclusive */

#define PL_OPT_FUNC		0x0100	/* Call handler function */
#define PL_OPT_BOOL		0x0200	/* Set *var = 1 */
#define PL_OPT_INT		0x0400	/* Set *var = atoi(optarg) */
#define PL_OPT_FLOAT		0x0800	/* Set *var = atof(optarg) */
#define PL_OPT_STRING		0x1000	/* Set var = optarg */

/* Global mode settings */
/* These override per-option settings */

#define PL_PARSE_PARTIAL	0x0000	/* For backward compatibility */
#define PL_PARSE_FULL		0x0001	/* Process fully & exit if error */
#define PL_PARSE_QUIET		0x0002	/* Don't issue messages */
#define PL_PARSE_NODELETE	0x0004	/* Don't delete options after */
					/* processing */
#define PL_PARSE_SHOWALL	0x0008	/* Show invisible options */
#define PL_PARSE_OVERRIDE	0x0010	/* Overrides internal option(s) */
#define PL_PARSE_NOPROGRAM	0x0020	/* Program name NOT in *argv[0].. */

typedef struct {
    char *opt;
    int  (*handler)	(char *, char *);
    void *var;
    long mode;
    char *syntax;
    char *desc;
} PLOptionTable;

/* Plplot Key structure */

#define PL_NKEYSTRING 20

typedef struct {
    unsigned long code;
    char string[PL_NKEYSTRING];
} PLKey;

/* Window structure for doing resizes without calling the X driver directly */
/* May add other attributes in time */

typedef struct {
    unsigned int width;
    unsigned int height;
} PLWindow;

/* See plcont.c for examples of the following */

/*
* PLfGrid is for passing (as a pointer to the first element) an arbitrarily
* dimensioned array.  The grid dimensions MUST be stored, with a maximum of 3
* dimensions assumed for now.
*/

typedef struct {
    PLFLT *f;
    PLINT nx, ny, nz;
} PLfGrid;

/*
* PLfGrid2 is for passing (as an array of pointers) a 2d function array.  The
* grid dimensions are passed for possible bounds checking.
*/

typedef struct {
    PLFLT **f;
    PLINT nx, ny;
} PLfGrid2;

/*
* NOTE: a PLfGrid3 is a good idea here but there is no way to exploit it yet
* so I'll leave it out for now.
*/

/*
* PLcGrid is for passing (as a pointer to the first element) arbitrarily
* dimensioned coordinate transformation arrays.  The grid dimensions MUST be
* stored, with a maximum of 3 dimensions assumed for now.
*/

typedef struct {
    PLFLT *xg, *yg, *zg;
    PLINT nx, ny, nz;
} PLcGrid;

/*
* PLcGrid2 is for passing (as arrays of pointers) 2d coordinate
* transformation arrays.  The grid dimensions are passed for possible bounds
* checking.
*/

typedef struct {
    PLFLT **xg, **yg, **zg;
    PLINT nx, ny;
} PLcGrid2;

/*
* NOTE: a PLcGrid3 is a good idea here but there is no way to exploit it yet
* so I'll leave it out for now.
*/

/* PLColor is the usual way to pass an rgb color value. */

typedef struct {
    unsigned char r, g, b;
} PLColor;

/*----------------------------------------------------------------------*\
*		BRAINDEAD-ness
*
*  Some systems allow the Fortran & C namespaces to clobber each other.
*  For plplot to work from Fortran on these systems, we must name the the
*  externally callable C functions something other than their Fortran entry
*  names.  In order to make this as easy as possible for the casual user,
*  yet reversible to those who abhor my solution, I have done the
*  following:
*
*	The C-language bindings are actually different from those
*	described in the manual.  Macros are used to convert the
*	documented names to the names used in this package.  The
*	user MUST include plplot.h in order to get the name
*	redefinition correct.
*
*  Sorry to have to resort to such an ugly kludge, but it is really the
*  best way to handle the situation at present.  If all available compilers
*  offer a way to correct this stupidity, then perhaps we can eventually
*  reverse it (there is a way now, by throwing the -DNOBRAINDEAD switch,
*  but I discourage you from doing this unless you know what you are
*  doing).  If you feel like screaming at someone (I sure do), please
*  direct it at your nearest system vendor who has a braindead shared
*  C/Fortran namespace.  Some vendors do offer compiler switches that
*  change the object names, but then everybody who wants to use the package
*  must throw these same switches, leading to no end of trouble.
*
*  Note that this definition should not cause any noticable effects, with
*  the exception of when doing PLPLOT debugging, in which case you will
*  need to remember the real function names (same as before but with a 'c_'
*  prepended).
*
*  Also, to avoid macro conflicts, the BRAINDEAD part must not be expanded
*  in the stub routines.
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
#define    plaxes	c_plaxes
#define    plbin	c_plbin
#define    plbox	c_plbox
#define    plbox3	c_plbox3
#define    plclr	c_plclr
#define    plcol	c_plcol
#define    plcont	c_plcont
#define    plcpstrm	c_plcpstrm
#define    plend	c_plend
#define    plend1	c_plend1
#define    plenv	c_plenv
#define    plerrx	c_plerrx
#define    plerry	c_plerry
#define    plfamadv	c_plfamadv
#define    plfill	c_plfill
#define    plflush	c_plflush
#define    plfont	c_plfont
#define    plfontld	c_plfontld
#define    plgchr	c_plgchr
#define    plgfam	c_plgfam
#define    plgfile	c_plgfile
#define    plgfnam	c_plgfnam
#define    plgpage	c_plgpage
#define    plgphy	c_plgphy
#define    plgra	c_plgra
#define    plgspa	c_plgspa
#define    plgstrm	c_plgstrm
#define    plgver	c_plgver
#define    plgxax	c_plgxax
#define    plgyax	c_plgyax
#define    plgzax	c_plgzax
#define    plhist	c_plhist
#define    plhls        c_plhls
#define    plinit	c_plinit
#define    pljoin	c_pljoin
#define    pllab	c_pllab
#define    plline	c_plline
#define    pllsty	c_pllsty
#define    plmesh	c_plmesh
#define    plmkstrm	c_plmkstrm
#define    plmtex	c_plmtex
#define    plot3d	c_plot3d
#define    plpage	c_plpage
#define    plpat	c_plpat
#define    plpoin	c_plpoin
#define    plprec	c_plprec
#define    plpsty	c_plpsty
#define    plptex	c_plptex
#define    plreplot	c_plreplot
#define    plrgb	c_plrgb
#define    plrgb1	c_plrgb1
#define    plsasp	c_plsasp
#define    plschr	c_plschr
#define    plscm0	c_plscm0
#define    plscm0n	c_plscm0n
#define    plscm1	c_plscm1
#define    plscm1f1	c_plscm1f1
#define    plscol0	c_plscol0
#define    plscolbg	c_plscolbg
#define    plscolor	c_plscolor
#define    plsdev	c_plsdev
#define    plsdiplt	c_plsdiplt
#define    plsdiplz	c_plsdiplz
#define    plsdidev	c_plsdidev
#define    plsdimap	c_plsdimap
#define    plsdiori	c_plsdiori
#define    plsesc	c_plsesc
#define    plsfam	c_plsfam
#define    plsfnam	c_plsfnam
#define    plslpb	c_plslpb
#define    plsmaj	c_plsmaj
#define    plsmin	c_plsmin
#define    plsori	c_plsori
#define    plspage	c_plspage
#define    plspause	c_plspause
#define    plsstrm	c_plsstrm
#define    plssub	c_plssub
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
#define    c_plaxes	plaxes
#define    c_plbin	plbin
#define    c_plbox	plbox
#define    c_plbox3	plbox3
#define    c_plclr	plclr
#define    c_plcol	plcol
#define    c_plcpstrm	plcpstrm
#define    c_plcont	plcont
#define    c_plend	plend
#define    c_plend1	plend1
#define    c_plenv	plenv
#define    c_plerrx	plerrx
#define    c_plerry	plerry
#define    c_plfamadv	plfamadv
#define    c_plfill	plfill
#define    c_plflush	plflush
#define    c_plfont	plfont
#define    c_plfontld	plfontld
#define    c_plgchr	plgchr
#define    c_plgfam	plgfam
#define    c_plgfile	plgfile
#define    c_plgfnam	plgfnam
#define    c_plgpage	plgpage
#define    c_plgra	plgra
#define    c_plgspa	plgspa
#define    c_plgstrm	plgstrm
#define    c_plgver	plgver
#define    c_plgxax	plgxax
#define    c_plgyax	plgyax
#define    c_plgzax	plgzax
#define    c_plhist	plhist
#define    c_plhls	plhls       
#define    c_plinit	plinit
#define    c_pljoin	pljoin
#define    c_pllab	pllab
#define    c_plline	plline
#define    c_pllsty	pllsty
#define    c_plmesh	plmesh
#define    c_plmkstrm	plmkstrm
#define    c_plmtex	plmtex
#define    c_plot3d	plot3d
#define    c_plpage	plpage
#define    c_plpat	plpat
#define    c_plpoin	plpoin
#define    c_plprec	plprec
#define    c_plpsty	plpsty
#define    c_plptex	plptex
#define    c_plreplot	plreplot
#define    c_plrgb	plrgb
#define    c_plrgb1	plrgb1
#define    c_plsasp	plsasp
#define    c_plschr	plschr
#define    c_plscm0	plscm0
#define    c_plscm0n	plscm0n
#define    c_plscm1	plscm1
#define    c_plscm1f1	plscm1f1
#define    c_plscol0	plscol0
#define    c_plscolbg	plscolbg
#define    c_plscolor	plscolor
#define    c_plsdev	plsdev
#define    c_plsdiplt	plsdiplt
#define    c_plsdiplz	plsdiplz
#define    c_plsdidev	plsdidev
#define    c_plsdimap	plsdimap
#define    c_plsdiori	plsdiori
#define    c_plsesc	plsesc
#define    c_plsfam	plsfam
#define    c_plsfnam	plsfnam
#define    c_plslpb	plslpb
#define    c_plsmaj	plsmaj
#define    c_plsmin	plsmin
#define    c_plsori	plsori
#define    c_plspage	plspage
#define    c_plspause	plspause
#define    c_plsstrm	plsstrm
#define    c_plssub	plssub
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

void c_plaxes	(PLFLT, PLFLT, char *, PLFLT, PLINT, char *,
		 PLFLT, PLINT);

void c_plbin	(PLINT, PLFLT *, PLFLT *, PLINT);

void c_plbox	(char *, PLFLT, PLINT, char *, PLFLT, PLINT);

void c_plbox3	(char *, char *, PLFLT, PLINT, 
		 char *, char *, PLFLT, PLINT, 
		 char *, char *, PLFLT, PLINT);

void c_plclr	(void);

void c_plcol	(PLINT);

void c_plcont	(PLFLT **, PLINT, PLINT, PLINT, PLINT, 
		 PLINT, PLINT, PLFLT *, PLINT, 
		 void (*)(PLFLT, PLFLT, PLFLT *, PLFLT *, PLPointer),
		 PLPointer);

void plcontf	(PLFLT (*) (PLINT, PLINT, PLPointer),
		 PLPointer,
		 PLINT, PLINT, PLINT, PLINT,
		 PLINT, PLINT, PLFLT *, PLINT,
		 void (*) (PLFLT, PLFLT, PLFLT *, PLFLT *, PLPointer),
		 PLPointer);

void c_plcpstrm	(PLINT *, char *, char *);

void c_plend	(void);

void c_plend1	(void);

void c_plenv	(PLFLT, PLFLT, PLFLT, PLFLT, PLINT, PLINT);

void c_plerrx	(PLINT, PLFLT *, PLFLT *, PLFLT *);

void c_plerry	(PLINT, PLFLT *, PLFLT *, PLFLT *);

void c_plfamadv (void);

void c_plfill	(PLINT, PLFLT *, PLFLT *);

void c_plflush	(void);

void c_plfont	(PLINT);

void c_plfontld	(PLINT);

void c_plgchr	(PLFLT *, PLFLT *);

void c_plgfam	( PLINT *, PLINT *, PLINT *);

void c_plgfnam	(char *);

void c_plgpage 	(PLFLT *, PLFLT *, PLINT *, PLINT *, PLINT *, PLINT *);

void c_plgra	(void);

void c_plgspa	(PLFLT *, PLFLT *, PLFLT *, PLFLT *);

void c_plgstrm	(PLINT *);

void c_plgver	(char *);

void c_plgxax	(PLINT *, PLINT *);

void c_plgyax	(PLINT *, PLINT *);

void c_plgzax	(PLINT *, PLINT *);

void c_plhist	(PLINT, PLFLT *, PLFLT, PLFLT, PLINT, PLINT);

void c_plhls	(PLFLT, PLFLT, PLFLT);

void c_plinit	(void);

void c_pljoin	(PLFLT, PLFLT, PLFLT, PLFLT);

void c_pllab	(char *, char *, char *);

void c_plline	(PLINT, PLFLT *, PLFLT *);

void c_pllsty	(PLINT);

void c_plmesh	(PLFLT *, PLFLT *, PLFLT **, 
		 PLINT, PLINT, PLINT);

void c_plmkstrm	(PLINT *);

void c_plmtex	(char *, PLFLT, PLFLT, PLFLT, char *);

void c_plot3d	(PLFLT *, PLFLT *, PLFLT **, 
		 PLINT, PLINT, PLINT, PLINT);

void c_plpage	(void);

void c_plpat	(PLINT, PLINT *, PLINT *);

void c_plpoin	(PLINT, PLFLT *, PLFLT *, PLINT);

void c_plprec	(PLINT, PLINT);

void c_plpsty	(PLINT);

void c_plptex	(PLFLT, PLFLT, PLFLT, PLFLT, PLFLT, char *);

void c_plreplot	(void);

void c_plrgb	(PLFLT, PLFLT, PLFLT);

void c_plrgb1	(PLINT, PLINT, PLINT);

void c_plsasp	(PLFLT);

void c_plschr	(PLFLT, PLFLT);

void c_plscm0	(PLINT *, PLINT *, PLINT *, PLINT);

void c_plscm0n	(PLINT);

void c_plscm1	(PLINT *, PLINT *, PLINT *);

void c_plscm1f1	(PLINT, PLFLT *);

void c_plscol0	(PLINT, PLINT, PLINT, PLINT);

void c_plscolbg	(PLINT, PLINT, PLINT);

void c_plscolor	(PLINT);

void c_plsdev	(char *);

void c_plsdiplt	(PLFLT, PLFLT, PLFLT, PLFLT);

void c_plsdiplz	(PLFLT, PLFLT, PLFLT, PLFLT);

void c_plsdidev	(PLFLT, PLFLT, PLFLT, PLFLT);

void c_plsdimap	(PLINT, PLINT, PLINT, PLINT, PLFLT, PLFLT);

void c_plsdiori	(PLFLT);

void c_plsesc	(char);

void c_plsfam	(PLINT, PLINT, PLINT);

void c_plsfnam	(char *);

void plshade	(PLFLT *, PLINT, PLINT, char *, PLFLT, 
		 PLFLT, PLFLT, PLFLT, void (*)(), 
		 PLFLT, PLFLT, PLINT, PLINT, PLINT, PLINT,
		 PLINT, PLINT, void (*)(), PLINT);

void c_plslpb	(PLFLT, PLFLT, PLFLT, PLFLT);

void c_plsmaj	(PLFLT, PLFLT);

void c_plsmin	(PLFLT, PLFLT);

void c_plsori	(PLINT);

void c_plspage 	(PLFLT, PLFLT, PLINT, PLINT, PLINT, PLINT);

void c_plspause	(PLINT);

void c_plsstrm	(PLINT);

void c_plssub	(PLINT, PLINT);

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

void plgdiplt	(PLFLT *, PLFLT *, PLFLT *, PLFLT *);

void plgdidev	(PLFLT *, PLFLT *, PLFLT *, PLFLT *);

void plgdiori	(PLFLT *);

void plgFileDevs(char ***, char ***, int *);

void plsKeyEH	(void (*)(PLKey *, void *, int *), void *);

	/* Transformation routines */

void  pltr0	(PLFLT, PLFLT, PLFLT *, PLFLT *, void *);

void  pltr1	(PLFLT, PLFLT, PLFLT *, PLFLT *, void *);

void  pltr2	(PLFLT, PLFLT, PLFLT *, PLFLT *, void *);

void  pltr2p	(PLFLT, PLFLT, PLFLT *, PLFLT *, void *);

void  pltr0f	(PLFLT, PLFLT, PLFLT *, PLFLT *, void *);

void  pltr2f	(PLFLT, PLFLT, PLFLT *, PLFLT *, void *);

void  xform	(PLFLT, PLFLT, PLFLT *, PLFLT *, void *);

	/* Function evaluators */

PLFLT plf2eval2 (PLINT, PLINT, void *);

PLFLT plf2eval	(PLINT, PLINT, void *);

PLFLT plf2evalr (PLINT, PLINT, void *);

	/* Command line parsing utilities */

void  plSyntax	(void);

void  plHelp	(void);

void  plNotes	(void);

int   plParseInternalOpts (int *, char **, PLINT);

int   plParseOpts	(int *, char **, PLINT, PLOptionTable *,
			 void (*) (char *));

int   plSetInternalOpt	(char *, char *);

	/* Miscellaneous */

void  plgesc	(char *);

void  pl_cmd	(PLINT, void *);

	/* Nice way to allocate space for a vectored 2d grid */

void  Alloc2dGrid (PLFLT ***, PLINT, PLINT);

void  Free2dGrid (PLFLT **, PLINT, PLINT);

#ifdef __cplusplus
}
#endif

#endif	/* __PLPLOT_H__ */
