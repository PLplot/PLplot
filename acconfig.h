/* -*-C-*- */
/* $Id$

    Maurice LeBrun
    IFS, University of Texas at Austin
    18-Jul-1994

    Contains macro definitions that determine miscellaneous PLplot library
    configuration defaults, such as macros for bin, font, lib, and tcl
    install directories, and various system dependencies.  On a Unix
    system, typically the configure script builds plConfig.h from
    plConfig.h.in.  Elsewhere, it's best to hand-configure a plConfig.h
    file and keep it with the system-specific files.
*/

#ifndef __PLCONFIG_H__
#define __PLCONFIG_H__

/* Define to `int' if <sys/types.h> doesn't define.  */
#undef pid_t

/* Define to `char *' if <sys/types.h> doesn't define.  */
#undef caddr_t

/* Define as the return type of signal handlers (int or void).  */
#undef RETSIGTYPE

/* Define vfork as fork if vfork does not work.  */
#undef vfork

/* Define if popen is available.  */
#undef HAVE_POPEN

/* Define if we're using a debugging malloc */
#undef DEBUGGING_MALLOC

/* If you don't know what this is for, you shouldn't be using it */
#undef NOBRAINDEAD

/* Define if dynamic drivers are enabled.*/
#undef ENABLE_DYNDRIVERS

/* Define if drivers database is specified.*/
#undef DRIVERS_DB

/* Define if [incr Tcl] is available */
#undef HAVE_ITCL

/* Define if [incr Tk] is available */
#undef HAVE_ITK

/* Define if [freetype] is available */
#undef HAVE_FREETYPE

/* Define if you want PLplot's float type to be double */
#undef PL_DOUBLE

/* Install directories. */

#undef LIB_DIR
#undef DATA_DIR
#undef BIN_DIR
#undef TCL_DIR

#endif	/* __PLCONFIG_H__ */

#undef PLD_cgm
#undef PLD_conex
#undef PLD_dg300
#undef PLD_gnome
#undef PLD_hp7470
#undef PLD_hp7580
#undef PLD_imp
#undef PLD_jpeg
#undef PLD_linuxvga
#undef PLD_lj_hpgl
#undef PLD_ljii
#undef PLD_ljiip
#undef PLD_mskermit
#undef PLD_ntk
#undef PLD_null
#undef PLD_pbm
#undef PLD_plmeta
#undef PLD_png
#undef PLD_ps
#undef PLD_psc
#undef PLD_pstex
#undef PLD_tek4010
#undef PLD_tek4010f
#undef PLD_tek4107
#undef PLD_tek4107f
#undef PLD_tkwin
#undef PLD_tk
#undef PLD_versaterm
#undef PLD_vlt
#undef PLD_xfig
#undef PLD_xterm
#undef PLD_xwin
