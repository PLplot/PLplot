# cmake/modules/installdirs.cmake
#
# Copyright (C) 2006  Alan W. Irwin
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

# Relative and absolute install locations.

# Data.
set(DATA_DIR ${CMAKE_INSTALL_PREFIX}/share/${PACKAGE}${VERSION})

# Libraries.
set(LIB_DIR ${CMAKE_INSTALL_PREFIX}/lib)

# Headers.
set(INCLUDE_DIR ${CMAKE_INSTALL_PREFIX}/include/${PACKAGE})

# Binaries.
set(BIN_DIR ${CMAKE_INSTALL_PREFIX}/bin)

# Tcl files.
set(TCL_DIR ${CMAKE_INSTALL_PREFIX}/share/${PACKAGE}${VERSION}/tcl)

# Drivers.
set(DRV_DIR ${CMAKE_INSTALL_PREFIX}/lib/${PACKAGE}${VERSION}/drivers${LIB_TAG})

# Documentation.
set(DOC_DIR ${CMAKE_INSTALL_PREFIX}/share/doc/${PACKAGE})

# Man pages.
set(MAN_DIR ${CMAKE_INSTALL_PREFIX}/share/man)

# Info pages.
set(INFO_DIR ${CMAKE_INSTALL_PREFIX}/share/info)

# Other path-related variables.

# Absolute path of top-level build directory.
set(BUILD_DIR ${CMAKE_BINARY_DIR})

# The following *_CMD variables are needed in 
# examples/*/Makefile.examples.cmake

set(LIB_DIR_CMD "-L${LIB_DIR}")
set(INCLUDE_DIR_CMD "-I${INCLUDE_DIR}")
