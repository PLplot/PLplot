/* $Id$
   $Log$
   Revision 1.6  1992/12/02 21:49:54  furnish
   Better Linux support.  Note that FORTRAN support is now orchestrated
   through f2c which has funny handling of names with trailing _.

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

#include "plplot.h"

/* These are needed for system-independent Fortran->C string linkage. */

extern char cstring1[300], cstring2[300], cstring3[300];
extern char cstring4[300], cstring5[300], cstring6[300];

/* To use, define with x the upper case routine name, y the lower case */
/* Should be adoptable to virtually any system */

#ifdef STUB_LAU
#define FNAME(x,y)     y##_
#define FNAME_(x,y)     y##_
#endif

#ifdef STUB_L
#define FNAME(x,y)     y
#define FNAME_(x,y)     y
#endif

#ifdef MSDOS
#define FNAME(x,y)     fortran x
#define FNAME_(x,y)     fortran x
#endif

#ifdef STUB_F2C
#ifdef FNAME_
#undef FNAME_
#endif
#define FNAME_(x,y)    y##__
#endif

#ifdef FNAME

#define    PLADV	FNAME(PLADV,pladv)
#define    PLANCOL_	FNAME_(PLANCOL_,plancol_)
#define    PLAXES_	FNAME_(PLAXES_,plaxes_)
#define    PLBEG	FNAME(PLBEG,plbeg)
#define    PLBIN	FNAME(PLBIN,plbin)
#define    PLBOX3_	FNAME_(PLBOX3_,plbox3_)
#define    PLBOX_	FNAME_(PLBOX_,plbox_)
#define    PLCLR	FNAME(PLCLR,plclr)
#define    PLCOL	FNAME(PLCOL,plcol)
#define    PLCON0_	FNAME_(PLCON0_,plcon0_)
#define    PLCON1_	FNAME_(PLCON1_,plcon1_)
#define    PLCON2_	FNAME_(PLCON2_,plcon2_)
#define    PLCONT_	FNAME_(PLCONT_,plcont_)
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
#define    PLGFAM	FNAME(PLGFAM,plgfam)
#define    PLGFNAM_	FNAME_(PLGFNAM_,plgfnam_)
#define    PLGPAGE	FNAME(PLGPAGE,plgpage)
#define    PLGRA	FNAME(PLGRA,plgra)
#define    PLGSPA	FNAME(PLGSPA,plgspa)
#define    PLGSTRM	FNAME(PLGSTRM,plgstrm)
#define    PLGXAX	FNAME(PLGXAX,plgxax)
#define    PLGYAX	FNAME(PLGYAX,plgyax)
#define    PLGZAX	FNAME(PLGZAX,plgzax)
#define    PLHIST	FNAME(PLHIST,plhist)
#define    PLHLS        FNAME(PLHLS,plhls)
#define    PLJOIN	FNAME(PLJOIN,pljoin)
#define    PLLAB_	FNAME_(PLLAB_,pllab_)
#define    PLLINE	FNAME(PLLINE,plline)
#define    PLLSTY	FNAME(PLLSTY,pllsty)
#define    PLMESH	FNAME(PLMESH,plmesh)
#define    PLMTEX_	FNAME_(PLMTEX_,plmtex_)
#define    PLOT3D	FNAME(PLOT3D,plot3d)
#define    PLPAT	FNAME(PLPAT,plpat)
#define    PLPOIN	FNAME(PLPOIN,plpoin)
#define    PLPREC	FNAME(PLPREC,plprec)
#define    PLPSTY	FNAME(PLPSTY,plpsty)
#define    PLPTEX_	FNAME_(PLPTEX_,plptex_)
#define    PLRGB        FNAME(PLRGB,plrgb)
#define    PLSASP       FNAME(PLSASP,plsasp)
#define    PLSCHR	FNAME(PLSCHR,plschr)
#define    PLSFAM	FNAME(PLSFAM,plsfam)
#define    PLSFNAM_	FNAME_(PLSFNAM_,plsfnam_)
#define    PLSMAJ	FNAME(PLSMAJ,plsmaj)
#define    PLSMIN	FNAME(PLSMIN,plsmin)
#define    PLSORI	FNAME(PLSORI,plsori)
#define    PLSPAGE	FNAME(PLSPAGE,plspage)
#define    PLSSTRM	FNAME(PLSSTRM,plsstrm)
#define    PLSSYM	FNAME(PLSSYM,plssym)
#define    PLSTAR	FNAME(PLSTAR,plstar)
#define    PLSTART_	FNAME_(PLSTART_,plstart_)
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
