/* $Id$
   $Log$
   Revision 1.1  1992/05/20 21:33:44  furnish
   Initial checkin of the whole PLPLOT project.

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

#ifdef STUB_LAU

#define    PLADV	pladv_
#define    PLANCOL_	plancol__
#define    PLAXES_	plaxes__
#define    PLBEG	plbeg_
#define    PLBIN	plbin_
#define    PLBOX3_	plbox3__
#define    PLBOX_	plbox__
#define    PLCLR	plclr_
#define    PLCOL	plcol_
#define    PLCON0_	plcon0__
#define    PLCON1_	plcon1__
#define    PLCON2_	plcon2__
#define    PLCONT_	plcont__
#define    PLEND	plend_
#define    PLENV	plenv_
#define    PLERRX	plerrx_
#define    PLERRY	plerry_
#define    PLFILL	plfill_
#define    PLFONT	plfont_
#define    PLFONTLD	plfontld_
#define    PLGFAM	plgfam_
#define    PLGFNAM_	plgfnam__
#define    PLGPAGE	plgpage_
#define    PLGRA	plgra_
#define    PLGSPA	plgspa_
#define    PLGSTRM	plgstrm_
#define    PLGXAX	plgxax_
#define    PLGYAX	plgyax_
#define    PLGZAX	plgzax_
#define    PLHIST	plhist_
#define    PLHLS        plhls_
#define    PLJOIN	pljoin_
#define    PLLAB_	pllab__
#define    PLLINE	plline_
#define    PLLSTY	pllsty_
#define    PLMESH	plmesh_
#define    PLMTEX_	plmtex__
#define    PLOT3D	plot3d_
#define    PLPAT	plpat_
#define    PLPOIN	plpoin_
#define    PLPREC	plprec_
#define    PLPSTY	plpsty_
#define    PLPTEX_	plptex__
#define    PLRGB        plrgb_
#define    PLSASP       plsasp_
#define    PLSCHR	plschr_
#define    PLSFAM	plsfam_
#define    PLSFNAM_	plsfnam__
#define    PLSMAJ	plsmaj_
#define    PLSMIN	plsmin_
#define    PLSORI	plsori_
#define    PLSPAGE	plspage_
#define    PLSSTRM	plsstrm_
#define    PLSSYM	plssym_
#define    PLSTAR	plstar_
#define    PLSTART_	plstart__
#define    PLSTYL	plstyl_
#define    PLSVPA	plsvpa_
#define    PLSXAX	plsxax_
#define    PLSYAX	plsyax_
#define    PLSYM	plsym_
#define    PLSZAX	plszax_
#define    PLTEXT	pltext_
#define    PLVASP	plvasp_
#define    PLVPAS	plvpas_
#define    PLVPOR	plvpor_
#define    PLVSTA	plvsta_
#define    PLW3D	plw3d_
#define    PLWID	plwid_
#define    PLWIND	plwind_

#endif

#ifdef STUB_L

#define    PLADV	pladv
#define    PLANCOL_	plancol_
#define    PLAXES_	plaxes_
#define    PLBEG	plbeg
#define    PLBIN	plbin
#define    PLBOX3_	plbox3_
#define    PLBOX_	plbox_
#define    PLCLR	plclr
#define    PLCOL	plcol
#define    PLCON0_	plcon0_
#define    PLCON1_	plcon1_
#define    PLCON2_	plcon2_
#define    PLCONT_	plcont_
#define    PLEND	plend
#define    PLENV	plenv
#define    PLERRX	plerrx
#define    PLERRY	plerry
#define    PLFILL	plfill
#define    PLFONT	plfont
#define    PLFONTLD	plfontld
#define    PLGFAM	plgfam
#define    PLGFNAM_	plgfnam_
#define    PLGPAGE	plgpage
#define    PLGRA	plgra
#define    PLGSPA	plgspa
#define    PLGSTRM	plgstrm
#define    PLGXAX	plgxax
#define    PLGYAX	plgyax
#define    PLGZAX	plgzax
#define    PLHIST	plhist
#define    PLHLS        plhls
#define    PLJOIN	pljoin
#define    PLLAB_	pllab_
#define    PLLINE	plline
#define    PLLSTY	pllsty
#define    PLMESH	plmesh
#define    PLMTEX_	plmtex_
#define    PLOT3D	plot3d
#define    PLPAT	plpat
#define    PLPOIN	plpoin
#define    PLPREC	plprec
#define    PLPSTY	plpsty
#define    PLPTEX_	plptex_
#define    PLRGB        plrgb
#define    PLSASP       plsasp
#define    PLSCHR	plschr
#define    PLSFAM	plsfam
#define    PLSFNAM_	plsfnam_
#define    PLSMAJ	plsmaj
#define    PLSMIN	plsmin
#define    PLSORI	plsori
#define    PLSPAGE	plspage
#define    PLSSTRM	plsstrm
#define    PLSSYM	plssym
#define    PLSTAR	plstar
#define    PLSTART_	plstart_
#define    PLSTYL	plstyl
#define    PLSVPA	plsvpa
#define    PLSXAX	plsxax
#define    PLSYAX	plsyax
#define    PLSYM	plsym
#define    PLSZAX	plszax
#define    PLTEXT	pltext
#define    PLVASP	plvasp
#define    PLVPAS	plvpas
#define    PLVPOR	plvpor
#define    PLVSTA	plvsta
#define    PLW3D	plw3d
#define    PLWID	plwid
#define    PLWIND	plwind

#endif

#ifdef MSDOS

#define    PLADV	fortran PLADV
#define    PLANCOL_	fortran PLANCOL_
#define    PLAXES_	fortran PLAXES_
#define    PLBEG	fortran PLBEG
#define    PLBIN	fortran PLBIN
#define    PLBOX3_	fortran PLBOX3_
#define    PLBOX_	fortran PLBOX_
#define    PLCLR	fortran PLCLR
#define    PLCOL	fortran PLCOL
#define    PLCON0_	fortran PLCON0_
#define    PLCON1_	fortran PLCON1_
#define    PLCON2_	fortran PLCON2_
#define    PLCONT_	fortran PLCONT_
#define    PLEND	fortran PLEND
#define    PLENV	fortran PLENV
#define    PLERRX	fortran PLERRX
#define    PLERRY	fortran PLERRY
#define    PLFILL	fortran PLFILL
#define    PLFONT	fortran PLFONT
#define    PLFONTLD	fortran PLFONTLD
#define    PLGFAM	fortran PLGFAM
#define    PLGFNAM_	fortran PLGFNAM_
#define    PLGPAGE	fortran PLGPAGE
#define    PLGRA	fortran PLGRA
#define    PLGSPA	fortran PLGSPA
#define    PLGSTRM	fortran PLGSTRM
#define    PLGXAX	fortran PLGXAX
#define    PLGYAX	fortran PLGYAX
#define    PLGZAX	fortran PLGZAX
#define    PLHIST	fortran PLHIST
#define    PLHLS	fortran PLHLS
#define    PLJOIN	fortran PLJOIN
#define    PLLAB_	fortran PLLAB_
#define    PLLINE	fortran PLLINE
#define    PLLSTY	fortran PLLSTY
#define    PLMESH	fortran PLMESH
#define    PLMTEX_	fortran PLMTEX_
#define    PLOT3D	fortran PLOT3D
#define    PLPAT	fortran PLPAT
#define    PLPOIN	fortran PLPOIN
#define    PLPREC	fortran PLPREC
#define    PLPSTY	fortran PLPSTY
#define    PLPTEX_	fortran PLPTEX_
#define    PLRGB	fortran PLRGB
#define    PLSASP	fortran PLSASP
#define    PLSCHR	fortran PLSCHR
#define    PLSFAM	fortran PLSFAM
#define    PLSFNAM_	fortran PLSFNAM_
#define    PLSMAJ	fortran PLSMAJ
#define    PLSMIN	fortran PLSMIN
#define    PLSORI	fortran PLSORI
#define    PLSPAGE	fortran PLSPAGE
#define    PLSSTRM	fortran PLSSTRM
#define    PLSSYM	fortran PLSSYM
#define    PLSTAR	fortran PLSTAR
#define    PLSTART_	fortran PLSTART_
#define    PLSTYL	fortran PLSTYL
#define    PLSVPA	fortran PLSVPA
#define    PLSXAX	fortran PLSXAX
#define    PLSYAX	fortran PLSYAX
#define    PLSYM	fortran PLSYM
#define    PLSZAX	fortran PLSZAX
#define    PLTEXT	fortran PLTEXT
#define    PLVASP	fortran PLVASP
#define    PLVPAS	fortran PLVPAS
#define    PLVPOR	fortran PLVPOR
#define    PLVSTA	fortran PLVSTA
#define    PLW3D	fortran PLW3D
#define    PLWID	fortran PLWID
#define    PLWIND	fortran PLWIND

#endif

#endif	/* INCLUDED_PLSTUBS */
