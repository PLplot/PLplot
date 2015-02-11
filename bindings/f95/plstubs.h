// Maurice LeBrun
// IFS, University of Texas
//
// Header file for plplot Fortran interface stubs.
// THIS FILE IS NOT FOR INCLUSION BY USER CODES!!
//
// The contents of this file are in the public domain.
//

#ifndef __PLSTUBS_H__
#define __PLSTUBS_H__

#include "plplotP.h"

//--------------------------------------------------------------------------
// Select name transformation based on system type.
//
// Define the STUB_LINKAGE flag to get proper C<->Fortran linkage on your
// system.  This flag describes what the compiler does to Fortran routine
// names, which we must duplicate on the C stubs so that the Fortran
// routines may call them.  You can often set STUB_LINKAGE by the
// construct -DSTUB_LINKAGE=<value> on the C compiler command line, but
// it is best to either rely on the default or explicitly handle your
// system below.
//
// Current choices for STUB_LINKAGE:
//
//      STUB_LAU	lower-case, append underscore
//      STUB_L		lower-case
//	STUB_U		upper-case
//	STUB_FORTRAN	use "fortran" keyword (MS-DOS convention)
//
// If no choice is made, the default is set to STUB_LAU.  This should
// handle most generic Unix boxes not already accounted for.
//
// ** Namespace collision **
//
// If you use the STUB_L option, the C & Fortran namespaces will collide
// if the Fortran compiler does lower case folding (they usually do).
// The problem is then that the stub names and actual function names will
// be exactly the same, if we insist on the Fortran and C bindings to be
// similar.  The solution is to give the externally callable C routines
// unique names, and provide macros to turn the documented entry names in
// to the real ones.  This is a horrible kludge, but the alternatives are
// worse.  Fortunately it has no effect on the user program, and you can
// forget that you ever read about it here.
//--------------------------------------------------------------------------

#define STUB_LAU        1
#define STUB_L          2
#define STUB_U          3
#define STUB_FORTRAN    4
#define STUB_STDCALL    5
#define STUB_MINGW      6
#define STUB_IVF        7

#ifndef STUB_LINKAGE

#if defined ( SX )                         // NEC Super-UX (SX-3)
#define STUB_LINKAGE    STUB_LAU
#endif

#if defined ( _IBMR2 ) && defined ( _AIX )    // AIX
#define STUB_LINKAGE    STUB_L
#endif

#ifdef __hpux                           // HP/UX
#define STUB_LINKAGE    STUB_L
#endif

#ifdef __mips                           // IRIX (SGI systems)
#define STUB_LINKAGE    STUB_LAU
#endif

#ifdef sun                              // Suns
#define STUB_LINKAGE    STUB_LAU
#endif

#ifdef CRAY                             // Cray/UNICOS
#define STUB_LINKAGE    STUB_U
#endif

#if defined ( __alpha ) && defined ( __osf__ ) // DEC Alpha AXP/OSF
#define STUB_LINKAGE    STUB_LAU
#endif

#ifdef __GO32__                         // dos386/djgpp
#ifdef MSDOS
#undef MSDOS
#endif
#endif

#ifdef WIN32                            // MS-DOS based
#ifdef IVF                              // Intel Visual Fortran
#define STUB_LINKAGE    STUB_IVF
#elif defined ( CVF )
#define STUB_LINKAGE    STUB_U
#elif defined ( MSDOS )
#define STUB_LINKAGE    STUB_FORTRAN
#elif defined ( _MSC_VER )
#define STUB_LINKAGE    STUB_STDCALL
#elif defined ( __GNUC__ )
#define STUB_LINKAGE    STUB_MINGW
#endif
#elif defined ( MSDOS )                    // MS-DOS based
#define STUB_LINKAGE    STUB_FORTRAN
#endif // Windows 32-bit

#ifndef STUB_LINKAGE                    // The default
#define STUB_LINKAGE    STUB_LAU
#endif

#endif  // ifndef STUB_LINKAGE

//--------------------------------------------------------------------------
// Define name-translation macro.
// To use, define with x the upper case routine name, y the lower case.
// Should be adaptable to virtually any system.
//--------------------------------------------------------------------------

#if STUB_LINKAGE == STUB_LAU
#define FNAME( x, y )     PLDLLIMPEXP_F95C y ## _
#define FNAME_( x, y )    y ## _

#elif STUB_LINKAGE == STUB_L
#define FNAME( x, y )     y
#define FNAME_( x, y )    y

#elif STUB_LINKAGE == STUB_U
#define FNAME( x, y )     PLDLLIMPEXP_F95C __stdcall x
#define FNAME_( x, y )    x

#elif STUB_LINKAGE == STUB_FORTRAN
#define FNAME( x, y )     fortran x
#define FNAME_( x, y )    x

#elif STUB_LINKAGE == STUB_STDCALL
#define FNAME( x, y )     PLDLLIMPEXP_F95C __stdcall x
#define FNAME_( x, y )    x

#elif STUB_LINKAGE == STUB_MINGW
#define FNAME( x, y )     PLDLLIMPEXP_F95C y ## _
#define FNAME_( x, y )    y

#elif STUB_LINKAGE == STUB_IVF
#define FNAME( x, y )     PLDLLIMPEXP_F95C x
#define FNAME_( x, y )    x

#else
#error "Illegal setting for STUB_LINKAGE"
#endif

//--------------------------------------------------------------------------
// Now to actually define the stub names.
// Each stub must have an entry here.
//--------------------------------------------------------------------------

// N.B. By default the gfortran compiler appends second underscore to name if
// the original name contains any underscore at all.  According to info
// gfortran, "This is done to ensure compatibility with code produced by many
// UNIX Fortran compilers."  However, other fortran compilers do not have
// this default naming scheme so to avoid trouble I have #defined two
// variations of the embedded underscore names, one with and one without
// the extra trailing underscore.
//

#define    PL_SETCONTLABELFORMAT     FNAME( PL_SETCONTLABELFORMAT, pl_setcontlabelformat )
#define    PL_SETCONTLABELFORMATa    FNAME( PL_SETCONTLABELFORMAT_, pl_setcontlabelformat_ )
#define    PL_SETCONTLABELPARAM      FNAME( PL_SETCONTLABELPARAM, pl_setcontlabelparam )
#define    PL_SETCONTLABELPARAMa     FNAME( PL_SETCONTLABELPARAM_, pl_setcontlabelparam_ )
#define    PLABORT7                  FNAME( PLABORT7, plabort7 )
#define    PLADV                     FNAME( PLADV, pladv )
#define    PLARC                     FNAME( PLARC, plarc )
#define    PLAXES7                   FNAME( PLAXES7, plaxes7 )
#define    PLBIN                     FNAME( PLBINF95, plbinf95 )
#define    PLBOP                     FNAME( PLBOP, plbop )
#define    PLBOX37                   FNAME( PLBOX37, plbox37 )
#define    PLBOX7                    FNAME( PLBOX7, plbox7 )
#define    PLBTIME                   FNAME( PLBTIME, plbtime )
#define    PLCALC_WORLD              FNAME( PLCALC_WORLD, plcalc_world )
#define    PLCALC_WORLDa             FNAME( PLCALC_WORLD_, plcalc_world_ )
#define    PLCLEAR                   FNAME( PLCLEAR, plclear )
#define    PLCLR                     FNAME( PLCLR, plclr )
#define    PLCOL0                    FNAME( PLCOL0, plcol0 )
#define    PLCOL1                    FNAME( PLCOL1, plcol1 )
#define    PLCOLORBAR_CNV_TEXT       FNAME( PLCOLORBAR07_CNV_TEXT, plcolorbar07_cnv_text )
#define    PLCOLORBAR                FNAME( PLCOLORBAR07, plcolorbar07 )
#define    PLCONFIGTIME              FNAME( PLCONFIGTIME, plconfigtime )
#define    PLCON07                   FNAME( PLCON07, plcon07 )
#define    PLCON17                   FNAME( PLCON17, plcon17 )
#define    PLCON27                   FNAME( PLCON27, plcon27 )
#define    PLCONT7                   FNAME( PLCONT7, plcont7 )
#define    PLCPSTRM                  FNAME( PLCPSTRMF95, plcpstrmf95 )
#define    PLCTIME                   FNAME( PLCTIME, plctime )
#define    PLEND                     FNAME( PLEND, plend )
#define    PLEND1                    FNAME( PLEND1, plend1 )
#define    PLENV                     FNAME( PLENV, plenv )
#define    PLENV0                    FNAME( PLENV0, plenv0 )
#define    PLEOP                     FNAME( PLEOP, pleop )
#define    PLERRX                    FNAME( PLERRXF95, plerrxf95 )
#define    PLERRY                    FNAME( PLERRYF95, plerryf95 )
#define    PLFAMADV                  FNAME( PLFAMADV, plfamadv )
#define    PLFILL                    FNAME( PLFILLF95, plfillf95 )
#define    PLFILL3                   FNAME( PLFILL3F95, plfill3f95 )
#define    PLFLUSH                   FNAME( PLFLUSH, plflush )
#define    PLFONT                    FNAME( PLFONT, plfont )
#define    PLFONTLD                  FNAME( PLFONTLD, plfontld )
#define    PLGCHR                    FNAME( PLGCHR, plgchr )
#define    PLGCMAP1_RANGE            FNAME( PLGCMAP1_RANGE, plgcmap1_range )
#define    PLGCOL0                   FNAME( PLGCOL0, plgcol0 )
#define    PLGCOL0A                  FNAME( PLGCOL0A, plgcol0a )
#define    PLGCOLBG                  FNAME( PLGCOLBG, plgcolbg )
#define    PLGCOLBGA                 FNAME( PLGCOLBGA, plgcolbga )
#define    PLGCOMPRESSION            FNAME( PLGCOMPRESSION, plgcompression )
#define    PLGDEV7                   FNAME( PLGDEV7, plgdev7 )
#define    PLGDIDEV                  FNAME( PLGDIDEV, plgdidev )
#define    PLGDIORI                  FNAME( PLGDIORI, plgdiori )
#define    PLGDIPLT                  FNAME( PLGDIPLT, plgdiplt )
#define    PLGETCURSOR               FNAME( PLGETCURSOR, plgetcursor )
#define    PLGFAM                    FNAME( PLGFAM, plgfam )
#define    PLGFCI                    FNAME( PLGFCI, plgfci )
#define    PLGFNAM7                  FNAME( PLGFNAM7, plgfnam7 )
#define    PLGFONT                   FNAME( PLGFONT, plgfont )
#define    PLGLEVEL                  FNAME( PLGLEVEL, plglevel )
#define    PLGPAGE                   FNAME( PLGPAGE, plgpage )
#define    PLGRA                     FNAME( PLGRA, plgra )
#define    PLGRADIENT                FNAME( PLGRADIENTF95, plgradientf95 )
#define    PLGRIDDATA                FNAME( PLGRIDDATAF95, plgriddataf95 )
#define    PLGSPA                    FNAME( PLGSPA, plgspa )
#define    PLGSTRM                   FNAME( PLGSTRM, plgstrm )
#define    PLGVER7                   FNAME( PLGVER7, plgver7 )
#define    PLGVPD                    FNAME( PLGVPD, plgvpd )
#define    PLGVPW                    FNAME( PLGVPW, plgvpw )
#define    PLGXAX                    FNAME( PLGXAX, plgxax )
#define    PLGYAX                    FNAME( PLGYAX, plgyax )
#define    PLGZAX                    FNAME( PLGZAX, plgzax )
#define    PLHIST                    FNAME( PLHISTF95, plhistf95 )
#define    PLHLSRGB                  FNAME( PLHLSRGB, plhlsrgb )
#define    PLIMAGE                   FNAME( PLIMAGEF95, plimagef95 )
#define    PLIMAGEFR07               FNAME( PLIMAGEFR07, plimagefr07 )
#define    PLIMAGEFR17               FNAME( PLIMAGEFR17, plimagefr17 )
#define    PLIMAGEFR27               FNAME( PLIMAGEFR27, plimagefr27 )
#define    PLIMAGEFR7                FNAME( PLIMAGEFR7, plimagefr7 )
#define    PLINIT                    FNAME( PLINIT, plinit )
#define    PLJOIN                    FNAME( PLJOIN, pljoin )
#define    PLLAB7                    FNAME( PLLAB7, pllab7 )
#define    PLLEGEND_CNV_TEXT         FNAME( PLLEGEND07_CNV_TEXT, pllegend07_cnv_text )
#define    PLLEGEND                  FNAME( PLLEGEND07, pllegend07 )
#define    PLLIGHTSOURCE             FNAME( PLLIGHTSOURCE, pllightsource )
#define    PLLINE                    FNAME( PLLINEF95, pllinef95 )
#define    PLLINE3                   FNAME( PLLINE3F95, plline3f95 )
#define    PLLSTY                    FNAME( PLLSTY, pllsty )
#define    PLMAP7                    FNAME( PLMAP7, plmap7 )
#define    PLMAPFILL7                FNAME( PLMAPFILL7, plmapfill7 )
#define    PLMAPLINE7                FNAME( PLMAPLINE7, plmapline7 )
#define    PLMAPSTRING7              FNAME( PLMAPSTRING7, plmapstring7 )
#define    PLMAPTEX7                 FNAME( PLMAPTEX7, plmaptex7 )
#define    PLMERIDIANS7              FNAME( PLMERIDIANS7, plmeridians7 )
#define    PLMESH                    FNAME( PLMESHF95, plmeshf95 )
#define    PLMESHC                   FNAME( PLMESHCF95, plmeshcf95 )
#define    PLMKSTRM                  FNAME( PLMKSTRM, plmkstrm )
#define    PLMTEX7                   FNAME( PLMTEX7, plmtex7 )
#define    PLMTEX37                  FNAME( PLMTEX37, plmtex37 )
#define    PLOT3D                    FNAME( PLOT3DF95, plot3df95 )
#define    PLOT3DC                   FNAME( PLOT3DCF95, plot3dcf95 )

#if STUB_LINKAGE == STUB_STDCALL || STUB_LINKAGE == STUB_FORTRAN
#define    CALL_PLOT3DC              PLOT3DCF95
#elif  STUB_LINKAGE == STUB_LAU
#define    CALL_PLOT3DC              plot3dcf95_
#else
#define    CALL_PLOT3DC              PLOT3DC
#endif

#define    PLPARSEOPTS7              FNAME( PLPARSEOPTS7, plparseopts7 )
#define    PLPAT                     FNAME( PLPAT, plpat )
#define    PLPATH                    FNAME( PLPATH, plpath )
#define    PLPOIN                    FNAME( PLPOINF95, plpoinf95 )
#define    PLPOIN3                   FNAME( PLPOIN3F95, plpoin3f95 )
#define    PLPOLY3                   FNAME( PLPOLY3F95, plpoly3f95 )
#define    PLPREC                    FNAME( PLPREC, plprec )
#define    PLPSTY                    FNAME( PLPSTY, plpsty )
#define    PLPTEX7                   FNAME( PLPTEX7, plptex7 )
#define    PLPTEX37                  FNAME( PLPTEX37, plptex37 )
#define    PLRANDD                   FNAME( PLRANDDF95, plranddf95 )
#define    PLREPLOT                  FNAME( PLREPLOT, plreplot )
#define    PLRGBHLS                  FNAME( PLRGBHLS, plrgbhls )
#define    PLSCHR                    FNAME( PLSCHR, plschr )
#define    PLSCMAP0                  FNAME( PLSCMAP0F95, plscmap0f95 )
#define    PLSCMAP0A                 FNAME( PLSCMAP0AF95, plscmap0af95 )
#define    PLSCMAP0N                 FNAME( PLSCMAP0N, plscmap0n )
#define    PLSCMAP1                  FNAME( PLSCMAP1F95, plscmap1f95 )
#define    PLSCMAP1A                 FNAME( PLSCMAP1AF95, plscmap1af95 )
#define    PLSCMAP1L                 FNAME( PLSCMAP1LF95, plscmap1lf95 )
#define    PLSCMAP1L2                FNAME( PLSCMAP1L2F95, plscmap1l2f95 )
#define    PLSCMAP1LA                FNAME( PLSCMAP1LAF95, plscmap1laf95 )
#define    PLSCMAP1LA2               FNAME( PLSCMAP1LA2F95, plscmap1la2f95 )
#define    PLSCMAP1N                 FNAME( PLSCMAP1N, plscmap1n )
#define    PLSCMAP1_RANGE            FNAME( PLSCMAP1_RANGE, plscmap1_range )
#define    PLSCOL0                   FNAME( PLSCOL0, plscol0 )
#define    PLSCOL0A                  FNAME( PLSCOL0A, plscol0a )
#define    PLSCOLBG                  FNAME( PLSCOLBG, plscolbg )
#define    PLSCOLBGA                 FNAME( PLSCOLBGA, plscolbga )
#define    PLSCOLOR                  FNAME( PLSCOLOR, plscolor )
#define    PLSCOMPRESSION            FNAME( PLSCOMPRESSION, plscompression )
#define    PLSDEV7                   FNAME( PLSDEV7, plsdev7 )
#define    PLSDIDEV                  FNAME( PLSDIDEV, plsdidev )
#define    PLSDIMAP                  FNAME( PLSDIMAP, plsdimap )
#define    PLSDIORI                  FNAME( PLSDIORI, plsdiori )
#define    PLSDIPLT                  FNAME( PLSDIPLT, plsdiplt )
#define    PLSDIPLZ                  FNAME( PLSDIPLZ, plsdiplz )
#define    PLSEED                    FNAME( PLSEED, plseed )
#define    PLSESC                    FNAME( PLSESC, plsesc )
#define    PLSETOPT7                 FNAME( PLSETOPT7, plsetopt7 )
#define    PLSFAM                    FNAME( PLSFAM, plsfam )
#define    PLSFCI                    FNAME( PLSFCI, plsfci )
#define    PLSFNAM7                  FNAME( PLSFNAM7, plsfnam7 )
#define    PLSFONT                   FNAME( PLSFONT, plsfont )
#define    PLSHADE07                 FNAME( PLSHADE07, plshade07 )
#define    PLSHADE17                 FNAME( PLSHADE17, plshade17 )
#define    PLSHADE27                 FNAME( PLSHADE27, plshade27 )
#define    PLSHADE7                  FNAME( PLSHADE7, plshade7 )
#define    PLSHADES07                FNAME( PLSHADES07, plshades07 )
#define    PLSHADES17                FNAME( PLSHADES17, plshades17 )
#define    PLSHADES27                FNAME( PLSHADES27, plshades27 )
#define    PLSHADES7                 FNAME( PLSHADES7, plshades7 )
#define    PLSLABELFUNC_ON           FNAME( PLSLABELFUNC_ON, plslabelfunc_on )
#define    PLSLABELFUNC_ONa          FNAME( PLSLABELFUNC_ON_, plslabelfunc_on_ )
#define    PLSLABELFUNC_OFF          FNAME( PLSLABELFUNC_OFF, plslabelfunc_off )
#define    PLSLABELFUNC_OFFa         FNAME( PLSLABELFUNC_OFF_, plslabelfunc_off_ )
#define    PLSLABELFUNC_NONE         FNAME( PLSLABELFUNC_NONE, plslabelfunc_none )
#define    PLSLABELFUNC_NONEa        FNAME( PLSLABELFUNC_NONE_, plslabelfunc_none_ )
#define    PLSMAJ                    FNAME( PLSMAJ, plsmaj )
#define    PLSMEM                    FNAME( PLSMEM, plsmem )
#define    PLSMEMA                   FNAME( PLSMEMA, plsmema )
#define    PLSMIN                    FNAME( PLSMIN, plsmin )
#define    PLSORI                    FNAME( PLSORI, plsori )
#define    PLSPAGE                   FNAME( PLSPAGE, plspage )
#define    PLSPAL07                  FNAME( PLSPAL07, plspal07 )
#define    PLSPAL17                  FNAME( PLSPAL17, plspal17 )
#define    PLSPAUSE                  FNAME( PLSPAUSEF95, plspausef95 )
#define    PLSSTRM                   FNAME( PLSSTRM, plsstrm )
#define    PLSSUB                    FNAME( PLSSUB, plssub )
#define    PLSSYM                    FNAME( PLSSYM, plssym )
#define    PLSTAR                    FNAME( PLSTAR, plstar )
#define    PLSTART7                  FNAME( PLSTART7, plstart7 )
#define    PLSTRANSFORM1             FNAME( PLSTRANSFORM1, plstransform1 )
#define    PLSTRANSFORM2             FNAME( PLSTRANSFORM2, plstransform2 )
#define    PLSTRANSFORM3             FNAME( PLSTRANSFORM3, plstransform3 )
#define    PLSTRING7                 FNAME( PLSTRING7, plstring7 )
#define    PLSTRING37                FNAME( PLSTRING37, plstring37 )
#define    PLSTRIPA                  FNAME( PLSTRIPA, plstripa )
#define    PLSTRIPC                  FNAME( PLSTRIPCF95, plstripcf95 )
#define    PLSTRIPD                  FNAME( PLSTRIPD, plstripd )
#define    PLSTYL                    FNAME( PLSTYL, plstyl )
#define    PLSURF3D                  FNAME( PLSURF3DF95, plsurf3df95 )
#define    PLSURF3DL                 FNAME( PLSURF3DLF95, plsurf3dlf95 )
#define    PLSVECT1                  FNAME( PLSVECT1F95, plsvect1f95 )
#define    PLSVECT2                  FNAME( PLSVECT2, plsvect2 )
#define    PLSVPA                    FNAME( PLSVPA, plsvpa )
#define    PLSXAX                    FNAME( PLSXAX, plsxax )
#define    PLSYAX                    FNAME( PLSYAX, plsyax )
#define    PLSYM                     FNAME( PLSYMF95, plsymf95 )
#define    PLSZAX                    FNAME( PLSZAX, plszax )
#define    PLTEXT                    FNAME( PLTEXT, pltext )
#define    PLTIMEFMT7                FNAME( PLTIMEFMT7, pltimefmt7 )
#define    PLVASP                    FNAME( PLVASP, plvasp )
#define    PLVEC07                   FNAME( PLVEC07, plvec07 )
#define    PLVEC17                   FNAME( PLVEC17, plvec17 )
#define    PLVEC27                   FNAME( PLVEC27, plvec27 )
#define    PLVECT7                   FNAME( PLVECT7, plvect7 )
#define    PLVPAS                    FNAME( PLVPAS, plvpas )
#define    PLVPOR                    FNAME( PLVPOR, plvpor )
#define    PLVSTA                    FNAME( PLVSTA, plvsta )
#define    PLW3D                     FNAME( PLW3D, plw3d )
#define    PLWIDTH                   FNAME( PLWIDTH, plwidth )
#define    PLWIND                    FNAME( PLWIND, plwind )
#define    PLXORMOD                  FNAME( PLXORMODF95, plxormodf95 )

#ifdef PL_DEPRECATE
#define    PLRGB                     FNAME( PLRGB, plrgb )
#define    PLRGB1                    FNAME( PLRGB1, plrgb1 )
#define    PLHLS                     FNAME( PLHLS, plhls )
#endif  // PL_DEPRECATED

#endif  // __PLSTUBS_H__
