/* $Id$
 * $Log$
 * Revision 1.24  1995/10/23 07:23:11  mjl
 * Added glevel() f77 binding.
 *
 * Revision 1.23  1995/10/16  18:22:23  mjl
 * Added support for plgdev API function in fortran bindings.
 *
 * Revision 1.22  1994/07/20  06:07:44  mjl
 * Added definitions for Fortran interface routines to handle the new 3d
 * functions plline3(), plpoin3(), and plpoly3().
 *
 * Revision 1.21  1994/03/23  07:11:49  mjl
 * Added stub name definitions for cmap1 selectors and plshade front-ends.
*/

/*
   plstubs.h

   Maurice LeBrun
   IFS, University of Texas

   Header file for plplot Fortran interface stubs.
   THIS FILE IS NOT FOR INCLUSION BY USER CODES!!

   The contents of this file are in the public domain. 
*/

#ifndef __PLSTUBS_H__
#define __PLSTUBS_H__

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

#ifdef __GO32__				/* dos386/djgpp */
#ifdef MSDOS
#undef MSDOS
#endif
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
#define    PLAXES7	FNAME(PLAXES7,plaxes7)
#define    PLBIN	FNAME(PLBIN,plbin)
#define    PLBOP	FNAME(PLBOP,plbop)
#define    PLBOX37	FNAME(PLBOX37,plbox37)
#define    PLBOX7	FNAME(PLBOX7,plbox7)
#define    PLCLR	FNAME(PLCLR,plclr)
#define    PLCOL	FNAME(PLCOL,plcol)
#define    PLCOL0	FNAME(PLCOL0,plcol0)
#define    PLCOL1	FNAME(PLCOL1,plcol1)
#define    PLCON07	FNAME(PLCON07,plcon07)
#define    PLCON17	FNAME(PLCON17,plcon17)
#define    PLCON27	FNAME(PLCON27,plcon27)
#define    PLCONT7	FNAME(PLCONT7,plcont7)
#define    PLEND	FNAME(PLEND,plend)
#define    PLEND1	FNAME(PLEND1,plend1)
#define    PLENV	FNAME(PLENV,plenv)
#define    PLEOP	FNAME(PLEOP,pleop)
#define    PLERRX	FNAME(PLERRX,plerrx)
#define    PLERRY	FNAME(PLERRY,plerry)
#define    PLFAMADV	FNAME(PLFAMADV,plfamadv)
#define    PLFILL	FNAME(PLFILL,plfill)
#define    PLFLUSH	FNAME(PLFLUSH,plflush)
#define    PLFONT	FNAME(PLFONT,plfont)
#define    PLFONTLD	FNAME(PLFONTLD,plfontld)
#define    PLGCHR	FNAME(PLGCHR,plgchr)
#define    PLGDEV7	FNAME(PLGDEV7,plgdev7)
#define    PLGFAM	FNAME(PLGFAM,plgfam)
#define    PLGFNAM7	FNAME(PLGFNAM7,plgfnam7)
#define    PLGLEVEL	FNAME(PLGLEVEL,plglevel)
#define    PLGPAGE	FNAME(PLGPAGE,plgpage)
#define    PLGRA	FNAME(PLGRA,plgra)
#define    PLGSPA	FNAME(PLGSPA,plgspa)
#define    PLGSTRM	FNAME(PLGSTRM,plgstrm)
#define    PLGVER7	FNAME(PLGVER7,plgver7)
#define    PLGXAX	FNAME(PLGXAX,plgxax)
#define    PLGYAX	FNAME(PLGYAX,plgyax)
#define    PLGZAX	FNAME(PLGZAX,plgzax)
#define    PLHIST	FNAME(PLHIST,plhist)
#define    PLHLS        FNAME(PLHLS,plhls)
#define    PLINIT       FNAME(PLINIT,plinit)
#define    PLJOIN	FNAME(PLJOIN,pljoin)
#define    PLLAB7	FNAME(PLLAB7,pllab7)
#define    PLLINE	FNAME(PLLINE,plline)
#define    PLLINE3	FNAME(PLLINE3,plline3)
#define    PLLSTY	FNAME(PLLSTY,pllsty)
#define    PLMESH	FNAME(PLMESH,plmesh)
#define    PLMTEX7	FNAME(PLMTEX7,plmtex7)
#define    PLOT3D	FNAME(PLOT3D,plot3d)
#define    PLPAT	FNAME(PLPAT,plpat)
#define    PLPOIN	FNAME(PLPOIN,plpoin)
#define    PLPOIN3	FNAME(PLPOIN3,plpoin3)
#define    PLPOLY3	FNAME(PLPOLY3,plpoly3)
#define    PLPREC	FNAME(PLPREC,plprec)
#define    PLPSTY	FNAME(PLPSTY,plpsty)
#define    PLPTEX7	FNAME(PLPTEX7,plptex7)
#define    PLRGB        FNAME(PLRGB,plrgb)
#define    PLRGB1	FNAME(PLRGB1,plrgb1)
#define    PLSASP       FNAME(PLSASP,plsasp)
#define    PLSCHR	FNAME(PLSCHR,plschr)
#define    PLSCMAP0	FNAME(PLSCMAP0,plscmap0)
#define    PLSCMAP1	FNAME(PLSCMAP1,plscmap1)
#define    PLSCMAP0N	FNAME(PLSCMAP0N,plscmap0n)
#define    PLSCMAP1L	FNAME(PLSCMAP1L,plscmap1l)
#define    PLSCOL0	FNAME(PLSCOL0,plscol0)
#define    PLSCOLBG	FNAME(PLSCOLBG,plscolbg)
#define    PLSCOLOR	FNAME(PLSCOLOR,plscolor)
#define    PLSDEV7	FNAME(PLSDEV7,plsdev7)
#define    PLSESC	FNAME(PLSESC,plsesc)
#define    PLSFAM	FNAME(PLSFAM,plsfam)
#define    PLSFNAM7	FNAME(PLSFNAM7,plsfnam7)
#define    PLSHADE07	FNAME(PLSHADE07,plshade07)
#define    PLSHADE17	FNAME(PLSHADE17,plshade17)
#define    PLSHADE27	FNAME(PLSHADE27,plshade27)
#define    PLSMAJ	FNAME(PLSMAJ,plsmaj)
#define    PLSMIN	FNAME(PLSMIN,plsmin)
#define    PLSORI	FNAME(PLSORI,plsori)
#define    PLSPAGE	FNAME(PLSPAGE,plspage)
#define    PLSPAUSE	FNAME(PLSPAUSE,plspause)
#define    PLSSTRM	FNAME(PLSSTRM,plsstrm)
#define    PLSSUB	FNAME(PLSSUB,plssub)
#define    PLSSYM	FNAME(PLSSYM,plssym)
#define    PLSTAR	FNAME(PLSTAR,plstar)
#define    PLSTART7	FNAME(PLSTART7,plstart7)
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

#endif	/* __PLSTUBS_H__ */
