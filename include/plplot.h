/* $Id$
   $Log$
   Revision 1.2  1992/07/31 06:05:16  mjl
   Added prototype for new function used in contour plots (pltrf0).

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

#ifndef INCLUDED_PLPLOT
#define INCLUDED_PLPLOT

#define _POSIX_SOURCE

#include "chdr.h"

/* change from chdr.h conventions to plplot ones */

typedef FLOAT PLFLT;
typedef INT   PLINT;
#define PLARGS(a) PROTO(a)

#ifdef USE_STDC
#define PLSTDC
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

/* Take care of malloc.h problem -- define INCLUDE_MALLOC if your system
   requires it.  From "C -- The Pocket Reference" by Herbert Schildt (1988):

  "The proposed ANSI standard specifies that the header information necessary
   to the dynamic allocation system will be in stdlib.h.  However, at the time
   of this writing, a great many C compilers require you to include the header
   malloc.h instead."
*/

/* Define INCLUDE_STDDEF to include stddef.h where necessary.  This is needed
   on some systems to get the size_t definition.  On others (non-POSIX) it
   causes it to be defined twice.
*/

#ifdef AMIGA
#define INCLUDE_STDDEF
#endif

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

void c_pladv	PLARGS((PLINT));

void c_plancol	PLARGS((PLINT, char *));

void c_plaxes	PLARGS((PLFLT, PLFLT, char *, PLFLT, PLINT, char *, \
			PLFLT, PLINT));

void c_plbeg	PLARGS((PLINT, PLINT, PLINT));

void c_plbin	PLARGS((PLINT, PLFLT *, PLFLT *, PLINT));

void c_plbox	PLARGS((char *, PLFLT, PLINT, char *, PLFLT, PLINT));

void c_plbox3	PLARGS((char *, char *, PLFLT, PLINT, \
			char *, char *, PLFLT, PLINT, \
			char *, char *, PLFLT, PLINT));

void c_plclr	PLARGS((void));

void c_plcol	PLARGS((PLINT));

void c_plconf	PLARGS((PLFLT **, PLINT, PLINT, PLINT, PLINT, \
			PLINT, PLINT, PLFLT *, PLINT, \
			void (*)(PLFLT, PLFLT, PLFLT *, PLFLT *, 
				 PLFLT *, PLFLT *, PLINT, PLINT), 
			PLFLT *, PLFLT *));

void c_plcont	PLARGS((PLFLT **, PLINT, PLINT, PLINT, PLINT, \
			PLINT, PLINT, PLFLT *, PLINT, \
			void (*)(PLFLT, PLFLT, PLFLT *, PLFLT *)));

void c_plend	PLARGS((void));

void c_plend1	PLARGS((void));

void c_plenv	PLARGS((PLFLT, PLFLT, PLFLT, PLFLT, PLINT, PLINT));

void c_plerrx	PLARGS((PLINT, PLFLT *, PLFLT *, PLFLT *));

void c_plerry	PLARGS((PLINT, PLFLT *, PLFLT *, PLFLT *));

void c_plfill	PLARGS((PLINT, PLFLT *, PLFLT *));

void c_plfont	PLARGS((PLINT));

void c_plfontld	PLARGS((PLINT));

void c_plgfam	PLARGS(( PLINT *, PLINT *, PLINT *));

void c_plgfnam	PLARGS((char *));

void c_plgpage 	PLARGS((PLFLT *, PLFLT *, PLINT *, PLINT *, PLINT *, PLINT *));

void c_plgra	PLARGS((void));

void c_plgspa	PLARGS((PLFLT *, PLFLT *, PLFLT *, PLFLT *));

void c_plgstrm	PLARGS((PLINT *));

void c_plgxax	PLARGS((PLINT *, PLINT *));

void c_plgyax	PLARGS((PLINT *, PLINT *));

void c_plgzax	PLARGS((PLINT *, PLINT *));

void c_plhist	PLARGS((PLINT, PLFLT *, PLFLT, PLFLT, PLINT, PLINT));

void c_plhls	PLARGS((PLFLT, PLFLT, PLFLT));

void c_pljoin	PLARGS((PLFLT, PLFLT, PLFLT, PLFLT));

void c_pllab	PLARGS((char *, char *, char *));

void c_plline	PLARGS((PLINT, PLFLT *, PLFLT *));

void c_pllsty	PLARGS((PLINT));

void c_plmesh	PLARGS((PLFLT *, PLFLT *, PLFLT **, \
			PLINT, PLINT, PLINT));

void c_plmtex	PLARGS((char *, PLFLT, PLFLT, PLFLT, char *));

void c_plot3d	PLARGS((PLFLT *, PLFLT *, PLFLT **, \
			PLINT, PLINT, PLINT, PLINT));

void c_plpage	PLARGS((void));

void c_plpat	PLARGS((PLINT, PLINT *, PLINT *));

void c_plpoin	PLARGS((PLINT, PLFLT *, PLFLT *, PLINT));

void c_plprec	PLARGS((PLINT, PLINT));

void c_plpsty	PLARGS((PLINT));

void c_plptex	PLARGS((PLFLT, PLFLT, PLFLT, PLFLT, PLFLT, char *));

void c_plrgb	PLARGS((PLFLT, PLFLT, PLFLT));

void c_plschr	PLARGS((PLFLT, PLFLT));

void c_plsfam	PLARGS(( PLINT, PLINT, PLINT));

void c_plsfnam	PLARGS((char *));

void c_plsasp	PLARGS((PLFLT));

void c_plslpb	PLARGS((PLFLT, PLFLT, PLFLT, PLFLT));

void c_plsmaj	PLARGS((PLFLT, PLFLT));

void c_plsmin	PLARGS((PLFLT, PLFLT));

void c_plsori	PLARGS((PLINT));

void c_plspage 	PLARGS((PLFLT, PLFLT, PLINT, PLINT, PLINT, PLINT));

void c_plspause	PLARGS((PLINT));

void c_plsstrm	PLARGS((PLINT));

void c_plssym	PLARGS((PLFLT, PLFLT));

void c_plstar	PLARGS((PLINT, PLINT));

void c_plstart	PLARGS((char *, PLINT, PLINT));

void c_plstyl	PLARGS((PLINT, PLINT *, PLINT *));

void c_plsvpa	PLARGS((PLFLT, PLFLT, PLFLT, PLFLT));

void c_plsxax	PLARGS((PLINT, PLINT));

void c_plsyax	PLARGS((PLINT, PLINT));

void c_plsym	PLARGS((PLINT, PLFLT *, PLFLT *, PLINT));

void c_plszax	PLARGS((PLINT, PLINT));

void c_pltext	PLARGS((void));

void c_plvasp	PLARGS((PLFLT));

void c_plvpas	PLARGS((PLFLT, PLFLT, PLFLT, PLFLT, PLFLT));

void c_plvpor	PLARGS((PLFLT, PLFLT, PLFLT, PLFLT));

void c_plvsta	PLARGS((void));

void c_plw3d	PLARGS((PLFLT, PLFLT, PLFLT, PLFLT, \
			PLFLT, PLFLT, PLFLT, PLFLT, \
			PLFLT, PLFLT, PLFLT));

void c_plwend	PLARGS((void));

void c_plwid	PLARGS((PLINT));

void c_plwind	PLARGS((PLFLT, PLFLT, PLFLT, PLFLT));

	/* The rest for use from C only */

	/* Contour plotters */

void  pltr0	PLARGS((PLFLT, PLFLT, PLFLT *, PLFLT *));

void  pltr1	PLARGS((PLFLT, PLFLT, PLFLT *, PLFLT *, \
			PLFLT *, PLFLT *, PLINT, PLINT));

void  pltr2	PLARGS((PLFLT, PLFLT, PLFLT *, PLFLT *, \
			PLFLT *, PLFLT *, PLINT, PLINT));

void  pltr0f	PLARGS((PLFLT x, PLFLT y, PLFLT * tx, PLFLT * ty));

void  pltr2f	PLARGS((PLFLT, PLFLT, PLFLT *, PLFLT *, \
			PLFLT *, PLFLT *, PLINT, PLINT));

void  plcntr	PLARGS((PLFLT **, PLINT, PLINT, PLINT, PLINT, \
			PLINT, PLINT, PLFLT, PLINT *, \
			PLINT *, PLINT *, PLINT, \
			void (*)(PLFLT, PLFLT, PLFLT *, PLFLT *)));

void  plcntf	PLARGS((PLFLT **, PLINT, PLINT, PLINT, PLINT,
			PLINT, PLINT, PLFLT, PLINT *,
			PLINT *, PLINT *, PLINT,
			void (*)(PLFLT, PLFLT, PLFLT *, PLFLT *, 
				 PLFLT *, PLFLT *, PLINT, PLINT), 
			PLFLT *, PLFLT *));

	/* These should not be called directly by the user */

void  plccal	PLARGS((PLFLT **, PLINT, PLINT, PLFLT, \
			PLINT, PLINT, PLINT, PLINT, PLFLT *));

void  pldeco	PLARGS((short **, PLINT *, char *));

void  pldtik	PLARGS((PLFLT, PLFLT, PLFLT *, PLINT *, \
			PLINT *, PLINT *, PLINT, PLINT *));

void  plerx1	PLARGS((PLFLT, PLFLT, PLFLT));

void  plery1	PLARGS((PLFLT, PLFLT, PLFLT));

void  plexit	PLARGS((char *));

void  plfontrel	PLARGS((void));

void  plform	PLARGS((PLFLT, PLINT, PLINT, char *));

void  plhrsh	PLARGS((PLINT, PLINT, PLINT));

void  pllclp	PLARGS((PLINT, PLINT, PLINT, PLINT));

void  plpoi1	PLARGS((PLFLT, PLFLT, PLINT));

void  plr135	PLARGS((PLINT *, PLINT *, PLINT));

void  plr45	PLARGS((PLINT *, PLINT *, PLINT));

void  plsetup	PLARGS((PLFLT, PLFLT, PLINT, PLINT));

void  plstik	PLARGS((PLFLT, PLFLT, PLFLT, PLFLT));

void  plstr	PLARGS((PLINT, PLFLT *, PLINT, PLINT, char *));

void  plsym1	PLARGS((PLFLT, PLFLT, PLINT));

void  plxtik	PLARGS((PLINT, PLINT, PLINT, PLINT));

void  plxybx	PLARGS((char *, char *, PLFLT, PLFLT, PLFLT, PLFLT, \
			PLFLT, PLFLT, PLFLT, PLINT, PLINT, PLINT *));

void  plytik	PLARGS((PLINT, PLINT, PLINT, PLINT));

void  plzbx	PLARGS((char *, char *, PLINT, PLFLT, PLFLT, PLFLT, \
			PLFLT, PLFLT, PLFLT, PLFLT, PLFLT, PLINT, PLINT *));

void  plgrid3a	PLARGS((PLFLT));

void  plnxtv	PLARGS((PLINT *, PLINT *, PLINT, PLINT));

void  plside3a	PLARGS((PLFLT *, PLFLT *, PLFLT **, PLINT, PLINT, PLINT));

void  plt3zz	PLARGS((PLINT, PLINT, PLINT, PLINT, \
			PLINT, PLINT, PLFLT *, PLFLT *, PLFLT **, \
			PLINT, PLINT, PLINT *, PLINT *));

void  glev	PLARGS((PLINT *));

void  slev	PLARGS((PLINT));

void  gbase	PLARGS((PLFLT *, PLFLT *, PLFLT *, PLFLT *));

void  sbase	PLARGS((PLFLT, PLFLT, PLFLT, PLFLT));

void  gnms	PLARGS((PLINT *));

void  snms	PLARGS((PLINT));

void  gdev	PLARGS((PLINT *, PLINT *, PLINT *));

void  sdev	PLARGS((PLINT, PLINT, PLINT));

void  smod	PLARGS((PLINT));

void  gcurr	PLARGS((PLINT *, PLINT *));

void  scurr	PLARGS((PLINT, PLINT));

void  gdom	PLARGS((PLFLT *, PLFLT *, PLFLT *, PLFLT *));

void  sdom	PLARGS((PLFLT, PLFLT, PLFLT, PLFLT));

void  grange	PLARGS((PLFLT *, PLFLT *, PLFLT *));

void  srange	PLARGS((PLFLT, PLFLT, PLFLT));

void  gw3wc	PLARGS((PLFLT *, PLFLT *, PLFLT *, PLFLT *, PLFLT *));

void  sw3wc	PLARGS((PLFLT, PLFLT, PLFLT, PLFLT, PLFLT));

void  gvpp	PLARGS((PLINT *, PLINT *, PLINT *, PLINT *));

void  svpp	PLARGS((PLINT, PLINT, PLINT, PLINT));

void  gspp	PLARGS((PLINT *, PLINT *, PLINT *, PLINT *));

void  sspp	PLARGS((PLINT, PLINT, PLINT, PLINT));

void  gclp	PLARGS((PLINT *, PLINT *, PLINT *, PLINT *));

void  sclp	PLARGS((PLINT, PLINT, PLINT, PLINT));

void  gphy	PLARGS((PLINT *, PLINT *, PLINT *, PLINT *));

void  sphy	PLARGS((PLINT, PLINT, PLINT, PLINT));

void  gsub	PLARGS((PLINT *, PLINT *, PLINT *));

void  ssub	PLARGS((PLINT, PLINT, PLINT));

void  gumpix	PLARGS((PLINT *, PLINT *));

void  sumpix	PLARGS((PLINT, PLINT));

void  gatt	PLARGS((PLINT *, PLINT *));

void  satt	PLARGS((PLINT, PLINT));

void  gcol	PLARGS((PLINT *));

void  scol	PLARGS((PLINT));

void  gwid	PLARGS((PLINT *));

void  swid	PLARGS((PLINT));

void  gspd	PLARGS((PLFLT *, PLFLT *, PLFLT *, PLFLT *));

void  sspd	PLARGS((PLFLT, PLFLT, PLFLT, PLFLT));

void  gvpd	PLARGS((PLFLT *, PLFLT *, PLFLT *, PLFLT *));

void  svpd	PLARGS((PLFLT, PLFLT, PLFLT, PLFLT));

void  gvpw	PLARGS((PLFLT *, PLFLT *, PLFLT *, PLFLT *));

void  svpw	PLARGS((PLFLT, PLFLT, PLFLT, PLFLT));

void  gpixmm	PLARGS((PLFLT *, PLFLT *));

void  spixmm	PLARGS((PLFLT, PLFLT));

void  setpxl	PLARGS((PLFLT, PLFLT));

void  gwp	PLARGS((PLFLT *, PLFLT *, PLFLT *, PLFLT *));

void  swp	PLARGS((PLFLT, PLFLT, PLFLT, PLFLT));

void  gwm	PLARGS((PLFLT *, PLFLT *, PLFLT *, PLFLT *));

void  swm	PLARGS((PLFLT, PLFLT, PLFLT, PLFLT));

void  gdp	PLARGS((PLFLT *, PLFLT *, PLFLT *, PLFLT *));

void  sdp	PLARGS((PLFLT, PLFLT, PLFLT, PLFLT));

void  gmp	PLARGS((PLFLT *, PLFLT *, PLFLT *,PLFLT *));

void  smp	PLARGS((PLFLT, PLFLT, PLFLT, PLFLT));

void  gchr	PLARGS((PLFLT *, PLFLT *));

void  schr	PLARGS((PLFLT, PLFLT));

void  gsym	PLARGS((PLFLT *, PLFLT *));

void  ssym	PLARGS((PLFLT, PLFLT));

void  gmaj	PLARGS((PLFLT *, PLFLT *));

void  smaj	PLARGS((PLFLT, PLFLT));

void  gmin	PLARGS((PLFLT *, PLFLT *));

void  smin	PLARGS((PLFLT, PLFLT));

void  gzback	PLARGS((PLINT **, PLINT **, PLFLT **));

void  spl3mode	PLARGS((PLINT));

void  spl3upv	PLARGS((PLINT));

void  goldhivw	PLARGS((PLINT **));

void  goldlovw	PLARGS((PLINT **));

void  cleanup	PLARGS((void));

void  genlin	PLARGS((PLINT, PLINT, PLINT, PLINT));

void  movphy	PLARGS((PLINT, PLINT));

void  draphy	PLARGS((PLINT, PLINT));

void  movwor	PLARGS((PLFLT, PLFLT));

void  drawor	PLARGS((PLFLT, PLFLT));

void  setphy	PLARGS((PLINT, PLINT, PLINT, PLINT));

void  xform	PLARGS((PLFLT, PLFLT, PLFLT *, PLFLT *));

	/* Functions that return floats */

PLFLT plstrl	PLARGS((char *));

	/* Stuff in convrt.c */

PLFLT dcmmx	PLARGS((PLFLT));

PLFLT dcmmy	PLARGS((PLFLT));

PLFLT dcscx	PLARGS((PLFLT));

PLFLT dcscy	PLARGS((PLFLT));

PLFLT mmdcx	PLARGS((PLFLT));

PLFLT mmdcy	PLARGS((PLFLT));

PLFLT scdcx	PLARGS((PLFLT));

PLFLT scdcy	PLARGS((PLFLT));

PLFLT wcmmx	PLARGS((PLFLT));

PLFLT wcmmy	PLARGS((PLFLT));

PLFLT w3wcx	PLARGS((PLFLT, PLFLT, PLFLT));

PLFLT w3wcy	PLARGS((PLFLT, PLFLT, PLFLT));

	/* Functions returning PLINTs */

PLINT plcvec	PLARGS((PLINT, SCHAR **));

PLINT stindex	PLARGS((char *, char *));

PLINT strpos	PLARGS((char *, int));

PLINT stsearch	PLARGS((char *, int));

	/* More stuff from convrt.c */

PLINT dcpcx	PLARGS((PLFLT));

PLINT dcpcy	PLARGS((PLFLT));

PLINT mmpcx	PLARGS((PLFLT));

PLINT mmpcy	PLARGS((PLFLT));

PLINT wcpcx	PLARGS((PLFLT));

PLINT wcpcy	PLARGS((PLFLT));

	/* Driver calls */

void grsetup	PLARGS((void));

void grorient	PLARGS((void));

void grinit	PLARGS((void));

void grline	PLARGS((PLINT, PLINT, PLINT, PLINT));

void grclear	PLARGS((void));

void grpage	PLARGS((void));

void grtidy	PLARGS((void));

void grcolor	PLARGS((void));

void grtext	PLARGS((void));

void grgraph	PLARGS((void));

void grwidth	PLARGS((void));

void gresc	PLARGS((PLINT, char *));

#ifdef __cplusplus
}
#endif

#endif	/* INCLUDED_PLPLOT */
