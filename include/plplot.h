/* $Id$
 * $Log$
 * Revision 1.59  1994/04/25 19:04:51  mjl
 * Added typedef for PLControlPt, used in specifying cmap1.  Added function
 * prototypes for plHLS_RGB (new) and plRGB_HLS (now global), for converting
 * between HLS and RGB color specifications.
 *
 * Revision 1.58  1994/04/18  20:02:59  furnish
 * Autodetect inclusion of tk.h.
 *
 * Revision 1.57  1994/04/18  19:23:25  furnish
 * Added new function Pltk_Init to allow users to easily construct PLPLOT
 * enhanced application specific extended wishen.  Works great, needs advert.
 *
 * Revision 1.56  1994/04/08  12:12:27  mjl
 * Moved PLESC tags to this file to make them more accessible to users.
 * Added PLESC_EH tag for flushing the event queue only.  Added PLMouse
 * event struct typedef.  Changed prototype for plsexit().
 *
 * Revision 1.55  1994/03/23  07:03:36  mjl
 * plplot.h now includes stdio.h and stdlib.h !  This change afforded a
 * significant simplification of the header file structure, and should
 * rarely be an imposition (most files require these anyway).  Many new
 * function prototypes added, such as the color and colormap setting
 * functions for dealing with cmap 1, and plshade and its siblings.
 *
 * Revision 1.54  1994/03/22  23:18:01  furnish
 * Include a prototype for plFrameCmd if using Tk.
 *
 * Revision 1.53  1994/01/17  21:34:21  mjl
 * Added function prototype for c_plgcol0().
 *
 * Revision 1.52  1994/01/15  17:33:42  mjl
 * Bumped version number, defined prototype wrapper macro.
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

#define PLPLOT_VERSION "4.99g"

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
* #define DOUBLE	or..
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

/* The majority of PLPLOT source files require these, so.. */
/* Under ANSI C, they can be included any number of times */

#include <stdio.h>
#include <stdlib.h>

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

/* This will hopefully catch compilers that don't fully conform */

#ifndef __STDC__
#ifndef __cplusplus
If you reach this line, it means your compiler is incapable of building
plplot (not ANSI-compliant).  Time to get a new one.
#endif
#endif

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
    defined(__convexc__) ||			/* CONVEX */            \
    (defined(__alpha) && defined(__osf__))	/* DEC Alpha AXP/OSF */

#ifndef __unix
#define __unix
#endif
#endif

/* A wrapper used in some header files so they can be compiled with cc */

#define PLARGS(a)	a

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

#if defined(PL_DOUBLE) || defined(DOUBLE)
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

/* Switches for escape function call. */
/* Some of these are obsolete but are retained in order to process
   old metafiles */

#define PLESC_SET_RGB		1	/* obsolete */
#define PLESC_ALLOC_NCOL	2	/* obsolete */
#define PLESC_SET_LPB		3	/* obsolete */
#define PLESC_EXPOSE		4	/* handle window expose */
#define PLESC_RESIZE		5	/* handle window resize */
#define PLESC_REDRAW		6	/* handle window redraw */
#define PLESC_TEXT		7	/* switch to text screen */
#define PLESC_GRAPH		8	/* switch to graphics screen */
#define PLESC_FILL		9	/* fill polygon */
#define PLESC_DI		10	/* handle DI command */
#define PLESC_FLUSH		11	/* flush output */
#define PLESC_EH		12      /* handle Window events */

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
    int  (*handler)	(char *, char *, void *);
    void *client_data;
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

/* Plplot Mouse structure */

typedef struct {
    int button;
    int state;
    PLFLT x;
    PLFLT y;
} PLMouse;

/* Window structure for doing resizes without calling the X driver directly */
/* May add other attributes in time */

typedef struct {
    unsigned int width;
    unsigned int height;
} PLWindow;

/* Macro used (in some cases) to ignore value of argument */
/* I don't plan on changing the value so you can hard-code it */

#define PL_NOTSET (-42)

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

/* PLControlPt is how cmap1 control points are represented. */

typedef struct {
    PLFLT h, l, s, i;
} PLControlPt;

/*----------------------------------------------------------------------*\
*		BRAINDEAD-ness
*
* Some systems allow the Fortran & C namespaces to clobber each other.
* For plplot to work from Fortran on these systems, we must name the the
* externally callable C functions something other than their Fortran entry
* names.  In order to make this as easy as possible for the casual user,
* yet reversible to those who abhor my solution, I have done the
* following:
*
*	The C-language bindings are actually different from those
*	described in the manual.  Macros are used to convert the
*	documented names to the names used in this package.  The
*	user MUST include plplot.h in order to get the name
*	redefinition correct.
*
* Sorry to have to resort to such an ugly kludge, but it is really the
* best way to handle the situation at present.  If all available compilers
* offer a way to correct this stupidity, then perhaps we can eventually
* reverse it (there is a way now, by throwing the -DNOBRAINDEAD switch,
* but I discourage you from doing this unless you know what you are
* doing).  If you feel like screaming at someone (I sure do), please
* direct it at your nearest system vendor who has a braindead shared
* C/Fortran namespace.  Some vendors do offer compiler switches that
* change the object names, but then everybody who wants to use the package
* must throw these same switches, leading to no end of trouble.
*
* Note that this definition should not cause any noticable effects, with
* the exception of when doing PLPLOT debugging, in which case you will
* need to remember the real function names (same as before but with a 'c_'
* prepended).
*
* Also, to avoid macro conflicts, the BRAINDEAD part must not be expanded
* in the stub routines.
*
* Aside: the reason why a shared Fortran/C namespace is deserving of the
* BRAINDEAD characterization is that it completely precludes the the kind
* of universal API that is attempted with PLPLOT, without Herculean
* efforts (e.g. remapping all of the c bindings by macros as done here).
* The vendors of such a scheme, in order to allow a SINGLE type of
* argument to be passed transparently to both C and Fortran, namely, a
* pointer to a conformable data type, have slammed the door on insertion
* of stub routines to handle the conversions needed for other data types.
* Intelligent linkers could solve this problem, but these are not anywhere
* close to becoming universal.  So meanwhile, one must live with either
* stub routines for the inevitable data conversions, or a different API.
* The former is what is used here, but is made far more difficult in a
* braindead shared Fortran/C namespace.
\*----------------------------------------------------------------------*/

#ifndef BRAINDEAD
#define BRAINDEAD
#endif

#ifdef NOBRAINDEAD
#undef BRAINDEAD
#endif

#ifdef BRAINDEAD

#ifndef __PLSTUBS_H__	/* i.e. do not expand this in the stubs */

#define    pladv	c_pladv
#define    plaxes	c_plaxes
#define    plbin	c_plbin
#define    plbop	c_plbop
#define    plbox	c_plbox
#define    plbox3	c_plbox3
#define    plcol0	c_plcol0
#define    plcol1	c_plcol1
#define    plcont	c_plcont
#define    plcpstrm	c_plcpstrm
#define    plend	c_plend
#define    plend1	c_plend1
#define    plenv	c_plenv
#define    pleop	c_pleop
#define    plerrx	c_plerrx
#define    plerry	c_plerry
#define    plfamadv	c_plfamadv
#define    plfill	c_plfill
#define    plflush	c_plflush
#define    plfont	c_plfont
#define    plfontld	c_plfontld
#define    plgchr	c_plgchr
#define    plgcol0	c_plgcol0
#define    plgcolbg	c_plgcolbg
#define    plgdidev	c_plgdidev
#define    plgdiori	c_plgdiori
#define    plgdiplt	c_plgdiplt
#define    plgfam	c_plgfam
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
#define    plscmap0	c_plscmap0
#define    plscmap1	c_plscmap1
#define    plscmap0n	c_plscmap0n
#define    plscmap1n	c_plscmap1n
#define    plscmap1l	c_plscmap1l
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
#define    plshade	c_plshade
#define    plshade1	c_plshade1
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

#endif /* __PLSTUBS_H__ */

#else

#define    c_pladv	pladv
#define    c_plaxes	plaxes
#define    c_plbin	plbin
#define    c_plbop	plbop
#define    c_plbox	plbox
#define    c_plbox3	plbox3
#define    c_plcol0	plcol0
#define    c_plcol1	plcol1
#define    c_plcpstrm	plcpstrm
#define    c_plcont	plcont
#define    c_plend	plend
#define    c_plend1	plend1
#define    c_plenv	plenv
#define    c_pleop	pleop
#define    c_plerrx	plerrx
#define    c_plerry	plerry
#define    c_plfamadv	plfamadv
#define    c_plfill	plfill
#define    c_plflush	plflush
#define    c_plfont	plfont
#define    c_plfontld	plfontld
#define    c_plgchr	plgchr
#define    c_plgcol0	plgcol0
#define    c_plgcolbg	plgcolbg
#define    c_plgdidev	plgdidev
#define    c_plgdiori	plgdiori
#define    c_plgdiplt	plgdiplt
#define    c_plgfam	plgfam
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
#define    c_plscmap0	plscmap0
#define    c_plscmap1	plscmap1
#define    c_plscmap0n	plscmap0n
#define    c_plscmap1n	plscmap1n
#define    c_plscmap1l	plscmap1l
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
#define    c_plshade	plshade
#define    c_plshade1	plshade1
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

/* Redefine some old function names for backward compatibility */

#ifndef __PLSTUBS_H__	/* i.e. do not expand this in the stubs */

#define    plclr	pleop
#define    plpage	plbop
#define    plcol	plcol0
#define    plcontf	plfcont
#define	   Alloc2dGrid	plAlloc2dGrid
#define	   Free2dGrid	plFree2dGrid

#endif /* __PLSTUBS_H__ */

/*----------------------------------------------------------------------*\
*		Function Prototypes
\*----------------------------------------------------------------------*/

#ifdef __cplusplus
extern "C" {
#endif

	/* All void types */

	/* C routines callable from stub routines come first */

/* Advance to subpage "page", or to the next one if "page" = 0. */

void
c_pladv(PLINT page);

/* This functions similarly to plbox() except that the origin of the axes */
/* is placed at the user-specified point (x0, y0). */

void
c_plaxes(PLFLT x0, PLFLT y0, const char *xopt, PLFLT xtick, PLINT nxsub,
	 const char *yopt, PLFLT ytick, PLINT nysub);

/* Plot a histogram using x to store data values and y to store frequencies */

void
c_plbin(PLINT nbin, PLFLT *x, PLFLT *y, PLINT center);

/* Start new page.  Should only be used with pleop(). */

void
c_plbop(void);

/* This draws a box around the current viewport. */

void
c_plbox(const char *xopt, PLFLT xtick, PLINT nxsub,
	const char *yopt, PLFLT ytick, PLINT nysub);

/* This is the 3-d analogue of plbox(). */

void
c_plbox3(const char *xopt, const char *xlabel, PLFLT xtick, PLINT nsubx,
	 const char *yopt, const char *ylabel, PLFLT ytick, PLINT nsuby,
	 const char *zopt, const char *zlabel, PLFLT ztick, PLINT nsubz);

/* Set color, map 0.  Argument is integer between 0 and 15. */

void
c_plcol0(PLINT icol0);

/* Set color, map 1.  Argument is a float between 0. and 1. */

void
c_plcol1(PLFLT col1);

/* Draws a contour plot from data in f(nx,ny).  Is just a front-end to
* plfcont, with a particular choice for f2eval and f2eval_data. */

void
c_plcont(PLFLT **f, PLINT nx, PLINT ny, PLINT kx, PLINT lx,
	 PLINT ky, PLINT ly, PLFLT *clevel, PLINT nlevel,
	 void (*pltr) (PLFLT, PLFLT, PLFLT *, PLFLT *, PLPointer),
	 PLPointer pltr_data);

/* Draws a contour plot using the function evaluator f2eval and data stored
* by way of the f2eval_data pointer.  This allows arbitrary organizations
* of 2d array data to be used. */

void
plfcont(PLFLT (*f2eval) (PLINT, PLINT, PLPointer),
	PLPointer f2eval_data,
	PLINT nx, PLINT ny, PLINT kx, PLINT lx,
	PLINT ky, PLINT ly, PLFLT *clevel, PLINT nlevel,
	void (*pltr) (PLFLT, PLFLT, PLFLT *, PLFLT *, PLPointer),
	PLPointer pltr_data);

/* Copies state parameters from the reference stream to the current stream. */

void
c_plcpstrm(PLINT iplsr, PLINT flags);

/* Converts input values from relative device coordinates to relative plot */
/* coordinates. */

void
pldid2pc(PLFLT *xmin, PLFLT *ymin, PLFLT *xmax, PLFLT *ymax);

/* Converts input values from relative plot coordinates to relative */
/* device coordinates. */

void
pldip2dc(PLFLT *xmin, PLFLT *ymin, PLFLT *xmax, PLFLT *ymax);

/* End a plotting session for all open streams. */

void
c_plend(void);

/* End a plotting session for the current stream only. */

void
c_plend1(void);

/* Simple interface for defining viewport and window. */

void
c_plenv(PLFLT xmin, PLFLT xmax, PLFLT ymin, PLFLT ymax,
	PLINT just, PLINT axis);

/* End current page.  Should only be used with plbop(). */

void
c_pleop(void);

/* Plot horizontal error bars (xmin(i),y(i)) to (xmax(i),y(i)) */

void
c_plerrx(PLINT n, PLFLT *xmin, PLFLT *xmax, PLFLT *y);

/* Plot vertical error bars (x,ymin(i)) to (x(i),ymax(i)) */

void
c_plerry(PLINT n, PLFLT *x, PLFLT *ymin, PLFLT *ymax);

/* Advance to the next family file on the next new page */

void
c_plfamadv(void);

/* Pattern fills the polygon bounded by the input points. */

void
c_plfill(PLINT n, PLFLT *x, PLFLT *y);

/* Flushes the output stream.  Use sparingly, if at all. */

void
c_plflush(void);

/* Sets the global font flag to 'ifont'. */

void
c_plfont(PLINT ifont);

/* Load specified font set. */

void
c_plfontld(PLINT fnt);

/* Get character default height and current (scaled) height */

void
c_plgchr(PLFLT *p_def, PLFLT *p_ht);

/* Returns 8 bit RGB values for given color from color map 0 */

void
c_plgcol0(PLINT icol0, PLINT *r, PLINT *g, PLINT *b);

/* Returns the background color by 8 bit RGB value */

void
c_plgcolbg(PLINT *r, PLINT *g, PLINT *b);

/* Retrieve current window into device space */

void
c_plgdidev(PLFLT *p_mar, PLFLT *p_aspect, PLFLT *p_jx, PLFLT *p_jy);

/* Get plot orientation */

void
c_plgdiori(PLFLT *p_rot);

/* Retrieve current window into plot space */

void
c_plgdiplt(PLFLT *p_xmin, PLFLT *p_ymin, PLFLT *p_xmax, PLFLT *p_ymax);

/* Get family file parameters */

void
c_plgfam(PLINT *p_fam, PLINT *p_num, PLINT *p_bmax);

/* Get the (current) output file name.  Must be preallocated to >80 bytes */

void
c_plgfnam(char *fnam);

/* Get output device parameters. */

void
c_plgpage(PLFLT *p_xp, PLFLT *p_yp,
	  PLINT *p_xleng, PLINT *p_yleng, PLINT *p_xoff, PLINT *p_yoff);

/* Switches to graphics screen. */

void
c_plgra(void);

/* Get subpage boundaries in absolute coordinates */

void
c_plgspa(PLFLT *xmin, PLFLT *xmax, PLFLT *ymin, PLFLT *ymax);

/* Get current stream number. */

void
c_plgstrm(PLINT *p_strm);

/* Get the current library version number */

void
c_plgver(char *p_ver);

/* Get x axis labeling parameters */

void
c_plgxax(PLINT *p_digmax, PLINT *p_digits);

/* Get y axis labeling parameters */

void
c_plgyax(PLINT *p_digmax, PLINT *p_digits);

/* Get z axis labeling parameters */

void
c_plgzax(PLINT *p_digmax, PLINT *p_digits);

/* Draws a histogram of n values of a variable in array data[0..n-1] */

void
c_plhist(PLINT n, PLFLT *data, PLFLT datmin, PLFLT datmax,
	 PLINT nbin, PLINT oldwin);

/* Set current color (map 0) by hue, lightness, and saturation. */

void
c_plhls(PLFLT h, PLFLT l, PLFLT s);

/* Initializes plplot, using preset or default options */

void
c_plinit(void);

/* Draws a line segment from (x1, y1) to (x2, y2). */

void
c_pljoin(PLFLT x1, PLFLT y1, PLFLT x2, PLFLT y2);

/* Simple routine for labelling graphs. */

void
c_pllab(const char *xlabel, const char *ylabel, const char *tlabel);

/* Draws line segments connecting a series of points. */

void
c_plline(PLINT n, PLFLT *x, PLFLT *y);

/* Set line style. */

void
c_pllsty(PLINT lin);

/* Plots a mesh representation of the function z[x][y]. */

void
c_plmesh(PLFLT *x, PLFLT *y, PLFLT **z, PLINT nx, PLINT ny, PLINT opt);

/* Creates a new stream and makes it the default.  */

void
c_plmkstrm(PLINT *p_strm);

/* Prints out "text" at specified position relative to viewport */

void
c_plmtex(const char *side, PLFLT disp, PLFLT pos, PLFLT just,
	 const char *text);

/* Plots a 3-d representation of the function z[x][y]. */

void
c_plot3d(PLFLT *x, PLFLT *y, PLFLT **z,
	 PLINT nx, PLINT ny, PLINT opt, PLINT side);

/* Set fill pattern directly. */

void
c_plpat(PLINT nlin, PLINT *inc, PLINT *del);

/* Plots array y against x for n points using ASCII code "code".*/

void
c_plpoin(PLINT n, PLFLT *x, PLFLT *y, PLINT code);

/* Set the floating point precision (in number of places) in numeric labels. */

void
c_plprec(PLINT setp, PLINT prec);

/* Set fill pattern, using one of the predefined patterns.*/

void
c_plpsty(PLINT patt);

/* Prints out "text" at world cooordinate (x,y). */

void
c_plptex(PLFLT x, PLFLT y, PLFLT dx, PLFLT dy, PLFLT just, const char *text);

/* Replays contents of plot buffer to current device/file. */

void
c_plreplot(void);

/* Set line color by red, green, blue from  0. to 1. */

void
c_plrgb(PLFLT r, PLFLT g, PLFLT b);

/* Set line color by 8 bit RGB values. */

void
c_plrgb1(PLINT r, PLINT g, PLINT b);

/* Obsolete.  Use page driver interface instead. */

void
c_plsasp(PLFLT asp);

/* Set character height. */

void
c_plschr(PLFLT def, PLFLT scale);

/* Set number of colors in cmap 0 */

void
c_plscmap0n(PLINT ncol0);

/* Set number of colors in cmap 1 */

void
c_plscmap1n(PLINT ncol1);

/* Set color map 0 colors by 8 bit RGB values */

void
c_plscmap0(PLINT *r, PLINT *g, PLINT *b, PLINT ncol0);

/* Set color map 1 colors by 8 bit RGB values */

void
c_plscmap1(PLINT *r, PLINT *g, PLINT *b, PLINT ncol1);

/* Set color map 1 colors using a piece-wise linear relationship between */
/* intensity [0,1] (cmap 1 index) and position in HLS or RGB color space. */

void
c_plscmap1l(PLINT itype, PLINT npts, PLFLT *intensity,
	    PLFLT *coord1, PLFLT *coord2, PLFLT *coord3);

/* Set a given color from color map 0 by 8 bit RGB value */

void
c_plscol0(PLINT icol0, PLINT r, PLINT g, PLINT b);

/* Set the background color by 8 bit RGB value */

void
c_plscolbg(PLINT r, PLINT g, PLINT b);

/* Used to globally turn color output on/off */

void
c_plscolor(PLINT color);

/* Set the device (keyword) name */

void
c_plsdev(const char *devname);

/* Set window into device space using margin, aspect ratio, and */
/* justification */

void
c_plsdidev(PLFLT mar, PLFLT aspect, PLFLT jx, PLFLT jy);

/* Set up transformation from metafile coordinates. */

void
c_plsdimap(PLINT dimxmin, PLINT dimxmax, PLINT dimymin, PLINT dimymax,
	   PLFLT dimxpmm, PLFLT dimypmm);

/* Set plot orientation, specifying rotation in units of pi/2. */

void
c_plsdiori(PLFLT rot);

/* Set window into plot space */

void
c_plsdiplt(PLFLT xmin, PLFLT ymin, PLFLT xmax, PLFLT ymax);

/* Set window into plot space incrementally (zoom) */

void
c_plsdiplz(PLFLT xmin, PLFLT ymin, PLFLT xmax, PLFLT ymax);

/* Set the escape character for text strings. */

void
c_plsesc(char esc);

/* Set family file parameters */

void
c_plsfam(PLINT fam, PLINT num, PLINT bmax);

/* Set the output file name. */

void
c_plsfnam(const char *fnam);

/* Shade region. */

void 
c_plshade(PLFLT **a, PLINT nx, PLINT ny, const char *defined,
	  PLFLT left, PLFLT right, PLFLT bottom, PLFLT top,
	  PLFLT shade_min, PLFLT shade_max,
	  PLINT sh_cmap, PLFLT sh_color, PLINT sh_width,
	  PLINT min_color, PLINT min_width,
	  PLINT max_color, PLINT max_width,
	  void (*fill) (PLINT, PLFLT *, PLFLT *), PLINT rectangular,
	  void (*pltr) (PLFLT, PLFLT, PLFLT *, PLFLT *, PLPointer),
	  PLPointer pltr_data);

void 
plshade1(PLFLT *a, PLINT nx, PLINT ny, const char *defined,
	 PLFLT left, PLFLT right, PLFLT bottom, PLFLT top,
	 PLFLT shade_min, PLFLT shade_max,
	 PLINT sh_cmap, PLFLT sh_color, PLINT sh_width,
	 PLINT min_color, PLINT min_width,
	 PLINT max_color, PLINT max_width,
	 void (*fill) (PLINT, PLFLT *, PLFLT *), PLINT rectangular,
	 void (*pltr) (PLFLT, PLFLT, PLFLT *, PLFLT *, PLPointer),
	 PLPointer pltr_data);

void 
plfshade(PLFLT (*f2eval) (PLINT, PLINT, PLPointer),
	 PLPointer f2eval_data,
	 PLFLT (*c2eval) (PLINT, PLINT, PLPointer),
	 PLPointer c2eval_data,
	 PLINT nx, PLINT ny, 
	 PLFLT left, PLFLT right, PLFLT bottom, PLFLT top,
	 PLFLT shade_min, PLFLT shade_max,
	 PLINT sh_cmap, PLFLT sh_color, PLINT sh_width,
	 PLINT min_color, PLINT min_width,
	 PLINT max_color, PLINT max_width,
	 void (*fill) (PLINT, PLFLT *, PLFLT *), PLINT rectangular,
	 void (*pltr) (PLFLT, PLFLT, PLFLT *, PLFLT *, PLPointer),
	 PLPointer pltr_data);

/* I've canned this for now */

void
c_plslpb(PLFLT lpbxmi, PLFLT lpbxma, PLFLT lpbymi, PLFLT lpbyma);

/* Set up lengths of major tick marks. */

void
c_plsmaj(PLFLT def, PLFLT scale);

/* Set up lengths of minor tick marks. */

void
c_plsmin(PLFLT def, PLFLT scale);

/* Set orientation.  Must be done before calling plinit. */

void
c_plsori(PLINT ori);

/* Set output device parameters.  Usually ignored by the driver. */

void
c_plspage(PLFLT xp, PLFLT yp, PLINT xleng, PLINT yleng,
	  PLINT xoff, PLINT yoff);

/* Set the pause (on end-of-page) status */

void
c_plspause(PLINT pause);

/* Set stream number.  */

void
c_plsstrm(PLINT strm);

/* Set the number of subwindows in x and y */

void
c_plssub(PLINT nx, PLINT ny);

/* Set symbol height. */

void
c_plssym(PLFLT def, PLFLT scale);

/* Initialize plplot, passing in the windows/page settings. */

void
c_plstar(PLINT nx, PLINT ny);

/* Initialize plplot, passing the device name and windows/page settings. */

void
c_plstart(const char *devname, PLINT nx, PLINT ny);

/* Set up a new line style */

void
c_plstyl(PLINT nms, PLINT *mark, PLINT *space);

/* Sets the edges of the viewport to the specified absolute coordinates */

void
c_plsvpa(PLFLT xmin, PLFLT xmax, PLFLT ymin, PLFLT ymax);

/* Set x axis labeling parameters */

void
c_plsxax(PLINT digmax, PLINT digits);

/* Set inferior X window */

void
plsxwin(PLINT window_id);

/* Set y axis labeling parameters */

void
c_plsyax(PLINT digmax, PLINT digits);

/* Plots array y against x for n points using Hershey symbol "code" */

void
c_plsym(PLINT n, PLFLT *x, PLFLT *y, PLINT code);

/* Set z axis labeling parameters */

void
c_plszax(PLINT digmax, PLINT digits);

/* Switches to text screen. */

void
c_pltext(void);

/* Sets the edges of the viewport with the given aspect ratio, leaving */
/* room for labels. */

void
c_plvasp(PLFLT aspect);

/* Creates the largest viewport of the specified aspect ratio that fits */
/* within the specified normalized subpage coordinates. */

void
c_plvpas(PLFLT xmin, PLFLT xmax, PLFLT ymin, PLFLT ymax, PLFLT aspect);

/* Creates a viewport with the specified normalized subpage coordinates. */

void
c_plvpor(PLFLT xmin, PLFLT xmax, PLFLT ymin, PLFLT ymax);

/* Defines a "standard" viewport with seven character heights for */
/* the left margin and four character heights everywhere else. */

void
c_plvsta(void);

/* Set up a window for three-dimensional plotting. */

void
c_plw3d(PLFLT basex, PLFLT basey, PLFLT height, PLFLT xmin0,
	PLFLT xmax0, PLFLT ymin0, PLFLT ymax0, PLFLT zmin0,
	PLFLT zmax0, PLFLT alt, PLFLT az);

/* Set pen width. */

void
c_plwid(PLINT width);

/* Set up world coordinates of the viewport boundaries (2d plots). */

void
c_plwind(PLFLT xmin, PLFLT xmax, PLFLT ymin, PLFLT ymax);

	/* The rest for use from C only */

/* Returns a list of file-oriented device names and their menu strings */

void
plgFileDevs(char ***p_menustr, char ***p_devname, int *p_ndev);

/* Set the function pointer for the keyboard event handler */

void
plsKeyEH(void (*KeyEH) (PLKey *, void *, int *), void *KeyEH_data);

/* Sets an optional user exit handler. */

void
plsexit(int (*handler) (char *));

	/* Transformation routines */

/* Identity transformation. */

void
pltr0(PLFLT x, PLFLT y, PLFLT *tx, PLFLT *ty, PLPointer pltr_data);

/* Does linear interpolation from singly dimensioned coord arrays. */

void
pltr1(PLFLT x, PLFLT y, PLFLT *tx, PLFLT *ty, PLPointer pltr_data);

/* Does linear interpolation from doubly dimensioned coord arrays */
/* (column dominant, as per normal C 2d arrays). */

void
pltr2(PLFLT x, PLFLT y, PLFLT *tx, PLFLT *ty, PLPointer pltr_data);

/* Just like pltr2() but uses pointer arithmetic to get coordinates from */
/* 2d grid tables.  */

void
pltr2p(PLFLT x, PLFLT y, PLFLT *tx, PLFLT *ty, PLPointer pltr_data);

/* Identity transformation for plots from Fortran. */

void
pltr0f(PLFLT x, PLFLT y, PLFLT *tx, PLFLT *ty, void *pltr_data);

/* Does linear interpolation from doubly dimensioned coord arrays */
/* (row dominant, i.e. Fortran ordering). */

void
pltr2f(PLFLT x, PLFLT y, PLFLT *tx, PLFLT *ty, void *pltr_data);

/* Example linear transformation function for contour plotter. */

void 
xform(PLFLT x, PLFLT y, PLFLT * tx, PLFLT * ty);

	/* Function evaluators */

/* Does a lookup from a 2d function array.  Array is of type (PLFLT **), */
/* and is column dominant (normal C ordering). */

PLFLT
plf2eval2(PLINT ix, PLINT iy, PLPointer plf2eval_data);

/* Does a lookup from a 2d function array.  Array is of type (PLFLT *), */
/* and is column dominant (normal C ordering). */

PLFLT
plf2eval(PLINT ix, PLINT iy, PLPointer plf2eval_data);

/* Does a lookup from a 2d function array.  Array is of type (PLFLT *), */
/* and is row dominant (Fortran ordering). */

PLFLT
plf2evalr(PLINT ix, PLINT iy, PLPointer plf2eval_data);

	/* Command line parsing utilities */

/* Front-end to Syntax() for external use. */

void
plSyntax(PLINT mode);

/* Front-end to Help() for external use. */

void
plHelp(PLINT mode);

/* Print usage notes. */

void
plNotes(void);

/* Process plplot internal options list */

int
plParseInternalOpts(int *p_argc, char **argv, PLINT mode);

/* Process options list */

int
plParseOpts(int *p_argc, char **argv, PLINT mode, PLOptionTable *option_table,
	    void (*usage_handler) (char *));

/* Process input strings, treating them as an option and argument pair. */

int
plSetInternalOpt(char *opt, char *optarg);

	/* Miscellaneous */

/* Set the output file pointer */

void
plgfile(FILE **p_file);

/* Get the output file pointer */

void
plsfile(FILE *file);

/* Get the escape character for text strings. */

void
plgesc(char *p_esc);

/* Front-end to driver escape function. */

void
pl_cmd(PLINT op, void *ptr);

/* Return full pathname for given file if executable */

int 
plFindName(char *p);

/* Looks for the specified executable file according to usual search path. */

char *
plFindCommand(char *fn);

/* Gets search name for file by concatenating the dir, subdir, and file */
/* name, allocating memory as needed.  */

void
plGetName(char *dir, char *subdir, char *filename, char **filespec);

/* Prompts human to input an integer in response to given message. */

PLINT
plGetInt(char *s);

/* Prompts human to input a float in response to given message. */

PLFLT
plGetFlt(char *s);

	/* Nice way to allocate space for a vectored 2d grid */

/* Allocates a block of memory for use as a 2-d grid of PLFLT's.  */

void
plAlloc2dGrid(PLFLT ***f, PLINT nx, PLINT ny);

/* Frees a block of memory allocated with plAlloc2dGrid(). */

void
plFree2dGrid(PLFLT **f, PLINT nx, PLINT ny);

/* Functions for converting between HLS and RGB color space */

void
plHLS_RGB(PLFLT h, PLFLT l, PLFLT s, PLFLT *p_r, PLFLT *p_g, PLFLT *p_b);

void
plRGB_HLS(PLFLT r, PLFLT g, PLFLT b, PLFLT *p_h, PLFLT *p_l, PLFLT *p_s);

/* plframe widget command */

#if defined(TK) || defined(_TK)
#include <tk.h>
int   plFrameCmd     	(ClientData, Tcl_Interp *, int, char **);
int   Pltk_Init( Tcl_Interp *interp );
#endif

#ifdef __cplusplus
}
#endif

#endif	/* __PLPLOT_H__ */
