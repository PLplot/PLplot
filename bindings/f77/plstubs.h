/* $Id$
   $Log$
   Revision 1.15  1993/08/26 18:29:08  mjl
   Put in fix for name translation under UNICOS.

 * Revision 1.14  1993/07/20  06:50:28  mjl
 * Eliminated obsolete type declarations for string conversions.
 *
 * Revision 1.13  1993/07/16  22:31:41  mjl
 * Changed method for getting stub conversion right.  Now need to define
 * the macro STUB_LINKAGE with any of the supported conversion types.
 *
 * Revision 1.12  1993/07/02  07:26:10  mjl
 * Changed include of plplot.h to plplotP.h.  Also added IRIX support.
 *
 * Revision 1.11  1993/04/26  20:00:55  mjl
 * Configuration info added for a DEC Alpha-based machine running OSF/1.
 *
 * Revision 1.10  1993/03/28  08:45:07  mjl
 * Added support for NEC SX-3.  Also moved determination of stub name handling
 * to plstubs.h (more appropriate).
*/

/*
   plstubs.h

   Maurice LeBrun
   IFS, University of Texas

   Header file for plplot Fortran interface stubs.
   THIS FILE IS NOT FOR INCLUSION BY USER CODES!!

   This software is in the public domain. 
*/

#ifndef INCLUDED_PLSTUBS
#define INCLUDED_PLSTUBS

#include "plplotP.h"

/*----------------------------------------------------------------------*\
* Select name transformation based on system type.
*
* Define the STUB_LINKAGE flag to get proper C<->Fortran linkage on your
* system.  This flag describes what the compiler does to Fortran routine
* names, which we must duplicate on the C stubs so that the Fortran
* routines may call them.  You can often set STUB_LINKAGE by the
* construct -DSTUB_LINKAGE=<value> on the C compiler command line, but
* it is best to either rely on the default or explicitly handle your
* system below.
*
* Current choices for STUB_LINKAGE:
*
* 	STUB_LAU	lower-case, append underscore
* 	STUB_L		lower-case
*	STUB_U		upper-case
*	STUB_FORTRAN	use "fortran" keyword (MS-DOS convention)
*
* If no choice is made, the default is set to STUB_LAU.  This should
* handle most generic Unix boxes not already accounted for.  
*
* ** Namespace collision **
*
* If you use the STUB_L option, the C & Fortran namespaces will collide
* if the Fortran compiler does lower case folding (they usually do).
* The problem is then that the stub names and actual function names will
* be exactly the same, if we insist on the Fortran and C bindings to be
* similar.  The solution is to give the externally callable C routines
* unique names, and provide macros to turn the documented entry names in
* to the real ones.  This is a horrible kludge, but the alternatives are
* worse.  Fortunately it has no effect on the user program, and you can
* forget that you ever read about it here.
\*----------------------------------------------------------------------*/

#define STUB_LAU	1
#define STUB_L		2
#define STUB_U		3
#define STUB_FORTRAN	4

#ifndef STUB_LINKAGE

#if defined(SX)				/* NEC Super-UX (SX-3) */
#define STUB_LINKAGE STUB_LAU
#endif

#if defined(_IBMR2) && defined(_AIX)	/* AIX */
#define STUB_LINKAGE STUB_L
#endif

#ifdef __hpux				/* HP/UX */
#define STUB_LINKAGE STUB_L
#endif

#ifdef __mips				/* IRIX (SGI systems) */
#define STUB_LINKAGE STUB_LAU
#endif

#ifdef sun				/* Suns */
#define STUB_LINKAGE STUB_LAU
#endif

#ifdef CRAY				/* Cray/UNICOS */
#define STUB_LINKAGE STUB_U
#endif

#if defined(__alpha) && defined(__osf__) /* DEC Alpha AXP/OSF */
#define STUB_LINKAGE STUB_LAU
#endif

#ifdef MSDOS				/* MS-DOS based */
#define STUB_LINKAGE STUB_FORTRAN
#endif

#ifndef STUB_LINKAGE			/* The default */
#define STUB_LINKAGE STUB_LAU
#endif

#endif	/* ifndef STUB_LINKAGE */

/*----------------------------------------------------------------------*\
* Define name-translation macro.
* To use, define with x the upper case routine name, y the lower case.
* Should be adoptable to virtually any system.
\*----------------------------------------------------------------------*/

#if STUB_LINKAGE==STUB_LAU
#define FNAME(x,y)	y##_

#elif STUB_LINKAGE == STUB_L
#define FNAME(x,y)	y

#elif STUB_LINKAGE == STUB_U
#define FNAME(x,y)	x

#elif STUB_LINKAGE == STUB_FORTRAN
#define FNAME(x,y)	fortran x

#else
#error "Illegal setting for STUB_LINKAGE"
#endif

/*----------------------------------------------------------------------*\
* Now to actually define the stub names.
* Each stub must have an entry here.
\*----------------------------------------------------------------------*/

#define    PLADV	FNAME(PLADV,pladv)
#define    PLAXES_	FNAME(PLAXES_,plaxes_)
#define    PLBIN	FNAME(PLBIN,plbin)
#define    PLBOX3_	FNAME(PLBOX3_,plbox3_)
#define    PLBOX_	FNAME(PLBOX_,plbox_)
#define    PLCLR	FNAME(PLCLR,plclr)
#define    PLCOL	FNAME(PLCOL,plcol)
#define    PLCON0_	FNAME(PLCON0_,plcon0_)
#define    PLCON1_	FNAME(PLCON1_,plcon1_)
#define    PLCON2_	FNAME(PLCON2_,plcon2_)
#define    PLCONT_	FNAME(PLCONT_,plcont_)
#define    PLEND	FNAME(PLEND,plend)
#define    PLEND1	FNAME(PLEND1,plend1)
#define    PLENV	FNAME(PLENV,plenv)
#define    PLERRX	FNAME(PLERRX,plerrx)
#define    PLERRY	FNAME(PLERRY,plerry)
#define    PLFAMADV	FNAME(PLFAMADV,plfamadv)
#define    PLFILL	FNAME(PLFILL,plfill)
#define    PLFLUSH	FNAME(PLFLUSH,plflush)
#define    PLFONT	FNAME(PLFONT,plfont)
#define    PLFONTLD	FNAME(PLFONTLD,plfontld)
#define    PLGCHR	FNAME(PLGCHR,plgchr)
#define    PLGFAM	FNAME(PLGFAM,plgfam)
#define    PLGFNAM_	FNAME(PLGFNAM_,plgfnam_)
#define    PLGPAGE	FNAME(PLGPAGE,plgpage)
#define    PLGRA	FNAME(PLGRA,plgra)
#define    PLGSPA	FNAME(PLGSPA,plgspa)
#define    PLGSTRM	FNAME(PLGSTRM,plgstrm)
#define    PLGVER_	FNAME(PLGVER_,plgver_)
#define    PLGXAX	FNAME(PLGXAX,plgxax)
#define    PLGYAX	FNAME(PLGYAX,plgyax)
#define    PLGZAX	FNAME(PLGZAX,plgzax)
#define    PLHIST	FNAME(PLHIST,plhist)
#define    PLHLS        FNAME(PLHLS,plhls)
#define    PLJOIN	FNAME(PLJOIN,pljoin)
#define    PLLAB_	FNAME(PLLAB_,pllab_)
#define    PLLINE	FNAME(PLLINE,plline)
#define    PLLSTY	FNAME(PLLSTY,pllsty)
#define    PLMESH	FNAME(PLMESH,plmesh)
#define    PLMTEX_	FNAME(PLMTEX_,plmtex_)
#define    PLOT3D	FNAME(PLOT3D,plot3d)
#define    PLPAT	FNAME(PLPAT,plpat)
#define    PLPOIN	FNAME(PLPOIN,plpoin)
#define    PLPREC	FNAME(PLPREC,plprec)
#define    PLPSTY	FNAME(PLPSTY,plpsty)
#define    PLPTEX_	FNAME(PLPTEX_,plptex_)
#define    PLRGB        FNAME(PLRGB,plrgb)
#define    PLRGB1	FNAME(PLRGB1,plrgb1)
#define    PLSASP       FNAME(PLSASP,plsasp)
#define    PLSCHR	FNAME(PLSCHR,plschr)
#define    PLSCM0	FNAME(PLSCM0,plscm0)
#define    PLSCM0N	FNAME(PLSCM0N,plscm0n)
#define    PLSCM1	FNAME(PLSCM1,plscm1)
#define    PLSCM1F1	FNAME(PLSCM1F1,plscm1f1)
#define    PLSCOL0	FNAME(PLSCOL0,plscol0)
#define    PLSCOLOR	FNAME(PLSCOLOR,plscolor)
#define    PLSDEV_	FNAME(PLSDEV_,plsdev_)
#define    PLSESC	FNAME(PLSESC,plsesc)
#define    PLSFAM	FNAME(PLSFAM,plsfam)
#define    PLSFNAM_	FNAME(PLSFNAM_,plsfnam_)
#define    PLSMAJ	FNAME(PLSMAJ,plsmaj)
#define    PLSMIN	FNAME(PLSMIN,plsmin)
#define    PLSORI	FNAME(PLSORI,plsori)
#define    PLSPAGE	FNAME(PLSPAGE,plspage)
#define    PLSSTRM	FNAME(PLSSTRM,plsstrm)
#define    PLSSUB	FNAME(PLSSUB,plssub)
#define    PLSSYM	FNAME(PLSSYM,plssym)
#define    PLSTAR	FNAME(PLSTAR,plstar)
#define    PLSTART_	FNAME(PLSTART_,plstart_)
#define    PLSTYL	FNAME(PLSTYL,plstyl)
#define    PLSVPA	FNAME(PLSVPA,plsvpa)
#define    PLSXAX	FNAME(PLSXAX,plsxax)
#define    PLSYAX	FNAME(PLSYAX,plsyax)
#define    PLSYM	FNAME(PLSYM,plsym)
#define    PLSZAX	FNAME(PLSZAX,plszax)
#define    PLTEXT	FNAME(PLTEXT,pltext)
#define    PLVASP	FNAME(PLVASP,plvasp)
#define    PLVPAS	FNAME(PLVPAS,plvpas)
#define    PLVPOR	FNAME(PLVPOR,plvpor)
#define    PLVSTA	FNAME(PLVSTA,plvsta)
#define    PLW3D	FNAME(PLW3D,plw3d)
#define    PLWID	FNAME(PLWID,plwid)
#define    PLWIND	FNAME(PLWIND,plwind)

#endif	/* INCLUDED_PLSTUBS */
