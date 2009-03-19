#ifndef __PL_DLL_H
#define __PL_DLL_H

#ifdef USINGDLL
  #if defined(WIN32)
    /* Visual C/C++, Borland, MinGW and Watcom */
    #if defined(__VISUALC__) || defined(_MSC_VER) || defined(__BORLANDC__) || defined(__GNUC__) || defined(__WATCOMC__)
      #define PLDLLEXPORT __declspec(dllexport)
      #define PLDLLIMPORT __declspec(dllimport)
    #else
      #define PLDLLEXPORT
      #define PLDLLIMPORT
    #endif
  #elif defined(__CYGWIN__)
    #define PLDLLEXPORT __declspec(dllexport)
    #define PLDLLIMPORT __declspec(dllimport)
  #elif defined(__GNUC__) && __GNUC__ > 3
    /* Follow ideas in http://gcc.gnu.org/wiki/Visibility for GCC version 4.x
     * The following forces exported symbols specifically designated with 
     * PLDLLEXPORT to be visible.  */
    #define PLDLLEXPORT __attribute__ ((visibility("default")))
    #define PLDLLIMPORT
  #endif
#endif

/* For an unknown compiler or static built we clear the macros */
#ifndef PLDLLEXPORT
  #define PLDLLEXPORT
  #define PLDLLIMPORT
#endif

/* The IMPEXP macros will always be set to DLLIMPORT (even for
   the static library, but DLLIMPORT is empty in this case), if
   cmake didn't set the corresponding macro xxxx_EXPORTS when the
   corresponding library is built (DLLIMPEXP is set to DLLEXPORT
   then) */
#if defined(plplotd_EXPORTS)
  #define PLDLLIMPEXP PLDLLEXPORT
  #define PLDLLIMPEXP_DATA(type) PLDLLEXPORT type
#else
  #define PLDLLIMPEXP PLDLLIMPORT
  #define PLDLLIMPEXP_DATA(type) PLDLLIMPORT type
#endif

/* for dynamic drivers set the macros correctly. If a shared library is built,
   but dyanmic drivers disabled, the driver dll macros are the same as the
   plplot dll macros */
#ifdef ENABLE_DYNDRIVERS
  #if defined(aqt_EXPORTS) || defined(cairo_EXPORTS) || defined(cgm_EXPORTS) || \
       defined(dg300_EXPORTS) || defined(gd_EXPORTS) || defined(gcw_EXPORTS) || \
       defined(hpgl_EXPORTS) || defined(impress_EXPORTS) || defined(linuxvga_EXPORTS) || \
       defined(ljii_EXPORTS) || defined(ljiip_EXPORTS) || defined(mem_EXPORTS) || \
       defined(ntk_EXPORTS) || defined(null_EXPORTS) || defined(pbm_EXPORTS) || \
       defined(pdf_EXPORTS) || defined(plmeta_EXPORTS) || defined(ps_EXPORTS) || \
       defined(pstex_EXPORTS) || defined(psttf_EXPORTS) || defined(svg_EXPORTS) || \
       defined(tek_EXPORTS) || defined(tk_EXPORTS) || defined(tkwin_EXPORTS) || \
       defined(wingcc_EXPORTS) || defined(wxwidgets_EXPORTS) || defined(xfig_EXPORTS) || \
       defined(xwin_EXPORTS) || defined(qt_EXPORTS)
    #define PLDLLIMPEXP_DRIVER PLDLLEXPORT
    #define PLDLLIMPEXP_DRIVER_DATA(type) PLDLLEXPORT type
  #else
    #define PLDLLIMPEXP_DRIVER PLDLLIMPORT
    #define PLDLLIMPEXP_DRIVER_DATA(type) PLDLLIMPORT type
  #endif
#else
  #define PLDLLIMPEXP_DRIVER PLDLLIMPEXP
  #define PLDLLIMPEXP_DRIVER_DATA(type) PLDLLIMPEXP_DATA(type)
#endif

#if defined(plplotcxxd_EXPORTS)
  #define PLDLLIMPEXP_CXX PLDLLEXPORT
  #define PLDLLIMPEXP_CXX_DATA(type) PLDLLEXPORT type
#else
  #define PLDLLIMPEXP_CXX PLDLLIMPORT
  #define PLDLLIMPEXP_CXX_DATA(type) PLDLLIMPORT type
#endif

#if defined(plplotf77cd_EXPORTS)
  #define PLDLLIMPEXP_F77C PLDLLEXPORT
  #define PLDLLIMPEXP_F77C_DATA(type) PLDLLEXPORT type
#else
  #define PLDLLIMPEXP_F77C PLDLLIMPORT
  #define PLDLLIMPEXP_F77C_DATA(type) PLDLLIMPORT type
#endif

#if defined(plplotf95cd_EXPORTS)
  #define PLDLLIMPEXP_F95C PLDLLEXPORT
  #define PLDLLIMPEXP_F95C_DATA(type) PLDLLEXPORT type
#else
  #define PLDLLIMPEXP_F95C PLDLLIMPORT
  #define PLDLLIMPEXP_F95C_DATA(type) PLDLLIMPORT type
#endif

#if defined(plplotwxwidgetsd_EXPORTS)
  #define PLDLLIMPEXP_WX PLDLLEXPORT
  #define PLDLLIMPEXP_WX_DATA(type) PLDLLEXPORT type
#else
  #define PLDLLIMPEXP_WX PLDLLIMPORT
  #define PLDLLIMPEXP_WX_DATA(type) PLDLLIMPORT type
#endif

#if defined(tclmatrixd_EXPORTS)
  #define PLDLLIMPEXP_TCLMAT PLDLLEXPORT
  #define PLDLLIMPEXP_TCLMAT_DATA(type) PLDLLEXPORT type
#else
  #define PLDLLIMPEXP_TCLMAT PLDLLIMPORT
  #define PLDLLIMPEXP_TCLMAT_DATA(type) PLDLLIMPORT type
#endif

#if defined(plplottcltkd_EXPORTS)
  #define PLDLLIMPEXP_TCLTK PLDLLEXPORT
  #define PLDLLIMPEXP_TCLTK_DATA(type) PLDLLEXPORT type
#else
  #define PLDLLIMPEXP_TCLTK PLDLLIMPORT
  #define PLDLLIMPEXP_TCLTK_DATA(type) PLDLLIMPORT type
#endif

#if defined(plplotgnome2d_EXPORTS)
  #define PLDLLIMPEXP_GNOME2 PLDLLEXPORT
  #define PLDLLIMPEXP_GNOME2_DATA(type) PLDLLEXPORT type
#else
  #define PLDLLIMPEXP_GNOME2 PLDLLIMPORT
  #define PLDLLIMPEXP_GNOME2_DATA(type) PLDLLIMPORT type
#endif

#if defined(cplplotcanvasmodule_EXPORTS)
  #define PLDLLIMPEXP_CPLPLOTCANVASMODULE PLDLLEXPORT
  #define PLDLLIMPEXP_CPLPLOTCANVASMODULE_DATA(type) PLDLLEXPORT type
#else
  #define PLDLLIMPEXP_CPLPLOTCANVASMODULE PLDLLIMPORT
  #define PLDLLIMPEXP_CPLPLOTCANVASMODULE_DATA(type) PLDLLIMPORT type
#endif

#if defined(plplot_widgetmodule_EXPORTS)
  #define PLDLLIMPEXP_PLPLOT_WIDGETMODULE PLDLLEXPORT
  #define PLDLLIMPEXP_PLPLOT_MODULE_DATA(type) PLDLLEXPORT type
#else
  #define PLDLLIMPEXP_PLPLOT_MODULE PLDLLIMPORT
  #define PLDLLIMPEXP_PLPLOT_MODULE_DATA(type) PLDLLIMPORT type
#endif

#endif /* __PL_DLL_H */
