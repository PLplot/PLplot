dnl configure.ac for PLplot  -*- autoconf -*-
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
AC_REVISION($Revision$)

dnl Date of release.  This can be set by hand, but the prefered method
dnl is by specifying one of the option --version of --date-version to
dnl bootstrap.sh

RELEASE_DATE=2004-02-05
AC_SUBST(RELEASE_DATE)

AC_CONFIG_SRCDIR(src/plcore.c)

dnl Use cf dir instead of the top dir for all autotools-related files

AC_CONFIG_AUX_DIR(cf)
AC_CONFIG_MACRO_DIR(cf)

### Configuration headers

AC_CONFIG_HEADERS([config.h include/plConfig.h include/plDevs.h])

### Automake initialization
AM_INIT_AUTOMAKE

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

PL_ARG_ENABLE(dyndrivers, [enable dynamic loading of drivers], yes)

m4_include(cf/libtool.ac)

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

### Add libm to LIBS, as some non linux unices might require this.
### This must be done before sysloc.in is included.
AC_CHECK_LIB(m, sin)

### The following seems to be necessary because the replacement of
### the default value for prefix only happens in AC_OUTPUT (it would
### already be too late).
test "x$prefix" = xNONE && prefix=$ac_default_prefix

### The following seems to be necessary because the replacement of
### the default value for exec_prefix only happens in AC_OUTPUT (it would
### already be too late).
test "x$exec_prefix" = xNONE && exec_prefix='${prefix}'

m4_include(cf/python.ac)

m4_include(cf/java.ac)

builtin([define], [PL_DRIVERS_DEVICE_LIST], [
  cgm:cgm:yes,
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

AC_MSG_CHECKING(for dynamic drivers)
PL_ARG_ENABLE_DRIVERS(PL_DRIVERS_DEVICE_LIST)
AC_MSG_RESULT

AC_SUBST(STATIC_DRIVERS)
AC_SUBST(DYNAMIC_DRIVERS)

m4_include(cf/gd.ac)
m4_include(cf/cgm.ac)
m4_include(cf/gnome.ac)
m4_include(cf/linuxvga.ac)
m4_include(cf/xwin.ac)

PL_ADD_DRIVERS(PL_DRIVERS_DEVICE_LIST)

### Modify system-dependent results based on availability
m4_include(cf/freetype.ac)
m4_include(cf/octave.ac)
m4_include(cf/csiro.ac)
m4_include(cf/swig.ac)
m4_include(cf/pthread.ac)

# ----------------------------------------------------------------------------
# Check for inclusion of sys/types.h before dirent.h in broken MacOSX systems
# ----------------------------------------------------------------------------

case "$host_os" in
    darwin*)
        AC_DEFINE(NEED_SYS_TYPE_H, [], [Include sys/type.h if needed])
    ;;
esac

# ----------------------------------------------------------------------------
# Now do the variable substitutions.
# ----------------------------------------------------------------------------

AC_SUBST(ARLIB_BUILD)
AC_SUBST(ARLIB_BUILD_CXX)
AC_SUBST(SHLIB_BUILD)
AC_SUBST(SHLIB_BUILD_CXX)
AC_SUBST(SONAME_FLAGS)
AC_SUBST(SA)
AC_SUBST(SO)
AC_SUBST(LDSHARED)

AC_SUBST(SHLIB_CCFLAGS)
AC_SUBST(SHLIB_CXXFLAGS)
AC_SUBST(SHLIB_F77FLAGS)
AC_SUBST(SHLIB_LIBS)

# Disable LIBS so we don't have all libraries mentioned
LIBS=""

# Optional packages

m4_include(cf/tcl.ac)

m4_include(cf/test.ac)

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

m4_include(cf/c++.ac)

m4_include(cf/f77.ac)

# Conditionals to control various Makefile.am files.

if test "$enable_dyndrivers" = yes -a "$enable_pstex" = yes ; then
  enable_ps=yes
fi

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

m4_include(cf/docbook.ac)

m4_include(cf/pkg-config.ac)

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

AM_CONDITIONAL(with_double, [test "$with_double" = yes])
AM_CONDITIONAL(enable_ljii, [test "$enable_ljii" = yes])
AM_CONDITIONAL(enable_ljiip, [test "$enable_ljiip" = yes])
AM_CONDITIONAL(enable_pbm, [test "$enable_pbm" = yes])
AM_CONDITIONAL(enable_ps, [test "$enable_ps" = yes])
AM_CONDITIONAL(enable_pstex, [test "$enable_pstex" = yes])
AM_CONDITIONAL(enable_xfig, [test "$enable_xfig" = yes])
AM_CONDITIONAL(with_rpath, [test "$with_rpath" = yes])
AM_CONDITIONAL(enable_dyndrivers, [test "$enable_dyndrivers" = yes])

### Build configuration files

AC_CONFIG_FILES([
	Makefile
	src/Makefile
	include/Makefile
	data/Makefile
	bindings/Makefile
	drivers/Makefile
	examples/Makefile
	examples/Makefile.examples
	examples/c/Makefile
	examples/c/Makefile.examples
	examples/perl/Makefile
	utils/Makefile
	scripts/Makefile
	doc/Makefile
	scripts/plplot-config
	])

AC_OUTPUT

m4_include(cf/summary.ac)
