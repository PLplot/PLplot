/* $Id$

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
#define STUB_STDCALL	5

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

#ifdef WIN32				/* MS-DOS based */
#ifdef IVF                              /* Intel Visual Fortran */
#define STUB_LINKAGE STUB_U
#elif defined(CVF)
#define STUB_LINKAGE_STUB_U
#elif defined(_MSC_VER)
#define STUB_LINKAGE STUB_STDCALL
#else
#define STUB_LINKAGE STUB_STDCALL
#endif
#elif defined(MSDOS)
#define STUB_LINKAGE STUB_FORTRAN
#endif

#ifndef STUB_LINKAGE			/* The default */
#define STUB_LINKAGE STUB_LAU
#endif

#endif	/* ifndef STUB_LINKAGE */

/*----------------------------------------------------------------------*\
 * Define name-translation macro.
 * To use, define with x the upper case routine name, y the lower case.
 * Should be adaptable to virtually any system.
\*----------------------------------------------------------------------*/

#if STUB_LINKAGE==STUB_LAU
#define FNAME(x,y)	PLDLLIMPEXP_F95C y##_

#elif STUB_LINKAGE == STUB_L
#define FNAME(x,y)	y

#elif STUB_LINKAGE == STUB_U
#define FNAME(x,y)	x

#elif STUB_LINKAGE == STUB_STDCALL
#define FNAME(x,y)	PLDLLIMPEXP_F95C __stdcall x

#elif STUB_LINKAGE == STUB_FORTRAN
#define FNAME(x,y)	fortran x

#else
#error "Illegal setting for STUB_LINKAGE"
#endif

/*----------------------------------------------------------------------*\
 * Now to actually define the stub names.
 * Each stub must have an entry here.
\*----------------------------------------------------------------------*/

/* N.B. By default the g77 compiler appends second underscore to name if
 * the original name contains any underscore at all.  According to info
 * g77, "This is done to ensure compatibility with code produced by many
 * UNIX Fortran compilers."  However, other fortran compilers do not have
 * this default naming scheme so to avoid trouble I have #defined two
 * variations of the embedded underscore names, one with and one without
 * the extra trailing underscore.
 */

#define    PL_SETCONTLABELFORMAT	FNAME(PL_SETCONTLABELFORMAT,pl_setcontlabelformat)
#define    PL_SETCONTLABELFORMATa	FNAME(PL_SETCONTLABELFORMAT_,pl_setcontlabelformat_)
#define    PL_SETCONTLABELPARAM		FNAME(PL_SETCONTLABELPARAM,pl_setcontlabelparam)
#define    PL_SETCONTLABELPARAMa	FNAME(PL_SETCONTLABELPARAM_,pl_setcontlabelparam_)
#define    PLABORT7	FNAME(PLABORT7,plabort7)
#define    PLADV	FNAME(PLADV,pladv)
#define    PLAXES7	FNAME(PLAXES7,plaxes7)
#define    PLBIN	FNAME(PLBINF77,plbinf77)
#define    PLBOP	FNAME(PLBOP,plbop)
#define    PLBOX37	FNAME(PLBOX37,plbox37)
#define    PLBOX7	FNAME(PLBOX7,plbox7)
#define    PLCALC_WORLD 	FNAME(PLCALC_WORLD,plcalc_world)
#define    PLCALC_WORLDa	FNAME(PLCALC_WORLD_,plcalc_world_)
#define    PLCLEAR	FNAME(PLCLEAR,plclear)
#define    PLCLR	FNAME(PLCLR,plclr)
#define    PLCOL	FNAME(PLCOL,plcol)
#define    PLCOL0	FNAME(PLCOL0,plcol0)
#define    PLCOL1	FNAME(PLCOL1,plcol1)
#define    PLCON07	FNAME(PLCON07,plcon07)
#define    PLCON17	FNAME(PLCON17,plcon17)
#define    PLCON27	FNAME(PLCON27,plcon27)
#define    PLCONT7	FNAME(PLCONT7,plcont7)
#define    PLCPSTRM	FNAME(PLCPSTRMF77,plcpstrmf77)
#define    PLEND	FNAME(PLEND,plend)
#define    PLEND1	FNAME(PLEND1,plend1)
#define    PLENV	FNAME(PLENV,plenv)
#define    PLENV0	FNAME(PLENV0,plenv0)
#define    PLEOP	FNAME(PLEOP,pleop)
#define    PLERRX	FNAME(PLERRXF77,plerrxf77)
#define    PLERRY	FNAME(PLERRYF77,plerryf77)
#define    PLFAMADV	FNAME(PLFAMADV,plfamadv)
#define    PLFILL	FNAME(PLFILLF77,plfillf77)
#define    PLFILL3	FNAME(PLFILL3F77,plfill3f77)
#define    PLFLUSH	FNAME(PLFLUSH,plflush)
#define    PLFONT	FNAME(PLFONT,plfont)
#define    PLFONTLD	FNAME(PLFONTLD,plfontld)
#define    PLGCHR	FNAME(PLGCHR,plgchr)
#define    PLGCOL0	FNAME(PLGCOL0,plgcol0)
#define    PLGCOL0A	FNAME(PLGCOL0A,plgcol0a)
#define    PLGCOLBG	FNAME(PLGCOLBG,plgcolbg)
#define    PLGCOLBGA	FNAME(PLGCOLBGA,plgcolbga)
#define    PLGCOMPRESSION	FNAME(PLGCOMPRESSION,plgcompression)
#define    PLGDEV7	FNAME(PLGDEV7,plgdev7)
#define    PLGDIDEV	FNAME(PLGDIDEV,plgdidev)
#define    PLGDIORI	FNAME(PLGDIORI,plgdiori)
#define    PLGDIPLT	FNAME(PLGDIPLT,plgdiplt)
#define    PLGETCURSOR  FNAME(PLGETCURSOR,plgetcursor)
#define    PLGFAM	FNAME(PLGFAM,plgfam)
#define    PLGFCI	FNAME(PLGFCI,plgfci)
#define    PLGFNAM7	FNAME(PLGFNAM7,plgfnam7)
#define    PLGFONT	FNAME(PLGFONT,plgfont)
#define    PLGLEVEL	FNAME(PLGLEVEL,plglevel)
#define    PLGPAGE	FNAME(PLGPAGE,plgpage)
#define    PLGRA	FNAME(PLGRA,plgra)
#define    PLGRIDDATA	FNAME(PLGRIDDATAF77,plgriddataf77)
#define    PLGSPA	FNAME(PLGSPA,plgspa)
#define    PLGSTRM	FNAME(PLGSTRM,plgstrm)
#define    PLGVER7	FNAME(PLGVER7,plgver7)
#define    PLGVPD	FNAME(PLGVPD,plgvpd)
#define    PLGVPW	FNAME(PLGVPW,plgvpw)
#define    PLGXAX	FNAME(PLGXAX,plgxax)
#define    PLGYAX	FNAME(PLGYAX,plgyax)
#define    PLGZAX	FNAME(PLGZAX,plgzax)
#define    PLHIST	FNAME(PLHISTF77,plhistf77)
#define    PLHLS	FNAME(PLHLS,plhls)
#define    PLHLSRGB	FNAME(PLHLSRGB,plhlsrgb)
#define    PLIMAGE	FNAME(PLIMAGEF77,plimagef77)
#define    PLIMAGEFR07	FNAME(PLIMAGEFR07,plimagefr07)
#define    PLIMAGEFR17	FNAME(PLIMAGEFR17,plimagefr17)
#define    PLIMAGEFR27	FNAME(PLIMAGEFR27,plimagefr27)
#define    PLIMAGEFR7	FNAME(PLIMAGEFR7,plimagefr7)
#define    PLINIT	FNAME(PLINIT,plinit)
#define    PLJOIN	FNAME(PLJOIN,pljoin)
#define    PLLAB7	FNAME(PLLAB7,pllab7)
#define    PLLIGHTSOURCE	FNAME(PLLIGHTSOURCE,pllightsource)
#define    PLLINE	FNAME(PLLINEF77,pllinef77)
#define    PLLINE3	FNAME(PLLINE3F77,plline3f77)
#define    PLLSTY	FNAME(PLLSTY,pllsty)
#define    PLMAP7	FNAME(PLMAP7,plmap7)
#define    PLMERIDIANS7	FNAME(PLMERIDIANS7,plmeridians7)
#define    PLMESH	FNAME(PLMESHF77,plmeshf77)
#define    PLMESHC	FNAME(PLMESHCF77,plmeshcf77)
#define    PLMKSTRM	FNAME(PLMKSTRM,plmkstrm)
#define    PLMTEX7	FNAME(PLMTEX7,plmtex7)
#define    PLMTEX37	FNAME(PLMTEX37,plmtex37)
#define    PLOT3D	FNAME(PLOT3DF77,plot3df77)
#define    PLOT3DC	FNAME(PLOT3DCF77,plot3dcf77)

#if STUB_LINKAGE == STUB_STDCALL || STUB_LINKAGE == STUB_FORTRAN
#define    CALL_PLOT3DC PLOT3DCF77
#elif  STUB_LINKAGE == STUB_LAU
#define    CALL_PLOT3DC plot3dcf77_
#else
#define    CALL_PLOT3DC PLOT3DC
#endif

#define    PLPARSEOPTS7	FNAME(PLPARSEOPTS7,plparseopts7)
#define    PLPAT	FNAME(PLPAT,plpat)
#define    PLPOIN	FNAME(PLPOINF77,plpoinf77)
#define    PLPOIN3	FNAME(PLPOIN3F77,plpoin3f77)
#define    PLPOLY3	FNAME(PLPOLY3F77,plpoly3f77)
#define    PLPREC	FNAME(PLPREC,plprec)
#define    PLPSTY	FNAME(PLPSTY,plpsty)
#define    PLPTEX7	FNAME(PLPTEX7,plptex7)
#define    PLPTEX37	FNAME(PLPTEX37,plptex37)
#define    PLRANDD      FNAME(PLRANDDF77,plranddf77)
#define    PLREPLOT	FNAME(PLREPLOT,plreplot)
#define    PLRGB	FNAME(PLRGB,plrgb)
#define    PLRGB1	FNAME(PLRGB1,plrgb1)
#define    PLRGBHLS     FNAME(PLRGBHLS,plrgbhls)
#define    PLSCHR	FNAME(PLSCHR,plschr)
#define    PLSCMAP0	FNAME(PLSCMAP0F77,plscmap0f77)
#define    PLSCMAP0A	FNAME(PLSCMAP0AF77,plscmap0af77)
#define    PLSCMAP0N	FNAME(PLSCMAP0N,plscmap0n)
#define    PLSCMAP1	FNAME(PLSCMAP1F77,plscmap1f77)
#define    PLSCMAP1A	FNAME(PLSCMAP1AF77,plscmap1af77)
#define    PLSCMAP1L	FNAME(PLSCMAP1LF77,plscmap1lf77)
#define    PLSCMAP1L2	FNAME(PLSCMAP1L2F77,plscmap1l2f77)
#define    PLSCMAP1LA	FNAME(PLSCMAP1LAF77,plscmap1laf77)
#define    PLSCMAP1LA2	FNAME(PLSCMAP1LA2F77,plscmap1la2f77)
#define    PLSCMAP1N	FNAME(PLSCMAP1N,plscmap1n)
#define    PLSCOL0	FNAME(PLSCOL0,plscol0)
#define    PLSCOL0A	FNAME(PLSCOL0A,plscol0a)
#define    PLSCOLBG	FNAME(PLSCOLBG,plscolbg)
#define    PLSCOLBGA	FNAME(PLSCOLBGA,plscolbga)
#define    PLSCOLOR	FNAME(PLSCOLOR,plscolor)
#define    PLSCOMPRESSION	FNAME(PLSCOMPRESSION,plscompression)
#define    PLSDEV7	FNAME(PLSDEV7,plsdev7)
#define    PLSDIDEV	FNAME(PLSDIDEV,plsdidev)
#define    PLSDIMAP	FNAME(PLSDIMAP,plsdimap)
#define    PLSDIORI	FNAME(PLSDIORI,plsdiori)
#define    PLSDIPLT	FNAME(PLSDIPLT,plsdiplt)
#define    PLSDIPLZ	FNAME(PLSDIPLZ,plsdiplz)
#define    PLSEED       FNAME(PLSEEDF77,plseedf77)
#define    PLSESC	FNAME(PLSESC,plsesc)
#define    PLSETOPT7	FNAME(PLSETOPT7,plsetopt7)
#define    PLSFAM	FNAME(PLSFAM,plsfam)
#define    PLSFCI	FNAME(PLSFCI,plsfci)
#define    PLSFNAM7	FNAME(PLSFNAM7,plsfnam7)
#define    PLSFONT	FNAME(PLSFONT,plsfont)
#define    PLSHADE07	FNAME(PLSHADE07,plshade07)
#define    PLSHADE17	FNAME(PLSHADE17,plshade17)
#define    PLSHADE27	FNAME(PLSHADE27,plshade27)
#define    PLSHADE7	FNAME(PLSHADE7,plshade7)
#define    PLSHADES07	FNAME(PLSHADES07,plshades07)
#define    PLSHADES17	FNAME(PLSHADES17,plshades17)
#define    PLSHADES27	FNAME(PLSHADES27,plshades27)
#define    PLSHADES7	FNAME(PLSHADES7,plshades7)
#define    PLSMAJ	FNAME(PLSMAJ,plsmaj)
#define    PLSMEM	FNAME(PLSMEM,plsmem)
#define    PLSMIN	FNAME(PLSMIN,plsmin)
#define    PLSORI	FNAME(PLSORI,plsori)
#define    PLSPAGE	FNAME(PLSPAGE,plspage)
#define    PLSPAUSE	FNAME(PLSPAUSEF77,plspausef77)
#define    PLSSTRM	FNAME(PLSSTRM,plsstrm)
#define    PLSSUB	FNAME(PLSSUB,plssub)
#define    PLSSYM	FNAME(PLSSYM,plssym)
#define    PLSTAR	FNAME(PLSTAR,plstar)
#define    PLSTART7	FNAME(PLSTART7,plstart7)
#define    PLSTRIPA	FNAME(PLSTRIPA,plstripa)
#define    PLSTRIPC	FNAME(PLSTRIPCF77,plstripcf77)
#define    PLSTRIPD	FNAME(PLSTRIPD,plstripd)
#define    PLSTYL	FNAME(PLSTYL,plstyl)
#define    PLSURF3D	FNAME(PLSURF3DF77,plsurf3df77)
#define    PLSVECT	FNAME(PLSVECTF77,plsvectf77)
#define    PLSVPA	FNAME(PLSVPA,plsvpa)
#define    PLSXAX	FNAME(PLSXAX,plsxax)
#define    PLSYAX	FNAME(PLSYAX,plsyax)
#define    PLSYM	FNAME(PLSYMF77,plsymf77)
#define    PLSZAX	FNAME(PLSZAX,plszax)
#define    PLTEXT	FNAME(PLTEXT,pltext)
#define    PLTIMEFMT7	FNAME(PLTIMEFMT7,pltimefmt7)
#define    PLVASP	FNAME(PLVASP,plvasp)
#define    PLVEC07	FNAME(PLVEC07,plvec07)
#define    PLVEC17	FNAME(PLVEC17,plvec17)
#define    PLVEC27	FNAME(PLVEC27,plvec27)
#define    PLVECT7	FNAME(PLVECT7,plvect7)
#define    PLVPAS	FNAME(PLVPAS,plvpas)
#define    PLVPOR	FNAME(PLVPOR,plvpor)
#define    PLVSTA	FNAME(PLVSTA,plvsta)
#define    PLW3D	FNAME(PLW3D,plw3d)
#define    PLWID	FNAME(PLWID,plwid)
#define    PLWIND	FNAME(PLWIND,plwind)
#define    PLXORMOD	FNAME(PLXORMODF77,plxormodf77)

#endif	/* __PLSTUBS_H__ */
