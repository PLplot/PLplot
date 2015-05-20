# cmake/modules/instdirs.cmake
#
# Copyright (C) 2006-2015  Alan W. Irwin
#
# This file is part of PLplot.
#
# PLplot is free software; you can redistribute it and/or modify
# it under the terms of the GNU Library General Public License as published
# by the Free Software Foundation; version 2 of the License.
#
# PLplot is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU Library General Public License for more details.
#
# You should have received a copy of the GNU Library General Public License
# along with the file PLplot; if not, write to the Free Software
# Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301  USA

# Module for determining all installation directories for PLplot.

# Note, use absolute install locations only since relative ones are
# automatically prefixed with ${CMAKE_INSTALL_PREFIX} which may destroy
# the fine-tuning of installation locations that we desire.

# Cached install locations following what is done for the autotools configure
# script: (CMAKE_INSTALL_SBINDIR, CMAKE_INSTALL_LIBEXECDIR,
# CMAKE_INSTALL_SHAREDSTATEDIR, CMAKE_INSTALL_LOCALSTATEDIR, and
# CMAKE_INSTALL_OLDINCLUDEDIR not set because PLplot does not use those
# install locations).

set(
CMAKE_INSTALL_EXEC_PREFIX
${CMAKE_INSTALL_PREFIX}
CACHE PATH "install location for architecture-dependent files"
)

set(
CMAKE_INSTALL_BINDIR
${CMAKE_INSTALL_EXEC_PREFIX}/bin
CACHE PATH "install location for user executables"
)

set(
CMAKE_INSTALL_DATADIR
${CMAKE_INSTALL_PREFIX}/share
CACHE PATH "install location for read-only architecture-independent data"
)

set(
CMAKE_INSTALL_LIBDIR
${CMAKE_INSTALL_EXEC_PREFIX}/lib
CACHE PATH "install location for object code libraries"
)

set(
CMAKE_INSTALL_INCLUDEDIR
${CMAKE_INSTALL_PREFIX}/include
CACHE PATH "install location for C header files"
)

set(
CMAKE_INSTALL_INFODIR
${CMAKE_INSTALL_DATADIR}/info
CACHE PATH "install location for info documentation"
)

set(
CMAKE_INSTALL_MANDIR
${CMAKE_INSTALL_DATADIR}/man
CACHE PATH "install location for man documentation"
)

set(
CMAKE_INSTALL_PKG_CONFIG_DIR
${CMAKE_INSTALL_LIBDIR}/pkgconfig
CACHE PATH "install location for pkg-config *.pc files"
)

# Configured PLplot install locations determined from user-updatable
# cached values above.

# Data.
set(DATA_DIR ${CMAKE_INSTALL_DATADIR}/${PACKAGE}${PLPLOT_VERSION})

# Libraries.
set(LIB_DIR ${CMAKE_INSTALL_LIBDIR})

# Headers.
set(INCLUDE_DIR ${CMAKE_INSTALL_INCLUDEDIR}/${PACKAGE})

# Binaries.
set(BIN_DIR ${CMAKE_INSTALL_BINDIR})

# Shared library path.
set(SHLIB_DIR ${CMAKE_INSTALL_LIBDIR})

# Tcl files.
set(TCL_DIR ${CMAKE_INSTALL_DATADIR}/${PACKAGE}${PLPLOT_VERSION}/tcl)

# Ada source files (*.adb, *.ads) (following recommendations in
# http://www.ada-france.org/debian/debian-ada-policy.html
set(ADA_INCLUDE_DIR ${CMAKE_INSTALL_DATADIR}/ada/adainclude/plplotada)

# Ada library information files (*.ali) (following recommendations in
# http://www.ada-france.org/debian/debian-ada-policy.html
set(ADA_LIB_DIR ${CMAKE_INSTALL_LIBDIR}/ada/adalib/plplotada)

# Drivers.
set(DRV_DIR ${CMAKE_INSTALL_LIBDIR}/${PACKAGE}${PLPLOT_VERSION}/drivers)

# Documentation.
set(DOC_DIR ${CMAKE_INSTALL_DATADIR}/doc/${PACKAGE})

# Info pages.
set(INFO_DIR ${CMAKE_INSTALL_INFODIR})

# Man pages.
set(MAN_DIR ${CMAKE_INSTALL_MANDIR})

# pkg-config *.pc files
set(PKG_CONFIG_DIR ${CMAKE_INSTALL_PKG_CONFIG_DIR})

# Other path-related variables.

# Absolute path of top-level build directory.
set(BUILD_DIR ${CMAKE_BINARY_DIR})

# Absolute path of top-level source directory.
set(SOURCE_DIR ${CMAKE_SOURCE_DIR})

# The following *_CMD variables are needed in
# examples/*/Makefile.examples.cmake

set(LIB_DIR_CMD "-L${LIB_DIR}")
set(INCLUDE_DIR_CMD "-I${INCLUDE_DIR}")
