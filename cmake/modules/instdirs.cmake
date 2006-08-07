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
set(DATA_INSTALL_DIR share/${PACKAGE}${VERSION})
set(DATA_DIR ${CMAKE_INSTALL_PREFIX}/${DATA_INSTALL_DIR})

# Libraries.
set(LIB_INSTALL_DIR lib)
set(LIB_DIR ${CMAKE_INSTALL_PREFIX}/${LIB_INSTALL_DIR})

# Headers.
set(INCLUDE_INSTALL_DIR include/${PACKAGE})
set(INCLUDE_DIR ${CMAKE_INSTALL_PREFIX}/${INCLUDE_INSTALL_DIR})

# Binaries.
set(BIN_INSTALL_DIR bin)
set(BIN_DIR ${CMAKE_INSTALL_PREFIX}/${BIN_INSTALL_DIR})

# Tcl files.
set(TCL_INSTALL_DIR ${DATA_INSTALL_DIR}/tcl)
set(TCL_DIR ${CMAKE_INSTALL_PREFIX}/${TCL_INSTALL_DIR})

# Drivers.
set(DRV_INSTALL_DIR ${LIB_INSTALL_DIR}/${PACKAGE}${VERSION}/drivers${LIB_TAG})
set(DRV_DIR ${CMAKE_INSTALL_PREFIX}/${DRV_INSTALL_DIR})

# Documentation.
set(DOC_INSTALL_DIR share/doc/${PACKAGE})
set(DOC_DIR ${CMAKE_INSTALL_PREFIX}/${DOC_INSTALL_DIR})

# Man pages.
set(MAN_INSTALL_DIR man)
set(MAN_DIR ${CMAKE_INSTALL_PREFIX}/${MAN_INSTALL_DIR})

# Info pages.
set(INFO_INSTALL_DIR share/info)
set(INFO_DIR ${CMAKE_INSTALL_PREFIX}/${INFO_INSTALL_DIR})

# Other path-related variables.

# Absolute path of top-level build directory.
set(BUILD_DIR ${CMAKE_BINARY_DIR})

# The following *_CMD variables are needed in 
# examples/*/Makefile.examples.cmake

set(LIB_DIR_CMD "-L${LIB_DIR}")
set(INCLUDE_DIR_CMD "-I${INCLUDE_DIR}")
