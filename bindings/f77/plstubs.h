/* $Id$
   $Log$
   Revision 1.12  1993/07/02 07:26:10  mjl
   Changed include of plplot.h to plplotP.h.  Also added IRIX support.

 * Revision 1.11  1993/04/26  20:00:55  mjl
 * Configuration info added for a DEC Alpha-based machine running OSF/1.
 *
 * Revision 1.10  1993/03/28  08:45:07  mjl
 * Added support for NEC SX-3.  Also moved determination of stub name handling
 * to plstubs.h (more appropriate).
 *
 * Revision 1.9  1993/03/02  19:00:21  mjl
 * Added prototype for plgver() and its stub name def'n.
 *
 * Revision 1.8  1993/02/23  04:59:00  mjl
 * Added and deleted some function prototypes.
 *
 * Revision 1.7  1993/01/23  05:39:55  mjl
 * Changes in function prototypes to reflect reorganization, new color
 * model support.
 *
 * Revision 1.5  1992/10/27  22:49:49  mjl
 * Reduced stub routine definition to ONE set of defines through a clever
 * macro, FNAME.  Will work in principle for all systems.
 *
 * Revision 1.4  1992/10/27  22:14:02  mjl
 * Support for plflush() function.
 *
 * Revision 1.3  1992/10/22  17:05:19  mjl
 * Fixed warnings, errors generated when compling with HP C++.
 *
 * Revision 1.2  1992/10/20  20:14:12  mjl
 * Added prototypes, definitions for new routine plfamadv(), for advancing
 * to next family member file.
 *
 * Revision 1.1  1992/05/20  21:33:44  furnish
 * Initial checkin of the whole PLPLOT project.
 *
*/

/*
  plstubs.h

  Header file for plplot Fortran interface stubs.
  THIS FILE IS NOT FOR INCLUSION BY USER CODES!!

  Define one of the STUB_ flags to get proper C<->Fortran linkage
  on your system.  The STUB_ flag describes what the compiler does to
  Fortran routine names, which we must duplicate on the C stubs so that
  the Fortran routines may call them.

  Current choices:
 	STUB_LAU	lower-case, append underscore
 	STUB_L		lower-case

  If no choice is made, stubs remain unaltered (upper case).

  ** Namespace collision **

  If you use the STUB_L option, the C & Fortran namespaces will collide if
  the Fortran compiler does lower case folding (they usually do).  On VMS the
  C compiler does lower case folding as well (yechh), which means you can
  just leave the stub routine names alone.  The problem is then that the
  stub names and actual function names will be exactly the same, if we
  insist on the Fortran and C bindings to be similar.  The solution is
  to give the externally callable C routines unique names, and provide
  macros to turn the documented entry names in to the real ones.  This
  is a horrible kludge, but the alternatives are worse.  Fortunately it
  has no effect on the user program, and you can forget that you ever
  read about it here.
*/

#ifndef INCLUDED_PLSTUBS
#define INCLUDED_PLSTUBS

#include "plplotP.h"

/* Select name transformation based on system type */

#if defined(SX)				/* NEC Super-UX */
#define STUB_LAU
#endif

#if defined(_IBMR2) && defined(_AIX)	/* AIX */
#define STUB_L
#endif

#ifdef __hpux				/* HP/UX */
#define STUB_L
#endif

#ifdef __mips				/* IRIX (SGI systems) */
#define STUB_LAU
#endif

#ifdef sun				/* SUN systems */
#define STUB_LAU
#endif

#ifdef CRAY				/* CRAY's */
#endif					/* Do nothing -- keep uppercase */

#if defined(__alpha) && defined(__osf__)	/* DEC Alpha AXP/OSF */
#ifndef STUB_LAU
#define STUB_LAU
#endif
#endif

/* These are needed for system-independent Fortran->C string linkage. */

extern char cstring1[300], cstring2[300], cstring3[300];
extern char cstring4[300], cstring5[300], cstring6[300];

/* To use, define with x the upper case routine name, y the lower case */
/* Should be adoptable to virtually any system */

#ifdef STUB_LAU
#define FNAME(x,y)     y##_
#endif

#ifdef STUB_L
#define FNAME(x,y)     y
#endif

#ifdef MSDOS
#define FNAME(x,y)     fortran x
#endif

#ifdef FNAME

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

#endif	/* FNAME */

#endif	/* INCLUDED_PLSTUBS */
