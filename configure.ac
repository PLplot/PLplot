dnl configure.ac for PLplot
dnl
dnl Process this file with autoconf to produce a configure script.
dnl
dnl Copyright (C) 1994, 2004  Maurice LeBrun
dnl Copyright (C) 2002, 2003, 2004  Rafael Laboissiere
dnl Copyright (C) 2002, 2003, 2004  Alan W. Irwin
dnl Copyright (C) 2003, 2004  Joao Cardoso
dnl
dnl This file is part of PLplot.
dnl
dnl PLplot is free software; you can redistribute it and/or modify
dnl it under the terms of the GNU Library General Public License as published
dnl by the Free Software Foundation; version 2 of the License.
dnl
dnl PLplot is distributed in the hope that it will be useful,
dnl but WITHOUT ANY WARRANTY; without even the implied warranty of
dnl MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
dnl GNU Library General Public License for more details.
dnl
dnl You should have received a copy of the GNU Library General Public License
dnl along with the file PLplot; if not, write to the Free Software
dnl Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307  USA
dnl
dnl Note: to recover legacy behavior as regards (site-specific) defaults
dnl files, configure using:
dnl
dnl $ CONFIG_SITE=~/config/cf_plplot.in ./configure

dnl Autoconf/automake initializations

dnl Package version number
dnl [ If changing the line below by hand,
dnl   please keep the format up to and including the
dnl   version number, otherwise it will fool up the bootstrap.sh script.  By
dnl   the way, the line below can be changed using options --version and
dnl   --date-version of that script. ]
AC_INIT(plplot, 5.3.0.cvs.20040211, plplot-devel@lists.sourceforge.net)
AC_CONFIG_SRCDIR(src/plcore.c)
AC_REVISION($Revision$)

### Use cf dir instead of the top dir for all autotools-related files
AC_CONFIG_AUX_DIR(cf)
AC_CONFIG_MACRO_DIR(cf)

### Configuration headers

AC_CONFIG_HEADERS([config.h include/plConfig.h include/plDevs.h])

### Automake initialization
AM_INIT_AUTOMAKE

### Date of release.  This can be set by hand, but the prefered method
### is by specifying one of the option --version of --date-version to
### bootstrap.sh

RELEASE_DATE=2004-02-05
AC_SUBST(RELEASE_DATE)

### Version number for the shared libraries.  Note we are keeping the shared
### library version consistent with the package version in direct
### contradiction to the libtool directive on this.  Thus, these numbers
### don't mean a lot in terms of binary compatibility.  Someday, we should
### start over with 0:0:0 and rigourously enforce number changes whenever
### binary compatibility changes. [N. B. (Rafael): we already departed from
### this rule for the release 5.2.1 of PLplot, which got a library soversion
### 5.3.0.]

### N.B. The "secret" libtool code for setting 5.y.x for the numerical
### suffix of the library version is 5+y:x:y.  Thus 8:0:3 ==> 5.3.0.
### N.B. [Rafael]: The above statement is not generally true.  In HPUX, for
### instance, 8:0:3 ==> 8.3.

SOVERSION=9:0:0
AC_SUBST(SOVERSION)

### Version number for the shared C++ library.  After version 5.2.1 was
### released, the C++ bindings were changed and backward incompatibilities
### were introduced.  Hence the different strings for SOVERSION and
### CPP_SOVERSION.

CPP_SOVERSION=9:0:0
AC_SUBST(CPP_SOVERSION)

### According to man package, PLPLOT_TCL_VERSION (used in the various
### pkgIndex.tcl scripts) should be strictly numbers alternativing with
### decimal points with the first number (the major number) signifying
### incompatible changes in the scripts.  To keep our life simple, let us
### use the major number from the package version for now.
### N.B. both LIBRARY_VERSION and PLPLOT_TCL_VERSION are used for separate
### purposes in the various configured pkgIndex.tcl scripts.

PLPLOT_TCL_VERSION=5
AC_SUBST(PLPLOT_TCL_VERSION)

AM_PROG_CC_STDC

### If the user don't want (or the system is not capable of) to build
### shared libraries, then disable dynamic drivers, as they are
### dynamically loaded objects.

if test -z "$enable_shared" ; then enable_shared=yes ; fi

if test "$enable_shared" != yes ; then
    AC_MSG_WARN([Dynamic drivers require enable-shared=yes,])
    AC_MSG_WARN([setting enable_dyndrivers=no])
    enable_dyndrivers=no
fi

### Libtool settings

PL_ARG_WITH(ltdlsystem,
  [Use installed version of libltdl library (if it exists), instead of
   building the convenience version shipped with the sources],
  no)

PL_ARG_ENABLE(dyndrivers, [enable dynamic loading of drivers], yes)

if test "$enable_dyndrivers" = "yes"; then
  AC_LIBTOOL_DLOPEN
  AC_CHECK_LIB(ltdl, lt_dlinit, [libltdl_in_system=yes],
    [libltdl_in_system=no])
  if test "$with_ltdlsystem" = no -o "$libltdl_in_system" = no ; then
    with_ltdlsystem=no
    AC_LIBLTDL_CONVENIENCE
    AC_CONFIG_SUBDIRS(libltdl)
  else
    AC_LIBLTDL_INSTALLABLE
  fi
fi
AC_SUBST(INCLTDL)
AC_SUBST(LIBLTDL)

AM_CONDITIONAL(with_ltdlsystem, [test "$with_ltdlsystem" = yes])

# This required for dll support under cygwin.  Must occur before
# AM_PROG_LIBTOOL
AC_LIBTOOL_WIN32_DLL

AM_PROG_LIBTOOL

PL_ARG_WITH(freetype,
  [enable driver options for using freetype library for fonts],
  yes)

# use pthreads with the xwin driver

PL_ARG_WITH(pthreads, [use pthreads with the xwin driver], no)

PL_ARG_WITH(qhull, [enable use of the Qhull library], yes)
PL_ARG_WITH(csa, [enable use of the csa library], yes)

PL_ARG_ENABLE(drivers, [enable all device drivers], yes)

# Set driver enable values
#
# You can enable/disable drivers either by the command line
# (--enable-<driver> or --disable-<driver>) or via the cf_plplot.in file
# (remember to use underscores instead of dashes here).  You can disable
# all drivers by default by using --disable-drivers.

# Special cases

case $system in
    Linux* )
	if test -z "$enable_linuxvga"; then
 	   enable_linuxvga=$enable_drivers
	fi
    ;;
    * )
	enable_linuxvga="no"
    ;;
esac

# ----------------------------------------------------------------------------
# Set flag to enable searching for dynloadable drivers, if appropriate.
# ----------------------------------------------------------------------------

if test "$enable_dyndrivers" = "yes"; then
    AC_DEFINE(ENABLE_DYNDRIVERS, [],
              [Define if there is support for dynamically loaded drivers])
fi

### define X_CFLAGS, X_PRE_LIBS, X_LIBS, and X_EXTRA_LIBS
### must occur before sysloc.in is included since sysloc.in logic depends
### on it.

AC_PATH_XTRA

### Add libm to LIBS, as some non linux unices might require this.
### This must be done before sysloc.in is included.
AC_CHECK_LIB(m, sin)

### The following seems to be necessary because the replacement of
### the default value for prefix only happens in AC_OUTPUT (it would
### already be too late).
test "x$prefix" = xNONE && prefix=$ac_default_prefix

### Python binding

PL_ARG_ENABLE(python, [compile Python interface code], yes)

if test "$enable_python" = "yes"; then
    ### Redefine AC_MSG_ERROR as AC_MSG_WARN around the call to
    ### AM_PATH_PYTHON, such that if python is not found, configure will
    ### not stop.
    pushdef([AC_MSG_ERROR],defn([AC_MSG_WARN]))
    AM_PATH_PYTHON(1.5)
    popdef([AC_MSG_ERROR])
    if test -n "$PYTHON_VERSION" ; then
### PYTHON_INSTDIR is the same as $pythondir except that the
### symbolic prefix is replaced by the actual prefix.
### The actual prefix is required in examples/python/plplot_python_start.py
### (the only place that PYTHON_INSTDIR is used).
        PL_EXPAND_EXPRESSION(PYTHON_INSTDIR, $pythondir)
    fi
fi
AC_SUBST(PYTHON_INSTDIR)

### Drivers

# ------------------------------------------------------------------------
# Support macros for driver inclusion and help entries.

# Including a driver in the below list includes it by default.
# Note that each driver should be treated separately even if
# they are in the same source code file.
# For example, the ps and psc drivers are defined in the ps.c source
# code file.

# N.B. This is ordered the same as the *driver pair* list below to help
# humans keep the associations straight, but this is not necessary.

# Note specifically which drivers are known to be loadable.  Eventually,
# hopefully, every driver will show up on this list.  However, we add them one
# at a time since each will have its own peculiarities in the build process
# (none missing at present).

# N.B. This is ordered the same as the *driver pair* list below to help
# humans keep the associations straight, but this is not necessary.

# We think of a "device" as the output medium.  Could be a machine
# style (Tektronix, X11), or a file format (Postscript).
#
# A "driver" is the PLplot code that produces the output on the
# medium.
#
# Some drivers support multiple devices.  But we we need to make sure
# we compile only the selected drivers.  That is, if a driver supports
# devices not selected, we don't want to compile it at all.  Or if it
# supports multiple selected devices, we only want to compile it
# once.  To support this, we build an association list between devices
# and drivers.  This will be inspected to build up the set of drivers
# to be compiled based on which devices are selected.

# Ordered alphabetically by second in each pair (the driver) for human
# consumption, but this is not necessary.

AC_MSG_CHECKING(for dynamic drivers)
AC_DEFUN([PL_DRIVERS_DEVICE_LIST],
  [cgm:cgm:yes,
  dg300:dg300:no,
  png:gd:yes, jpeg:gd:yes,
  gnome:gnome:no,
  hp7470:hpgl:yes, hp7580:hpgl:yes, lj_hpgl:hpgl:yes,
  imp:impress:no,
  linuxvga:linuxvga:yes,
  ljii:ljii:no,
  ljiip:ljiip:no,
  mem:mem:yes,
  ntk:ntk:no,
  null:null:yes,
  pbm:pbm:yes,
  plmeta:plmeta:yes,
  ps:ps:yes, psc:ps:yes,
  pstex:pstex:yes,
  xterm:tek:no, tek4010:tek:no, tek4107:tek:no, mskermit:tek:no,
  versaterm:tek:no, vlt:tek:no, conex:tek:no, tek4010f:tek:no,
  tek4107f:tek:no,
  tk:tk:yes,
  tkwin:tkwin:yes,
  xfig:xfig:yes,
  xwin:xwin:yes])
PL_ARG_ENABLE_DRIVERS(PL_DRIVERS_DEVICE_LIST)
AC_MSG_RESULT
AC_SUBST(STATIC_DRIVERS)
AC_SUBST(DYNAMIC_DRIVERS)

### Modify system-dependent results based on availability
PL_INCLUDE(sysloc.in)

PL_ADD_DRIVERS(PL_DRIVERS_DEVICE_LIST)

# Disable LIBS so we don't have all libraries mentioned
LIBS=""

# Optional packages

# Need to AC_SUBST some of these for plplot-test.sh script.
AC_SUBST(enable_cxx)
AC_SUBST(enable_f77)
AC_SUBST(enable_python)
AC_SUBST(enable_java)
AC_SUBST(enable_tcl)
AC_SUBST(enable_octave)

### Create list of valid examples subdirectories

EXAMPLES_DIRECTORIES=c
if test "$enable_cxx" = yes; then
   EXAMPLES_DIRECTORIES="$EXAMPLES_DIRECTORIES c++"
fi
if test "$enable_f77" = yes; then
   EXAMPLES_DIRECTORIES="$EXAMPLES_DIRECTORIES f77"
fi
if test "$enable_tk" = yes; then
   EXAMPLES_DIRECTORIES="$EXAMPLES_DIRECTORIES tk"
fi
AC_SUBST(EXAMPLES_DIRECTORIES)

### tcl binding

PL_ARG_ENABLE(tcl, [compile Tcl interface code], yes)

PL_ARG_ENABLE(itcl, [enable incr Tcl interface code], yes)

# ----------------------------------------------------------------------------
# Make some guesses where the Tcl/Tk include files and libraries are.
# Look first in the Tcl/Tk distributions and then move on to the standard
# include file/library locations.  If it's still not found I assume it's
# not here.
#
# Note: it is important to check in user directories _before_ checking
# anywhere else.  That way you can easily compile custom versions (of Tcl, Tk,
# etc), put them in your file space, and have them be found first.  Some
# examples: libraries compiled with debugging turned on, or from a different
# revision of the (Tcl, Tk, etc) package.  Also, when dealing with multiple
# versions, check for the newer version first.
# ----------------------------------------------------------------------------

# Default directories to search for include files

# Linux has a peculiar ambiguity where identical tcl.h files are found both in
# /usr/include and /usr/include/tcl<version>/generic.  The latter is
# *much* preferred because it has everything else as well having to do with tcl.
# So the generic include site is first on the list.  Note that prefix can be
# "/usr" which would give bad result under Linux without this ordering.

incdirs_default="\
/usr/include/tcl8.4/tcl-private/generic \
/usr/include/tcl8.4/tk-private/generic \
/usr/include/tcl8.4/itcl-private/generic \
/usr/include/tcl8.4 \
/usr/include/tcl8.3/tcl-private/generic \
/usr/include/tcl8.3/tk-private/generic \
/usr/include/tcl8.3/itcl-private/generic \
/usr/include/tcl8.3 \
/usr/include/tcl8.2/generic \
$prefix/include/itcl \
/usr/include/itcl3.1/generic/ \
$prefix/include \
$HOME/local/include \
$HOME/include \
/usr/local/include \
/usr/include/tcl \
/usr/include"

# Default directories to search for library files

libdirs_default="\
$prefix/lib/itcl \
$prefix/lib \
$HOME/local/lib \
$HOME/lib \
/usr/local/lib \
/usr/lib"

incdirs=$incdirs_default
libdirs=$libdirs_default

if test -n "$with_pkgdir"; then
    incdirs="$with_pkgdir/include $incdirs"
    libdirs="$with_pkgdir/lib $libdirs"
fi

# ----------------------------------------------------------------------------
# tcl.h
# ----------------------------------------------------------------------------

if test "$enable_tcl" = "yes"; then
    if test -z "$TCLINCDIR"; then
        PL_FIND_INC(tcl, TCLINCDIR, enable_tcl)
    fi
fi

# ----------------------------------------------------------------------------
# libtcl
#
# Includes checks against old versions of tcl no longer supported.
# But you can *try* linking against ANY non-versioned tcl.
# ----------------------------------------------------------------------------

if test "$enable_tcl" = "yes"; then
    if test -z "$TCLLIBDIR"; then
        PL_FIND_LIB_BEGIN(tcl, TCLLIBDIR)
        for version in 8.4 8.3 8.2 8.1 8.0 ""; do
            PL_FIND_LIB_SET(tcl$version, TCLLIBDIR, TCLLIBSTR)
            if test -n "$TCLLIBDIR"; then
                break
            fi
        done
        if test -n "$version"; then
            for oldversion in 8.1 8.0; do
                if test "$version" = "$oldversion"; then
                    AC_MSG_WARN([Tcl version $oldversion not supported.])
		    AC_MSG_WARN([Please upgrade!])
                    enable_tcl="no"
                    break
                fi
            done
        fi
        PL_FIND_LIB_END(tcl, TCLLIBDIR, enable_tcl)
    fi
    if test "$enable_tcl" = "yes"; then
        if test -r $TCLLIBDIR/tclConfig.sh; then
            . $TCLLIBDIR/tclConfig.sh
        fi
    fi
fi

# ----------------------------------------------------------------------------
# itcl.h
# ----------------------------------------------------------------------------

if test "$enable_tcl" = "no"; then
    enable_itcl=no
fi

if test "$enable_itcl" = "yes"; then
    if test -z "$ITCLINCDIR"; then
        PL_FIND_INC(itcl, ITCLINCDIR, enable_itcl)
    fi
fi

# There should be an itclDecls.h with the other itcl includes.
# This is missing on some distributions, so define HAVE_ITCLDECLS_H if found.

if test "$enable_itcl" = "yes"; then
    old_CPPFLAGS=$CPPFLAGS
    CPPFLAGS=-I$ITCLINCDIR
    AC_CHECK_HEADERS([itclDecls.h], [], [], [#include <itcl.h>])
    CPPFLAGS=$old_CPPFLAGS
fi

# ----------------------------------------------------------------------------
# libitcl ([incr Tcl])
# ----------------------------------------------------------------------------

if test "$enable_itcl" = "yes"; then
    if test -z "$ITCLLIBDIR"; then
        PL_FIND_LIB_BEGIN(itcl, ITCLLIBDIR)
        for version in 3.3 3.2 3.1 3.0 2.1 2.0 ""; do
            if test -n "$ITCLLIBDIR"; then
                break
            fi
            PL_FIND_LIB_SET(itcl$version, ITCLLIBDIR, ITCLLIBSTR)
        done
        PL_FIND_LIB_END(itcl, ITCLLIBDIR, enable_itcl)
    fi
    if test "$enable_itcl" = "yes"; then
        if test -r $ITCLLIBDIR/itclConfig.sh; then
            . $ITCLLIBDIR/itclConfig.sh
        fi
    fi
fi

# ----------------------------------------------------------------------------
# figure out how to build tclIndex
# ----------------------------------------------------------------------------

if test "$enable_tcl" = "yes"; then
    if test "$enable_itcl" = "yes"; then
        MKINDEX="$srcdir/scripts/mktclIndex -itcl"
    else
        MKINDEX="$srcdir/scripts/mktclIndex -tcl"
    fi
else
# shouldn't ever be used
    MKINDEX=echo
fi

AC_SUBST(MKINDEX)

# ----------------------------------------------------------------------------
# tk.h
# ----------------------------------------------------------------------------

if test "$enable_tcl" = "no" -o "$have_x" != "yes"; then
    enable_tk=no
fi

if test "$enable_tk" = "yes"; then
    if test -z "$TKINCDIR"; then
        PL_FIND_INC(tk, TKINCDIR, enable_tk)
    fi
fi

# ----------------------------------------------------------------------------
# tkInt.h (and indirectly tclInt.h) required by plplotter.c which is
# required by tkwin device
# ----------------------------------------------------------------------------

if test "$enable_tcl" = "no" -o "$have_x" != "yes"; then
    enable_tkwin=no
fi

if test "$enable_tkwin" = "yes"; then
    if test -z "$TKPRIVATEINCDIR"; then
        PL_FIND_INC(tkInt, TKPRIVATEINCDIR, enable_tkwin)
    fi
fi

if test "$enable_tkwin" = "yes"; then
    if test -z "$TCLPRIVATEINCDIR"; then
        PL_FIND_INC(tclInt, TCLPRIVATEINCDIR, enable_tkwin)
    fi
fi

# ----------------------------------------------------------------------------
# libtk
# ----------------------------------------------------------------------------

if test "$enable_tk" = "yes"; then
    if test -z "$TKLIBDIR"; then
        PL_FIND_LIB_BEGIN(tk, TKLIBDIR)
        for version in 8.4 8.3 8.2 8.1 8.0 4.2i 4.1i 4.2 4.1 4.0 ""; do
            if test -n "$TKLIBDIR"; then
                break
            fi
            PL_FIND_LIB_SET(tk$version, TKLIBDIR, TKLIBSTR)
        done
        PL_FIND_LIB_END(tk, TKLIBDIR, enable_tk)
    fi
fi

# ----------------------------------------------------------------------------
# libitk
# ----------------------------------------------------------------------------

enable_itk="yes"
if test "$enable_tk" = "no" -o "$enable_itcl" = "no"; then
    enable_itk="no"
fi

if test "$enable_itk" = "yes"; then
    if test -z "$ITKLIBDIR"; then
        PL_FIND_LIB_BEGIN(itk, ITKLIBDIR)
        for version in 3.3 3.2 3.1 3.0 2.1 2.0 ""; do
            if test -n "$ITKLIBDIR"; then
                break
            fi
            PL_FIND_LIB_SET(itk$version, ITKLIBDIR, ITKLIBSTR)
        done
        PL_FIND_LIB_END(itk, ITKLIBDIR, enable_itk)
    fi
fi

if test "$enable_itcl" = "yes"; then
    AC_DEFINE(HAVE_ITCL, [], [Define if [incr Tcl] is available])
fi

if test "$enable_itk" = "yes"; then
    AC_DEFINE(HAVE_ITK, [], [Define if [incr Tk] is available])
fi

#-----------------------------------------------------------------------------
# ntk driver
#-----------------------------------------------------------------------------
if test "$enable_tk" = "no" -o "$enable_tcl" = "no"; then
    enable_ntk="no"
fi

### automake symbols for system-dependent results

if test "$enable_itcl" = yes; then
   if test "$ITCLINCDIR" != default; then
      ITCLINCCMD="-I$ITCLINCDIR"
   fi
   if test "$ITCLLIBDIR" = default; then
      ITCLLIBCMD="$ITCLLIBSTR"
   else
      ITCLLIBCMD="-L$ITCLLIBDIR $ITCLLIBSTR"
   fi
fi
AC_SUBST(ITCLINCCMD)
AC_SUBST(ITCLLIBCMD)

###sysloc.in doesn't define all itk related symbols so this stanza changed
###accordingly
if test "$enable_itk" = yes; then
#   if test "$ITKINCDIR" != default; then
#      ITKINCCMD="-I"$ITKINCDIR
#   fi
   if test "$ITKLIBDIR" = default; then
      ITKLIBCMD="$ITKLIBSTR"
   else
      ITKLIBCMD="-L$ITKLIBDIR $ITKLIBSTR"
   fi
fi
#AC_SUBST(ITKINCCMD)
AC_SUBST(ITKLIBCMD)

if test "$enable_tcl" = yes; then
   if test "$TCLINCDIR" != default; then
      TCLINCCMD="-I$TCLINCDIR"
   fi
   if test "$TCLLIBDIR" = default; then
      TCLLIBCMD="$TCLLIBSTR"
   else
      TCLLIBCMD="-L$TCLLIBDIR $TCLLIBSTR"
   fi
fi
AC_SUBST(TCLINCCMD)
AC_SUBST(TCLLIBCMD)

if test "$enable_tk" = yes; then
# Must add in X_CFLAGS for some platforms to work.
   if test "$TKINCDIR" = default; then
      TKINCCMD="$X_CFLAGS"
   else
      TKINCCMD="-I$TKINCDIR $X_CFLAGS"
   fi
   if test "$TKLIBDIR" = default; then
      TKLIBCMD="$TKLIBSTR"
   else
      TKLIBCMD="-L$TKLIBDIR $TKLIBSTR"
   fi
fi
AC_SUBST(TKINCCMD)
AC_SUBST(TKLIBCMD)

### The following seems to be necessary because the replacement of
### the default value for exec_prefix only happens in AC_OUTPUT (it would
### already be too late).
test "x$exec_prefix" = xNONE && exec_prefix='${prefix}'

if test "$enable_java" = yes; then
### Java stanza completely different from others.  sysloc.in must have
### found both $JAVAINCDIR and $JAVAMDINCDIR if $enable_java is still yes.
   JAVAINCCMD="-I$JAVAINCDIR -I$JAVAMDINCDIR"
### bindings/java/config.java.in requires JAVA_INSTDIR
### The symbolic exec_prefix must be replaced by the actual exec_prefix.
   PL_EXPAND_EXPRESSION(JAVA_INSTDIR, $libdir/java/plplot/core)
fi
AC_SUBST(JAVAINCCMD)
AC_SUBST(JAVA_INSTDIR)

### Double vs single precison

PL_ARG_WITH(double,
  [compile the library with double precision floating point],
  yes)

if test "$with_double" = yes; then
  AC_DEFINE(PL_DOUBLE, [],
            [PLplot's PLFLT floating-point is "double" by default, undefine
	    PL_DOUBLE if single precision is required])
  DBL_FLAG_M4=-DDOUBLE
  LIB_TAG="d"
else
  LIB_TAG=""
fi
AC_SUBST(LIB_TAG)

# The following is for use in bindings/octave/plplot_octave.h.in (RL)

DEFINE_PL_DOUBLE=
test "$with_double" = yes && DEFINE_PL_DOUBLE="#define PL_DOUBLE"
AC_SUBST(DEFINE_PL_DOUBLE)


### These various DLNAME variables are used to configure the names of
### the libplplot and libplplottcltk libraries for use in the various
### pkgIndex.tcl scripts.  At run time those scripts currently search first
### for the double-precision form of DLNAME, and if that does not exist
### searches for the single-precision version.  Thus, we must supply both
### double- and single-precision names for libplplot and libplplottcltk.

### This macro may only be used after AM_PROG_LIBTOOL and the definitions of
### $SOVERSION (and $LIB_TAG if we ever have separate double and single
### precision forms of the pkgIndex.tcl scripts).
PL_GET_DLNAME([plplot], [$SOVERSION], [DLNAME_LIBPLPLOT])
PL_GET_DLNAME([plplotd], [$SOVERSION], [DLNAME_LIBPLPLOTD])
PL_GET_DLNAME([plplottcltk], [$SOVERSION], [DLNAME_LIBPLPLOTTCLTK])
PL_GET_DLNAME([plplottcltkd], [$SOVERSION], [DLNAME_LIBPLPLOTTCLTKD])
AC_SUBST(DLNAME_LIBPLPLOT)
AC_SUBST(DLNAME_LIBPLPLOTD)
AC_SUBST(DLNAME_LIBPLPLOTTCLTK)
AC_SUBST(DLNAME_LIBPLPLOTTCLTKD)

### bindings/java/config.java.in needs the complete name of the
### plplotjavac_wrap DLL.
PL_GET_DLLNAME([plplotjavac_wrap], [PLPLOTJAVAC_WRAP_DLL])
AC_SUBST(PLPLOTJAVAC_WRAP_DLL)

### C++ binding

PL_ARG_ENABLE(cxx, [compile C++ interface code], yes)

PL_ARG_WITH(debug, [Adjust debug compilation flags for KAI C++], no)

if test "$enable_cxx" = yes; then

    # If KAI C++ (Kuck and Associates) is available, use that.
    found_cxx=no
    if test -z "$CXX" -o "$CXX" = "KCC"; then
	found_kcc=
	AC_CHECK_PROG(found_kcc, KCC, yes, no)
	if test "$found_kcc" = "yes" ; then
	    found_cxx=yes
	    AC_MSG_WARN([Found KAI C++, using that!])
	    CXX=KCC
	    if test -z "$CXXFLAGS"; then
		if test -z "$with_debug"; then
		    CXXFLAGS="+K0"
		else
		    CXXFLAGS="+K2 -O2"
		fi
	    fi
	fi
    fi
fi
AC_PROG_CXX

### Fortran 77 binding

PL_ARG_ENABLE(f77, [compile Fortran-77 interface code], yes)

if test "$enable_f77" = yes ; then
  AC_PROG_F77
  # If no F77 compiler is found, then disable f77 bindings building
  if test -z "$F77" ; then
    enable_f77=no
    AC_MSG_WARN([Disabling F77 bindings building because no compiler was found])
  fi
fi
if test "$enable_f77" = yes ; then
  HAVE_F77PARSE_CL=yes
  AC_LANG_PUSH([Fortran 77])
  AC_MSG_CHECKING([if fortran command-line parsing is possible])
  AC_RUN_IFELSE([
      implicit none
      character*100 command
      integer narg, iargc
c      get number of command-line arguments
      narg = iargc()
c      get command name.  Apparently this does not always give back the
c      command name on all systems, but all that is required for the
c      getarg(0 call by the interface logic is that a valid string is
c      returned.  (All we are really interested in is the subsequent
c      arguments.)
      call getarg(0, command)
      if(narg.gt.0) call getarg(1, command)
c     ran into one case (Cygwin) where under certain badly linked
c     circumstances, iargc() could return -1
      if(narg.lt.0) then
        call exit(1)
      else
        call exit(0)
      endif
      stop
      end],
  AC_MSG_RESULT(yes),
  AC_MSG_RESULT(no); HAVE_F77PARSE_CL=no)
  AC_LANG_POP([Fortran 77])
  if test "$HAVE_F77PARSE_CL" = yes ; then
    HAVE_F77PARSE_CL_TRUE=""
    HAVE_F77PARSE_CL_FALSE="C "
  else
    HAVE_F77PARSE_CL_TRUE="C "
    HAVE_F77PARSE_CL_FALSE=""
  fi
fi

AC_SUBST(HAVE_F77PARSE_CL_TRUE)
AC_SUBST(HAVE_F77PARSE_CL_FALSE)

PL_ARG_ENABLE(java, [compile Java interface code], yes)

PL_ARG_ENABLE(octave, [compile Octave interface code], no)

# Conditionals to control various Makefile.am files.

if test "$enable_dyndrivers" = yes -a "$enable_pstex" = yes ; then
  enable_ps=yes
fi

AM_CONDITIONAL(with_double, [test "$with_double" = yes])
AM_CONDITIONAL(enable_dyndrivers, [test "$enable_dyndrivers" = yes])
AM_CONDITIONAL(enable_cxx, [test "$enable_cxx" = yes])
AM_CONDITIONAL(enable_f77, [test "$enable_f77" = yes])
AM_CONDITIONAL(enable_gnome, [test "$enable_gnome" = yes])
AM_CONDITIONAL(enable_itcl, [test "$enable_itcl" = yes])
AM_CONDITIONAL(enable_java, [test "$enable_java" = yes])
AM_CONDITIONAL(enable_ljii, [test "$enable_ljii" = yes])
AM_CONDITIONAL(enable_ljiip, [test "$enable_ljiip" = yes])
AM_CONDITIONAL(enable_octave, [test "$enable_octave" = yes])
AM_CONDITIONAL(enable_pbm, [test "$enable_pbm" = yes])
AM_CONDITIONAL(enable_ps, [test "$enable_ps" = yes])
AM_CONDITIONAL(enable_pstex, [test "$enable_pstex" = yes])
AM_CONDITIONAL(enable_python, [test "$enable_python" = yes])
AM_CONDITIONAL(enable_tcl, [test "$enable_tcl" = yes])
AM_CONDITIONAL(enable_tk, [test "$enable_tk" = yes])
AM_CONDITIONAL(enable_tkwin, [test "$enable_tkwin" = yes])
AM_CONDITIONAL(enable_xfig, [test "$enable_xfig" = yes])
AM_CONDITIONAL(enable_xwin, [test "$enable_xwin" = yes])
AM_CONDITIONAL(with_qhull, [test "$with_qhull" = yes])
AM_CONDITIONAL(with_csa, [test "$with_csa" = yes])

### Checks for existence of headers

AC_HEADER_STDC
AC_CHECK_HEADERS(unistd.h termios.h)
AC_HEADER_SYS_WAIT
AC_HEADER_DIRENT

### Check for typedefs

# ----------------------------------------------------------------------------
# Some X11 headers require "caddr_t" even on systems that claim POSIX.1
# compliance, which is illegal.  This makes it impossible to compile
# programs that include X11 headers if _POSIX_SOURCE is defined.  I work
# around this potential problem by just defining caddr_t to 'char *' on all
# systems (unless it is set already), whether it will be needed or not.

AC_CHECK_TYPE(caddr_t, char *)

# The following is primarily for use in drivers/get-drv-info.c
AC_TYPE_SIGNAL

### Check for functions

AC_FUNC_VFORK
AC_CHECK_FUNC(popen, AC_DEFINE(HAVE_POPEN, [],
                               [Define if popen is available]))
AC_CHECK_FUNC(usleep, AC_DEFINE(HAVE_USLEEP, [],
                               [Define if usleep is available]))
AC_CHECK_FUNC(isinf, AC_DEFINE(HAVE_ISINF, [],
                               [Define if isinf is available]))
AC_CHECK_FUNC(finite, AC_DEFINE(HAVE_FINITE, [],
                               [Define if finite is available]))
AC_CHECK_FUNC(isnan, AC_DEFINE(HAVE_ISNAN, [],
                               [Define if isnan is available]))

### Define PLplot default install locations:
### DATA_DIR, LIB_DIR, INCLUDE_DIR, BIN_DIR, TCL_DIR

if test -z "$DATA_DIR"; then
    DATA_DIR='${datadir}/${PACKAGE}${VERSION}'
fi
if test -z "$LIB_DIR"; then
    LIB_DIR='${libdir}'
fi
if test -z "$INCLUDE_DIR"; then
    INCLUDE_DIR='${prefix}/include/plplot'
fi
if test -z "$BIN_DIR"; then
    BIN_DIR='${bindir}'
fi
if test -z "$TCL_DIR"; then
    TCL_DIR='${datadir}/${PACKAGE}${VERSION}/tcl'
fi
if test -z "$DRV_DIR"; then
    DRV_DIR='${libdir}/${PACKAGE}${VERSION}/drivers${LIB_TAG}'
fi

# The following *_CMD variables are needed in examples/*/Makefile.examples.in

PL_EXPAND_EXPRESSION(LIB_DIR_CMD, -L$LIB_DIR)
AC_SUBST(LIB_DIR_CMD)
PL_EXPAND_EXPRESSION(INCLUDE_DIR_CMD, -I$INCLUDE_DIR)
AC_SUBST(INCLUDE_DIR_CMD)

# The following *_HARDDIR variables are needed in bindings/*/pkgIndex.tcl.in

PL_EXPAND_EXPRESSION(LIB_HARDDIR, $LIB_DIR)
AC_SUBST(LIB_HARDDIR)
PL_EXPAND_EXPRESSION(DRV_HARDDIR, $DRV_DIR)
AC_SUBST(DRV_HARDDIR)

BUILD_DIR=`pwd`

### Documentation directory
DOC_DIR='${datadir}/doc/plplot'
AC_SUBST(DOC_DIR)

# Location substitutions for makefiles only

AC_SUBST(INCLUDE_DIR)

# Location substitutions for header files as well

PL_EXPAND_EXPRESSION(TMPVAR, $DATA_DIR)
AC_DEFINE_UNQUOTED(DATA_DIR, "$TMPVAR", [Location of package data files])

PL_EXPAND_EXPRESSION(TMPVAR, $DRV_DIR)
AC_DEFINE_UNQUOTED(DRV_DIR, "$TMPVAR", [Location of dynamically loaded drivers])

PL_EXPAND_EXPRESSION(TMPVAR, $BIN_DIR)
AC_DEFINE_UNQUOTED(BIN_DIR, "$TMPVAR", [Location of executables])

PL_EXPAND_EXPRESSION(TMPVAR, $TCL_DIR)
AC_DEFINE_UNQUOTED(TCL_DIR, "$TMPVAR", [Location of Tcl stuff])

AC_DEFINE_UNQUOTED(BUILD_DIR, "$BUILD_DIR", [Location of Build tree])

AC_SUBST(DATA_DIR)
AC_SUBST(LIB_DIR)
AC_SUBST(TCL_DIR)
AC_SUBST(DRV_DIR)
AC_SUBST(BUILD_DIR)


### DocBook documentation

PL_ARG_ENABLE(builddoc,
  [Enable configuration checks and build of DocBook documentation], no)

PL_ARG_WITH(prebuiltdoc,
  [Assume that the DocBook documentation is already built and present in
   doc/docbook/src, ready to be installed. This option is useful for
   package maintainers.],
  no)

if test $enable_builddoc = yes -a $with_prebuiltdoc = yes ; then
  AC_MSG_ERROR([Options --enable-builddoc and --with-prebuiltdoc are logically
exclusive and must not be given simultaneously.  Giving up.])
fi

AM_CONDITIONAL(docbook_build, [test $with_prebuiltdoc = no])
AM_CONDITIONAL(enable_docbook,
               [test $enable_builddoc = yes -o $with_prebuiltdoc = yes])

PL_INCLUDE(docbook.in)

CHECK_PROG(perl, [INFO MAN], http://www.perl.com)
PERL=${PERL:-"-@echo perl program not available #"}

if test $enable_builddoc = yes -o \
\( $enable_octave = yes -a $with_prebuiltdoc = no \) ; then
CHECK_PM(XML::DOM, xml_dom, [INFO MAN])
CHECK_PM(XML::Parser, xml_parser, [INFO MAN])
fi

if test $enable_builddoc = yes ; then

dnl Check for GNU make
CHECK_GNU_MAKE

dnl Check programs

CHECK_PROG(openjade, [HTML PRINT], http://www.openjade.org)
CHECK_PROG(jadetex, PRINT, http://jadetex.sourceforge.net/)
CHECK_PROG(pdfjadetex, PRINT, http://jadetex.sourceforge.net/)
CHECK_PROG(dvips, PRINT, http://www.ctan.org)
CHECK_PROG(onsgmls, [], http://www.openjade.org)
CHECK_PROG(makeinfo, [INFO], [http://www.gnu.org])

dnl Check public identifiers

CHECK_DTD(
  [DSSSL Style Sheet DTD],
  [dsssl_dtd],
  [],
  [],
  [],
  [[[<!ELEMENT book - O (#PCDATA)>]]],
  [sgml],
  [\"$DSSSL_DTD_PUBID\"],
  [style-sheet.dtd],
  [jade],
  [PRINT HTML])

CHECK_DTD(
  [DocBook HTML Stylesheet],
  [html_ss],
  [[[<!ENTITY dbstyle PUBLIC \"$DB_SS_HTML_PUBID\" CDATA DSSSL>]]],
  [use=\"docbook\"],
  [<external-specification id=\"docbook\" document=\"dbstyle\">],
  [[[<!ELEMENT book - O (#PCDATA)>]]],
  [sgml],
  [\"$DB_SS_HTML_PUBID\"],
  [html/docbook.dsl],
  [docbook-stylesheets],
  [HTML])

CHECK_DTD(
  [DocBook Print Stylesheet],
  [print_ss],
  [[[<!ENTITY dbstyle PUBLIC \"$DB_SS_PRINT_PUBID\" CDATA DSSSL>]]],
  [use=\"docbook\"],
  [<external-specification id=\"docbook\" document=\"dbstyle\">],
  [[[<!ELEMENT book - O (#PCDATA)>]]],
  [tex],
  [\"$DB_SS_PRINT_PUBID\"],
  [print/docbook.dsl],
  [docbook-stylesheets],
  [PRINT])

CHECK_DTD(
  [DocBook DTD],
  [db_dtd],
  [],
  [],
  [],
  [PUBLIC \"$DOCBOOK_DTD_PUBID\"],
  [sgml],
  [\"$DOCBOOK_DTD_PUBID\"],
  [docbookx.dtd],
  [docbook-xml (DTD version 3.1.3)],
  [PRINT HTML])

# These checks only work if perl is checked first (see above)!
CHECK_PM(XML::Parser::PerlSAX, xml_parser_perlsax, [INFO])
CHECK_PM(XML::Writer, xml_writer, [INFO])

DOC_OUTPUT_COMMANDS

fi # enable_builddoc

AM_CONDITIONAL(has_onsgmls, [test "$has_onsgmls" = found])
# This is for conditional compilation of plplot_octave_txt in bindings/octave
AM_CONDITIONAL(has_xml_pm,
  [test "$pldb_cv_xml_dom" = yes -a "$pldb_cv_xml_parser" = yes])

BASE=$PACKAGE-$VERSION
AC_SUBST(BASE)

HTML_MANIFEST=HTML-MANIFEST
AC_SUBST(HTML_MANIFEST)

BASE_HTML=$PACKAGE-html-$VERSION
AC_SUBST(BASE_HTML)

INFO_MANIFEST=INFO-MANIFEST
AC_SUBST(INFO_MANIFEST)

BASE_INFO=$PACKAGE-info-$VERSION
AC_SUBST(BASE_INFO)

MAN_MANIFEST=MAN-MANIFEST
AC_SUBST(MAN_MANIFEST)

BASE_MAN=$PACKAGE-man-$VERSION
AC_SUBST(BASE_MAN)

MANVOL=3plplot
AC_SUBST(MANVOL)


### pkg-config checks and settings.

PL_ARG_WITH(pkg-config,
  [Use pkg-config in Makefile.examples.  If value is "yes"
   then check for availability of the program in the system.],
  no)

if test "$with_pkg_config" = yes ; then
  AC_CHECK_PROG(PKG_CONFIG, pkg-config, found, "not found")
  test "$PKG_CONFIG" != found  &&  with_pkg_config=no
  if test "$has_perl" = no ; then
    AC_MSG_WARN([pkg-config support needs perl to be built.])
    AC_MSG_WARN([Setting with_pkg_config=no.])
    with_pkg_config=no
  fi
fi

if test "$with_pkg_config" = yes ; then
  pkg_config_true=""
  pkg_config_false="# "
else
  pkg_config_true="# "
  pkg_config_false=""
fi

PKG_CONFIG_DIR='${libdir}/pkgconfig'

if test -z "$PKG_CONFIG_ENV" ; then
  PL_EXPAND_EXPRESSION(PKG_CONFIG_ENV, PKG_CONFIG_PATH=$PKG_CONFIG_DIR)
fi

AC_SUBST(pkg_config_true)
AC_SUBST(pkg_config_false)
AC_SUBST(PKG_CONFIG_DIR)
AC_SUBST(PKG_CONFIG_ENV)
AM_CONDITIONAL(with_pkg_config, [test "$with_pkg_config" = yes])

### rpath setting

PL_ARG_WITH(rpath, [use -rpath when linking libraries], yes)

if test "$with_rpath" = yes ; then
  RPATH='-rpath $(libdir)'
  PL_EXPAND_EXPRESSION(RPATHCMD, [-Wl,-rpath -Wl,$libdir])
else
  RPATH=
  RPATHCMD=
fi
AC_SUBST(RPATH)
AC_SUBST(RPATHCMD)
AM_CONDITIONAL(with_rpath, [test "$with_rpath" = yes])


### Variables for mkoctfile
### ******************************************************************
### WARNING: The stuff below is just transient and should disappear as
### soon as the brokenness of NetBSD is fixed.
### ******************************************************************
### Variables are set only if the C++ compiler is g++

MKOCTFILE_VARS=
case "$system" in
    NetBSD* )
        if test "$CXX" = g++ ; then
            MKOCTFILE_VARS='SH_LD="g++" SH_LDFLAGS="-shared"'
	    AC_MSG_WARN([Setting mkoctfile variables for broken NetBSD system])
        fi
    ;;
esac
AC_SUBST(MKOCTFILE_VARS)


### Build configuration files

AC_CONFIG_FILES([
	Makefile
	src/Makefile
	include/Makefile
	lib/Makefile
	lib/nn/Makefile
	lib/csa/Makefile
	data/Makefile
	bindings/Makefile
	bindings/c++/Makefile
	bindings/f77/Makefile
	bindings/f77/configurable.f
	bindings/python/Makefile
	bindings/tcl/Makefile
	bindings/tcl/pkgIndex.tcl
	bindings/tk/Makefile
	bindings/tk/pkgIndex.tcl
	bindings/tk-x-plat/pkgIndex.tcl
	bindings/octave/Makefile
        bindings/octave/plplot_octave.h
        bindings/octave/PLplot/Makefile
	bindings/octave/PLplot/plplot_octave_path.m
	bindings/octave/PLplot/support/Makefile
	bindings/octave/demos/Makefile
	bindings/octave/misc/Makefile
	bindings/java/Makefile
	bindings/java/config.java
	drivers/Makefile
	examples/Makefile
	examples/Makefile.examples
	examples/c/Makefile
	examples/c/Makefile.examples
	examples/c++/Makefile
	examples/c++/Makefile.examples
	examples/f77/Makefile
	examples/f77/Makefile.examples
	examples/python/Makefile
	examples/python/plplot_python_start.py
	examples/tcl/Makefile
	examples/tk/Makefile
	examples/tk/Makefile.examples
	examples/java/Makefile
	examples/perl/Makefile
	test/Makefile
	test/plplot-test.sh
	test/test_f77.sh
	test/test_python.sh
	utils/Makefile
	scripts/Makefile
	doc/Makefile
	scripts/plplot-config
	pkgcfg/Makefile
	pkgcfg/gen-pc-files.pl
	doc/docbook/Makefile
	doc/docbook/src/Makefile
	doc/docbook/src/plplotdoc.xml
	doc/docbook/src/plplotdoc-html.dsl
	doc/docbook/src/plplotdoc-print.dsl
	doc/docbook/bin/api2man.pl
	doc/docbook/www/index.html.in
	])

AC_OUTPUT

# ----------------------------------------------------------------------------
# Print out some of the more important settings.
#
# In a reversal of previous practice, I print the actual variables in the
# configure log.  This is to remind the user of the difference between the
# command line syntax (which you can always get help on by issuing a --help
# option) and the internal representation.  It's necessary to remember this
# for when you want to set the variable directly via ~/config/cf_plplot.in.
# ----------------------------------------------------------------------------

plplot_config_invocation="$0 `./config.status --version \
  | grep 'with options' \
  | grep \' | cut -d\' -f2`"

plplot_config_result_msg="
Configure results:

command:	$plplot_config_invocation
system:		$host
have_x:		$have_x
prefix:		$prefix
CC:		$CC $CC_FLAGS"

if test "$enable_cxx" = "yes"; then
    plplot_config_result_msg=\
"$plplot_config_result_msg
CXX:		$CXX $CXX_FLAGS"
fi

if test "$enable_f77" = "yes"; then
    plplot_config_result_msg=\
"$plplot_config_result_msg
F77:		$F77 $F77_FLAGS"
fi

plplot_config_result_msg=\
"$plplot_config_result_msg
LIB_TAG:	$LIB_TAG
devices:	$DEVICES

Available device drivers:
static:		$STATIC_DRIVERS
dynamic:	$DYNAMIC_DRIVERS

Compilation options:
with_debug:	$with_debug

Library options:
enable_shared:	$enable_shared		enable_static:	$enable_static
with_rpath:	$with_rpath		with_double:	$with_double

Optional libraries:
with_qhull:	$with_qhull		with_csa:	$with_csa
with_freetype:	$with_freetype		with_pthreads:	$with_pthreads


Language Bindings:
enable_tcl:	$enable_tcl		enable_itcl:	$enable_itcl
enable_cxx:	$enable_cxx		enable_f77:	$enable_f77
enable_java:	$enable_java		enable_python:	$enable_python
enable_octave:	$enable_octave
"

cat << EOF > config.summary
$plplot_config_result_msg
EOF

AC_MSG_NOTICE($plplot_config_result_msg)
