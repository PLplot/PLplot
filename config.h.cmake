/* Generated from configure.ac by autoheader as of 2006-07-08
 * and converted to cmake form.  cmake has no equivalent to
 * autoheader yet so this file must be maintained manually from
 * now on, i.e., if there is an addition of a configurable symbol,
 * (all done from the top-level CMakeLists.txt file)
 * it must be entered here as well in order for C/C++ programmes
 * to have access to the configured symbol.*/

/* Location of executables */
#define BIN_DIR "${BIN_DIR}"

/* isnan is in math.h but not cmath */
#cmakedefine BROKEN_ISNAN_CXX

/* Location of Build tree */
#define BUILD_DIR "${BUILD_DIR}"

/* Location of package data files */
#define DATA_DIR "${DATA_DIR}"

/* Location of dynamically loaded drivers */
#define DRV_DIR "${DRV_DIR}"

/* Define if there is support for dynamically loaded drivers */
#cmakedefine ENABLE_DYNDRIVERS

/* Define to 1 if you have the <cmath> header file. */
#cmakedefine HAVE_CMATH 1

/* Define if you have c++ accessible stdint.h */
#cmakedefine HAVE_CXX_STDINT_H

/* Define to 1 if you have the <dirent.h> header file, and it defines `DIR'.
   */
#cmakedefine HAVE_DIRENT_H 1

/* Define to 1 if you have the <dlfcn.h> header file. */
#cmakedefine HAVE_DLFCN_H 1

/* Define if finite is available */
#cmakedefine HAVE_FINITE

/* Define if [freetype] is available */
#cmakedefine HAVE_FREETYPE

/* Define if [agg] is available */
#cmakedefine HAVE_AGG

/* Define to 1 if you have the <glib.h> header file. */
#cmakedefine HAVE_GLIB_H 1

/* Define to 1 if you have the <glib-object.h> header file. */
#cmakedefine HAVE_GLIB_OBJECT_H 1

/* Define to 1 if you have the <gtk/gtk.h> header file. */
#cmakedefine HAVE_GTK_GTK_H 1

/* Define to 1 if you have the <inttypes.h> header file. */
#cmakedefine HAVE_INTTYPES_H 1

/* Define if isinf is available */
#cmakedefine HAVE_ISINF

/* Define if isnan is available */
#cmakedefine HAVE_ISNAN

/* Define if [incr], [Tcl] is available */
#cmakedefine HAVE_ITCL

/* Define to 1 if you have the <itclDecls.h> header file. */
#cmakedefine HAVE_ITCLDECLS_H 1

/* Define if Tk is available */
#cmakedefine ENABLE_tk

/* Define if [incr], [Tk] is available */
#cmakedefine HAVE_ITK

/* Define to 1 if you have the <jni.h> header file. */
#cmakedefine HAVE_JNI_H 1

/* Define to 1 if you have the <libart_lgpl/libart.h> header file. */
#cmakedefine HAVE_LIBART_LGPL_LIBART_H 1

/* Define to 1 if you have the <libgnomecanvas/libgnomecanvas.h> header file.
   */
#cmakedefine HAVE_LIBGNOMECANVAS_LIBGNOMECANVAS_H 1

/* Define to 1 if you have the <libgnomeprint/gnome-print.h> header file. */
#cmakedefine HAVE_LIBGNOMEPRINT_GNOME_PRINT_H 1

/* Define if libunicode is available */
#cmakedefine HAVE_LIBUNICODE

/* Define to 1 if you have the <math.h> header file. */
#cmakedefine HAVE_MATH_H 1

/* Define to 1 if you have the <memory.h> header file. */
#cmakedefine HAVE_MEMORY_H 1

/* Define to 1 if you have the <ndir.h> header file, and it defines `DIR'. */
#cmakedefine HAVE_NDIR_H 1

/* Define if python numpy is available */
#cmakedefine HAVE_NUMPY

/* Define if libpango is available */
#cmakedefine HAVE_PANGO

/* Define if popen is available */
#cmakedefine HAVE_POPEN

/* Define if pthreads is available */
#cmakedefine HAVE_PTHREAD

/* Define if Qhull is available */
#cmakedefine HAVE_QHULL

/* Define to 1 if you have the <stdint.h> header file. */
#cmakedefine HAVE_STDINT_H 1

/* Define to 1 if you have the <stdlib.h> header file. */
#cmakedefine HAVE_STDLIB_H 1

/* Define to 1 if you have the <sys/dir.h> header file, and it defines `DIR'.
   */
#cmakedefine HAVE_SYS_DIR_H 1

/* Define to 1 if you have the <sys/ndir.h> header file, and it defines `DIR'.
   */
#cmakedefine HAVE_SYS_NDIR_H 1

/* Define to 1 if you have the <sys/stat.h> header file. */
#cmakedefine HAVE_SYS_STAT_H 1

/* Define to 1 if you have the <sys/types.h> header file. */
#cmakedefine HAVE_SYS_TYPES_H 1

/* Define to 1 if you have <sys/wait.h> that is POSIX.1 compatible. */
#cmakedefine HAVE_SYS_WAIT_H 1

/* Define to 1 if you have the <termios.h> header file. */
#cmakedefine HAVE_TERMIOS_H 1

/* Define to 1 if you have the <unistd.h> header file. */
#cmakedefine HAVE_UNISTD_H 1

/* Define if usleep is available */
#cmakedefine HAVE_USLEEP

/* Define to 1 if you have the `vfork' function. */
#cmakedefine HAVE_VFORK 1

/* Define to 1 if you have the <vfork.h> header file. */
#cmakedefine HAVE_VFORK_H 1

/* Include sys/type.h if needed */
#cmakedefine NEED_SYS_TYPE_H

/* Name of package */
#define PACKAGE "${PACKAGE}"

/* Define if aqt driver is present */
#cmakedefine PLD_aqt

/* Define if cgm driver is present */
#cmakedefine PLD_cgm

/* Define if conex driver is present */
#cmakedefine PLD_conex

/* Define if dg300 driver is present */
#cmakedefine PLD_dg300

/* Define if gcw driver is present */
#cmakedefine PLD_gcw

/* Define if gif driver is present */
#cmakedefine PLD_gif

/* Define if gnome driver is present */
#cmakedefine PLD_gnome

/* Define if hp7470 driver is present */
#cmakedefine PLD_hp7470

/* Define if hp7580 driver is present */
#cmakedefine PLD_hp7580

/* Define if imp driver is present */
#cmakedefine PLD_imp

/* Define if jpeg driver is present */
#cmakedefine PLD_jpeg

/* Define if linuxvga driver is present */
#cmakedefine PLD_linuxvga

/* Define if lj_hpgl driver is present */
#cmakedefine PLD_lj_hpgl

/* Define if ljii driver is present */
#cmakedefine PLD_ljii

/* Define if ljiip driver is present */
#cmakedefine PLD_ljiip

/* Define if mem driver is present */
#cmakedefine PLD_mem

/* Define if mskermit driver is present */
#cmakedefine PLD_mskermit

/* Define if ntk driver is present */
#cmakedefine PLD_ntk

/* Define if null driver is present */
#cmakedefine PLD_null

/* Define if pbm driver is present */
#cmakedefine PLD_pbm

/* Define if pdf driver is present */
#cmakedefine PLD_pdf

/* Define if Cairo PDF driver is present */
#cmakedefine PLD_pdfcairo

/* Define if plmeta driver is present */
#cmakedefine PLD_plmeta

/* Define if Cairo memory driver is present */
#cmakedefine PLD_memcairo

/* Define if png driver is present */
#cmakedefine PLD_png

/* Define if Cairo PNG driver is present */
#cmakedefine PLD_pngcairo

/* Define if ps driver is present */
#cmakedefine PLD_ps

/* Define if Cairo PS driver is present */
#cmakedefine PLD_pscairo

/* Define if pstex driver is present */
#cmakedefine PLD_pstex

/* Define if psttf driver is present */
#cmakedefine PLD_psttf

/* Define if svg driver is present */
#cmakedefine PLD_svg

/* Define if Cairo SVG driver is present */
#cmakedefine PLD_svgcairo

/* Define if tek4010 driver is present */
#cmakedefine PLD_tek4010

/* Define if tek4010f driver is present */
#cmakedefine PLD_tek4010f

/* Define if tek4107 driver is present */
#cmakedefine PLD_tek4107

/* Define if tek4107f driver is present */
#cmakedefine PLD_tek4107f

/* Define if tk driver is present */
#cmakedefine PLD_tk

/* Define if tkwin driver is present */
#cmakedefine PLD_tkwin

/* Define if versaterm driver is present */
#cmakedefine PLD_versaterm

/* Define if vlt driver is present */
#cmakedefine PLD_vlt

/* Define if wingcc driver is present */
#cmakedefine PLD_wingcc

/* Define if wxwidgets driver is present */
#cmakedefine PLD_wxwidgets

/* Define if the cairo X windows driver is present */
#cmakedefine PLD_xcairo

/* Define if xfig driver is present */
#cmakedefine PLD_xfig

/* Define if xterm driver is present */
#cmakedefine PLD_xterm

/* Define if xwin driver is present */
#cmakedefine PLD_xwin

/* Portable definition for PTHREAD_MUTEX_RECURSIVE */
#define PLPLOT_MUTEX_RECURSIVE ${PLPLOT_MUTEX_RECURSIVE}

/* PLplot's PLFLT floating-point is "double" by default, undefine PL_DOUBLE if
   single precision is required */
#cmakedefine PL_DOUBLE

/* Directory containing fonts that are accessible from freetype */
#define PL_FREETYPE_FONT_DIR "${PL_FREETYPE_FONT_DIR}"

/* MONO font accessible from freetype */
#define PL_FREETYPE_MONO "${PL_FREETYPE_MONO}"

/* MONO_BOLD font accessible from freetype */
#define PL_FREETYPE_MONO_BOLD "${PL_FREETYPE_MONO_BOLD}"

/* MONO_BOLD_ITALIC font accessible from freetype */
#define PL_FREETYPE_MONO_BOLD_ITALIC "${PL_FREETYPE_MONO_BOLD_ITALIC}"

/* MONO_BOLD_OBLIQUE font accessible from freetype */
#define PL_FREETYPE_MONO_BOLD_OBLIQUE "${PL_FREETYPE_MONO_BOLD_OBLIQUE}"

/* MONO_ITALIC font accessible from freetype */
#define PL_FREETYPE_MONO_ITALIC "${PL_FREETYPE_MONO_ITALIC}"

/* MONO_OBLIQUE font accessible from freetype */
#define PL_FREETYPE_MONO_OBLIQUE "${PL_FREETYPE_MONO_OBLIQUE}"

/* SANS font accessible from freetype */
#define PL_FREETYPE_SANS "${PL_FREETYPE_SANS}"

/* SANS_BOLD font accessible from freetype */
#define PL_FREETYPE_SANS_BOLD "${PL_FREETYPE_SANS_BOLD}"

/* SANS_BOLD_ITALIC font accessible from freetype */
#define PL_FREETYPE_SANS_BOLD_ITALIC "${PL_FREETYPE_SANS_BOLD_ITALIC}"

/* SANS_BOLD_OBLIQUE font accessible from freetype */
#define PL_FREETYPE_SANS_BOLD_OBLIQUE "${PL_FREETYPE_SANS_BOLD_OBLIQUE}"

/* SANS_ITALIC font accessible from freetype */
#define PL_FREETYPE_SANS_ITALIC "${PL_FREETYPE_SANS_ITALIC}"

/* SANS_OBLIQUE font accessible from freetype */
#define PL_FREETYPE_SANS_OBLIQUE "${PL_FREETYPE_SANS_OBLIQUE}"

/* SCRIPT font accessible from freetype */
#define PL_FREETYPE_SCRIPT "${PL_FREETYPE_SCRIPT}"

/* SCRIPT_BOLD font accessible from freetype */
#define PL_FREETYPE_SCRIPT_BOLD "${PL_FREETYPE_SCRIPT_BOLD}"

/* SCRIPT_BOLD_ITALIC font accessible from freetype */
#define PL_FREETYPE_SCRIPT_BOLD_ITALIC "${PL_FREETYPE_SCRIPT_BOLD_ITALIC}"

/* SCRIPT_BOLD_OBLIQUE font accessible from freetype */
#define PL_FREETYPE_SCRIPT_BOLD_OBLIQUE "${PL_FREETYPE_SCRIPT_BOLD_OBLIQUE}"

/* SCRIPT_ITALIC font accessible from freetype */
#define PL_FREETYPE_SCRIPT_ITALIC "${PL_FREETYPE_SCRIPT_ITALIC}"

/* SCRIPT_OBLIQUE font accessible from freetype */
#define PL_FREETYPE_SCRIPT_OBLIQUE "${PL_FREETYPE_SCRIPT_OBLIQUE}"

/* SERIF font accessible from freetype */
#define PL_FREETYPE_SERIF "${PL_FREETYPE_SERIF}"

/* SERIF_BOLD font accessible from freetype */
#define PL_FREETYPE_SERIF_BOLD "${PL_FREETYPE_SERIF_BOLD}"

/* SERIF_BOLD_ITALIC font accessible from freetype */
#define PL_FREETYPE_SERIF_BOLD_ITALIC "${PL_FREETYPE_SERIF_BOLD_ITALIC}"

/* SERIF_BOLD_OBLIQUE font accessible from freetype */
#define PL_FREETYPE_SERIF_BOLD_OBLIQUE "${PL_FREETYPE_SERIF_BOLD_OBLIQUE}"

/* SERIF_ITALIC font accessible from freetype */
#define PL_FREETYPE_SERIF_ITALIC "${PL_FREETYPE_SERIF_ITALIC}"

/* SERIF_OBLIQUE font accessible from freetype */
#define PL_FREETYPE_SERIF_OBLIQUE "${PL_FREETYPE_SERIF_OBLIQUE}"

/* Symbol font accessible from freetype */
#define PL_FREETYPE_SYMBOL "${PL_FREETYPE_SYMBOL}"

/* SYMBOL_BOLD font accessible from freetype */
#define PL_FREETYPE_SYMBOL_BOLD "${PL_FREETYPE_SYMBOL_BOLD}"

/* SYMBOL_BOLD_ITALIC font accessible from freetype */
#define PL_FREETYPE_SYMBOL_BOLD_ITALIC "${PL_FREETYPE_SYMBOL_BOLD_ITALIC}"

/* SYMBOL_BOLD_OBLIQUE font accessible from freetype */
#define PL_FREETYPE_SYMBOL_BOLD_OBLIQUE "${PL_FREETYPE_SYMBOL_BOLD_OBLIQUE}"

/* SYMBOL_ITALIC font accessible from freetype */
#define PL_FREETYPE_SYMBOL_ITALIC "${PL_FREETYPE_SYMBOL_ITALIC}"

/* SYMBOL_OBLIQUE font accessible from freetype */
#define PL_FREETYPE_SYMBOL_OBLIQUE "${PL_FREETYPE_SYMBOL_OBLIQUE}"

/* Define as the return type of signal handlers (`int' or `void'). */
#define RETSIGTYPE ${RETSIGTYPE}

/* Define to 1 if you have the ANSI C header files. */
#cmakedefine STDC_HEADERS 1

/* Location of Tcl stuff */
#define TCL_DIR "${TCL_DIR}"

/* Define if C++ compiler accepts using namespace */
#cmakedefine USE_NAMESPACE

/* Version number of package */
#define VERSION "${VERSION}"

/* Define if csa is desired */
#cmakedefine WITH_CSA

/* Define to `char *' if <sys/types.h> does not define. */
#cmakedefine caddr_t

/* Define to `int' if <sys/types.h> does not define. */
#cmakedefine pid_t

/* Define as `fork' if `vfork' does not work. */
#cmakedefine vfork
