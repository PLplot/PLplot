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


dnl =======================================================================
dnl Autoconf/automake initializations
dnl =======================================================================

dnl Package version number
dnl [ If changing the line below by hand, please keep the format up to and
dnl   including the version number, otherwise it will fool up the
dnl   bootstrap.sh script.  By the way, the line below can be changed using
dnl   options --version and --date-version of that script. ]

AC_INIT(plplot, 5.3.0.cvs.20040211, plplot-devel@lists.sourceforge.net)
AC_REVISION($Revision$)

dnl Date of release.  This can be set by hand, but the prefered method
dnl is by specifying one of the option --version of --date-version to
dnl bootstrap.sh

RELEASE_DATE=2004-02-05
AC_SUBST(RELEASE_DATE)

dnl Check if we are in the right directory (this used to be done by
dnl AC_INIT)

AC_CONFIG_SRCDIR(src/plcore.c)

dnl Use cf dir instead of the top dir for all autotools-related files

AC_CONFIG_AUX_DIR(cf)
AC_CONFIG_MACRO_DIR(cf)

dnl Configuration headers

AC_CONFIG_HEADERS([config.h include/plConfig.h include/plDevs.h])

dnl Automake initialization

AM_INIT_AUTOMAKE

dnl Default installation prefix

AC_PREFIX_DEFAULT(/usr/local)


dnl =======================================================================
dnl PLplot library sonames and soversions
dnl =======================================================================

dnl Version numbers for the shared C libraries.

SOVERSION=9:0:0
AC_SUBST(SOVERSION)

dnl Version number for the shared C++ library.  After version 5.2.1 was
dnl released, the C++ bindings were changed and backward incompatibilities
dnl were introduced.  Hence the different strings for SOVERSION and
dnl CPP_SOVERSION.

CPP_SOVERSION=9:0:0
AC_SUBST(CPP_SOVERSION)

dnl According to man package, PLPLOT_TCL_VERSION (used in the various
dnl pkgIndex.tcl scripts) should be strictly numbers alternativing with
dnl decimal points with the first number (the major number) signifying
dnl incompatible changes in the scripts.  To keep our life simple, let us
dnl use the major number from the package version for now.
dnl N.B. both LIBRARY_VERSION and PLPLOT_TCL_VERSION are used for separate
dnl purposes in the various configured pkgIndex.tcl scripts.

PLPLOT_TCL_VERSION=5
AC_SUBST(PLPLOT_TCL_VERSION)

dnl =======================================================================
dnl Check for programs
dnl =======================================================================

dnl Require C compile

AC_PROG_CC

dnl Libtool configuration

m4_include(cf/libtool.ac)


dnl =======================================================================
dnl Libraires
dnl =======================================================================

dnl Disable LIBS so we don't have all libraries mentioned

LIBS=""

dnl Add libm to LIBS, as some non linux unices might require this.

AC_CHECK_LIB(m, sin)

m4_include(cf/pthread.ac)
m4_include(cf/freetype.ac)
m4_include(cf/csiro.ac)


dnl =======================================================================
dnl Headers
dnl =======================================================================

AC_HEADER_STDC
AC_CHECK_HEADERS(unistd.h termios.h)
AC_HEADER_SYS_WAIT
AC_HEADER_DIRENT

dnl Check for inclusion of sys/types.h before dirent.h in broken MacOSX
dnl systems 

case "$host_os" in
    darwin*)
        AC_DEFINE(NEED_SYS_TYPE_H, [], [Include sys/type.h if needed])
    ;;
esac


dnl =======================================================================
dnl Typedefs
dnl =======================================================================

dnl Some X11 headers require "caddr_t" even on systems that claim POSIX.1
dnl compliance, which is illegal.  This makes it impossible to compile
dnl programs that include X11 headers if _POSIX_SOURCE is defined.  I work
dnl around this potential problem by just defining caddr_t to 'char *' on all
dnl systems (unless it is set already), whether it will be needed or not.

AC_CHECK_TYPE(caddr_t, char *)

dnl The following is primarily for use in drivers/get-drv-info.c

AC_TYPE_SIGNAL


dnl =======================================================================
dnl Functions
dnl =======================================================================

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


m4_include(cf/rpath.ac)

m4_include(cf/double.ac)

dnl =======================================================================
dnl PLplot drivers
dnl =======================================================================

dnl m4_include(cf/drivers.ac)

dnl Including a driver in the below list includes it by default.
dnl Note that each driver should be treated separately even if
dnl they are in the same source code file.
dnl For example, the ps and psc drivers are defined in the ps.c source
dnl code file.

dnl N.B. This is ordered the same as the *driver pair* list below to help
dnl humans keep the associations straight, but this is not necessary.

dnl Note specifically which drivers are known to be loadable.  Eventually,
dnl hopefully, every driver will show up on this list.  However, we add them one
dnl at a time since each will have its own peculiarities in the build process
dnl (none missing at present).

dnl N.B. This is ordered the same as the *driver pair* list below to help
dnl humans keep the associations straight, but this is not necessary.

dnl We think of a "device" as the output medium.  Could be a machine
dnl style (Tektronix, X11), or a file format (Postscript).
dnl
dnl A "driver" is the PLplot code that produces the output on the
dnl medium.
dnl
dnl Some drivers support multiple devices.  But we we need to make sure
dnl we compile only the selected drivers.  That is, if a driver supports
dnl devices not selected, we don't want to compile it at all.  Or if it
dnl supports multiple selected devices, we only want to compile it
dnl once.  To support this, we build an association list between devices
dnl and drivers.  This will be inspected to build up the set of drivers
dnl to be compiled based on which devices are selected.

dnl Ordered alphabetically by second in each pair (the driver) for human
dnl consumption, but this is not necessary.

PL_ARG_ENABLE(dyndrivers, [enable dynamic loading of drivers], yes)

PL_ARG_ENABLE(drivers, [enable all device drivers], yes)

# Set driver enable values
#
# You can enable/disable drivers either by the command line
# (--enable-<driver> or --disable-<driver>) or via the cf_plplot.in file
# (remember to use underscores instead of dashes here).  You can disable
# all drivers by default by using --disable-drivers.

# Special cases

# ----------------------------------------------------------------------------
# Set flag to enable searching for dynloadable drivers, if appropriate.
# ----------------------------------------------------------------------------

if test "$enable_dyndrivers" = "yes"; then
    AC_DEFINE(ENABLE_DYNDRIVERS, [],
              [Define if there is support for dynamically loaded drivers])
fi

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
    xwin:xwin:yes
])

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

if test "$enable_dyndrivers" = yes -a "$enable_pstex" = yes ; then
    enable_ps=yes
fi

AM_CONDITIONAL(enable_ljii, [test "$enable_ljii" = yes])
AM_CONDITIONAL(enable_ljiip, [test "$enable_ljiip" = yes])
AM_CONDITIONAL(enable_pbm, [test "$enable_pbm" = yes])
AM_CONDITIONAL(enable_ps, [test "$enable_ps" = yes])
AM_CONDITIONAL(enable_pstex, [test "$enable_pstex" = yes])
AM_CONDITIONAL(enable_xfig, [test "$enable_xfig" = yes])
AM_CONDITIONAL(enable_dyndrivers, [test "$enable_dyndrivers" = yes])


dnl =======================================================================
dnl Language bindings
dnl =======================================================================

m4_include(cf/swig.ac)
m4_include(cf/python.ac)
m4_include(cf/java.ac)
m4_include(cf/octave.ac)
m4_include(cf/tcl.ac)
m4_include(cf/c++.ac)
m4_include(cf/f77.ac)


dnl =======================================================================
dnl Other parts of PLplot (test suite, DocBook manual, pkg-config support)
dnl =======================================================================

m4_include(cf/test.ac)
m4_include(cf/docbook.ac)
m4_include(cf/pkg-config.ac)


dnl =======================================================================
dnl PLplot default install locations
dnl =======================================================================

m4_include(cf/instdirs.ac)


dnl =======================================================================
dnl Build configuration files and show summary
dnl =======================================================================

dnl Notice that there are other calls to AC_CONFIG_FILES in the cf/*.ac
dnl files.

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
